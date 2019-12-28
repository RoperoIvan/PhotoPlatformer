#include "Slider.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "p2Log.h"

Slider::Slider(const int& pos_x, const int& pos_y, const SDL_Rect& slider_rect, UI* parent, Slider_TYPE slider_type) : UI(pos_x, pos_y, parent, slider_rect.w, slider_rect.h, true)
{
	position = { pos_x, pos_y, slider_rect.w, slider_rect.h };
	image = slider_rect;
	type = slider_type;
	ui_type = UI::Type::SLIDER;
	SetSliderValueStart();
}

Slider::~Slider()
{
}

void Slider::SetSliderValueStart()
{
	min_place = (position.x);
	max_place = (position.x + 470);
	current_place_thumb.x = min_place;
	current_place_thumb.y = position.y;
	position.x = current_place_thumb.x;
	position.y = current_place_thumb.y;
}

void Slider::CleanUp()
{
}

void Slider::InnerDraw()
{
	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &image, false, SDL_FLIP_NONE, 0.0f, 255, true);
}


void Slider::CalculateFinalValue()
{
	float maximum_value = max_place - min_place;
	float actual_value = max_place - current_place_thumb.x;

	float final_val = actual_value / maximum_value;

	value = 1 - final_val;
}

bool Slider::IsInLimits()
{
	bool ret = false;

	
	App->input->GetMousePosition(m_pos.x, m_pos.y);
	if (m_pos.x >= min_place && m_pos.x <= max_place)
		ret = true;

	return ret;
}


void Slider::PostUpdate()
{
	if (m_state == MouseState::PUSH)
	{
		
		if (IsInLimits())
		{
			position.x = m_pos.x - image.w * 0.5f;
			current_place_thumb.x = m_pos.x;
		}
	}
	CalculateFinalValue();
}
