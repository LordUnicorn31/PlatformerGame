#pragma once
#include "Defs.h"
#include "DynArray.h"
#include "Point.h"
#include <vector>
#include "SString.h"

class SDL_Texture;

struct RenderInfo
{
	int x, y, textureId;
};

struct Property
{
	SString name;
	int value;
	int id;
};

struct TempProperty
{
	std::vector<int> tilesetIds;
	Property property;
};

struct MapTile 
{
	DynArray<RenderInfo> renderTiles;
	DynArray<int> propertyIDs;
	//TODO: Do I have to call delete on this when I delete the renderTiles buffer?
};

class Map 
{
public:
	Map(const Map&) = delete;

	static Map& Get()
	{
		static Map instance;
		return instance;
	}
	static void Draw() { return Get().SDraw(); }
	static bool Load(const char* mapPath,const char* mapName) { return Get().SLoad(mapPath,mapName); }
	static bool UnLoad() { return Get().SUnLoad(); }
	static iPoint MapToWorld(int x, int y) { return Get().SMapToWorld(x,y); }
	static iPoint WorldToMap(int x, int y) { return Get().SWorldToMap(x, y); }
	static int GetTileProperty(int id, const char* name) { return Get().SGetTileProperty(id, name); }
	//TODO: getters are static?
	static int GetMapWidth() { return Get().width; }
	static int GetMapHeight() { return Get().height; }
	static int GetTileWidth() { return Get().tileWidth; }
	static int GetTileHeight() { return Get().tileHeight; }
	static bool MapLoaded() { return Get().mapLoaded; }

private:
	void SDraw();
	bool SLoad(const char* mapPath, const char* fileName);
	bool SUnLoad();
	iPoint SMapToWorld(int x, int y) const;
	iPoint SWorldToMap(int x, int y) const;
	int SGetTileProperty(int id, const char* name);
	//Draw collisions / walkability ?
	Map() {};

	int width, height, tileWidth, tileHeight;
	bool mapLoaded = false;
	MapTile* mapTiles = nullptr;
	DynArray<Property> properties;
	DynArray<SDL_Texture*> textures;
};