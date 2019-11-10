#include "j1App.h"
#include "GroundEnemy.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1Map.h"

GroundEnemy::GroundEnemy(const fPoint position) : Enemy(position, "GroundEnemy",ENTITY_TYPE::GROUND_ENEMY)
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
	if (state == EnemyState::IDLE)
	{
		fPoint direction;
		iPoint enemy_pos = App->map->WorldToMap(position.x + offset.x, position.y + offset.y);
		if (position.DistanceManhattan(App->entityManager->player->position) <= 150)
		{
			iPoint player_pos = App->map->WorldToMap(App->entityManager->player->position.x + App->entityManager->player->size.x / 2, App->entityManager->player->position.y + App->entityManager->player->size.y);


			if (App->pathfinding->CreatePath(enemy_pos, player_pos, ENTITY_TYPE::GROUND_ENEMY) != -1 && App->entityManager->player)
			{
				enemy_path = App->pathfinding->GetLastPath();
				App->pathfinding->DrawPath(enemy_path);

				if (enemy_path->Count() > 0)
				{
					fPoint next_node(enemy_path->At(0)->x, enemy_path->At(0)->y);

					direction.create(next_node.x - enemy_pos.x, next_node.y - enemy_pos.y);
				}
			}
		}

		else
		{
			fPoint objective;
			iPoint enemy_size_pos = App->map->WorldToMap(position.x + size.x, position.y + size.y);

			iPoint cell(enemy_size_pos.x + 1, enemy_size_pos.y + 2);
			iPoint cell1(enemy_size_pos.x + 2, enemy_size_pos.y);
			iPoint cell2(enemy_pos.x - 1, enemy_size_pos.y + 2);
			iPoint cell3(enemy_pos.x - 2, enemy_size_pos.y);

			//go right
			if (App->pathfinding->IsWalkable(cell) || !App->pathfinding->IsWalkable(cell1))
				go_right = false;
			//go left
			else if (App->pathfinding->IsWalkable(cell2) || !App->pathfinding->IsWalkable(cell3))
				go_right = true;

			if (go_right)
				objective.create(enemy_pos.x + 0.5, enemy_pos.y);
			else
				objective.create(enemy_pos.x - 0.5, enemy_pos.y);


			if (objective.x != 0)
			{
				direction.create(objective.x - enemy_pos.x, objective.y - enemy_pos.y);

			}
		}


		position.y += gravity * dt;
		position.x += direction.x * speed.x * dt;

		if (direction.x > 0)
		{
			current_animation = &anim_walking;
			flip = SDL_RendererFlip::SDL_FLIP_NONE;
		}
		else if (direction.x < 0)
		{
			current_animation = &anim_walking;
			flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
		}
		else
		{
			current_animation = &anim_idle;
		}
		collider->SetPos((int)position.x + offset.x, (int)position.y + offset.y);
	}
	else if (state == EnemyState::DEAD)
	{
		current_animation->speed = 0.0f;
		alpha -= 10;
		if (alpha < 20)
			to_delete = true;
	}
}

void GroundEnemy::Draw()
{
	if (current_animation != nullptr)
		App->render->Blit(data.tiled.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(), flip, 1.0F, alpha);
}

void GroundEnemy::CleanUp()
{
	App->tex->UnLoad(data.tiled.texture);
	if (collider != nullptr)
		collider->to_delete = true;
	current_animation = nullptr;
}

