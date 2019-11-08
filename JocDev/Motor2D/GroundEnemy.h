#ifndef _GROUNDENEMY_
#define _GROUNDENEMY_

#include "Enemy.h"

class GroundEnemy : public Enemy
{
public:
	GroundEnemy(const fPoint position);
	~GroundEnemy();
	bool Start() override;
	void PreUpdate(float dt)override {};
	void OnCollision(Collider* c1)override;
	void IdAnimToEntityState();
	void Move(float dt) override;
	void Draw();
	void CleanUp();
};











#endif
