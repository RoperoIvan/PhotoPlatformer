#ifndef __COIN_H__
#define __COIN_H__

#include "Entity.h"

class Coin :public Entity {
public:
	Coin(const fPoint&);
	~Coin();

	bool Start() override { return true; }
	void PreUpdate(float dt)override {};
	void Move(float dt) override;
	void CleanUp() override;
	void Draw();
	void OnCollision(Collider*) override;
	void PushBack() override;

};
#endif // !__COIN_H__
