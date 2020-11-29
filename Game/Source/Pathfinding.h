#pragma once
#include "Point.h"
#include "List.h"
#include "DynArray.h"

class Pathfinding {
public:
	Pathfinding(const Pathfinding&) = delete;

	static Pathfinding& Get()
	{
		static Pathfinding instance;
		return instance;
	}

	static void SetMap(unsigned int width, unsigned int height, unsigned char* data) { return Get().ISetMap(width, height, data); }
	static int CreatePath(const iPoint& origin, const iPoint& destination) { return Get().ICreatePath(origin, destination); }
	static const DynArray<iPoint>* GetLastPath() { return &Get().lastPath; }
	static bool CheckBoundaries(const iPoint& pos) { return Get().ICheckBoundaries(pos); }
	static uchar GetTileAt(const iPoint& pos) { return Get().IGetTileAt(pos); }

private:
	void ISetMap(unsigned int width, unsigned int height, unsigned char* data);
	int ICreatePath(const iPoint& origin, const iPoint& destination);
	bool ICheckBoundaries(const iPoint& pos) const;
	uchar IGetTileAt(const iPoint& pos);

	Pathfinding();

	uint width;
	uint height;
	uchar* map;
	DynArray<iPoint> lastPath;
};

struct PathList;

struct PathNode
{
	PathNode();
	PathNode(int g, int h, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	uint FindWalkableAdjacents(PathList& list_to_fill) const;

	int Score() const;

	int CalculateF(const iPoint& destination);

	int g;
	int h;
	iPoint pos;
	const PathNode* parent;
};

struct PathList
{
	ListItem<PathNode>* Find(const iPoint& point) const;

	ListItem<PathNode>* GetNodeLowestScore() const;

	List<PathNode> list;
};