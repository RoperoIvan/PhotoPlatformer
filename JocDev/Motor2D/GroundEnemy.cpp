#include "j1App.h"
#include "GroundEnemy.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"

GroundEnemy::GroundEnemy(const fPoint position) : Enemy(position, "GroundEnemy")
{
	LoadData("GroundEnemy.tsx");
	collider = App->collisions->AddCollider({ (int)position.x + offset.x,(int)position.y + offset.y, size.x, size.y }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->entityManager);
}

GroundEnemy::~GroundEnemy()
{
}

bool GroundEnemy::Start()
{
	data.tiled.texture = App->tex->Load(data.tiled.image_path.GetString());
	return false;
}

void GroundEnemy::OnCollision(Collider * c1)
{
}

void GroundEnemy::IdAnimToEntityState()
{
	for (p2List_item<EntitiesAnim*> *anim = data.animations.start; anim != data.animations.end; anim = anim->next)
	{
		switch (anim->data->id)
		{
		case 4:
			anim->data->states = EntityState::IDLE;
			break;
		case 5:
			anim->data->states = EntityState::WALKING;
			break;
		/*case 12:
			anim->data->states = EntityState::DEAD;
			break;*/
		default:
			anim->data->states = EntityState::UNKNOWN;
			break;
		}
	}
}

void GroundEnemy::Move(float dt)
{
	if (position.DistanceManhattan(App->entityManager->player->position) <= 10)
	{
		App->pathfinding->CreatePath(iPoint(position.x,position.y), iPoint(App->entityManager->player->position.x, App->entityManager->player->position.y), ENTITY_TYPE::FLYING_ENEMY);
	}
}

void GroundEnemy::Draw()
{
	if (current_animation != nullptr)
		App->render->Blit(data.tiled.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(), flip, 1.0F);
}

void GroundEnemy::CleanUp()
{
	App->tex->UnLoad(data.tiled.texture);
	if (collider != nullptr)
		collider->to_delete = true;
	current_animation = nullptr;
}

