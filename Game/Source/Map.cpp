#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
	name = "map";
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder = config.child("folder").child_value();

	return ret;
}

void Map::Draw()
{
	if (mapLoaded == false)
		return;

	iPoint cam_size(WorldToMap(-app->render->camera.x + app->render->camera.w, -app->render->camera.y + app->render->camera.h));
	iPoint cam_pos = WorldToMap(-app->render->camera.x, -app->render->camera.y);
	ListItem<MapLayer*>* item;
	for (item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;
		/*if (layer->properties.Get("Navigation", 0) == 1)
			continue;*/

		for (int y = cam_pos.y; y <= cam_size.y; ++y)
		{
			if (y < 0 || y >= data.height)
				continue;
			for (int x = cam_pos.x; x <= cam_size.x; ++x)
			{
				if (x<0 || x>data.width)
					continue;
				int tile_id = layer->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->GetTileRect(tile_id);
					iPoint pos = MapToWorld(x, y);

					app->render->DrawTexture(tileset->texture, pos.x, pos.y, &r);
				}
			}
		}
	}
}

/*
int Properties::Get(const char* value, int default_value) const
{
	ListItem<Property*>* item = list.start;

	while (item != NULL)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}
*/

void Map::DrawGrid()
{
	iPoint point_a = { data.tileWidth / 2 + 3,data.tileHeight / 2 + 3 };
	iPoint point_b = { -((data.width * data.tileWidth) / 2 + data.tileWidth / 2) + data.tileWidth + 3,
		(data.height * data.tileHeight) / 2 + data.tileHeight / 2 + 3 };

	for (int i = 0; i <= data.width; ++i)
	{
		app->render->DrawLine(point_a.x, point_a.y, point_b.x, point_b.y, 255, 255, 255, 50);

		point_a.x += data.tileWidth / 2;
		point_a.y += data.tileHeight / 2;

		point_b.x += data.tileWidth / 2;
		point_b.y += data.tileHeight / 2;
	}

	//Back to the first tile
	point_a.x = data.tileWidth / 2;
	point_a.y = data.tileHeight / 2 + 3;

	point_b.x = (data.width * data.tileWidth) / 2 + data.tileWidth / 2;
	point_b.y = (data.height * data.tileHeight) / 2 + data.tileHeight / 2 + 3;

	for (int i = 0; i <= data.height; ++i)
	{
		app->render->DrawLine(point_a.x, point_a.y, point_b.x, point_b.y, 255, 255, 255, 50);

		point_a.x -= data.tileWidth / 2;
		point_a.y += data.tileHeight / 2;

		point_b.x -= data.tileWidth / 2;
		point_b.y += data.tileHeight / 2;
	}
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item != NULL)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

void Map::GetMapSize(int& w, int& h) const
{
	w = app->map->data.width * app->map->data.tileWidth;
	h =app->map->data.height * app->map->data.tileHeight;
}

iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tileWidth * 0.5f);
		ret.y = (x + y) * (data.tileHeight * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tileWidth * 0.5f;
		float half_height = data.tileHeight * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2) - 1;
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relative_id % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relative_id / numTilesWidth));
	return rect;
}

bool Map::GetTileProperty(uint id, SString propertyname) {
	//Provisioonal function (we have to improve performance with something like an unordered container using a hash function)
	/*//convertir l'id en el index correcte
	id = id % numpropertytiles;
	//solucio a les collisions
	if (id >= numpropertytiles)
		return false;
	for (int i = 0; i < Propertytiels[id].numproperties; ++i) {
		if (Propertytiels[id].properties[i].name == propertyname) {
			return Propertytiels[id].properties[i].value;
		}
	}*/
	TileSet* tileset = GetTilesetFromTileId(id);
	uint relativeid = id - tileset->firstgid;
	for (int i = 0; i < tileset->numPropertyTiles; ++i) {
		if (relativeid == tileset->PropertyTiles[i].id) {
			for (int j = 0; j < tileset->PropertyTiles[i].numproperties; ++j) {
				if (tileset->PropertyTiles[i].properties[j].name == propertyname) {
					return tileset->PropertyTiles[i].properties[j].value;
				}
			}
		}
	}
	return false;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	ListItem<TileSet*>* item = data.tilesets.start;

	while (item != NULL)
	{
		app->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers

	ListItem<MapLayer*>* item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	return true;
}

// Load new map
bool Map::Load(const char* file_name)
{
	bool ret = true;
	SString tmp;
	tmp = folder.GetString();
	tmp += file_name;

	pugi::xml_document mapFile;
	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap(mapFile);
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		if (ret == true)
		{
			LoadTileProperties(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.add(lay);
	}

	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tileWidth, data.tileHeight);

		ListItem<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tileWidth, s->tileHeight);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item= item->next;
		}

		ListItem<MapLayer*>* item_layer = data.layers.start;
		while (item_layer != data.layers.end)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
		//App->render->cam_limit_x = data.width * data.tile_width - App->render->camera.w;
		//App->render->cam_limit_y = data.height * data.tile_height - App->render->camera.h;
	}

	mapLoaded = ret;
	// Clean up the pugui tree
	mapFile.reset();

	return ret;
}

// Load map general properties
bool Map::LoadMap(pugi::xml_document& mapFile)
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		SString bg_color = map.attribute("backgroundcolor").as_string();

		data.backgroundColor.r = 0;
		data.backgroundColor.g = 0;
		data.backgroundColor.b = 0;
		data.backgroundColor.a = 0;

		if (bg_color.Length() > 0)
		{

			SString  red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.b = v;
		}

		SString orientation = map.attribute("orientation").as_string();

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name = tileset_node.attribute("name").as_string();
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offsetX = offset.attribute("x").as_int();
		set->offsetY = offset.attribute("y").as_int();
	}
	else
	{
		set->offsetX = 0;
		set->offsetY = 0;
	}

	return ret;
}

bool Map::LoadTileProperties(pugi::xml_node& tileset_node, TileSet* set) {
	pugi::xml_node firstnode = tileset_node.child("tile");
	set->numPropertyTiles = 0;
	for (pugi::xml_node CountingTile = firstnode; CountingTile; CountingTile = CountingTile.next_sibling("tile"))
	{
		set->numPropertyTiles ++;
	}
	set->PropertyTiles = new Tile[set->numPropertyTiles];

	int i = 0;
	for (pugi::xml_node tile = firstnode; tile; tile = tile.next_sibling("tile"), ++i)
	{
		set->PropertyTiles[i].id = tile.attribute("id").as_uint(0);
		set->PropertyTiles[i].numproperties = 0;
		pugi::xml_node Propertiesfirstnode = tile.child("properties").child("property");
		for (pugi::xml_node CountnodeProperties = Propertiesfirstnode; CountnodeProperties; CountnodeProperties = CountnodeProperties.next_sibling("property")) {
			set->PropertyTiles[i].numproperties++;
		}
		set->PropertyTiles[i].properties = new Tile::Property[set->PropertyTiles[i].numproperties];
		for (pugi::xml_node nodeProperties = Propertiesfirstnode; nodeProperties; nodeProperties = nodeProperties.next_sibling("property")) {
			set->PropertyTiles[i].properties->name = nodeProperties.attribute("name").as_string(0);
			set->PropertyTiles[i].properties->value = nodeProperties.attribute("value").as_bool(0);
		}
	}
	return true;
}

bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->texWidth = image.attribute("width").as_int();

		if (set->texWidth <= 0)
		{
			set->texWidth = w;
		}

		set->texHeight = image.attribute("height").as_int();

		if (set->texHeight <= 0)
		{
			set->texHeight = h;
		}

		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
	}

	return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	//LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width * layer->height];
		memset(layer->data, 0, layer->width * layer->height);

		int i = 0;
		for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = static_cast<uint>(tile.attribute("gid").as_int(0));
		}
	}

	return ret;
}

/*
// Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			properties.list.add(p);
		}
	}

	return ret;
}*/

/*
bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;

	ListItem<MapLayer*>* item;
	item = data.layers.start;
	for (; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? (uchar)0 : (uchar)1;
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}
*/