#include "Enemy.h"



Enemy::Enemy(const fPoint position, const char* name) : Entity(position, name)
{
	current_animation = &anim_idle;
}

Enemy::~Enemy()
{

}

bool Enemy::Update(float dt)
{
	return false;
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
	return false;
}

bool Enemy::Save(pugi::xml_node &) const
{
	return false;
}

