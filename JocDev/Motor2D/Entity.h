#ifndef __ENTITY_H__
#define __ENTITY_H__

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

enum class EntityState {
	IDLE,
	WALKING,
	DEAD,
	JUMP,
	FALL,
	LAND,
	UNKNOWN
};

struct EntitiesAnim {
	int id = 0;
	p2SString type;
	uint num_frames = 0;
	p2List<SDL_Rect*> frames;
	EntityState states = EntityState::UNKNOWN;
	uint FrameCount(pugi::xml_node&);
};

struct EntityInfo {
	TileEntity tiled;
	p2List<EntitiesAnim*> animations;
	uint num_animations = 0;
};

enum class ENTITY_TYPE
{
	NO_ENTITY,
	PLAYER,
	PLATFORM
};

class Entity
{
public:

	Entity(const fPoint &position);
	~Entity();

	virtual bool Start() = 0;
	virtual void PreUpdate(float dt) = 0;
	virtual void Move(float dt) = 0;
	virtual void Draw();
	virtual void OnCollision(Collider*) = 0;
	virtual void LoadProperties(pugi::xml_node&);
	virtual void IdAnimToEntityState();
	virtual void PushBack() = 0;
	bool LoadData(const char*);

public:
	Collider* collider = nullptr;
	SDL_Texture* texture = nullptr;
	pugi::xml_document	entity_data_file;
	fPoint position;
	fPoint speed;
	EntityInfo data;
	SDL_RendererFlip flip = (SDL_RendererFlip)SDL_FLIP_NONE;
	float anim_speed;
	Animation anim_idle;
	Animation* current_animation = nullptr;
	bool to_delete = false;
};

#endif
