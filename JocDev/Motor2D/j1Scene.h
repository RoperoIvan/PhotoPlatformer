#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class Image;
class Label;
class Button;
class Slider;
class CheckBox;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt) override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called before all Updates
	bool PostUpdate(float dt) override;

	// Called before quitting
	bool CleanUp();

	void DebugKeys();  // Manage all the debug functionalities

	void LevelChange(int lvl); // Change between levels

	bool Load(pugi::xml_node& node) override;

	bool Save(pugi::xml_node& node) const override;

	void CreatePauseMenu();

	void DestroyPauseMenu();

	void CreateSettingsMenu();

	void DestroySettingsMenu();

	void CreateHUD();

	void DestroyHUD();

	void TimerManage();

	void ScoreManage();

public:

	iPoint	respawn;
	fPoint scene_spawn;

	Image* banner;
	Label* text;

	//Pause Menu
	Image* panel = nullptr;
	Button* resume_button = nullptr;
	Label* resume_label = nullptr;
	Button* exit_button = nullptr;
	Label* exit_label = nullptr;
	Button* settings_button = nullptr;
	Label* settings_label = nullptr;

	//Settings Menu
	Image* settings_panel = nullptr;
	Image* volume_level = nullptr;
	Slider* slider_volume = nullptr;
	Label* slider_label = nullptr;
	Button* to_pause_menu_button = nullptr;
	Label* to_pause_menu_label = nullptr;
	CheckBox* fullscreen_checkbox = nullptr;
	Label* fullscreen_label = nullptr;

	//HUD
	Image* heart_1 = nullptr;
	Image* heart_2 = nullptr;
	Image* heart_3 = nullptr;
	Image* coin = nullptr;
	Label* coins_label = nullptr;
	Image* points_image = nullptr;
	Image* points_panel = nullptr;
	Label* points_label = nullptr;
	Label* timer_label = nullptr;

	p2SString str_time;
	int start_time = 0;
	int score = 100;
	int minus_time = 10;
	int timer = 0;
};

#endif // __j1SCENE_H__