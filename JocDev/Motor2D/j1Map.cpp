#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Collisions.h"
#include "j1EntityManager.h"
#include "j1Scene.h"
#include <math.h>
#include "Brofiler/Brofiler.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("MAP DRAW", Profiler::Color::DeepPink);

	if (map_loaded == false)
		return;

	for (p2List_item<TileSet*>* tilesetIterator = data.tilesets.start; tilesetIterator != NULL; tilesetIterator = tilesetIterator->next) {
		for (p2List_item<MapLayer*>* layerIterator = data.mapLayers.start; layerIterator != NULL; layerIterator = layerIterator->next) {
			for (int column = 0; column < layerIterator->data->columns; ++column) {
				for (int row = 0; row < layerIterator->data->rows; ++row) {
					uint gid = layerIterator->data->tileArray[layerIterator->data->GetArrayPos(column, row)];
					if (gid != 0) {
						iPoint worldPos = MapToWorld(column, row);
						App->render->Blit(tilesetIterator->data->texture, worldPos.x, worldPos.y, &tilesetIterator->data->GetTileRect(gid), true,SDL_RendererFlip::SDL_FLIP_NONE, layerIterator->data->speed);
						
					}
				}
			}

		}
	}
}

iPoint j1Map::MapToWorld(int column, int row) const
{
	iPoint retVec(0, 0);

	switch (data.type) {
	case MapTypes::MAPTYPE_ORTHOGONAL:
		retVec.x = column * data.tile_width;
		retVec.y = row * data.tile_height;
		break;
	case MapTypes::MAPTYPE_ISOMETRIC:
		retVec.x = (column - row) * data.tile_width * 0.5f;
		retVec.y = (column + row) * data.tile_height * 0.5f;
		break;
	default:
		LOG("ERROR: Map type not identified.");
		break;
	}

	return retVec;
}


iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint retVec(0, 0);
	
	switch (data.type) {
	case MapTypes::MAPTYPE_ORTHOGONAL:
		retVec.x = x / data.tile_width;
		retVec.y = y / data.tile_height;
		break;
	case MapTypes::MAPTYPE_ISOMETRIC:
		retVec.y = y / data.tile_height - x / data.tile_width;
		retVec.x = (2 * x) / data.tile_width + retVec.y;
		break;
	default:
		LOG("ERROR: Map type not set.");
		break;
	}
	return retVec;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect = { 0, 0, 0, 0 };

	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % columns));
	rect.y = margin + ((rect.h + spacing) * (relative_id / columns));
	return rect;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
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
// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while (item != data.tilesets.end)
	{
		App->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = data.mapLayers.start;

	while (item2 != data.mapLayers.end)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.mapLayers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
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

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.mapLayers.add(lay);
	}

	pugi::xml_node object;
	for (object = map_file.child("map").child("objectgroup"); object && ret; object = object.next_sibling("objectgroup"))
	{
		ret = LoadObjects(object);

	}

	if (ret == true)
	{
		p2List_item<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.mapLayers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.columns = map.attribute("width").as_int();
		data.rows = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MapTypes::MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MapTypes::MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MapTypes::MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MapTypes::MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
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
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->columns = set->tex_width / set->tile_width;
		set->rows = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->columns = node.attribute("width").as_int();
	layer->rows = node.attribute("height").as_int();
	LoadProperties(node, *layer);
	pugi::xml_node layer_data = node.child("data");

	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->tileArray = new uint[layer->columns*layer->rows];
		memset(layer->tileArray, 0, layer->columns*layer->rows);

		int i = 0;
		for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->tileArray[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

bool j1Map::LoadProperties(pugi::xml_node& node, MapLayer& layer)
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

			if(strcmp(p->name.GetString(), "speed") == 0)
				layer.speed = prop.attribute("value").as_float();
			layer.properties.list.add(p);
		}
	}

	return ret;
}

bool j1Map::LoadObjects(pugi::xml_node & node)
{
	bool ret = true;
	if (node == NULL)
		LOG("Error");
	p2SString name(node.attribute("name").as_string());
	for (pugi::xml_node obj = node.child("object"); obj && ret; obj = obj.next_sibling("object"))
	{
		if (name == "Colliders")
		{
			pugi::xml_node properties = obj.child("properties").child("property");
			bool is_win = properties.attribute("value").as_bool();
			App->collisions->AddCollider({ obj.attribute("x").as_int(),obj.attribute("y").as_int() ,obj.attribute("width").as_int() ,obj.attribute("height").as_int() }, COLLIDER_TYPE::COLLIDER_WALL);
		}

		else if (name == "MapDamage")
		{

			App->collisions->AddCollider({ obj.attribute("x").as_int(),obj.attribute("y").as_int() ,obj.attribute("width").as_int() ,obj.attribute("height").as_int() }, COLLIDER_TYPE::COLLIDER_ENEMY);
		}

		else if (name == "CheckPoint")
		{
			pugi::xml_node properties = obj.child("properties").child("property");

			p2SString check_point_type = properties.attribute("name").as_string();

			bool value = properties.attribute("value").as_bool();

			if (check_point_type == "win" && value)
				App->collisions->AddCollider({ obj.attribute("x").as_int(),obj.attribute("y").as_int() ,obj.attribute("width").as_int() ,obj.attribute("height").as_int() }, COLLIDER_TYPE::COLLIDER_WIN);
			else if (check_point_type == "first" && value)
			{
				App->entityManager->player = App->entityManager->CreateEntity({ obj.attribute("x").as_float(),obj.attribute("y").as_float() }, ENTITY_TYPE::PLAYER);
				App->scene->scene_spawn = { obj.attribute("x").as_float(),obj.attribute("y").as_float() };
				App->render->camera.x = -App->entityManager->player->position.x;
				App->render->camera.y = -App->entityManager->player->position.y + App->render->camera.h;
			}
			else
			{
				App->collisions->AddCollider({ obj.attribute("x").as_int(),obj.attribute("y").as_int() ,obj.attribute("width").as_int() ,obj.attribute("height").as_int() }, COLLIDER_TYPE::COLLIDER_CHECKPOINT);
			}

		}
		else if (name == "GroundEnemy")
		{
			App->entityManager->CreateEntity({ obj.attribute("x").as_float(),obj.attribute("y").as_float() }, ENTITY_TYPE::GROUND_ENEMY);
		}
		else if (name == "FlyingEnemy")
		{
			App->entityManager->CreateEntity({ obj.attribute("x").as_float(),obj.attribute("y").as_float() }, ENTITY_TYPE::FLYING_ENEMY);
		}
		else if (name == "Coins")
		{
			App->entityManager->CreateEntity({ obj.attribute("x").as_float(),obj.attribute("y").as_float() }, ENTITY_TYPE::COIN);
		}
	}
			

	return ret;
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	p2List_item<MapLayer*>* item;
	item = data.mapLayers.start;

	for (item = data.mapLayers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->columns * layer->rows];
		memset(map, 1, layer->columns * layer->rows);

		for (int y = 0; y < layer->columns * layer->rows; ++y)
		{
			if (layer->tileArray[y] == NULL)
				map[y] = 1;
			else
				map[y] = 0;
		}

		*buffer = map;
		width = data.columns;
		height = data.rows;
		ret = true;

		break;
	}
	return ret;
}

int Properties::Get(const char* name, int default_value) const
{
	p2List_item<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == name)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}
