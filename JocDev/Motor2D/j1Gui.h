#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_keyboard.h"
#include "Animation.h"
#include "j1Textures.h"

#define CURSOR_WIDTH 2
#define MAX_CHARACTERS 10

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
	UI(const int pos_x, const int pos_y, UI* parent, const int &width = 0, const int &height = 0, bool drawable = true) : position({pos_x, pos_y, width, height}), parent(parent), drawable(drawable) {}
	~UI() {}

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
		Slider,
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

	iPoint GetGlobalPosition() const;
	virtual void InnerDraw() {};
	virtual void PostUpdate() {};

public:
	SDL_Rect position;
	iPoint draw_offset = { 0,0 };
	UI::Type ui_type = UI::Type::NO_TYPE;
	MouseState m_state = MouseState::IDLE;
	bool to_delete = false;
	UI* parent = nullptr;
	bool drawable;
};

class Image :public UI {
public:
	Image(int pos_x, int pos_y, const SDL_Rect &r, UI* parent, bool drawable = true) :UI(pos_x, pos_y, parent, r.w, r.h,drawable)
	{ 
		dimension = r;
		ui_type = UI::Type::IMAGE;
	}
	void InnerDraw();
	SDL_Rect dimension;
};

class Button :public UI {

public:
	Button(int pos_x, int pos_y, UI* parent,const SDL_Rect & idle, const SDL_Rect & hover, const SDL_Rect & push, const UI::Button_Type& type, bool drawable = true)
		:UI(pos_x, pos_y, parent, idle.w, idle.h, drawable), idle(idle), hover(hover), push(push), type(type)
	{
		//this->text = new Label({position.x + width / 4, position.y + height / 3}, text.GetString(),nullptr,1);
		ui_type = UI::Type::BUTTON;
	}
	~Button() {}
	void InnerDraw();
	void ClickLogic();
	
	Button_Type type = Button_Type::No_button;	
	SDL_Rect idle;
	SDL_Rect hover;
	SDL_Rect push;
};

class Label :public UI {
public:
	Label(int pos_x, int pos_y, const char* txt, const Color & c, const char* path_font, const uint &size, UI* parent, bool drawable = true, uint32 wrap_length = 0U) :UI(pos_x, pos_y, parent, 0, 0, drawable) {
		font = App->fonts->Load(path_font, size);
		text.create(txt);
		SetColor(c);
		if (wrap_length == 0U)
			texture = App->fonts->Print(text.GetString(), color, font);
		else 
			texture = App->fonts->PrintWrapped(text.GetString(), color, font, wrap_length);

		App->fonts->CalcSize(txt, position.w, position.h, font);
		ui_type = UI::Type::LABEL;
	}
	~Label() {}
	void InnerDraw();
	void SetColor(Color color);
	void SetColor(SDL_Color color);
	void SetText(const char* text);

	_TTF_Font*		font;
	SDL_Texture*	texture;
	p2SString		text;
	uint			size;
	SDL_Color		color = { 255, 255, 255, 255 };
};

class CheckBox :public UI {
public:
	CheckBox(int pos_x, int pos_y, const bool &is_active, UI* parent, bool drawable, CheckBox_Type type, const SDL_Rect& active_idle, const SDL_Rect& active_hover, const SDL_Rect& active_push, const SDL_Rect& disactive_idle,
		const SDL_Rect& disactive_hover, const SDL_Rect& disactive_push) :UI(pos_x, pos_y, parent, active_idle.w, active_idle.h, drawable), active_idle(active_idle), active_hover(active_hover), active_push(active_push) ,
		disactive_idle(disactive_idle), disactive_hover(disactive_hover), disactive_push(disactive_push), is_active(is_active), type(type)
	{
		ui_type = UI::Type::CHECK_BOX;
	}
	~CheckBox() {}

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

class Slider : public UI
{
public:

	Slider(const int &pos_x, const int &pos_y, const SDL_Rect &slider_rect, UI* parent = nullptr, Slider_TYPE slider_type = Slider_TYPE::Y) : UI(pos_x, pos_y, parent, slider_rect.w, slider_rect.h, true)
	{
		position = { pos_x, pos_y, slider_rect.w, slider_rect.h };
		image = slider_rect;
		type = slider_type;
		ui_type = UI::Type::SLIDER;
		value = 0.0F;
	}

	void AddTargets(UI*);
	void AddThumb(Button*);
	void SetSliderValueStart(float slider_value);

	void InnerDraw();
	void PostUpdate();

	float GetSliderValue() const;
	Button* GetSliderButton() const;

private:

	float value = 0.0F;
	Slider_TYPE type = Slider_TYPE::Y;
	Button* thumb = nullptr;
	SDL_Rect image = { 0,0,0,0 };
	p2List<UI*> control;
};

class InputBox : public UI
{
public:
	InputBox(const int& pos_x, const int& pos_y, const char* txt, const Color& c, const char* path_font, const SDL_Rect& rect, bool drawable, UI* parent) :UI(pos_x, pos_y, parent, 0, 0, drawable)
	{
		font = App->fonts->Load(path_font, 15);
		ui_type = UI::Type::INPUT_BOX;
		text.create(txt);
		box = rect;
		SetColor(c);
		texture = App->fonts->Print(text.GetString(), color, font);
		App->fonts->CalcSize(txt, position.w, position.h, font);
		cursor = new Animation();
		cursor->PushBack({ 103,1345, 2,15 });
		cursor->PushBack({ 0,0,0,0 });
		cursor->speed = 2.0F;
		uint width_ = 0u;
		App->tex->GetSize(texture, width_, HEIGHT);
		
	}
	~InputBox()
	{
		SDL_StopTextInput();
		delete cursor;
		cursor = nullptr;
	}

	void InnerDraw();

	void SetText(const char* txt);

	void SetColor(const Color& c);

	void SetColor(const SDL_Color& c);

	p2SString GetText();

	void ChangeFont(const char* f, const int& size);

public:
	void AddText(const char* txt);
	void DeleteText();
	SDL_Rect box = { 0,0,0,0 };
protected:
	SDL_Texture* texture = nullptr;
	SDL_Color color = { 255,255,255,255 };
	p2SString text;
	SDL_Rect input_box{ 0,0,0,0 };
	_TTF_Font* font;
	Animation* cursor = nullptr;
	uint HEIGHT = 0u;
};
// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called after all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions
	Button* CreateButton(const fPoint & pos, UI* parent, const SDL_Rect & idle, const SDL_Rect & hover, const SDL_Rect & push, const UI::Button_Type& type);
	Image * CreateImage(const fPoint & pos, UI* parent, const SDL_Rect & rect, bool drawable);
	Label * CreateLabel(const fPoint & pos, UI* parent, const char * text, const Color& c, const char * font,const uint& size = DEFAULT_FONT_SIZE, uint32 wrap_length = 0U);
	Slider * CreateSlider(const fPoint & pos, const SDL_Rect &slider_rect, Slider_TYPE type, UI* parent = nullptr);
	CheckBox* CreateCheckbox(const fPoint & pos, const bool &is_active, UI* parent, bool drawable, UI::CheckBox_Type type,const SDL_Rect& active_idle, const SDL_Rect& active_hover, const SDL_Rect& active_push, const SDL_Rect& disactive_idle,
		const SDL_Rect& disactive_hover, const SDL_Rect& disactive_push);
	InputBox* CreateInputBox(const fPoint& pos, const char* text, UI* parent, Color color, const char* font_path, const SDL_Rect& rect, bool drawable);
	void CreateScreen();
	void CheckMouse(UI*);
	bool Select()const;
	const SDL_Texture* GetAtlas() const;

	void DeleteElement(UI* ui);
	void SetPosition(UI*ui, const int& x, const int& y);
private:

	SDL_Texture * atlas;
	p2SString atlas_file_name;
	p2List<UI*> objects;
	UI* selected = nullptr;

public:
	UI * screen = nullptr;
};

#endif // __j1GUI_H__