#include "j1App.h"
#include "FlyingEnemy.h"
#include "j1Collisions.h"
#include "j1Render.h"


FlyingEnemy::FlyingEnemy(const fPoint position) : Enemy(position, "FlyingEnemy")
{
	LoadData("FlyingEnemy.tsx");
	collider = App->collisions->AddCollider({ (int)position.x + offset.x,(int)position.y + offset.y, size.x, size.y }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->entityManager);
}

FlyingEnemy::~FlyingEnemy()
{
}

bool FlyingEnemy::Start()
{
	data.tiled.texture = App->tex->Load(data.tiled.image_path.GetString());
	return false;
}

void FlyingEnemy::OnCollision(Collider * c1)
{
}

void FlyingEnemy::IdAnimToEntityState()
{
	for (p2List_item<EntitiesAnim*> *anim = data.animations.start; anim != data.animations.end; anim = anim->next)
	{
		switch (anim->data->id)
		{
		case 0:
			anim->data->states = EntityState::IDLE;
			break;
		/*case 0:
			anim->data->states = EntityState::WALKING;
			break;
		case 12:
			anim->data->states = EntityState::DEAD;
			break;
		default:
			anim->data->states = EntityState::UNKNOWN;
			break;*/
		}
	}
}

void FlyingEnemy::Draw()
{
	if (current_animation != nullptr)
		App->render->Blit(data.tiled.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(), flip, 1.0F);
}

void FlyingEnemy::CleanUp()
{
}

