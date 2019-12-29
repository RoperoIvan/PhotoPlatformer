#ifndef _UI_H
#define _UI_H

#include "p2Point.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_keyboard.h"

struct _TTF_Font;

enum Color
{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	GREY,
	BLACK,
	WHITE,

	COLOR_NOT_DEF
};

enum class Slider_TYPE {
	X,
	Y
};

class UI {
public:
	UI(const int pos_x, const int pos_y, UI* parent, const int& width = 0, const int& height = 0, bool drawable = true);
	~UI();

	enum class Type {
		BUTTON,
		IMAGE,
		LABEL,
		CHECK_BOX,
		SLIDER,
		INPUT_BOX,
		NO_TYPE,
		MAX
	};
	enum class MouseState {
		IDLE,
		ONHOVER,
		PUSH,
		REPEAT,
	};
	enum class Button_Type
	{
		Go_to_Menu,
		No_button,
		Settings_Menu,
		Exit,
		Credits,
		Play,
		Continue,
		Resume,
		Slider_Up,
		Slider_Down,
		Return,
		Main_Settings_Menu,
		Return_MainMenu,
		Profile_Didac,
		Profile_Ivan,
		Webpage,
		Command,
	};

	enum class CheckBox_Type
	{
		No_checkbox,
		Fullscreen,
	};

	bool Draw();
	void DebugDraw();
	iPoint GetGlobalPosition() const;
	virtual void InnerDraw() {}
	virtual void PostUpdate() {}
	virtual void CleanUp() {}
public:
	SDL_Rect position;
	iPoint draw_offset = { 0,0 };
	UI::Type ui_type = UI::Type::NO_TYPE;
	MouseState m_state = MouseState::IDLE;
	bool to_delete = false;
	UI* parent = nullptr;
	bool drawable;
};


#endif

