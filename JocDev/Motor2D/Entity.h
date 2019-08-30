#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "j1Textures.h"
#include "Animation.h"

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
	virtual void Move(float dt) = 0;
	virtual void Draw() = 0;
	virtual void OnCollision(Collider*) = 0;

public:
	Collider* collider = nullptr;
	SDL_Texture* texture = nullptr;
	Animation* currentAnimation = nullptr;

	fPoint position;
	fPoint speed;
};

#endif
