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
	void CleanUp() override;

	void OnCollision(Collider*) override{}
	void PushBack() override;

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;

	void SaveDataXML(pugi::xml_node&)const;

	void Draw();


private:
	Animation idle;
	uint alpha = 80;
};

#endif