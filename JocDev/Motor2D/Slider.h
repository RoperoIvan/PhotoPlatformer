#ifndef _SLIDER_H
#define _SLIDER_H

#include "UI.h"

class Slider : public UI
{
public:

	Slider(const int& pos_x, const int& pos_y, const SDL_Rect& slider_rect, UI* parent = nullptr, Slider_TYPE slider_type = Slider_TYPE::Y);
	~Slider();
	void SetSliderValueStart();
	void CleanUp();
	void InnerDraw();
	void PostUpdate();

public:
	float slider_value;
private:

	float value = 0.0F;
	Slider_TYPE type = Slider_TYPE::Y;
	SDL_Rect image = { 0,0,0,0 };
	int size_of_limits = 0;
	int min_pos;
	int max_pos;
	iPoint dot;
};

#endif

