#ifndef _SLIDER_H
#define _SLIDER_H

#include "UI.h"

class Slider : public UI
{
public:

	Slider(const int& pos_x, const int& pos_y, const SDL_Rect& slider_rect, int value_to_calculate, UI* parent = nullptr, Slider_TYPE slider_type = Slider_TYPE::Y);
	~Slider();
	void SetSliderValueStart(int pos_start);
	void CleanUp();
	void InnerDraw();
	void PostUpdate();
	void CalculateFinalValue();
	bool IsInLimits();
public:
	float value;
private:
	Slider_TYPE type = Slider_TYPE::X;
	SDL_Rect image = { 0,0,0,0 };
	int size_of_limits = 0;
	int min_place;
	int max_place;
	iPoint current_place_thumb;
	iPoint m_pos;
};

#endif

