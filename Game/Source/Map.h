#pragma once
#include "Point.h"
#include "Module.h"
#include "List.h"
//#include "SDL_pixels.h"

class SDL_Color;
class SDL_Rect;
class SDL_Texture;

/*struct Properties
{
	struct Property
	{
		SString name;
		int value;
	};

	~Properties()
	{
		ListItem<Properties::Property*>* item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;
	List<Property*> list;
};*/

// ----------------------------------------------------
struct MapLayer
{
	SString	name;
	int			width;
	int			height;
	uint* data;
	//Properties	properties;

	MapLayer() : data(NULL), width(0), height(0)
	{}

	~MapLayer()
	{
		RELEASE_ARRAY(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

// ----------------------------------------------------
struct Tile {

	struct Property {
		SString name;
		bool value;
	};

	uint id;
	uint numproperties;
	Property* properties = nullptr;

	~Tile() {
		RELEASE_ARRAY(properties);
	}
};

struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	SString name;
	int	firstgid;
	int	margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	SDL_Texture* texture;
	int texWidth;
	int	texHeight;
	int	numTilesWidth;
	int	numTilesHeight;
	int	offsetX;
	int	offsetY;

	Tile* PropertyTiles;
	uint numPropertyTiles;

	~TileSet() {
		RELEASE_ARRAY(PropertyTiles);
	}
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int	width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	SDL_Color backgroundColor;
	MapTypes type;
	List<TileSet*> tilesets;
	List<MapLayer*> layers;
};

// ----------------------------------------------------
class Map : public Module
{
public:

	Map();

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	//Testing and Debug purposes
	void DrawGrid();

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	//bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	void GetMapSize(int& w, int& h) const;

	TileSet* GetTilesetFromTileId(int id) const;
	bool GetTileProperty(uint id, SString propertyname);

private:

	bool LoadMap(pugi::xml_document& mapFile);
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTileProperties(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	//bool LoadProperties(pugi::xml_node& node, Properties& properties);


public:

	MapData data;

private:

	SString	folder;
	bool mapLoaded;

};