#include "Render.h"
#include "Textures.h"
#include "App.h"
#include "Map.h"
#include "PugiXml/src/pugixml.hpp"
#include "Log.h"
#include "List.h"
#include "SString.h"
#include "Pathfinding.h"

#ifdef OPTICKPROFILE
#include "optick.h"
#endif // OPTICKPROFILER


bool Map::ILoad(const char* mapPath, const char* fileName)
{
	SString tmp(mapPath);
	tmp += fileName;
	
	pugi::xml_document mapFile;
	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load Map xml file %s. pugi error: %s", mapPath, result.description());
		return false;
	}
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		return false;
	}

	/*if (map.attribute("orientation").as_string() != "orthogonal")
	{
		LOG("Map is not orthogonal");
		return false;
	}*/

	width = map.attribute("width").as_int();
	height = map.attribute("height").as_int();
	tileWidth = map.attribute("tilewidth").as_int();
	tileHeight = map.attribute("tileheight").as_int();

	List<uint*>layersData;

	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer; layer = layer.next_sibling("layer"))
	{
		SString layerName = layer.attribute("name").as_string();
		SString navigation = "navigation";

		if (layerName == navigation) 
		{
			continue;
		}
		
		pugi::xml_node layerData = layer.child("data");

		if (layerData == NULL)
		{
			LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
			return false;
		}
		uint * data = new uint[width * height];
		memset(data, 0, width * height);

		int i = 0;
		for (pugi::xml_node tile = layerData.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			data[i++] = static_cast<uint>(tile.attribute("gid").as_int(0));
		}

		layersData.add(data);
	}

	int numLayers = layersData.count();
	if (numLayers <= 0)
	{
		LOG("No layer data");
		return false;
	}

	mapTiles = new MapTile[width * height];

	pugi::xml_node tileset;
	int prevLastId = 0;
	int numTilesets = 0;
	for (tileset = mapFile.child("map").child("tileset"); tileset; tileset = tileset.next_sibling("tileset"), numTilesets++) 
	{
		if (tileset.attribute("tilewidth").as_int() != tileWidth && tileset.attribute("tileheight").as_int() != tileHeight)
		{
			LOG("Tileset %s: tilesize is different than map tilesize", tileset.attribute("name").as_string());
			//release the map data
			delete[] mapTiles;
			ListItem<uint*>* layerIt = layersData.start;
			for (layerIt; layerIt != NULL; layerIt = layerIt->next) 
			{
				delete[] layerIt->data;
				layerIt->data = nullptr;
			}
			layersData.clear();
			return false;
		}

		int firstGid = tileset.attribute("firstgid").as_int();
		int lastGid = tileset.attribute("tilecount").as_int() - firstGid;
		int margin = tileset.attribute("margin").as_int();
		int spacing = tileset.attribute("spacing").as_int();
		if (tileset.child("image") == NULL)
		{
			LOG("Tileset %s doesn't have an image tag", tileset.attribute("name").as_string());
			//release the map data
			delete[] mapTiles;
			ListItem<uint*>* layerIt = layersData.start;
			for (layerIt; layerIt != NULL; layerIt = layerIt->next)
			{
				delete[] layerIt->data;
				layerIt->data = nullptr;
			}
			layersData.clear();
			return false;
		}
		SDL_Texture* tilesetTexture = app->tex->Load(PATH(mapPath, tileset.child("image").attribute("source").as_string()));
		if (tilesetTexture == NULL) 
		{
			//release tha map data
			delete[] mapTiles;
			ListItem<uint*>* layerIt = layersData.start;
			for (layerIt; layerIt != NULL; layerIt = layerIt->next)
			{
				delete[] layerIt->data;
				layerIt->data = nullptr;
			}
			layersData.clear();
			return false;
		}
		textures.PushBack(tilesetTexture);

		int tilesWidth = tileset.child("image").attribute("width").as_int() / tileWidth;

		DynArray<TempProperty>tempProperties;

		pugi::xml_node propertyNode = tileset.child("tile");
		for (propertyNode; propertyNode; propertyNode = propertyNode.next_sibling("tile")) 
		{
			pugi::xml_node nodeProperties = propertyNode.child("properties").child("property");
			for(nodeProperties; nodeProperties; nodeProperties = nodeProperties.next_sibling("property"))
			{
				bool alreadyExists = false;
				for(int x = 0; x < tempProperties.Count();++x)
				{
					SString propertyName = nodeProperties.attribute("name").as_string();

					SString tempName = tempProperties[x].property.name;
					if( propertyName == tempName)
					{
						tempProperties[x].tilesetIds.push_back(propertyNode.attribute("id").as_int());
						alreadyExists = true;
						break;
					}
				}
				
				if (!alreadyExists) 
				{
					TempProperty temp;
					Property current;
					temp.property.name = nodeProperties.attribute("name").as_string();
					temp.property.value = nodeProperties.attribute("value").as_bool();
					temp.tilesetIds.push_back(propertyNode.attribute("id").as_int());

					current.name = nodeProperties.attribute("name").as_string();
					current.value = propertyNode.attribute("id").as_int();

					tempProperties.PushBack(temp);
					properties.PushBack(current);
				}
			}
		}

		ListItem<uint*>* layerIt = layersData.start;
		for (layerIt; layerIt != NULL; layerIt = layerIt->next) 
		{
			for (int i = 0; i < width * height; ++i) {
				int gid = layerIt->data[i];
				int tilesetId =  gid - firstGid;
				if (gid > 0 && ((firstGid -1) <= tilesetId <= lastGid)) 
				{
					//create a tile to render and push it
					RenderInfo renderTile;
					renderTile.x = margin + ((tileWidth + spacing) * (tilesetId % tilesWidth));
					renderTile.y = margin + ((tileHeight + spacing) * (tilesetId / tilesWidth));
					renderTile.textureId = numTilesets;
					mapTiles[i].renderTiles.PushBack(renderTile);

					//check if the tile has any property and push it
					for (int k = 0; k < tempProperties.Count(); ++k) 
					{
						for (int f = 0; f < tempProperties[k].tilesetIds.size(); ++f)
						{
							if (tempProperties[k].tilesetIds[f] == tilesetId) 
							{
								mapTiles[i].propertyIDs.PushBack(k + prevLastId);
							}
						}
					}
				}
			}
		}
		prevLastId += tempProperties.Count() - 1;
	}

	ListItem<uint*>* layerIt = layersData.start;
	for (layerIt; layerIt != NULL; layerIt = layerIt->next)
	{
		delete[] layerIt->data;
		layerIt->data = nullptr;
	}
	layersData.clear();
	mapFile.reset();
	mapLoaded = true;

	int index;
	uchar* navigationMap = new uchar[width * height];
	memset(navigationMap, (uchar)1, width * height);
	
	for (int y = 0; y < height; ++y) 
	{
		for (int x = 0; x < width; ++x) 
		{
			index = y * width + x;
			if (GetTileProperty(index, "Blocked"))
				navigationMap[index] = (uchar)0;
		}
	}
	Pathfinding::SetMap(width, height, navigationMap);
	delete[] navigationMap;

	return true;
}

bool Map::IUnLoad() 
{
	for (int i = 0; i < textures.Count(); i++)
	{
		app->tex->UnLoad(textures[i]);
		textures[i] = nullptr;
		//TODO: do i need to delete this memory manually? (call delete)
	}
	textures.Clear();
	//TODO: do i need to delete the properties dynArray memory?
	//properties.~DynArray();
	//properties.Clear();
	properties.Clear();
	delete[] mapTiles;
	mapTiles = nullptr;
	mapLoaded = false;

	return true;
}

iPoint Map::IMapToWorld(int x, int y) const
{
	return iPoint(x * tileWidth, y * tileHeight);
}

iPoint Map::IWorldToMap(int x, int y) const
{
	return iPoint(x / tileWidth, y / tileHeight);
}

bool Map::IGetTileProperty(int id, const char* iName)
{
	for (int i = 0; i < mapTiles[id].propertyIDs.Count(); ++i)
	{
		SString testName = properties[mapTiles[id].propertyIDs[i]].name;
		if ( testName == iName) 
		{
			return properties[mapTiles[id].propertyIDs[i]].value;
		}
	}
	return false;
}

void Map::IDraw() 
{
#ifdef OPTICKPROFILE
	OPTICK_EVENT();
#endif // OPTICKPROFILER

	
	if (!mapLoaded)
		return;

	iPoint camSize(IWorldToMap(-app->render->camera.x + app->render->camera.w, -app->render->camera.y + app->render->camera.h));
	iPoint camPos = IWorldToMap(-app->render->camera.x, -app->render->camera.y);

	for (int y = camPos.y; y <= camSize.y; ++y)
	{
		if (y < 0 || y >= height)
			continue;
		for (int x = camPos.x; x <= camSize.x; ++x)
		{
			if (x<0 || x>width)
				continue;

			int tileId = y * width + x;
			for(int i = 0; i < mapTiles[tileId].renderTiles.Count(); i++)
			{
				iPoint pos = IMapToWorld(x, y);
				SDL_Rect r = { mapTiles[tileId].renderTiles[i].x, mapTiles[tileId].renderTiles[i].y, tileWidth, tileHeight };
				app->render->DrawTexture(textures[mapTiles[tileId].renderTiles[i].textureId], pos.x, pos.y, &r);
			}

		}
	}
}