#ifndef __j1COLLISIONS_H__
#define __j1COLLISIONS_H__

#define MAX_COLLIDERS 300

#include "j1Module.h"

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_PLAYER,
	COLLIDER_CHECKPOINT,
	COLLIDER_WIN,
	COLLIDER_DAMAGE,
	COLLIDER_ENEMY,
	COLLIDER_COIN,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;
	float damage = 1.0;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(float x, float y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class j1Collisions : public j1Module
{
public:

	j1Collisions();
	~j1Collisions();

	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool CleanUp() override;
	void GodMode();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	void DebugDraw();

public:

	Collider * colliders[MAX_COLLIDERS];
	bool debug = false;
	bool god_mode = false;
private:

	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
};

#endif