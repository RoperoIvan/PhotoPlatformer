#include "Player.h"
#include"j1App.h"
#include "j1Collisions.h"
#include "j1EntityManager.h"
#include "j1Input.h"
#include "j1FadetoBlack.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "Entity.h"
#include "Platform.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "Button.h"
#include "Label.h"
#include "Image.h"
#include "CheckBox.h"
#include "Slider.h"
#include "InputBox.h"
#include "SDL/include/SDL_scancode.h"

Player::Player(const fPoint &position) : Entity(position,"player",ENTITY_TYPE::PLAYER)
{
	LoadData("Player.tsx");
	collider = App->collisions->AddCollider({ (int)position.x+ offset.x,(int)position.y + offset.y, size.x, size.y }, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)App->entityManager);
	//put in config

	

	grav = gravity;
	respawn = position;
}

Player::~Player()
{
	
}

bool Player::Start()
{
	data.tiled.texture = App->tex->Load(data.tiled.image_path.GetString());
	current_animation = &anim_jump;
	jump_sfx = App->audio->LoadFx(data.property.jump_sfx.GetString());
	copy_sfx = App->audio->LoadFx(data.property.copy_sfx.GetString());
	death_sfx = App->audio->LoadFx(data.property.death_sfx.GetString());
	respawn_sfx = App->audio->LoadFx(data.property.respawn_sfx.GetString());

	initialJumpSpeed = -speed.y;

	//HUD
	App->gui->CreateScreen();

	return true;
}

void Player::PreUpdate(float dt)
{
	if (hit_time)
		HitTimeManagement();
}

void Player::Move(float dt)
{
	if(state != Player_States::die_state)
		InPut(dt);

	if (state == Player_States::fall_State)
	{
		gravity = grav;
		/*if (speed.y == 0)*/
		jumpTime.Start();

		if (speed.y < -initialJumpSpeed)
			speed.y += gravity * 3.5;
		

		position.y += speed.y * dt;
	}
	if (state == Player_States::jump_State)
	{
		gravity = grav;
		if (speed.y > -5)
			state = Player_States::fall_State;
		else if (speed.y <= -initialJumpSpeed)
			speed.y += gravity;
		

		position.y += speed.y*dt ;
	}

	if (state == Player_States::die_state && anim_death.Done())
	{
		//App->audio->PlayFx(respawn_sfx);
		//position = respawn;
		//lifes = 3;
		anim_death.Reset();
		state = Player_States::fall_State;
		App->scene->DestroyPauseMenu();
		App->current_level = 0;
		App->fade->StartfadetoBlack();
	}		

	if (state == Player_States::god_mode_state)
	{
		if (!App->collisions->god_mode)
			state = Player_States::fall_State;
	}
	
	collider->SetPos(position.x + offset.x, position.y + offset.y);
	//for next iteration

	if (state != check_state)
	{
		ChangeAnim();
		check_state = state;
	}

	if (state == Player_States::walking_state || state == Player_States::idle_State)
	{
		state = Player_States::fall_State;
	}
	/*if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		heart_1->to_delete = true;
		App->gui->DeleteElement(heart_1);
		heart_1 = nullptr;
	}*/

	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
		coins++;

	RestartAlpha(restart_alpha);
	if (App->scene->heart_3)
	{
		ManageLifesHUD();
		CoinsManagement();
	}
	if (App->fade->IsFading())
	{
		position = respawn;
	}
}

void Player::Draw()
{
	if (current_animation != nullptr)
	App->render->Blit(data.tiled.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(), true ,flip, 1.0F,alpha);
}

void Player::CleanUp()
{
	DeletePlatforms();
	if (collider != nullptr)
		collider->to_delete = true;
	current_animation = nullptr;
	App->tex->UnLoad(data.tiled.texture);
	data.tiled.texture = nullptr;
}

bool Player::Load(pugi::xml_node& node)
{
	bool ret = true;

	pugi::xml_node p_stats = node.child("player_stats");
	gravity = p_stats.attribute("gravity").as_float();
	position.x = p_stats.attribute("position_x").as_int();
	position.y = p_stats.attribute("position_y").as_int();
	alpha = p_stats.attribute("alpha").as_int();

	DeletePlatforms();
	for (pugi::xml_node platform_stats = p_stats.child("platform_stats"); platform_stats != nullptr; platform_stats = platform_stats.next_sibling("platform_stats"))
	{
		Platform* p = dynamic_cast<Platform*>(App->entityManager->CreateEntity({ platform_stats.attribute("position_x").as_float(),platform_stats.attribute("position_y").as_float() }, ENTITY_TYPE::PLATFORM));
		p->SetAlpha(platform_stats.attribute("alpha").as_uint());
		platforms.add(p);
	}

	return ret;
}

bool Player::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node p_stats = node.append_child("player_stats");
	p_stats.append_attribute("gravity") = (float)gravity;
	p_stats.append_attribute("position_x") = (int)position.x;
	p_stats.append_attribute("position_y") = (int)position.y;
	p_stats.append_attribute("num_platforms") = (int)platforms.count();
	p_stats.append_attribute("alpha") = (int)alpha;

	for (p2List_item<Entity*>* iter = platforms.start; iter != NULL; iter = iter->next)
	{
		dynamic_cast<Platform*>(iter->data)->SaveDataXML(p_stats);
	}
	return ret;
}

void Player::ManageLifesHUD()
{
		switch (lifes)
		{
		case 0:
			state = Player_States::die_state;
			break;
		case 1:
			App->scene->heart_3->drawable = false;
			App->scene->heart_2->drawable = false;
			App->scene->heart_1->drawable = true;
			break;
		case 2:
			App->scene->heart_3->drawable = false;
			App->scene->heart_2->drawable = true;
			App->scene->heart_1->drawable = false;
			break;
		case 3:
			App->scene->heart_1->drawable = false;
			App->scene->heart_2->drawable = false;
			App->scene->heart_3->drawable = true;
			break;
		}			
}

void Player::RestartAlpha(bool& reset_alpha)
{
	if (alpha >= 255)
	{
		alpha = 255;
		reset_alpha = !reset_alpha;
	}
	else if(reset_alpha == true)
		alpha += 10;
}

void Player::HitTimeManagement()
{
	if (time_to_hit.ReadSec() >= 2)
	{
		hit_time = false;
	}
}

void Player::CoinsManagement()
{
	
	App->scene->coins_label->SetText(p2SString("%i", coins).GetString());

	if (lifes < 3 && coins >= 10)
	{
		lifes++;
		coins -= 10;
		App->scene->score -= 20;
		App->audio->PlayFx(App->entityManager->win_life_sfx);
	}
}

void Player::Flash()
{
	platforms.add(App->entityManager->CreateEntity(position, ENTITY_TYPE::PLATFORM));
	position = respawn;
	speed.y = -initialJumpSpeed/2;
}

void Player::InPut(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && !App->collisions->god_mode && is_on_platform)
	{
		speed.y = initialJumpSpeed;
		if (state == Player_States::idle_State)
			App->audio->PlayFx(jump_sfx);

		state = Player_States::jump_State;
		position.y -= initialJumpSpeed * dt;
	}
	is_on_platform = false;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->collisions->god_mode)
	{
		position.y -= speed.y * dt;
		state = Player_States::god_mode_state;
		jumpTime.Start();
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && App->collisions->god_mode)
	{
		position.y += speed.y * dt;
	}

	if (state != Player_States::die_state)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			position.x += speed.x * dt;
			if (flip != SDL_FLIP_HORIZONTAL)
				flip = SDL_FLIP_HORIZONTAL;

			if (state == Player_States::idle_State)
				state = Player_States::walking_state;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			position.x += -speed.x * dt;
			if (flip != SDL_FLIP_NONE)
				flip = SDL_FLIP_NONE;

			if (state == Player_States::idle_State)
				state = Player_States::walking_state;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && alpha >100)
	{
		alpha -= 20;
		Flash();
		position = respawn;
		state = Player_States::fall_State;
		App->audio->PlayFx(copy_sfx);
	}
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		restart_alpha = true;
		DeletePlatforms();
	}
}

void Player::DeletePlatforms()
{
	p2List_item<Entity*>* item = platforms.start;
	while (item != NULL)
	{
		item->data->collider->to_delete = true;
		item->data->to_delete = true;
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
			if (collider->rect.y + collider->rect.h > col1->rect.y && collider->rect.y < col1->rect.y && state != Player_States::jump_State && state != Player_States::walking_state)
			{
				speed.y = -initialJumpSpeed/2;
				state = Player_States::idle_State;
				is_on_platform = true;
			}

			else if (collider->rect.y < col1->rect.y + col1->rect.h && collider->rect.y + collider->rect.h > col1->rect.y + col1->rect.h)
			{
				state = Player_States::fall_State;
				speed.y = -initialJumpSpeed/2;
			}
		}
		
		//horitzontal collisions
		if (collider->rect.y < col1->rect.y + col1->rect.h - 5 && collider->rect.y + collider->rect.h > col1->rect.y + 5)
		{
			if (collider->rect.x < col1->rect.x + col1->rect.w && collider->rect.x + collider->rect.w > col1->rect.x + col1->rect.w)
			{
				position.x += speed.x * App->GetDT();
			}

			else if (collider->rect.x + collider->rect.w > col1->rect.x && collider->rect.x < col1->rect.x)
			{
				position.x -= speed.x * App->GetDT();
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
		if (collider->rect.y + collider->rect.h - 5 > col1->rect.y && !hit_time)
		{
			lifes--;
			hit_time = true;
			time_to_hit.Start();	
			App->audio->PlayFx(App->entityManager->player_damage_sfx);
		}
		
	}
	else if (col1->type == COLLIDER_TYPE::COLLIDER_WIN)
	{
			num_lvl++;

			col1->to_delete = true;
		if (App->current_level == 1 && num_lvl == 1)
		{
			
			App->current_level = 2;
			App->scene->DestroyHUD();
			App->fade->StartfadetoBlack();
		}
		else if(App->current_level == 2 && num_lvl == 2)
		{
			App->current_level = 1;
			App->scene->DestroyHUD();
			App->fade->StartfadetoBlack();
			num_lvl = 0;
		}
	}
	else if (col1->type == COLLIDER_TYPE::COLLIDER_COIN)
	{
		coins++;
		App->scene->score += 20;
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
		current_animation = &anim_death;
		if(current_animation->GetCurrentFrameNumber() == 0)
			App->audio->PlayFx(death_sfx);
		break;
	case Player_States::walking_state:
		current_animation = &anim_walking;
	default:
		break;
	}
}
