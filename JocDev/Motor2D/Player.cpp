#include "Player.h"
#include"j1App.h"
#include "j1Collisions.h"
#include "j1EntityManager.h"
#include "j1Input.h"
#include "j1FadetoBlack.h"
#include "j1Render.h"
#include "SDL/include/SDL_scancode.h"

Player::Player(const fPoint &position) : Entity(position)
{
	LoadData("Player.tsx");

	collider = App->collisions->AddCollider({ (int)position.x,(int)position.y, 10, 10 }, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)App->entityManager);
	//put in config
	gravity = 0.1F;
	initialJumpSpeed = { 1,-0.5f };
	speed = { 1.f,-0.2f };
}

Player::~Player()
{
}

bool Player::Start()
{
	current_animation = &anim_idle;

	return true;
}

void Player::PreUpdate(float dt)
{
	InPut();
	current_animation->GetCurrentFrame();
}

void Player::Move(float dt)
{
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
	collider->SetPos((int)position.x, (int)position.y);


	if (state == Player_States::idle_State)
	{
		state = Player_States::fall_State;
	}
}

void Player::Draw()
{
	if (current_animation != nullptr)
	App->render->Blit(data.tiled.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(), 1.0F, flip);

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
}


void Player::Flash()
{
	App->fade->fadetoBlack(2.F);
	platforms.add(App->entityManager->CreateEntity(position, ENTITY_TYPE::PLATFORM));
}

void Player::InPut()
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		state = Player_States::jump_State;
		position.y -= initialJumpSpeed.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{		
		position.x += speed.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x += -speed.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		Flash();
		position = { 100,-500 };
		state = Player_States::fall_State;
	}
}

void Player::PushBack()
{
	for (uint i = 0; i < data.num_animations; ++i) {
		for (uint j = 0; j < data.animations[i].num_frames; ++j) {
			switch (data.animations[i].id) {
			case EntityState::IDLE:
				anim_idle.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::WALKING:
				anim_walking.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::JUMP:
				anim_jump.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::FALL:
				anim_fall.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::LAND:
				anim_land.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::DEAD:
				anim_death.PushBack(data.animations[i].frames[j]);
				break;
			default:
				break;
			}
		}
	}

	anim_jump.loop = false;
	anim_land.loop = false;
	anim_death.loop = false;
}

void Player::IdAnimToEntityState()
{
	for (uint i = 0; i < data.num_animations; ++i) 
	{
		switch (data.animations[i].id) {
		case 0:
			data.animations[i].states = EntityState::IDLE;
			break;
		/*case 16:
			data.animations[i].states = EntityState::WALKING;
			break;
		case 32:
			data.animations[i].states = EntityState::JUMP;
			break;
		case 35:
			data.animations[i].states = EntityState::FALL;
			break;
		case 36:
			data.animations[i].states = EntityState::LAND;
			break;
		case 64:
			data.animations[i].states = EntityState::DEAD;
			break;*/
		default:
			data.animations[i].states = EntityState::UNKNOWN;
			break;
		}
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