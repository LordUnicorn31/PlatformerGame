#pragma once
#include "Point.h"
#include "Defs.h"
#include "List.h"
#include "DynArray.h"
#include "PriorityQueue.h"
#include <stack>

#define MAXMAPTILES 512


enum HeuristicFormula {
	Manhattan,
	Euclidean,
	EuclideanNoSqt
};

struct CompleteNode {
	int f;
	int g;
	int h;
	unsigned short px;
	unsigned short py;
	unsigned short x;
	unsigned short y;
};

struct PathNode {
	//PathNode();
	int f;
	int g;
	unsigned short int px; //parent x
	unsigned short int py; //parent y
	unsigned short pz; // parent list index
	short jumpLength;
	unsigned char status;
};

struct Location
{
	Location():xy(0),z(0){}
	Location(int xy, int z)
	{
		this->xy = xy;
		this->z = z;
	}

	int xy;
	int z;
};

class ComparePFNodeMatrix : public IComparer<Location>
{
public:

	ComparePFNodeMatrix(List<PathNode> Imatrix[], int size)
	{
		for (int i = 0; i < size; ++i)
			matrix[i] = Imatrix[i];
	}

	int Compare(Location a, Location b) override
	{
		if (matrix[a.xy][a.z].f > matrix[b.xy][b.z].f)
			return 1;
		else if (matrix[a.xy][a.z].f < matrix[b.xy][b.z].f)
			return -1;
		return 0;
	}

private:
	List<PathNode> matrix[];
};


class Pathfinding {
public:
	Pathfinding(const Pathfinding&) = delete;

	static Pathfinding& Get()
	{
		static Pathfinding instance;
		return instance;
	}

	static void SetMap(unsigned int width, unsigned int height, unsigned char* data) { return Get().ISetMap(width, height, data); }
	static bool FindPath(iPoint& start, iPoint& end, short maxCharacterJumpHeight, int characterHeight = 1, int characterWidth = 1) { return Get().IFindPath(start, end, maxCharacterJumpHeight, characterHeight, characterWidth); }
	static const List<iPoint>* GetLastPath() { return &Get().close; }

private:
	void ISetMap(unsigned int width, unsigned int height, unsigned char* data);
	bool IFindPath(iPoint& start, iPoint& end, short maxCharacterJumpHeight, int characterHeight = 1, int characterWidth = 1);

	Pathfinding() {};

	/*!!private*/std::stack<int> touchedLocations; //to remember the locations we need to clear after the algorithm run
	HeuristicFormula formula;
	unsigned short gridX;//limit x del mapa en tile (per chekejar bounderies) (width + 1)
	unsigned short gridY;//limit y del mapa en tile (per chekejar bounderies) (height + 1)
	unsigned short gridWidth;
	unsigned char openNodeValue = 1;
	unsigned char closedNodeValue = 2;
	bool diagonals = true;
	//bool diagonalsAdditionalCost = true;
	Location location;
	unsigned short locationX;
	unsigned short locationY;
	int newLocation;
	unsigned short newLocationX;
	unsigned short newLocationY;
	int endLocation;
	int closedNodeCounter;

	//bool stop;
	//bool stopped;
	int hEstimate = 2;
	bool found;
	char directions[8][2]{ {0,-1} , {1,0}, {0,1}, {-1,0}, {1,-1}, {1,1}, {-1,1}, {-1,-1} };
	int newG;
	int h;
	PriorityQueue<Location> open = nullptr;
	List<iPoint> close;
	unsigned char costGrid[MAXMAPTILES][MAXMAPTILES]; //TODO (maybe better to allocate it here on the class and not use a pointer) (in the actual implementation they use unsigned char[][])
	List<PathNode> nodes[];
};