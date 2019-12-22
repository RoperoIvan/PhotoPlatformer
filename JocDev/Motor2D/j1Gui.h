#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"

#define CURSOR_WIDTH 2


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

class UI {
public:
	UI(const int& pos_x, const int& pos_y, UI* parent, const int &width = 0, const int &height = 0, bool drawable = true) :position({pos_x, pos_y, width, height}), parent(parent), drawable(drawable) {}
	~UI() {}

	enum class Type {
		BUTTON,
		IMAGE,
		LABEL,
		CHECK_BOX,
		NO_TYPE,
		MAX
	};
	enum class MouseState {
		IDLE,
		ONHOVER,
		PUSH
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
	};

	bool Draw();

	iPoint GetGlobalPosition() const;
	virtual void InnerDraw() {};

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
	Label(int pos_x, int pos_y, const char* txt, const Color & c, const char* path_font, const uint &size, UI* parent, bool drawable = true) :UI(pos_x, pos_y, parent, 0, 0, drawable) {
		font = App->fonts->Load(path_font, size);
		text.create(txt);
		SetColor(c);
		App->fonts->CalcSize(txt, position.w, position.h, font);
		ui_type = UI::Type::LABEL;
	}
	~Label() {}
	void InnerDraw();
	void SetColor(Color color);
	void SetColor(SDL_Color color);

	_TTF_Font*		font;
	SDL_Texture*	texture;
	p2SString		text;
	uint			size;
	SDL_Color		color = { 255, 255, 255, 255 };
};

class CheckBox :public UI {
public:
	CheckBox(int pos_x, int pos_y, const SDL_Rect& active, const SDL_Rect& disactive, const bool &is_active, UI* parent) :UI(pos_x, pos_y, parent, 0, 0, drawable), active(active), disactive(disactive), is_active(is_active)
	{
		ui_type = UI::Type::CHECK_BOX;
	}
	~CheckBox() {}

	void InnerDraw();

	bool is_active;
	SDL_Rect active;
	SDL_Rect disactive;
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
	Label * CreateLabel(const fPoint & pos, UI* parent, const char * text, const Color& c, const char * font,const uint& size = DEFAULT_FONT_SIZE);

	//virtual bool Draw() { return false; };
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