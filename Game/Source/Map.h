#pragma once
#include "Defs.h"
#include "DynArray.h"
#include "Point.h"
#include <vector>

class SDL_Texture;

struct RenderInfo
{
	int x, y, textureId;
};

struct Property
{
	SString name;
	bool value;
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

class Map {
public:
	Map(const Map&) = delete;

	static Map& Get() {
		static Map instance;
		return instance;
	}
	static void Draw() { return Get().IDraw(); }
	static bool Load(const char* mapPath,const char* mapName) { return Get().ILoad(mapPath,mapName); }
	static bool UnLoad() { return Get().IUnLoad(); }
	static iPoint MapToWorld(int x, int y) { return Get().IMapToWorld(x,y); }
	static iPoint WorldToMap(int x, int y) { return Get().IWorldToMap(x, y); }
	static bool GetTileProperty(int id, const char* name) { return Get().IGetTileProperty(id, name); }
	static bool GetTileProperty(int x, int y, const char* name) { return Get().IGetTileProperty(x, y, name); }
	//TODO: getters are static?
	static int GetMapWidth() { return Get().width; }
	static int GetLogWidth() { return Get().logWidth; }
	static int GetMapHeight() { return Get().height; }
	static int GetTileWidth() { return Get().tileWidth; }
	static int GetTileHeight() { return Get().tileHeight; }
	static bool MapLoaded() { return Get().mapLoaded; }

private:
	void IDraw();
	bool ILoad(const char* mapPath, const char* fileName);
	bool IUnLoad();
	iPoint IMapToWorld(int x, int y) const;
	iPoint IWorldToMap(int x, int y) const;
	bool IGetTileProperty(int id, const char* iName);
	bool IGetTileProperty(int x, int y, const char* iName);
	//Draw collisions / walkability ?
	Map() {};

	int width, height, tileWidth, tileHeight, logWidth;
	bool mapLoaded = false;
	MapTile* mapTiles = nullptr;
	DynArray<Property> properties;
	DynArray<SDL_Texture*> textures;
};