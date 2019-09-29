#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "Entity.h"

class Platform : public Entity
{
public:

	Platform(const fPoint&);
	~Platform();

	bool Start() override { return true; }
	void PreUpdate(float dt)override {};
	void Move(float dt) override {}
	void OnCollision(Collider*) override{}
	void Draw() override;

private:
	Animation idle;
};

#endif