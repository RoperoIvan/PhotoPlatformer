#include "Platform.h"
#include "j1App.h"
#include "j1Collisions.h"
#include "j1EntityManager.h"
#include "j1Render.h"

Platform::Platform(const fPoint & position): Entity(position,"platform")
{
	LoadData("platforms.tsx");

	data.tiled.texture = App->entityManager->player->data.tiled.texture;
	collider = App->collisions->AddCollider({ (int)position.x,(int)position.y + offset.y, size.x, size.y }, COLLIDER_TYPE::COLLIDER_WALL, (j1Module*)App->entityManager);
	current_animation = &anim_idle;
}

Platform::~Platform()
{
	if(collider != nullptr)
		collider->to_delete = true;

	data.tiled.texture = nullptr;

}

void Platform::PushBack()
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
			break;
		case EntityState::JUMP:			
			break;
		case EntityState::FALL:	
			break;
		case EntityState::DEAD:
			break;
		default:
			break;
		}
	}

	anim_idle.loop = false;
}

void Platform::Draw()
{
	if (current_animation != nullptr)
		App->render->Blit(data.tiled.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame());
}
