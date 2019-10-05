#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "j1Timer.h"




enum class Player_States
{
	jump_State,
	fall_State,
	idle_State,
	die_state,
	walking_state,
	god_mode_state,
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
	void PushBack() override;
	void IdAnimToEntityState();
	//void LoadProperties(pugi::xml_node&);
	void DeletePlatforms();
	void ChangeAnim();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;
private:

	Animation anim_walking;
	Animation anim_jump;
	Animation anim_fall;
	Animation anim_land;
	Animation anim_death;
	Animation anim_default;
	
	Player_States state = Player_States::fall_State;
	Player_States check_state;
	j1Timer jumpTime;
	float initialJumpSpeed;
	fPoint initialSpeed;
	fPoint respawn;
	float grav;

	p2List<Entity*> platforms;

};

#endif
