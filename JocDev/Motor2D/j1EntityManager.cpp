#include "j1EntityManager.h"

j1EntityManager::j1EntityManager()
{
}

j1EntityManager::~j1EntityManager()
{
}

bool j1EntityManager::Start()
{
	return true;
}

bool j1EntityManager::PreUpdate()
{
	bool ret = true;
	for (p2List_item<Entity*> *entityItem = entities.start; entityItem != entities.end; entityItem = entityItem->next)
	{

	}
	return ret;
}

bool j1EntityManager::Update()
{
	return true;
}

bool j1EntityManager::PostUpdate()
{
	return true;
}

bool j1EntityManager::CleanUp()
{
	return true;
}

void j1EntityManager::CreateEntity(const fPoint & position)
{
}

void j1EntityManager::DeleteEntity(Entity * entity)
{
}
