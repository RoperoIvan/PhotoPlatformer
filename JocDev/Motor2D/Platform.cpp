#include "Platform.h"
#include "j1App.h"
#include "j1Collisions.h"

Platform::Platform(const fPoint & position): Entity(position)
{
	collider = App->collisions->AddCollider({ (int)position.x,(int)position.y, 10, 10 }, COLLIDER_TYPE::COLLIDER_WALL, (j1Module*)App->entityManager);
}

Platform::~Platform()
{
	if(collider != nullptr)
		collider->to_delete = true;
}

void Platform::Draw()
{
}