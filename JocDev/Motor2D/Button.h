#ifndef _BUTTON_H
#define _BUTTON_H

#include "UI.h"

class Button :public UI {

public:
	Button(int pos_x, int pos_y, UI* parent, const SDL_Rect& idle, const SDL_Rect& hover, const SDL_Rect& push, const UI::Button_Type& type, bool drawable = true);
	~Button();
	void CleanUp();
	void InnerDraw();
	void ClickLogic();

	Button_Type type = Button_Type::No_button;
	SDL_Rect idle;
	SDL_Rect hover;
	SDL_Rect push;
};

#endif

