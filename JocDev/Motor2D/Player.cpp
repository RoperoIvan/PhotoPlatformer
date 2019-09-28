#include "Player.h"
#include"j1App.h"
#include "j1Collisions.h"
#include "j1EntityManager.h"
#include "j1Input.h"
#include "j1FadetoBlack.h"
#include "SDL/include/SDL_scancode.h"

Player::Player(const fPoint &position) : Entity(position)
{
	collider = App->collisions->AddCollider({ (int)position.x,(int)position.y, 30, 30 }, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)App->entityManager);
	//put in config
	gravity = 0.1F;
	initialJumpSpeed = { 1,-0.2f };
	speed = { 0.2f,-0.2f };
}

Player::~Player()
{
}

bool Player::Start()
{
	
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
		gravity = 0.01F;

		if (speed.y == 0)
			jumpTime.Start();

		if (speed.y < 0)
			speed.y = -initialJumpSpeed.y;
		

		position.y += speed.y + (gravity*jumpTime.Read() / 1000);
	}
	if (state == Player_States::jump_State)
	{
		gravity = 0.01F;
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
