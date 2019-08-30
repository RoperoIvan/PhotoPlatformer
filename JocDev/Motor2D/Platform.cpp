#include "Platform.h"
#include "j1App.h"
#include "j1Collisions.h"

Platform::Platform(const fPoint & position): Entity(position)
{
	collider = App->collisions->AddCollider({ (int)position.x,(int)position.y, 30, 30 }, COLLIDER_TYPE::COLLIDER_WALL, (j1Module*)App->entityManager);
}

Platform::~Platform()
{
}

void Platform::Draw()
{
}
