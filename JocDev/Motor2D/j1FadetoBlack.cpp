#include <math.h>
#include "j1App.h"
#include "j1FadetoBlack.h"
#include "j1Window.h"
#include "j1Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

j1FadetoBlack::j1FadetoBlack()
{
}

j1FadetoBlack::~j1FadetoBlack(){}

bool  j1FadetoBlack::Start()
{
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
	uint width, height;
	App->win->GetWindowSize(width, height);
	screen = { 0,0,(int)width,(int)height};
}

// Actualización: dibujar fondo
bool j1FadetoBlack::Update(float id)
{
	if (current_step == fade_step::NONE)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch(current_step)
	{
		case fade_step::FADE_TO_BLACK:
		{
			if (now >= total_time)
			{
				total_time += total_time;
				start_time = SDL_GetTicks();
				current_step = fade_step::FADE_FROM_BLACK;
			}
		} 
		break;

		case fade_step::FADE_FROM_BLACK:
		{
			normalized = 1.0f - normalized;

			if (now >= total_time)
				current_step = fade_step::NONE;
		} 
		break;
	}

	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);
	return true;
}

// Se desvanece a negro. En el punto medio, desactiva un módulo, luego activa el otro
bool j1FadetoBlack::fadetoBlack(float time)
{
	bool ret = false;

	if (current_step == fade_step::NONE)
	{
		current_step = fade_step::FADE_TO_BLACK;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		
		ret = true;
	}

	return ret;
}

bool j1FadetoBlack::IsFading() const
{
	return current_step != fade_step::NONE;
}