#include "Entity.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"

Entity::Entity(const fPoint &position):position(position)
{
}

Entity::~Entity()
{
}

bool Entity::LoadData(const char* ent_data)
{
	bool ret = true;

	pugi::xml_parse_result result = entity_data_file.load_file(ent_data);

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", ent_data, result.description());
		ret = false;
	}

	//Info data entity loading
	pugi::xml_node node = entity_data_file.child("tileset");
	data.tiled.name.create(node.attribute("name").as_string());
	data.tiled.tile_width = node.attribute("tilewidth").as_uint();
	data.tiled.tile_height = node.attribute("tileheight").as_uint();
	data.tiled.spacing = node.attribute("spacing").as_uint();
	data.tiled.margin = node.attribute("margin").as_uint();
	data.tiled.tile_count = node.attribute("tilecount").as_uint();
	data.tiled.columns = node.attribute("columns").as_uint();
	data.tiled.image_path = node.child("image").attribute("source").as_string();
	data.tiled.width = node.child("image").attribute("width").as_uint();
	data.tiled.height = node.child("image").attribute("height").as_uint();

	//Number of animations that has the entity
	
	/*data.num_animations = 0;
	while (node != NULL) {
		data.num_animations++;
		node = node.next_sibling("tile");
	}*/

	for(node = node.child("tile"); node; node = node.next_sibling("tile"))
	{
		pugi::xml_node anima = node.child("animation");
		EntitiesAnim* anim = new EntitiesAnim();
		
		uint id = node.attribute("id").as_uint();

		if (id == 0)
			anim->states = EntityState::IDLE;
		else if (id == 3)
			anim->states = EntityState::WALKING;
		else if (id == 7)
			anim->states = EntityState::JUMP;
		else if (id == 10)
			anim->states = EntityState::FALL;
		else if (id == 12)
			anim->states = EntityState::DEAD;

		for (pugi::xml_node frame = anima.child("frame"); frame; frame = frame.next_sibling("frame"))
		{
			SDL_Rect *rect = new SDL_Rect(data.tiled.GetTileRect(frame.attribute("tileid").as_uint()));
			anim->frames.add(rect);
		}

		data.animations.add(anim);
	}


	//node = entity_data_file.child("tileset").child("tile");
	//for (uint i = 0; i < data.num_animations; ++i) {
	//	
	//	data.animations[i].frames = new SDL_Rect[data.animations[i].num_frames];
	//	data.animations[i].id = node.attribute("id").as_uint();
	//	data.animations[i].type.create(node.attribute("type").as_string());
	//	node = node.next_sibling("tile");
	//}

	////We take the info from the entities and we save it inside each frame of each animation
	//node = entity_data_file.child("tileset").child("tile");
	//pugi::xml_node node_frame;
	//for (uint i = 0; i < data.num_animations; ++i) {	
	//	node_frame = node.child("animation").child("frame");
	//	for (uint j = 0; j < data.animations[i].num_frames; ++j) {
	//		data.animations[i].frames[j] = data.tiled.GetTileRect(node_frame.attribute("tileid").as_uint());
	//		node_frame = node_frame.next_sibling("frame");
	//	}
	//	node = node.next_sibling("tile");
	//}

	/*LoadProperties(entity_data_file.child("tileset").child("properties").child("property"));*/
	/*IdAnimToEntityState();*/
	PushBack();

	/*for (uint i = 0; i < data.num_animations; ++i) {		
		if (data.animations[i].frames != nullptr) {			
			delete[] data.animations[i].frames;				
			data.animations[i].frames = nullptr;
		}
	}
	if (data.animations != nullptr) {
		delete[] data.animations;
		data.animations = nullptr;
	}*/
	return ret;
}

void Entity::Draw()
{
	if (current_animation != nullptr)
		App->render->Blit(data.tiled.texture, position.x, position.y, &current_animation->frames[current_animation->GetCurrentFrameNumber()], flip, 1.0F, true);
	else
		App->render->Blit(data.tiled.texture, position.x, position.y);
}

void Entity::LoadProperties(pugi::xml_node & node)
{
	p2SString nameIdentificator;
	while (node) {
		nameIdentificator = node.attribute("name").as_string();

		if (nameIdentificator == "AnimationSpeed")
			anim_speed = node.attribute("value").as_float();

	}
}

void Entity::IdAnimToEntityState()
{
	data.animations.start->data->states = EntityState::IDLE;
}

SDL_Rect TileEntity::GetTileRect(int id) const {
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (id % columns));
	rect.y = margin + ((rect.h + spacing) * (id / columns));
	return rect;
}

//Return how many frames are in one animation
uint EntitiesAnim::FrameCount(pugi::xml_node& n) {
	num_frames = 0;
	pugi::xml_node node = n;
	for (; node != NULL; node = node.next_sibling("frame")) {
		num_frames++;
	}

	return num_frames;
}