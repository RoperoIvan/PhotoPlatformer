#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_keyboard.h"
#include "Animation.h"
#include "j1Textures.h"
#include "UI.h"


#define CURSOR_WIDTH 2

class UI;
class Image;
class Button;
class Label;
class CheckBox;
class Slider;
class InputBox;

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