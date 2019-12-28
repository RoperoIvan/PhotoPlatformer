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
	value = 0.0F;
	SetSliderValueStart();
}

Slider::~Slider()
{
}

void Slider::SetSliderValueStart()
{
	min_pos = (position.x);
	max_pos = (position.x + 470);
	dot.x = min_pos;
	dot.y = position.y;
	position.x = dot.x;
	position.y = dot.y;
	position.w = image.w;
	position.h = image.h;
}

void Slider::CleanUp()
{
}

void Slider::InnerDraw()
{
	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &image, false, SDL_FLIP_NONE, 0.0f, 255, true);
}

void Slider::PostUpdate()
{
	if (m_state == MouseState::PUSH)
	{
		iPoint m_pos;
		App->input->GetMousePosition(m_pos.x, m_pos.y);
		if (m_pos.x >= min_pos && m_pos.x <= max_pos)
		{
			position.x = m_pos.x - image.w / 2;
			dot.x = m_pos.x;
		}
	}
	float total = max_pos - min_pos;
	float actual = max_pos - dot.x;

	float val = actual / total;

	slider_value = 1 - val;
}
