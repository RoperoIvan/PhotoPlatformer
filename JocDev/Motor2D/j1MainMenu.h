#ifndef __J1MAINMENU_H__
#define __J1MAINMENU_H__

#include "j1Module.h"
#include "p2DynArray.h"

class Button;
class Label;
class Image;
class CheckBox;
class Slider;
class InputBox;

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

	void CreateSettingsMenu();

	void DestroySettingsMenu();

	void CreateCreditsMenu();

	void DestroyCreditsMenu();

public:
	uint win_width = 0;
	uint win_height = 0;
	bool can_load = false;

	//Main Menu
	Image* panel = nullptr;
	Button* play_button = nullptr;
	Label* play_label = nullptr;
	Button* continue_button = nullptr;
	Label* continue_label = nullptr;
	Button* main_settings_button = nullptr;
	Label* main_settings_label = nullptr;
	Button* credits_button = nullptr;
	Label* credits_label = nullptr;
	Button* exit_button = nullptr;
	Label* exit_label = nullptr;

	//Settings Menu
	Image* settings_panel = nullptr;
	Image* volume_level = nullptr;
	Slider* slider_volume = nullptr;
	Button* to_main_menu_button = nullptr;
	Label* to_main_menu_label = nullptr;
	CheckBox* fullscreen_checkbox = nullptr;
	Label* fullscreen_label = nullptr;

	//Credits Menu
	Image* credits_panel = nullptr;
	Label* authors_label = nullptr;
	Button* didac_button = nullptr;
	Label* didac_label = nullptr;
	Button* ivan_button = nullptr;
	Label* ivan_label = nullptr;
	Label* license_text_label = nullptr;
	Label* license_label = nullptr;
	Button* page_button = nullptr;
	Label* page_label = nullptr;

	InputBox* input_console = nullptr;
};
#endif