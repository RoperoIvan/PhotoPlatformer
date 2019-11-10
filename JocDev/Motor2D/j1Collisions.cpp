#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "p2Log.h"
#include "Brofiler/Brofiler.h"

j1Collisions::j1Collisions()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_WALL][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_WALL][COLLIDER_WIN] = false;
	matrix[COLLIDER_WALL][COLLIDER_DAMAGE] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_CHECKPOINT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WIN] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_DAMAGE] = false;

	matrix[COLLIDER_CHECKPOINT][COLLIDER_WALL] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_WIN] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_DAMAGE] = false;

	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_WIN] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_DAMAGE] = false;

	matrix[COLLIDER_WIN][COLLIDER_WALL] = false;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WIN][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_WIN][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_WIN][COLLIDER_WIN] = false;
	matrix[COLLIDER_WIN][COLLIDER_DAMAGE] = false;

	//matrix[COLLIDER_DAMAGE][COLLIDER_WALL] = false;
	//matrix[COLLIDER_DAMAGE][COLLIDER_PLAYER] = true;
	//matrix[COLLIDER_DAMAGE][COLLIDER_CHECKPOINT] = false;
	//matrix[COLLIDER_DAMAGE][COLLIDER_ENEMY] = false;
	//matrix[COLLIDER_DAMAGE][COLLIDER_WIN] = false;
	//matrix[COLLIDER_DAMAGE][COLLIDER_DAMAGE] = false;
}

// Destructor
j1Collisions::~j1Collisions()
{}

bool j1Collisions::PreUpdate(float dt)
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	// Calculate collisions
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}

	return true;
}

// Called before render is available
bool j1Collisions::Update(float dt)
{
	BROFILER_CATEGORY("Collisions Update", Profiler::Color::Purple);
	if (debug == true)
		DebugDraw();
	
	return true;
}

void j1Collisions::DebugDraw()
{

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: 
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALL:
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER:
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_CHECKPOINT:
			App->render->DrawQuad(colliders[i]->rect, 125, 125, 0, alpha);
			break;
		case COLLIDER_ENEMY:
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_WIN:
			App->render->DrawQuad(colliders[i]->rect, 255, 125, 0, alpha);
			break;
		}
	}
}

// Called before quitting
bool j1Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	*colliders = nullptr;

	return true;
}

Collider* j1Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	// TODO 0: Return true if there is an overlap
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}

void j1Collisions::GodMode()
{
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = god_mode;
	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = god_mode;
	god_mode = !god_mode;
}