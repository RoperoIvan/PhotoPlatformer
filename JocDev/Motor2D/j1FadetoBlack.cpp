#include <math.h>
#include "j1App.h"
#include "j1FadetoBlack.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1EntityManager.h"
#include "j1Audio.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Pathfinding.h"
#include "p2Log.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "Brofiler/Brofiler.h"

j1FadetoBlack::j1FadetoBlack() : j1Module()
{
	name.create("fade");
}

j1FadetoBlack::~j1FadetoBlack() {}

bool  j1FadetoBlack::Start()
{
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	uint width, height;
	App->win->GetWindowSize(width, height);
	screen = { 0,0,(int)width,(int)height };
	return true;
}

bool j1FadetoBlack::PostUpdate(float id)
{
	BROFILER_CATEGORY("FadeToBlack PostUpdate", Profiler::Color::Bisque);
	if (current_step == fade_step::NONE)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;

	switch (current_step)
	{
	case fade_step::FADE_TO_BLACK:
	{
		normalized = MIN(1.0f, ((float)now*2.0F) / (float)total_time);
		if (now >= total_time * 0.5F)
		{
			NewLevel();
			current_step = fade_step::FADE_FROM_BLACK;
		}
	}
	break;

	case fade_step::FADE_FROM_BLACK:
	{
		normalized = MAX(0.0F, 2.0F - ((float)now * 2.0F) / (float)total_time);
		if (now >= total_time)
		{
			current_step = fade_step::NONE;
			normalized = 0.0F;
		}
	}
	break;
	}

	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);
	return true;
}

void j1FadetoBlack::NewLevel()
{
	App->collisions->CleanUp();
	App->entityManager->CleanUp();
	App->map->CleanUp();
	App->audio->UnLoadFx();
	switch (App->scene->current_level)
	{
	case 1:

		if (App->map->Load("Level1.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;
			if (App->map->CreateWalkabilityMap(w, h, &data))
				App->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}
		App->audio->PlayMusic("audio/music/awesomeness.ogg", 2.0);
		break;
	case 2:
		if (App->map->Load("Level2.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;
			if (App->map->CreateWalkabilityMap(w, h, &data))
				App->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}
		App->audio->PlayMusic("audio/music/awesomeness.ogg", 2.0);
		break;
	default:
		LOG("Error, that level doesn't exist.");
	}
	App->entityManager->Start();
}

bool j1FadetoBlack::StartfadetoBlack(float time)
{
	bool ret = false;

	if (current_step == fade_step::NONE)
	{
		current_step = fade_step::FADE_TO_BLACK;
		total_time = (Uint32)(time * 0.5F * 1000.0F);
		start_time = SDL_GetTicks();
		ret = true;
	}


	return ret;
}



bool j1FadetoBlack::IsFading() const
{
	return current_step != fade_step::NONE;
}