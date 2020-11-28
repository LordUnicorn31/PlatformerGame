#include "Render.h"
#include "Textures.h"
#include "App.h"
#include "Map2.h"
#include "PugiXml/src/pugixml.hpp"
#include "Log.h"
#include "List.h"

bool Map2::ILoad(const char* mapPath, const char* fileName)
{
	SString tmp(mapPath);
	tmp += fileName;
	
	pugi::xml_document mapFile;
	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load Map2 xml file %s. pugi error: %s", mapPath, result.description());
		return false;
	}
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		return false;
	}

	if (map.attribute("orientation").as_string() != "orthogonal")
	{
		LOG("Map is not orthogonal");
		return false;
	}

	width = map.attribute("width").as_int();
	height = map.attribute("height").as_int();
	tileWidth = map.attribute("tilewidth").as_int();
	tileHeight = map.attribute("tileheight").as_int();

	List<uint*>layersData;

	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer; layer = layer.next_sibling("layer"))
	{
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
	if (numLayers)
	{
		LOG("No layer data");
		return false;
	}

	renderTiles = new MapRenderTile[width * height];
	propertyTiles = new MapPropertyTile[width * height];

	pugi::xml_node tileset;
	int numTilesets = 0;
	for (tileset = mapFile.child("map").child("tileset"); tileset; tileset = tileset.next_sibling("tileset"), numTilesets++) 
	{
		if (tileset.attribute("tilewidth").as_int() != tileWidth && tileset.attribute("tileheight").as_int() != tileHeight)
		{
			LOG("Tileset %s: tilesize is different than map tilesize", tileset.attribute("name").as_string());
			//release the map data
			delete[] renderTiles;
			delete[] propertyTiles;
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
			delete[] renderTiles;
			delete[] propertyTiles;
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
		if (tilesetTexture) 
		{
			//release tha map data
			delete[] renderTiles;
			delete[] propertyTiles;
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

		DynArray<uint> propertyIds;
		DynArray<PropertyTile> properties;

		pugi::xml_node propertyNode = tileset.child("tile");
		for (propertyNode; propertyNode; propertyNode = propertyNode.next_sibling("tile")) 
		{
			pugi::xml_node nodeProperties = propertyNode.child("properties").child("property");
			for(nodeProperties; nodeProperties; nodeProperties = nodeProperties.next_sibling("property"))
			{
				propertyIds.PushBack(propertyNode.attribute("id").as_int());
				PropertyTile propertyTile;
				propertyTile.name = nodeProperties.attribute("name").as_string();
				propertyTile.value = nodeProperties.attribute("value").as_bool();
				properties.PushBack(propertyTile);
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
					RenderTile renderTile;
					renderTile.x = margin + ((tileWidth + spacing) * (tilesetId % tilesWidth));
					renderTile.y = margin + ((tileHeight + spacing) * (tilesetId / tilesWidth));
					renderTile.textureId = numTilesets;
					renderTiles[i].tiles.PushBack(renderTile);

					//check if the tile has any property and push it
					for (int i=0; i <propertyIds.Count();i++ )
					{
						if (tilesetId == propertyIds[i]) 
						{
							propertyTiles->tiles.PushBack(properties[i]);
						}
					}
				}
			}
		}
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

	return true;
}

bool Map2::IUnLoad() 
{
	for (int i = 0; i < textures.Count(); i++)
	{
		app->tex->UnLoad(textures[i]);
		textures[i] = nullptr;
		//TODO: do i need to delete this memory manually? (call delete)
	}
	textures.Clear();
	delete[] renderTiles;
	renderTiles = nullptr;
	delete[] propertyTiles;
	propertyTiles = nullptr;
	mapLoaded = false;

	return true;
}

iPoint Map2::IMapToWorld(int x, int y) const
{
	return iPoint(x * tileWidth, y * tileHeight);
}

iPoint Map2::IWorldToMap(int x, int y) const
{
	return iPoint(x / tileWidth, y / tileHeight);
}

void Map2::IDraw() 
{
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
			DynArray<RenderTile> tiles = renderTiles[tileId].tiles;
			for(int i = 0; tiles.Count();i++)
			{
				iPoint pos = IMapToWorld(x, y);
				SDL_Rect r = { tiles[i].x, tiles[i].y, tileWidth, tileHeight };
				app->render->DrawTexture(textures[tiles[i].textureId], pos.x, pos.y, &r);
			}

		}
	}
}