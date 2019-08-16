#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "j1Textures.h"
#include "Animation.h"

enum class ENTITY_TYPE
{
	NO_ENTITY,
	PLAYER
};

class Entity
{
public:

	Entity(fPoint position);
	~Entity();

	bool Start();
	void Move();
	void Draw();

public:
	Collider* collider = nullptr;
	SDL_Texture* texture = nullptr;
	Animation* currentAnimation = nullptr;

	fPoint position;
	fPoint speed;
};

#endif
