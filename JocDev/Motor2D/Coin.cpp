#include "Coin.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collisions.h"
Coin::Coin(const fPoint& pos) :Entity(pos, "coin", ENTITY_TYPE::COIN)
{
	LoadData("Coins.tsx");
	data.tiled.texture = App->tex->Load(data.tiled.image_path.GetString());
	collider = App->collisions->AddCollider({ (int)position.x,(int)position.y, 30, 30 }, COLLIDER_TYPE::COLLIDER_COIN, (j1Module*)App->entityManager);
	current_animation = &anim_idle;
}

Coin::~Coin()
{
}

void Coin::Move(float dt)
{
	current_animation->GetCurrentFrame();
}

void Coin::CleanUp()
{
	if (collider != nullptr)
		collider->to_delete = true;
	App->tex->UnLoad(data.tiled.texture);
	data.tiled.texture = nullptr;
}

void Coin::PushBack()
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

	anim_idle.loop = true;
}
void Coin::Draw()
{
	if (current_animation != nullptr)
		App->render->Blit(data.tiled.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(), true, SDL_FLIP_NONE, 1.F);
}

void Coin::OnCollision(Collider* col1)
{
	if (col1->type == COLLIDER_TYPE::COLLIDER_PLAYER)
	{
		/*collider->to_delete = true;*/
		this->to_delete = true;
	}
}
