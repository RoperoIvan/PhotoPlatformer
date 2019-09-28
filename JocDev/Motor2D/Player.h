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
	Player(const fPoint &position);
	~Player();

	bool Start() override;
	void PreUpdate(float dt)override;
	void Move(float dt) override;
	void Draw()override;

	void OnCollision(Collider* c1) override;

private:

	void Flash();
	void InPut();

private:

	Animation idle;
	Animation run;
	Animation jump;
	
	Player_States state = Player_States::fall_State;
	float gravity;
	
	j1Timer jumpTime;
	fPoint initialJumpSpeed;
	fPoint initialSpeed;
	fPoint respawn;

	p2List<Entity*> platforms;

};

#endif
