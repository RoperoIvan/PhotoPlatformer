#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

class Player : public Entity
{
public:
	Player(const fPoint &position);
	~Player();

	bool Start();
	void Move();
	void Draw();

private:

	Animation idle;
	Animation run;
	Animation jump;
};

#endif
