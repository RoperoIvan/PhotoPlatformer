#ifndef _FLYINGENEMY_H
#define _FLYINGENEMY_H

#include "Enemy.h"

class FlyingEnemy :public Enemy
{
public:
	FlyingEnemy(const fPoint position);
	~FlyingEnemy();
	bool Start() override { return true; }
	void PreUpdate(float dt)override {};
	void OnCollision(Collider* c1)override;
	void IdAnimToEntityState();
	void Draw();
	void CleanUp();
};













#endif