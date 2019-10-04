#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "Player.h"
#include "Platform.h"
#include "p2Log.h"

j1EntityManager::j1EntityManager() : j1Module()
{
	name.create("entities");
}

j1EntityManager::~j1EntityManager()
{
}

bool j1EntityManager::Awake(pugi::xml_node& info)
{
	LOG("Loading TSX files");
	bool ret = true;

	// TSX of each enemy
	pugi::xml_node node = info.child("entity");
	for (int i = 0; node; node = node.next_sibling()) {
		queue[i].tsx_file.create(node.attribute("file").as_string());
		++i;
	}

	return ret;
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
		if (entityItem->data->to_delete == true)
		{
			delete[] entityItem;
			entityItem = nullptr;
		}
		else
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
	for (p2List_item<Entity*> *entityItem = entities.start; entityItem != nullptr; entityItem = entityItem->next) {
		if (entityItem != nullptr) {
			entityItem->data->Draw();
		}
	}
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
	else if (col2->type == COLLIDER_TYPE::COLLIDER_PLAYER)
	{
		dynamic_cast<Player*>(player)->OnCollision(col1);
	}
	else
		dynamic_cast<Player*>(player)->Fall();
}

bool j1EntityManager::Load(pugi::xml_node& file)
{
	bool ret = true;

	p2List_item<Entity*> *item = entities.start;
	for (; item != nullptr; item = item->next)
	{
		if (item != nullptr)
			item->data->Load(file);
	}

	return ret;
}

bool j1EntityManager::Save(pugi::xml_node& file) const
{
	bool ret = true;
	p2List_item<Entity*> *item = entities.start;
	for (; item != nullptr; item = item->next)
	{
		if(item != nullptr)
			item->data->Save(file);		
	}

	return ret;
}