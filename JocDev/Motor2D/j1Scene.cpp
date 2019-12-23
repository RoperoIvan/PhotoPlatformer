#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1FadetoBlack.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "Player.h"
#include "j1Collisions.h"
#include "j1Pathfinding.h"
#include "j1Gui.h"
#include "Brofiler/Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->win->SetTitle("PhotoPlatformer 0.1.1");
	App->audio->PlayMusic("audio/music/awesomeness.ogg",2.0);
	if (App->map->Load("Level1.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	/*SDL_Rect r = { 0,0,363,178 };
	App->gui->CreateImage(fPoint(177,0), r);
	text = App->gui->CreateLabel(fPoint(0, 0), "TEST","fonts/open_sans/OpenSans-Regular.ttf", 34);*/
	/*panel = App->gui->CreateImage(fPoint(0, 0), { 1504,960,503,717 });
	App->gui->SetPosition(panel, (App->render->camera.x + App->render->camera.w), (App->render->camera.y + App->render->camera.h) );*/
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate(float dt)
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("SCENE UPDATE", Profiler::Color::DeepSkyBlue);

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	DebugKeys();

	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate(float dt)
{
	bool ret = true;

	/*if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;*/

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		(App->Pause()) ? CreatePauseMenu() : DestroyPauseMenu();
	}

	LOG(" POS X: %i POS Y: %i", App->render->camera.x, App->render->camera.y);
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::DebugKeys()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		current_level = 1;
		App->fade->StartfadetoBlack();
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		current_level = 2;
		App->fade->StartfadetoBlack();
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		App->entityManager->player->position = scene_spawn;

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		App->collisions->debug = !App->collisions->debug;

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		App->collisions->GodMode();
	}
}

void j1Scene::LevelChange(int lvl)
{
	//current_level = lvl;
	//App->fade->StartfadetoBlack(2);
	//
}

bool j1Scene::Load(pugi::xml_node& node)
{
	bool ret = true;
	pugi::xml_node lvl_stats = node.child("lvl_stats");
	if (current_level != lvl_stats.attribute("level").as_uint())
		current_level = lvl_stats.attribute("level").as_uint();
	
	App->fade->NewLevel();
	return ret;
}

bool j1Scene::Save(pugi::xml_node& node) const
{

	bool ret = true;
	pugi::xml_node lvl_stats = node.append_child("lvl_stats");
	lvl_stats.append_attribute("level") = current_level;
	return ret;
}

//-------------------MENUS--------------------------------------

//------------------PAUSE MENU----------------------------------
void j1Scene::CreatePauseMenu()
{
	//Panel
	panel = App->gui->CreateImage(fPoint(0, 0), App->gui->screen,{ 1504,960,503,717 }, true);
	App->gui->SetPosition(panel,(App->win->GetWindowWidth() - panel->position.w) / 2, (App->win->GetWindowHeight() - panel->position.h) / 2);
	
	//Buttons
	resume_button = App->gui->CreateButton(fPoint((panel->position.w / 2) +80, (panel->position.h / 2) -250), panel, {77, 109, 363, 178}, {462, 109, 362, 178}, {845, 109, 362, 178}, UI::Button_Type::Resume);
	resume_label = App->gui->CreateLabel(fPoint((panel->position.w / 2) + 180, (panel->position.h / 2) - 220), resume_button, "Resume", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 100);

	exit_button = App->gui->CreateButton(fPoint((panel->position.w / 2) + 80, (panel->position.h / 2) + 120), panel, { 77, 109, 363, 178 }, { 462, 109, 362, 178 }, { 845, 109, 362, 178 }, UI::Button_Type::Exit);
	exit_label = App->gui->CreateLabel(fPoint((panel->position.w / 2) + 215, (panel->position.h / 2) + 150), resume_button, "Exit", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 100);

	settings_button = App->gui->CreateButton(fPoint((panel->position.w / 2) + 80, (panel->position.h / 2) - 65), panel, { 77, 109, 363, 178 }, { 462, 109, 362, 178 }, { 845, 109, 362, 178 }, UI::Button_Type::Settings_Menu);
	settings_label = App->gui->CreateLabel(fPoint((panel->position.w / 2) + 150, (panel->position.h / 2) - 25), settings_button, "Settings", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 100);
}

void j1Scene::DestroyPauseMenu()
{
	if (panel != nullptr)
	{
		panel->to_delete = true;
		App->gui->DeleteElement(panel);
		panel = nullptr;

		resume_button->to_delete = true;
		App->gui->DeleteElement(resume_button);
		resume_button = nullptr;
		resume_label->to_delete = true;
		App->gui->DeleteElement(resume_label);
		resume_label = nullptr;

		exit_button->to_delete = true;
		App->gui->DeleteElement(exit_button);
		exit_button = nullptr;
		exit_label->to_delete = true;
		App->gui->DeleteElement(exit_label);
		exit_label = nullptr;

		settings_button->to_delete = true;
		App->gui->DeleteElement(settings_button);
		settings_button = nullptr;
		settings_label->to_delete = true;
		App->gui->DeleteElement(settings_label);
		settings_label = nullptr;
	}	
}

void j1Scene::CreateSettingsMenu()
{
	//Panel
	settings_panel = App->gui->CreateImage(fPoint(0, 0), App->gui->screen, { 1504,960,503,717 }, true);
	App->gui->SetPosition(settings_panel, (App->win->GetWindowWidth() - settings_panel->position.w) / 2, (App->win->GetWindowHeight() - settings_panel->position.h) / 2);
	//Slider
	/*volume_level = App->gui->CreateImage(fPoint(0, 0), settings_panel, {}, true);*/
	slider_volume = App->gui->CreateSlider(fPoint((settings_panel->position.w / 2) + 80, (settings_panel->position.h / 2) - 250), {193, 423, 469, 10}, Slider_TYPE::X, settings_panel);
	thumb_button = App->gui->CreateButton(fPoint(500, 200), slider_volume, { 77, 400, 30, 45 }, { 77, 400, 30, 45 }, { 77, 456, 30, 45 }, UI::Button_Type::Slider);
	slider_volume->AddThumb(thumb_button);
	
	//CheckBox
	fullscreen_checkbox = App->gui->CreateCheckbox(fPoint((settings_panel->position.w / 2) + 280, (settings_panel->position.h / 2) - 60), false, settings_panel, true, UI::CheckBox_Type::Fullscreen, { 829, 573, 177, 178 }, { 1026, 573, 176, 178 }, { 1223, 573, 176, 178 }, { 829, 334, 177, 178 }, { 1026, 334, 176, 178 }, { 1223, 334, 176, 178 });
	fullscreen_label = App->gui->CreateLabel(fPoint((settings_panel->position.w / 2) + 60, (settings_panel->position.h / 2) - 10), to_pause_menu_button, "Fullscreen", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 90);
	//Buttons	 
	to_pause_menu_button = App->gui->CreateButton(fPoint((settings_panel->position.w / 2) + 80, (settings_panel->position.h / 2) + 120), settings_panel, { 77, 109, 363, 178 }, { 462, 109, 362, 178 }, { 845, 109, 362, 178 }, UI::Button_Type::Return);
	to_pause_menu_label = App->gui->CreateLabel(fPoint((settings_panel->position.w / 2) + 200, (settings_panel->position.h / 2) + 150), to_pause_menu_button, "Return", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 100);
}

void j1Scene::DestroySettingsMenu()
{
	settings_panel->to_delete = true;
	App->gui->DeleteElement(settings_panel);
	settings_panel = nullptr;

	slider_volume->to_delete = true;
	App->gui->DeleteElement(slider_volume);
	slider_volume = nullptr;

	thumb_button->to_delete = true;
	App->gui->DeleteElement(thumb_button);
	thumb_button = nullptr;

	fullscreen_checkbox->to_delete = true;
	App->gui->DeleteElement(fullscreen_checkbox);
	fullscreen_checkbox = nullptr;

	fullscreen_label->to_delete = true;
	App->gui->DeleteElement(fullscreen_label);
	fullscreen_label = nullptr;

	to_pause_menu_button->to_delete = true;
	App->gui->DeleteElement(to_pause_menu_button);
	to_pause_menu_button = nullptr;

	to_pause_menu_label->to_delete = true;
	App->gui->DeleteElement(to_pause_menu_label);
	to_pause_menu_label = nullptr;
}
