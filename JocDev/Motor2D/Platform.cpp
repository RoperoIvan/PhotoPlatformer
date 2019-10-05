#include "Platform.h"
#include "j1App.h"
#include "j1Collisions.h"
#include "j1EntityManager.h"
#include "j1Render.h"

Platform::Platform(const fPoint & position): Entity(position,"platform")
{
	data.tiled.texture = App->entityManager->player->data.tiled.texture;
	collider = App->collisions->AddCollider({ (int)position.x,(int)position.y + offset.y, size.x, size.y }, COLLIDER_TYPE::COLLIDER_WALL, (j1Module*)App->entityManager);
	current_animation = &anim_idle;
}

Platform::~Platform()
{
	if(collider != nullptr)
		collider->to_delete = true;

	data.tiled.texture = nullptr;

}

void Platform::Draw()
{
	if (current_animation != nullptr)
		App->render->Blit(data.tiled.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(), flip, 1.0F);
}
