#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "Player.h"
#include "Platform.h"

j1EntityManager::j1EntityManager()
{
	name.create("entities");
}

j1EntityManager::~j1EntityManager()
{
}

bool j1EntityManager::Start()
{
	for (p2List_item<Entity*> *entityItem = entities.start; entityItem != nullptr; entityItem = entityItem->next)
	{		
		entityItem->data->Start();
	}
	return true;
}

bool j1EntityManager::PreUpdate(float dt)
{
	bool ret = true;
	for (p2List_item<Entity*> *entityItem = entities.start; entityItem != nullptr; entityItem = entityItem->next)
	{
		
		entityItem->data->PreUpdate(dt);
	}

	return ret;
}

bool j1EntityManager::Update(float dt)
{
	bool ret = true;
	for (p2List_item<Entity*> *entityItem = entities.start; entityItem != nullptr; entityItem = entityItem->next)
	{
		if (App->render->IsOnCamera(entityItem->data->collider->rect.x, entityItem->data->collider->rect.y, entityItem->data->collider->rect.w, entityItem->data->collider->rect.h))
		{
			interactive_entities.add(entityItem->data);
		}
		entityItem->data->Move(dt);
	}
	return ret;
}

bool j1EntityManager::PostUpdate(float dt)
{
	bool ret = true;
	
	interactive_entities.clear();
	return ret;
}

bool j1EntityManager::CleanUp()
{
	p2List_item<Entity*> *entityItem = entities.start;

	while (entityItem != NULL)
	{
		RELEASE(entityItem->data);
		entityItem = entityItem->next;
	}
	entities.clear();
	interactive_entities.clear();
	return true;
}

Entity* j1EntityManager::CreateEntity(const fPoint & position, ENTITY_TYPE type)
{
	Entity* entity = nullptr;
	switch (type)
	{
	case ENTITY_TYPE::NO_ENTITY:
		break;
	case ENTITY_TYPE::PLAYER:
		entity = new Player(position);
		break;
	case ENTITY_TYPE::PLATFORM:
		entity = new Platform(position);
		break;
	default:
		break;
	}
	if (entity != nullptr)
		entities.add(entity);
	return entity;
}

void j1EntityManager::DeleteEntity(Entity * entity)
{
}

void j1EntityManager::OnCollision(Collider *col1, Collider *col2)
{
	if (col1->type == COLLIDER_TYPE::COLLIDER_PLAYER)
	{
		dynamic_cast<Player*>(player)->OnCollision(col2);
	}
	else if(col2->type == COLLIDER_TYPE::COLLIDER_PLAYER)
		dynamic_cast<Player*>(player)->OnCollision(col1);
}
