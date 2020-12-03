/*#include "Pathfinding.h"

PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint & pos, const PathNode * parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode & node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

uint PathNode::FindWalkableAdjacents(PathList & list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if ( IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	//diagonals
	cell.create(pos.x + 1, pos.y + 1);
	if (IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(pos.x + 1, pos.y - 1);
	if (IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(pos.x - 1, pos.y + 1);
	if (IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(pos.x - 1, pos.y - 1);
	if (IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
}

int PathNode::Score() const
{
	return g + h;
}

int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

ListItem<PathNode>* PathList::Find(const iPoint& point) const
{
	ListItem<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

ListItem<PathNode>* PathList::GetNodeLowestScore() const
{
	ListItem<PathNode>* ret = NULL;
	int min = 65535;

	ListItem<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

void Pathfinding::ISetMap(unsigned int width, unsigned int height, unsigned char* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}

int Pathfinding::ICreatePath(const iPoint& origin, const iPoint& destination)
{
	int ret = -1;
	// TODO 1: if origin or destination are not walkable, return -1
	if (!(IsWalkable(origin) && IsWalkable(destination) && origin != destination))
		return ret;
	// TODO 2: Create two lists: open, close
	// Add the origin tile to open
	// Iterate while we have tile in the open list
	PathList open;
	PathList closed;
	open.list.add(PathNode(0, 0, origin, nullptr));
	while (open.list.count() > 0) {
		// TODO 3: Move the lowest score cell from open list to the closed list
		ListItem<PathNode>* lowest = open.GetNodeLowestScore();
		ListItem<PathNode>* node = closed.list.add(lowest->data);
		open.list.del(lowest);
		// TODO 4: If we just added the destination, we are done!
		// Backtrack to create the final path
		// Use the Pathnode::parent and Flip() the path when you are finish
		if (node->data.pos == destination) {
			lastPath.Clear();
			const PathNode* path_node = &node->data;
			while (path_node) {
				lastPath.PushBack(path_node->pos);
				path_node = path_node->parent;
			}
			lastPath.Flip();
			ret = lastPath.Count();
			break;
		}
		// TODO 5: Fill a list of all adjancent nodes
		PathList adjacent;
		node->data.FindWalkableAdjacents(adjacent);
		// TODO 6: Iterate adjancent nodes:
		ListItem<PathNode>* item = adjacent.list.start;
		for (; item; item = item->next) {
			// ignore nodes in the closed list
			if (closed.Find(item->data.pos) != NULL)
				continue;
			// If it is NOT found, calculate its F and add it to the open list
			item->data.CalculateF(destination);
			if (open.Find(item->data.pos) == NULL)
				open.list.add(item->data);
			else {
				// If it is already in the open list, check if it is a better path (compare G)
				// If it is a better path, Update the parent
				if (open.Find(item->data.pos)->data.g > item->data.g)
					open.Find(item->data.pos)->data.parent = item->data.parent;
			}
		}
	}

	return ret;
}

bool Pathfinding::ICheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

uchar Pathfinding::IGetTileAt(const iPoint& pos)
{
	if (CheckBoundaries(pos))
		return map[(pos.y * width) + pos.x];

	return INVALID_WALK_CODE;
}

bool Pathfinding::IIsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}
*/