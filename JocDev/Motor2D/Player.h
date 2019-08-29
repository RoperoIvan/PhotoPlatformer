#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "j1Timer.h"

enum class Player_States
{
	jump_State,
	fall_State,
	idle_State
};

class Player : public Entity
{
public:
	Player(const fPoint &position, const fPoint &speed);
	~Player();

	bool Start() override;
	void Move(float dt) override;
	void Draw()override;

	void OnCollision(Collider*) override;

private:

	void Jump();
	void InPut();

private:

	Animation idle;
	Animation run;
	Animation jump;
	
	Player_States state = Player_States::fall_State;
	float gravity = 0.2;
	
	j1Timer jumpTime;
	fPoint initialJumpSpeed;

};

#endif
