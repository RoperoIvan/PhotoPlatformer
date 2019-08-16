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
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

public:

	void CreateEntity(const fPoint &position);
	void DeleteEntity(Entity* entity);

private:

	p2List<Entity*> entities;
	p2List<Entity*> interactive_entities;
};


#endif