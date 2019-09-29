#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "j1Timer.h"

enum class Player_States
{
	jump_State,
	fall_State,
	idle_State,
	die_state
};

class Player : public Entity
{
public:
	Player(const fPoint &position);
	~Player();

	bool Start() override;
	void PreUpdate(float dt)override;
	void Move(float dt) override;
	void Draw();

	void OnCollision(Collider* c1) override;

private:

	void Flash();
	void InPut();
	void PushBack();
	void IdAnimToEntityState();
	//void LoadProperties(pugi::xml_node&);
	void DeletePlatforms();

private:

	Animation anim_walking;
	Animation anim_jump;
	Animation anim_fall;
	Animation anim_land;
	Animation anim_death;
	Animation anim_default;
	
	Player_States state = Player_States::fall_State;
	float gravity;
	
	j1Timer jumpTime;
	fPoint initialJumpSpeed;
	fPoint initialSpeed;
	fPoint respawn;

	p2List<Entity*> platforms;

};

#endif
