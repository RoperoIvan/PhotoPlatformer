#include "Player.h"
#include"j1App.h"
#include "j1Collisions.h"
#include "j1EntityManager.h"
#include "j1Input.h"
#include "j1FadetoBlack.h"
#include "j1Render.h"
#include "Entity.h"

#include "SDL/include/SDL_scancode.h"

Player::Player(const fPoint &position) : Entity(position,"player")
{
	LoadData("Player.tsx");
	collider = App->collisions->AddCollider({ (int)position.x+ offset.x,(int)position.y + offset.y, size.x, size.y }, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)App->entityManager);
	//put in config

	initialJumpSpeed = -speed.y;

	grav = gravity;
	respawn = position;

	anim_jump.speed = 0.1F;
	anim_death.speed = 0.2F;
	anim_fall.speed = 0.1F;
	anim_idle.speed = 0.07F;
	anim_walking.speed = 0.1F;
}

Player::~Player()
{
	DeletePlatforms();
	if(collider != nullptr)
		collider->to_delete = true;

}

bool Player::Start()
{
	current_animation = &anim_jump;
	return true;
}

void Player::PreUpdate(float dt)
{
	InPut();
}

void Player::Move(float dt)
{
	if (state == Player_States::fall_State)
	{
		gravity = grav;

		if (speed.y == 0)
			jumpTime.Start();

		if (speed.y < 0)
			speed.y = -initialJumpSpeed;
		

		position.y += speed.y + (gravity*jumpTime.Read() / 1000);
	}
	if (state == Player_States::jump_State)
	{
		gravity = grav;
		speed.y += (gravity*jumpTime.Read()/10000) * (gravity*jumpTime.Read() / 10000);

		if (speed.y >= 0)
			state = Player_States::fall_State;

		position.y += speed.y;
	}

	if (state == Player_States::die_state && anim_death.Done())
	{
		position = respawn;
		state = Player_States::fall_State;
	}		

	if (state != check_state)
	{
		ChangeAnim();
		check_state = state;
	}
	
	collider->SetPos(position.x + offset.x, position.y + offset.y);

}

void Player::Draw()
{
	if (current_animation != nullptr)
	App->render->Blit(data.tiled.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(), flip, 1.0F);

}

bool Player::Load(pugi::xml_node& node)
{
	bool ret = true;

	pugi::xml_node p_stats = node.child("stats");
	gravity = p_stats.attribute("gravity").as_float();
	position.x = p_stats.attribute("position_x").as_int();
	position.y = p_stats.attribute("position_y").as_int();

	return ret;
}

bool Player::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node p_stats = node.append_child("stats");
	p_stats.append_attribute("gravity") = (float)gravity;
	p_stats.append_attribute("position_x") = (int)position.x;
	p_stats.append_attribute("position_y") = (int)position.y;

	return ret;
}

void Player::Flash()
{
	App->fade->fadetoBlack(2.F);
	platforms.add(App->entityManager->CreateEntity(fPoint(collider->rect.x,collider->rect.y), ENTITY_TYPE::PLATFORM));
	position = respawn;
}

void Player::InPut()
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		state = Player_States::jump_State;
		position.y -= 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{		
		position.x += speed.x;
		if(flip != SDL_FLIP_HORIZONTAL)
			flip = SDL_FLIP_HORIZONTAL;	

		if (state == Player_States::idle_State)
			state = Player_States::walking_state;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x += -speed.x;
		if(flip != SDL_FLIP_NONE)
			flip = SDL_FLIP_NONE;	

		if (state == Player_States::idle_State)
			state = Player_States::walking_state;
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		Flash();
		state = Player_States::fall_State;
	}

}

void Player::DeletePlatforms()
{
	p2List_item<Entity*>* item = platforms.start;
	while (item != NULL)
	{
		item->data->collider->to_delete = true;
		item = item->next;
	}
	
	platforms.clear();
}

void Player::OnCollision(Collider *col1)
{
	if (col1->type == COLLIDER_TYPE::COLLIDER_WALL)
	{
		//vertical collisions
		if (collider->rect.x < col1->rect.x + col1->rect.w - 5 && collider->rect.x + collider->rect.w > col1->rect.x + 5)
		{
			if (collider->rect.y + collider->rect.h > col1->rect.y && collider->rect.y < col1->rect.y)
			{
				gravity = 0;
				speed.y = initialJumpSpeed;
				state = Player_States::idle_State;
				jumpTime.Start();
			}

			else if (collider->rect.y < col1->rect.y + col1->rect.h && collider->rect.y + collider->rect.h > col1->rect.y + col1->rect.h)
			{
				state = Player_States::fall_State;
			}
		}
		
		//horitzontal collisions
		else if (collider->rect.y < col1->rect.y + col1->rect.h - 5 && collider->rect.y + collider->rect.h > col1->rect.y + 5)
		{
			if (collider->rect.x < col1->rect.x + col1->rect.w && collider->rect.x + collider->rect.w > col1->rect.x + col1->rect.w)
			{
				position.x += speed.x;
			}

			else if (collider->rect.x + collider->rect.w > col1->rect.x && collider->rect.x < col1->rect.x)
			{
				position.x -= speed.x;
			}
		}
		else
		{
			state = Player_States::fall_State;
		}
	}
	else if (col1->type == COLLIDER_TYPE::COLLIDER_CHECKPOINT)
	{
		respawn = position;
		col1->to_delete = true;
	}
	else if (col1->type == COLLIDER_TYPE::COLLIDER_ENEMY)
	{
		state = Player_States::die_state;
	}

}

void Player::Fall()
{
	state = Player_States::fall_State;
}

void Player::PushBack()
{
	for(p2List_item<EntitiesAnim*> *animation = data.animations.start; animation != nullptr; animation = animation->next)
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
			case EntityState::JUMP:
				for (frames_item = animation->data->frames.start; frames_item != nullptr; frames_item = frames_item->next)
					anim_jump.PushBack(*(frames_item)->data);
				anim_jump.speed = animation->data->speed;

				break;
			case EntityState::FALL:
				for (frames_item = animation->data->frames.start; frames_item != nullptr; frames_item = frames_item->next)
					anim_fall.PushBack(*(frames_item)->data);
				anim_fall.speed = animation->data->speed;
				break;
			case EntityState::DEAD:
				for (frames_item = animation->data->frames.start; frames_item != nullptr; frames_item = frames_item->next)
					anim_death.PushBack(*(frames_item)->data);
				anim_death.speed = animation->data->speed;
				break;
			default:
				break;
			}	
	}

	anim_death.loop = false;
}

void Player::IdAnimToEntityState()
{
	for (p2List_item<EntitiesAnim*> *anim = data.animations.start; anim != data.animations.end; anim = anim->next)
	{
		switch (anim->data->id) 
		{
		case 0:
			anim->data->states = EntityState::IDLE;
			break;
		case 3:
			anim->data->states = EntityState::WALKING;
			break;
		case 7:
			anim->data->states = EntityState::JUMP;
			break;
		case 10:
			anim->data->states = EntityState::FALL;
			break;
		case 12:
			anim->data->states = EntityState::DEAD;
			break;
		default:
			anim->data->states = EntityState::UNKNOWN;
			break;
		}
	}
}

void Player::ChangeAnim()
{
	switch (state)
	{
	case Player_States::idle_State:
		current_animation = &anim_idle;
		break;
	case Player_States::jump_State:
		current_animation = &anim_jump;
		break;
	case Player_States::fall_State:
		current_animation = &anim_jump;
		break;
	case Player_States::die_state:
		anim_death.Reset();
		current_animation = &anim_death;
		break;
	case Player_States::walking_state:
		current_animation = &anim_walking;
	default:
		break;
	}
}
