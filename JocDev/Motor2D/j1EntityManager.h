#ifndef __J1ENTITYMANAGER_H__
#define __J1ENTITYMANAGER_H__

#include "j1Module.h"
#include "Entity.h"
#include "p2Point.h"
#include "p2List.h"

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Start();
	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate(float dt) override;
	bool CleanUp();

public:

	Entity* CreateEntity(const fPoint &position, ENTITY_TYPE type);
	void DeleteEntity(Entity* entity);

	void OnCollision(Collider*, Collider*);

private:

	p2List<Entity*> entities;
	p2List<Entity*> interactive_entities;

public:

	Entity* player = nullptr;
};


#endif