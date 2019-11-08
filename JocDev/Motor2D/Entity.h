#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "j1App.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "Animation.h"
#include "SDL_image/include/SDL_image.h"

#include "p2List.h"

enum class COLLIDER_DIRECTION;

struct TileEntity {

	SDL_Rect GetTileRect(int id) const;

	p2SString name;
	uint tile_width = 0;
	uint tile_height = 0;
	uint spacing = 0;
	uint margin = 0;
	uint tile_count = 0;
	uint columns = 0;
	p2SString image_path;
	SDL_Texture* texture = nullptr;
	uint width = 0;
	uint height = 0;
};
struct PropertyEntity {
	//SFX
	p2SString jump_sfx;
	p2SString copy_sfx;
	p2SString death_sfx;
	p2SString respawn_sfx;
};

enum class EntityState {
	IDLE,
	WALKING,
	JUMP,
	FALL,
	DEAD,
	UNKNOWN
};

struct EntitiesAnim {
	int id = 0;
	p2SString type;
	uint num_frames = 0;
	p2List<SDL_Rect*> frames;
	EntityState states = EntityState::UNKNOWN;
	uint FrameCount(pugi::xml_node&);
	float speed = 0.F;

	~EntitiesAnim()
	{
		p2List_item<SDL_Rect*>* iter = frames.start;
		while (iter != frames.end)
		{
			RELEASE(iter->data);
			iter = iter->next;
		}
	}
};

struct EntityInfo {
	TileEntity tiled;
	PropertyEntity property;
	p2List<EntitiesAnim*> animations;
	uint num_animations = 0;

	~EntityInfo()
	{
		p2List_item<EntitiesAnim*>* iter = animations.start;
		while (iter != animations.end)
		{
			RELEASE(iter->data);
			iter = iter->next;
		}

		App->tex->UnLoad(tiled.texture);
		tiled.texture = nullptr;
	}
};

enum class ENTITY_TYPE
{
	NO_ENTITY,
	PLAYER,
	PLATFORM,
	FLYING_ENEMY,
	GROUND_ENEMY,
};

class Entity
{
public:

	Entity(const fPoint &position, const char* name);
	~Entity();

	virtual bool Start() = 0;
	virtual void PreUpdate(float dt) = 0;
	virtual void Move(float dt) = 0;
	virtual void Draw();
	virtual void CleanUp() = 0;

	virtual void OnCollision(Collider*) = 0;
	virtual void LoadProperties(pugi::xml_node&);
	virtual void IdAnimToEntityState();
	virtual void PushBack() = 0;
	virtual bool Load(pugi::xml_node&) { return true; };
	virtual bool Save(pugi::xml_node&) const { return true; };
	bool LoadData(const char*);
public:
	p2SString name;
	Collider* collider = nullptr;
	pugi::xml_document	entity_data_file;
	
	fPoint position;
	fPoint speed;
	iPoint size;
	iPoint offset;
	float gravity;

	EntityInfo data;
	SDL_RendererFlip flip = (SDL_RendererFlip)SDL_FLIP_NONE;
	float anim_speed;
	Animation anim_idle;
	Animation* current_animation = nullptr;
	bool to_delete = false;
};

#endif
