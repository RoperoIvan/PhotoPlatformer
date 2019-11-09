#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "j1Render.h"
#include "j1Map.h"
#include "Brofiler/Brofiler.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}


// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
const p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while(item)
	{
		if(item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while(item)
	{
		if(item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill,ENTITY_TYPE type) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	if (type == ENTITY_TYPE::FLYING_ENEMY)
	{
		// north
		cell.create(pos.x, pos.y + 1);
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, cell, this));

		// south
		cell.create(pos.x, pos.y - 1);
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, cell, this));

		// east
		cell.create(pos.x + 1, pos.y);
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, cell, this));

		// west
		cell.create(pos.x - 1, pos.y);
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, cell, this));
	}

	// north-west
	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// north-est
	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south-east
	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south - west
	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination, ENTITY_TYPE type)
{
	BROFILER_CATEGORY("CreatePath", Profiler::Color::Azure);
	last_path.Clear();

	if (IsWalkable(origin) == false|| IsWalkable(destination) == false)
		return -1;

	PathList open;
	PathList close;
	PathNode origin_path(0, origin.DistanceManhattan(destination), origin, nullptr);

	open.list.add(origin_path);


	while ( open.list.count() != 0)
	{
		close.list.add(open.GetNodeLowestScore()->data);
		open.list.del(open.GetNodeLowestScore());
		if (close.list.end->data.pos == destination)
		{
			const PathNode* parent = &close.list.end->data;
			while (parent)
			{
				last_path.PushBack(parent->pos);
				parent = parent->parent;
			}
			last_path.Flip();
			return last_path.Count();
		}
		else
		{
			PathList frontier;
			close.list.end->data.FindWalkableAdjacents(frontier,type);
			for (p2List_item<PathNode>* iterator = frontier.list.start; iterator != nullptr; iterator = iterator->next)
			{
				if (close.Find(iterator->data.pos))
					continue;
				else if (open.Find(iterator->data.pos))
				{
					iterator->data.CalculateF(destination);
					PathNode new_node = open.Find(iterator->data.pos)->data;
					if (new_node.g > iterator->data.g)
					{
						new_node.parent = iterator->data.parent;
					}
				}
				else
				{
					iterator->data.CalculateF(destination);
					open.list.add(iterator->data);
				}
			}
		}
	}
	return -1;
}


bool j1PathFinding::DrawPath()
{
	if (last_path.Count() <= 0)
		last_path;
	else
	{
		for (uint i = 0; i < last_path.Count(); ++i)
		{
			App->render->DrawQuad({ App->map->MapToWorld(last_path.At(i)->x,last_path.At(i)->y).x, App->map->MapToWorld(last_path.At(i)->x,last_path.At(i)->y).y , App->map->data.tile_width,App->map->data.tile_height }, 255, 0, 0, 250);
		}
	}


	return false;
}