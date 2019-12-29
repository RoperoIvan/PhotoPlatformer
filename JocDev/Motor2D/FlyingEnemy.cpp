#include "j1App.h"
#include "FlyingEnemy.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1EntityManager.h"

FlyingEnemy::FlyingEnemy(const fPoint position) : Enemy(position, "FlyingEnemy",ENTITY_TYPE::FLYING_ENEMY)
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
	initial_pos = position;
	move_radius = 150;
	return true;
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

void FlyingEnemy::Move(float dt)
{
	if (state == EnemyState::IDLE)
	{
		fPoint direction;
		iPoint enemy_pos = App->map->WorldToMap(position.x + offset.x, position.y + offset.y);
		if (position.DistanceManhattan(App->entityManager->player->position) <= search)
		{
			iPoint player_pos = App->map->WorldToMap(App->entityManager->player->position.x + App->entityManager->player->size.x / 2, App->entityManager->player->position.y + App->entityManager->player->size.y);


			if (App->pathfinding->CreatePath(enemy_pos, player_pos, ENTITY_TYPE::FLYING_ENEMY) != -1 && App->entityManager->player)
			{
				enemy_path = App->pathfinding->GetLastPath();
				if(App->collisions->debug)
					App->pathfinding->DrawPath(enemy_path);

				if (enemy_path->Count() > 0)
				{
					fPoint next_node(enemy_path->At(0)->x, enemy_path->At(0)->y);

					direction.create(next_node.x - enemy_pos.x, next_node.y - enemy_pos.y);
					direction.x *= path_speed;
					direction.y *= path_speed;
					position.x += direction.x * speed.x * dt;
					position.y += direction.y * speed.y * dt;
				}
			}
			if (direction.x > 0)
			{
				flip = SDL_RendererFlip::SDL_FLIP_NONE;
			}
			else if (direction.x < 0)
			{
				flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
			}
			
		}

		else
		{
			fPoint objective;
			iPoint enemy_size_pos = App->map->WorldToMap(position.x + size.x, position.y + size.y);

			iPoint cell_right(enemy_size_pos.x + 2, enemy_size_pos.y);
			iPoint cell_left(enemy_pos.x - 2, enemy_size_pos.y);
			iPoint cell_down(enemy_size_pos.x, enemy_size_pos.y + 2);
			iPoint cell_up(enemy_pos.x, enemy_size_pos.y - 2);

			if (position.DistanceTo(initial_pos) > move_radius)
			{
				if (initial_pos.x < position.x || !App->pathfinding->IsWalkable(cell_right))
				{
					go_right = false;
					flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
				}
				else if(initial_pos.x > position.x || !App->pathfinding->IsWalkable(cell_left))
				{
					go_right = true;
					flip = SDL_RendererFlip::SDL_FLIP_NONE;
				}

				if (initial_pos.y < position.y || !App->pathfinding->IsWalkable(cell_up))
				{
					objective.create(enemy_pos.x - path_speed, enemy_pos.y);
				}
				else if (initial_pos.y > position.y || !App->pathfinding->IsWalkable(cell_down))
				{
				objective.create(enemy_pos.x + path_speed, enemy_pos.y);
				}
			}
			if (go_right)
				objective.create(enemy_pos.x + path_speed, enemy_pos.y);
			else
				objective.create(enemy_pos.x - path_speed, enemy_pos.y);


			if (objective.x != 0)
			{
				direction.create(objective.x - enemy_pos.x, objective.y - enemy_pos.y);
				position.x += direction.x * speed.x * dt;
				position.y += direction.y * speed.y * dt;
			}
		}
		
		
	}
	else if (state == EnemyState::DEAD)
	{
		position.y += gravity * dt;
		flip = SDL_RendererFlip::SDL_FLIP_VERTICAL;
		current_animation->speed = 0.0f;
		if (!App->render->IsOnCamera(position.x, position.y, size.x, size.y))
			to_delete = true;
	}
	collider->SetPos((int)position.x + offset.x, (int)position.y + offset.y);
}

void FlyingEnemy::Draw()
{
	if (current_animation != nullptr)
		App->render->Blit(data.tiled.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(), true ,flip, 1.0F);
}

void FlyingEnemy::CleanUp()
{
	App->tex->UnLoad(data.tiled.texture);
	if (collider != nullptr)
		collider->to_delete = true;
	current_animation = nullptr;
}

