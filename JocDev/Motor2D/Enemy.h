#ifndef _ENEMY_H
#define _ENEMY_H

#include "Entity.h"
#include "j1Pathfinding.h"
#include "p2Point.h"

enum class EnemyState {
	IDLE,
	WALKING,
	HIT,
	SAW,
	DEAD,
	NONE,
};

class Enemy :public Entity
{
public:
	Enemy(const fPoint position, const char* name);
	~Enemy();

public:
	bool Update(float dt);

public:
	//void LoadProperties(pugi::xml_node&);
	void PushBack();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:
	EnemyState state;
	Animation anim_idle;
	Animation anim_walking;
	Animation anim_dying;

};











#endif 
