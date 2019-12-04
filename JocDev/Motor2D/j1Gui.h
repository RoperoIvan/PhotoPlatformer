#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
struct _TTF_Font;

class UI {
public:
	UI(const fPoint &pos) :position(pos) {}
	~UI() {}

	enum class Type {
		BUTTON,
		IMAGE,
		LABEL,

		MAX
	};

	virtual bool Draw() { return true; }
	fPoint position;
	UI::Type ui_type;
};

class Image :public UI {
public:
	Image(const fPoint &pos, const SDL_Rect &r) :UI(pos), dimension(r) {}

	bool Draw();

	SDL_Rect dimension;
};
enum class Mouse {
	IDLE,
	ONHOVER,
	PUSH
};
class Button :public UI {
public:
	Button(const fPoint &position, const SDL_Rect &idle, const SDL_Rect &hover, const SDL_Rect &push) :UI(position), idle(idle), hover(hover), push(push) {}
	~Button() {}

	bool Draw();


	Mouse	 mouse = Mouse::IDLE;
	SDL_Rect idle;
	SDL_Rect hover;
	SDL_Rect push;
};

class Label :public UI {
public:
	Label(fPoint pos, const char* txt, const char* path_font) :UI(pos) {
		font = App->fonts->Load(path_font);
		text.create(txt);
		SDL_Color color = { 255, 255, 255, 255 };
		texture = App->fonts->Print(text.GetString(), color, font);
	}
	~Label() {}

	bool Draw();

	_TTF_Font*		font;
	SDL_Texture*	texture;
	p2SString		text;
};

class CheckBox :public UI {
public:
	CheckBox(const fPoint& position, const SDL_Rect& active, const SDL_Rect& disactive, const bool &is_active) :UI(position), active(active), disactive(disactive), is_active(is_active) {}
	~CheckBox() {}

	bool Draw();

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
	Button* CreateButton(const fPoint &pos, const SDL_Rect &idle, const SDL_Rect &hover, const SDL_Rect &push);
	Image * CreateImage(const fPoint & pos, const SDL_Rect & rect);
	Label * CreateLabel(const fPoint & pos, const char * text, const char * font);

	//virtual bool Draw() { return false; };
	void CheckMouse(Button*);
	const SDL_Texture* GetAtlas() const;

	void DeleteElement(UI* ui);

private:

	SDL_Texture * atlas;
	p2SString atlas_file_name;
	p2List<UI*> objects;
};

#endif // __j1GUI_H__