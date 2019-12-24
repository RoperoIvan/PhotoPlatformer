#ifndef __J1MAINMENU_H__
#define __J1MAINMENU_H__

#include "j1Module.h"
#include "p2DynArray.h"

class Button;
class Label;
class Image;
class CheckBox;
class Slider;

class j1MainMenu : public j1Module
{
public:

	j1MainMenu();

	// Destructor
	virtual ~j1MainMenu();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	void CreateMainMenu();

	void DestroyMainMenu();
public:
	uint win_width = 0;
	uint win_height = 0;
	bool can_load = false;

	//GUI
	Image* panel = nullptr;
	Button* play_button = nullptr;
	Label* play_label = nullptr;
	Button* continue_button = nullptr;
	Label* continue_label = nullptr;
	Button* main_settings_button = nullptr;
	Label* main_settings_label = nullptr;
	Button* credits_button = nullptr;
	Label* credits_label = nullptr;

};
#endif