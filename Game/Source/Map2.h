#pragma once
#include "Defs.h"
#include "DynArray.h"
#include "Point.h"

class SDL_Texture;

struct RenderTile
{
	int x, y, textureId;
};

struct PropertyTile
{
	const char* name;
	bool value;
};

struct MapRenderTile 
{
	DynArray<RenderTile> tiles;
	//TODO: Do I have to call delete on this when I delete the renderTiles buffer?
};

struct MapPropertyTile 
{
	DynArray<PropertyTile> tiles;
	//TODO: Do I have to call delete on this when I delete the propertyTiles buffer?
};

class Map2 {
public:
	Map2(const Map2&) = delete;

	static Map2& Get() {
		static Map2 instance;
		return instance;
	}
	static void Draw() { return Get().IDraw(); }
	static bool Load(const char* mapPath,const char* mapName) { return Get().ILoad(mapPath,mapName); }
	static bool UnLoad() { return Get().IUnLoad(); }
	static iPoint MapToWorld(int x, int y) { return Get().IMapToWorld(x,y); }
	static iPoint WorldToMap(int x, int y) { return Get().IWorldToMap(x, y); }

private:
	void IDraw();
	bool ILoad(const char* mapPath, const char* fileName);
	bool IUnLoad();
	iPoint IMapToWorld(int x, int y) const;
	iPoint IWorldToMap(int x, int y) const;
	//Draw collisions / walkability ?
	Map2();

	int width, height, tileWidth, tileHeight;
	bool mapLoaded = false;
	MapRenderTile* renderTiles = nullptr;
	MapPropertyTile* propertyTiles = nullptr;
	DynArray<SDL_Texture*> textures;
};