#ifndef __J1ENTITYMANAGER_H__
#define __J1ENTITYMANAGER_H__

#include "j1Module.h"
#include "Entity.h"
#include "p2Point.h"
#include "p2List.h"
#include "p2DynArray.h"

enum class ENTITY_TYPE;

struct EntityData
{
	ENTITY_TYPE type = ENTITY_TYPE::NO_ENTITY;
	p2SString tsx_file;
};

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();
	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate(float dt) override;
	bool CleanUp();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;
public:

	Entity* CreateEntity(const fPoint &position, ENTITY_TYPE type);
	void DeleteEntity(Entity* entity);

	void OnCollision(Collider*, Collider*);

private:

	p2List<Entity*> entities;

public:
	EntityData queue[15];
	Entity* player = nullptr;
	int coin_sfx;
	int win_life_sfx;
	int player_damage_sfx;
};


#endif