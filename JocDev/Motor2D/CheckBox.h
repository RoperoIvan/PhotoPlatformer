#ifndef _CHECKBOX_H
#define _CHECKBOX_H

#include "UI.h"

class CheckBox :public UI {
public:
	CheckBox(int pos_x, int pos_y, const bool& is_active, UI* parent, bool drawable, CheckBox_Type type, const SDL_Rect& active_idle, const SDL_Rect& active_hover, const SDL_Rect& active_push, const SDL_Rect& disactive_idle,
		const SDL_Rect& disactive_hover, const SDL_Rect& disactive_push);
	~CheckBox();

	void CleanUp();
	void InnerDraw();
	void ClickLogic();

	bool is_active = false;
	SDL_Rect active_idle;
	SDL_Rect active_hover;
	SDL_Rect active_push;
	SDL_Rect disactive_idle;
	SDL_Rect disactive_hover;
	SDL_Rect disactive_push;
	CheckBox_Type type;
};


#endif

