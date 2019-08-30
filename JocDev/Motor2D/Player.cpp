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
	initialJumpSpeed = { 1,-0.2 };
	gravity = 0.1F;
}

Player::~Player()
{
}

bool Player::Start()
{
	speed = { 0.08,-0.2 };
	return true;
}

void Player::Move(float dt)
{
	InPut();

	if (state == Player_States::fall_State)
	{
		position.y += -speed.y + (gravity*jumpTime.Read() / 1000);
	}
	if (state == Player_States::jump_State)
	{
		gravity = 0.1F;
		speed.y += (gravity*jumpTime.Read()/1000) * (gravity*jumpTime.Read() / 1000);

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

void Player::OnCollision(Collider *col)
{
	if (col->type == COLLIDER_TYPE::COLLIDER_WALL)
	{
		speed.y = initialJumpSpeed.y;
		state = Player_States::idle_State;
		jumpTime.Start();
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
	}
}
