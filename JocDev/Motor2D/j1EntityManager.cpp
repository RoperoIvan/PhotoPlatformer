#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "Player.h"
#include "Platform.h"
#include "FlyingEnemy.h"
#include "GroundEnemy.h"
#include "Coin.h"
#include "p2Log.h"
#include"j1Audio.h"

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

	coin_sfx = App->audio->LoadFx("audio/fx/coin_fx.wav");
	win_life_sfx = App->audio->LoadFx("audio/fx/win_life_fx.wav");
	return true;
}

bool j1EntityManager::PreUpdate(float dt)
{
	bool ret = true;
	p2List_item<Entity*> *entityItem = entities.start;
	while ( entityItem != nullptr)
	{
		if (entityItem->data->to_delete == true)
		{
			entityItem->data->CleanUp();
			entities.del(entityItem);
		}
		else
			entityItem->data->PreUpdate(dt);
		
		entityItem = entityItem->next;
	
	}

	return ret;
}

bool j1EntityManager::Update(float dt)
{
	bool ret = true;
	App->entityManager->player;
	for (p2List_item<Entity*> *entityItem = entities.start; entityItem != nullptr; entityItem = entityItem->next)
	{
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
	return ret;
}

bool j1EntityManager::CleanUp()
{
	p2List_item<Entity*> *entityItem = entities.start;
	while (entityItem != NULL)
	{
		entityItem->data->CleanUp();
		delete entityItem->data;
		entityItem->data = nullptr;
		entityItem = entityItem->next;
	}
	entities.clear();
	/*delete player;
	player = nullptr;*/
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
	case ENTITY_TYPE::FLYING_ENEMY:
		entity = new FlyingEnemy(position);
		break;
	case ENTITY_TYPE::GROUND_ENEMY:
		entity = new GroundEnemy(position);
		break;
	case ENTITY_TYPE::COIN:
		entity = new Coin(position);
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
	for (p2List_item<Entity*>* entityItem = entities.start; entityItem != nullptr; entityItem = entityItem->next)
	{
		if (entityItem != nullptr && entityItem->data->collider == col1)
		{
			if (col1->type == COLLIDER_TYPE::COLLIDER_PLAYER && entityItem->data->type == ENTITY_TYPE::PLAYER)
			{
				entityItem->data->OnCollision(col2);
			}
			if (col1->type == COLLIDER_TYPE::COLLIDER_ENEMY && (entityItem->data->type == ENTITY_TYPE::FLYING_ENEMY || entityItem->data->type == ENTITY_TYPE::GROUND_ENEMY))
			{
				entityItem->data->OnCollision(col2);
			}
			if (col1->type == COLLIDER_TYPE::COLLIDER_COIN && entityItem->data->type == ENTITY_TYPE::COIN )
			{
				entityItem->data->OnCollision(col2);
			}
		}
	}
}

bool j1EntityManager::Load(pugi::xml_node& file)
{
	bool ret = true;

	p2List_item<Entity*> *item = entities.start;
	while (item != nullptr)
	{
		if (item->data == player)
			item->data->Load(file);
		else
			item->data->to_delete = true;
		item = item->next;
	}
	for (pugi::xml_node enemy_stats = file.child("enemy_stats"); enemy_stats != nullptr; enemy_stats = enemy_stats.next_sibling("enemy_stats"))
	{
		Entity* ent = CreateEntity(fPoint(enemy_stats.attribute("position_x").as_float(), enemy_stats.attribute("position_y").as_float()), ENTITY_TYPE(enemy_stats.attribute("type").as_int()));
		ent->Start();
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