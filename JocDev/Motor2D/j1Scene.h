#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class Image;
class Label;
class Button;

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

public:

	iPoint	respawn;
	fPoint scene_spawn;
	uint current_level = 1;

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

};

#endif // __j1SCENE_H__