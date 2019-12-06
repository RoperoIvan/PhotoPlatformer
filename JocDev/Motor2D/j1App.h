#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Timer.h"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1FadetoBlack;
class j1EntityManager;
class j1Collisions;
class j1PathFinding;
class j1Fonts;
class j1Gui;


class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;
	float GetDT();
private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	j1Window*			win	= nullptr;
	j1Input*			input = nullptr;
	j1Render*			render = nullptr;
	j1Textures*			tex = nullptr;
	j1Audio*			audio = nullptr;
	j1FadetoBlack*		fade = nullptr;
	j1Scene*			scene = nullptr;
	j1Map*				map = nullptr;
	j1EntityManager*	entityManager = nullptr;
	j1Collisions*		collisions = nullptr;
	j1PathFinding*		pathfinding = nullptr;
	j1Fonts*			fonts = nullptr;
	j1Gui*				gui;

private:

	p2List<j1Module*>	modules;
	uint				frames;
	float				dt;
	bool				cap_fps;
	uint				frames_cap = 0;
	j1Timer				timer_frames;
	j1Timer				timer_frames_second;
	j1Timer				timer_starting;
	uint64				time_last_frames_second;
	uint64				time_frame_second = 0;
	uint64				frame_counting = 0;
	uint32				fps_cap = 0;
	int					argc;
	char**				args;
	bool				pause_game = false;
	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save;
	bool				want_to_load;
	p2SString			load_game;
	mutable p2SString	save_game;
};

extern j1App* App;

#endif