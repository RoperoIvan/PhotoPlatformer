#include "CheckBox.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"

CheckBox::CheckBox(int pos_x, int pos_y, const bool& is_active, UI* parent, bool drawable, CheckBox_Type type, const SDL_Rect& active_idle, const SDL_Rect& active_hover, const SDL_Rect& active_push, const SDL_Rect& disactive_idle,
	const SDL_Rect& disactive_hover, const SDL_Rect& disactive_push) : UI(pos_x, pos_y, parent, active_idle.w, active_idle.h, drawable), active_idle(active_idle), active_hover(active_hover), active_push(active_push),
	disactive_idle(disactive_idle), disactive_hover(disactive_hover), disactive_push(disactive_push), is_active(is_active), type(type)
{
	ui_type = UI::Type::CHECK_BOX;
}

CheckBox::~CheckBox()
{
}

void CheckBox::CleanUp()
{
}

void CheckBox::InnerDraw()
{
	if (drawable)
	{
		if (is_active)
		{
			switch (m_state)
			{
			case UI::MouseState::ONHOVER:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &active_hover, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			case UI::MouseState::IDLE:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &active_idle, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			case UI::MouseState::PUSH:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &active_push, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			case UI::MouseState::REPEAT:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &active_push, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			}
		}


		else
		{
			switch (m_state)
			{
			case UI::MouseState::ONHOVER:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &disactive_hover, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			case UI::MouseState::IDLE:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &disactive_idle, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			case UI::MouseState::PUSH:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &disactive_push, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			case UI::MouseState::REPEAT:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &disactive_push, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			}
		}
	}
}

void CheckBox::ClickLogic()
{
	switch (type)
	{
	case CheckBox_Type::No_checkbox:
		break;
	case CheckBox_Type::Fullscreen:
		App->win->fullscreen = !App->win->fullscreen;
		if (App->win->fullscreen)
			SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		else
			SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_SHOWN);
		break;
	}
	is_active = !is_active;
}