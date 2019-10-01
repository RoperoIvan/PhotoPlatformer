#include "Player.h"
#include"j1App.h"
#include "j1Collisions.h"
#include "j1EntityManager.h"
#include "j1Input.h"
#include "j1FadetoBlack.h"
#include "j1Render.h"
#include "Entity.h"

#include "SDL/include/SDL_scancode.h"

Player::Player(const fPoint &position) : Entity(position)
{
	LoadData("Player.tsx");

	collider = App->collisions->AddCollider({ (int)position.x+ 2,(int)position.y + 38, 25, 26 }, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)App->entityManager);
	//put in config
	gravity = 0.1F;
	initialJumpSpeed = { 1,-0.5f };
	speed = { 0.5f,-0.2f };
	respawn = position;
}

Player::~Player()
{
	DeletePlatforms();
	if(collider != nullptr)
		collider->to_delete = true;

}

bool Player::Start()
{
	current_animation = &anim_idle;
	current_animation->speed = 2;
	return true;
}

void Player::PreUpdate(float dt)
{
	
	InPut();
	current_animation->GetCurrentFrame();
	
}

void Player::Move(float dt)
{
	Player_States checker = state;

	if (state == Player_States::fall_State)
	{
		gravity = 0.25F;

		if (speed.y == 0)
			jumpTime.Start();

		if (speed.y < 0)
			speed.y = -initialJumpSpeed.y;
		

		position.y += speed.y + (gravity*jumpTime.Read() / 1000);
	}
	if (state == Player_States::jump_State)
	{
		gravity = 0.25F;
		speed.y += (gravity*jumpTime.Read()/1000) * (gravity*jumpTime.Read() / 1000);

		if (speed.y == 0)
			state = Player_States::fall_State;

		position.y += speed.y;
	}
	

	if (state == Player_States::idle_State)
	{
		state = Player_States::fall_State;
	}

	if (state == Player_States::die_state)
	{
		DeletePlatforms();
		state = Player_States::fall_State;
	}		
		
	collider->SetPos((int)position.x + 6, (int)position.y + 38);

	if (checker == state)
	{
		ChangeAnim();
	}
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
	current_animation = &anim_idle;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		state = Player_States::jump_State;
		position.y -= initialJumpSpeed.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{		
		position.x += speed.x;
		current_animation = &anim_walking;
		if(flip != SDL_FLIP_HORIZONTAL)
			flip = SDL_FLIP_HORIZONTAL;	
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x += -speed.x;
		current_animation = &anim_walking;
		if(flip != SDL_FLIP_NONE)
			flip = SDL_FLIP_NONE;	
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
		if (collider->rect.x < col1->rect.x + col1->rect.w - 1 && collider->rect.x + collider->rect.w > col1->rect.x + 1)
		{
			if (collider->rect.y + collider->rect.h > col1->rect.y && collider->rect.y < col1->rect.y)
			{
				gravity = 0;
				speed.y = initialJumpSpeed.y;
				state = Player_States::idle_State;
				jumpTime.Start();
			}

			if (collider->rect.y < col1->rect.y + col1->rect.h && collider->rect.y + collider->rect.h > col1->rect.y + col1->rect.h)
			{
				state = Player_States::fall_State;
			}
		}
		//horitzontal collisions
		if (collider->rect.y < col1->rect.y + col1->rect.h - 1 && collider->rect.y + collider->rect.h > col1->rect.y + 1)
		{
			if (collider->rect.x < col1->rect.x + col1->rect.w && collider->rect.x + collider->rect.w > col1->rect.x + col1->rect.w)
			{
				position.x += speed.x;
			}

			if (collider->rect.x + collider->rect.w > col1->rect.x && collider->rect.x < col1->rect.x)
			{
				position.x -= speed.x;
			}
		}
	}
	else if (col1->type == COLLIDER_TYPE::COLLIDER_CHECKPOINT)
	{
		respawn = position;
		col1->to_delete = true;
	}
	else if (col1->type == COLLIDER_TYPE::COLLIDER_ENEMY)
	{
		position = respawn;
		state = Player_States::die_state;
	}

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
				break;
			case EntityState::WALKING:
				for (frames_item = animation->data->frames.start; frames_item != nullptr; frames_item = frames_item->next)
					anim_walking.PushBack(*(frames_item)->data);
				break;
			case EntityState::JUMP:
				for (frames_item = animation->data->frames.start; frames_item != nullptr; frames_item = frames_item->next)
					anim_jump.PushBack(*(frames_item)->data);
				break;
			case EntityState::FALL:
				for (frames_item = animation->data->frames.start; frames_item != nullptr; frames_item = frames_item->next)
					anim_fall.PushBack(*(frames_item)->data);
				break;
			case EntityState::DEAD:
				for (frames_item = animation->data->frames.start; frames_item != nullptr; frames_item = frames_item->next)
					anim_death.PushBack(*(frames_item)->data);
				break;
			default:
				break;
			}
		
	}

	anim_jump.loop = false;
	anim_fall.loop = false;
	anim_death.loop = false;

	//anim_idle.speed = 2.0F;
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
		current_animation->Reset();
		break;
	case Player_States::fall_State:
		current_animation = &anim_fall;
		break;
	case Player_States::die_state:
		current_animation = &anim_death;
		current_animation->Reset();
		break;
	default:
		break;
	}
}

//void Player::LoadProperties(pugi::xml_node& node)
//{
//	p2SString nameIdentificator;
//	while (node) {
//		nameIdentificator = node.attribute("name").as_string();
//
//		if (nameIdentificator == "animationSpeed")
//			animationSpeed = node.attribute("value").as_float();
//
//		else if (nameIdentificator == "incrementSpeedX")
//			incrementSpeedX = node.attribute("value").as_float();
//
//		else if (nameIdentificator == "jumpSpeed")
//			jumpSpeed = node.attribute("value").as_float();
//
//		else if (nameIdentificator == "maxSpeedX")
//			maxSpeedX = node.attribute("value").as_float();
//
//		node = node.next_sibling();
//	}
//}

