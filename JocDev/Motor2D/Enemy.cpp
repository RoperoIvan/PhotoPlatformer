#include "Enemy.h"
#include "j1Collisions.h"
#include "j1Map.h"

Enemy::Enemy(const fPoint position, const char* name, ENTITY_TYPE type) : Entity(position, name,type)
{
	current_animation = &anim_idle;

	pugi::xml_document config_file;
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	ret = config_file.child("config").child("entities").child("GroundEnemy");
	search = ret.child("path").attribute("search").as_int();
	path_speed = ret.child("path").attribute("speed").as_float();
}

Enemy::~Enemy()
{

}

bool Enemy::Update(float dt)
{
	return true;
}

void Enemy::PushBack()
{
	for (p2List_item<EntitiesAnim*> *animation = data.animations.start; animation != nullptr; animation = animation->next)
	{
		p2List_item<SDL_Rect*> *frames_item;
		switch (animation->data->states)
		{
		case EntityState::IDLE:
			for (frames_item = animation->data->frames.start; frames_item != nullptr; frames_item = frames_item->next)
				anim_idle.PushBack(*(frames_item)->data);
			anim_idle.speed = animation->data->speed;
			break;
		case EntityState::WALKING:
			for (frames_item = animation->data->frames.start; frames_item != nullptr; frames_item = frames_item->next)
				anim_walking.PushBack(*(frames_item)->data);
			anim_walking.speed = animation->data->speed;
			break;
		case EntityState::DEAD:
			for (frames_item = animation->data->frames.start; frames_item != nullptr; frames_item = frames_item->next)
				anim_dying.PushBack(*(frames_item)->data);
			anim_dying.speed = animation->data->speed;
			break;
		default:
			break;
		}
	}
	anim_dying.loop = false;
}

bool Enemy::Load(pugi::xml_node &)
{
	return true;
}

bool Enemy::Save(pugi::xml_node &) const
{
	return true;
}

void Enemy::OnCollision(Collider* col1)
{
	if (col1->type == COLLIDER_TYPE::COLLIDER_WALL)
	{
		iPoint enemy_pos = App->map->WorldToMap(position.x, position.y);
		iPoint objective = App->map->WorldToMap(col1->rect.x, col1->rect.y);
		fPoint direction(enemy_pos.x - objective.x, enemy_pos.y - objective.y);

		//vertical collisions
		if (collider->rect.x < col1->rect.x + col1->rect.w - 5 && collider->rect.x + collider->rect.w > col1->rect.x + 5)
		{
			position.y -= gravity * App->GetDT();
			
		}

		//horitzontal collisions
		if (collider->rect.y < col1->rect.y + col1->rect.h - 5 && collider->rect.y + collider->rect.h > col1->rect.y + 5)
		{
			position.x += speed.x * direction.x * App->GetDT();
		}

	}

	if (col1->type == COLLIDER_TYPE::COLLIDER_PLAYER)
	{
		if (collider->rect.y < col1->rect.y + col1->rect.h && collider->rect.y + collider->rect.h > col1->rect.y + col1->rect.h)
		{
			state = EnemyState::DEAD;
			collider->to_delete = true;
		}
		else
		{
			enemy_path = nullptr;
		}
	}
}

