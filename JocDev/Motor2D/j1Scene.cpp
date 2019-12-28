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
	App->map->Load("MainMenu.tmx");
	//App->entityManager->CreateEntity(fPoint(680, 1487), ENTITY_TYPE::COIN);
	start_time = SDL_GetTicks();
	str_time.create("00:00");

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

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		(App->Pause()) ? CreatePauseMenu() : DestroyPauseMenu();
	}
	TimerManage();
	ScoreManage();

	if (slider_volume)
	{
		float vol = slider_volume->slider_value;
		int final_vol = (int)(vol * 180);
		App->audio->SetVolume(final_vol);
	}

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
		App->current_level = 1;
		App->scene->DestroyHUD();
		App->fade->StartfadetoBlack();
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->current_level = 2;
		App->scene->DestroyHUD();
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

	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		App->current_level = 0;
		App->fade->StartfadetoBlack();
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
	if (App->current_level != lvl_stats.attribute("level").as_uint())
		App->current_level = lvl_stats.attribute("level").as_uint();
	
	App->fade->NewLevel();
	return ret;
}

bool j1Scene::Save(pugi::xml_node& node) const
{

	bool ret = true;
	pugi::xml_node lvl_stats = node.append_child("lvl_stats");
	lvl_stats.append_attribute("level") = App->current_level;
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
	resume_label = App->gui->CreateLabel(fPoint((panel->position.w / 2) + 178, (panel->position.h / 2) - 220), resume_button, "Resume", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 100);

	exit_button = App->gui->CreateButton(fPoint((panel->position.w / 2) + 80, (panel->position.h / 2) + 120), panel, { 77, 109, 363, 178 }, { 462, 109, 362, 178 }, { 845, 109, 362, 178 }, UI::Button_Type::Go_to_Menu);
	exit_label = App->gui->CreateLabel(fPoint((panel->position.w / 2) + 152, (panel->position.h / 2) + 160), exit_button, "Go to Menu", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 80);

	settings_button = App->gui->CreateButton(fPoint((panel->position.w / 2) + 80, (panel->position.h / 2) - 65), panel, { 77, 109, 363, 178 }, { 462, 109, 362, 178 }, { 845, 109, 362, 178 }, UI::Button_Type::Settings_Menu);
	settings_label = App->gui->CreateLabel(fPoint((panel->position.w / 2) + 160, (panel->position.h / 2) - 30), settings_button, "Settings", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 100);
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
	volume_level = App->gui->CreateImage(fPoint((settings_panel->position.w / 2) + 80, (settings_panel->position.h / 2) - 250), settings_panel, {193, 423, 469, 10}, true);
	slider_volume = App->gui->CreateSlider(fPoint((settings_panel->position.w / 2) + 80, (settings_panel->position.h / 2) - 250), { 77, 400, 30, 45 }, Slider_TYPE::X, settings_panel);
	slider_label = App->gui->CreateLabel(fPoint((settings_panel->position.w / 2 - 40), (settings_panel->position.h / 2) - 270), settings_panel, "Music volume :", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 50);
	//CheckBox
	fullscreen_checkbox = App->gui->CreateCheckbox(fPoint((settings_panel->position.w / 2) + 280, (settings_panel->position.h / 2) - 60), false, settings_panel, true, UI::CheckBox_Type::Fullscreen, { 829, 573, 177, 178 }, { 1026, 573, 176, 178 }, { 1223, 573, 176, 178 }, { 829, 334, 177, 178 }, { 1026, 334, 176, 178 }, { 1223, 334, 176, 178 });
	fullscreen_label = App->gui->CreateLabel(fPoint((settings_panel->position.w / 2) + 60, (settings_panel->position.h / 2) - 10), to_pause_menu_button, "Fullscreen", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 90);
	//Buttons	 
	to_pause_menu_button = App->gui->CreateButton(fPoint((settings_panel->position.w / 2) + 80, (settings_panel->position.h / 2) + 120), settings_panel, { 77, 109, 363, 178 }, { 462, 109, 362, 178 }, { 845, 109, 362, 178 }, UI::Button_Type::Return);
	to_pause_menu_label = App->gui->CreateLabel(fPoint((settings_panel->position.w / 2) + 170, (settings_panel->position.h / 2) + 150), to_pause_menu_button, "Return", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 100);
}

void j1Scene::DestroySettingsMenu()
{
	settings_panel->to_delete = true;
	App->gui->DeleteElement(settings_panel);
	settings_panel = nullptr;

	slider_volume->to_delete = true;
	App->gui->DeleteElement(slider_volume);
	slider_volume = nullptr;

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

void j1Scene::CreateHUD()
{
	heart_1 = App->gui->CreateImage(fPoint(0, 14), App->gui->screen, { 77, 788, 210, 59 }, false);
	heart_2 = App->gui->CreateImage(fPoint(0, 14), App->gui->screen, { 77, 716, 210, 59 }, false);
	heart_3 = App->gui->CreateImage(fPoint(0, 14), App->gui->screen, { 77, 647, 210, 59 }, true);
	coin = App->gui->CreateImage(fPoint(App->win->GetWindowWidth() - 150, 20), App->gui->screen, { 77, 873, 67, 66 }, true);
	coins_label = App->gui->CreateLabel(fPoint(App->win->GetWindowWidth() - 70, -25), coin, "", BLACK, "fonts/Final_Fantasy_font.ttf", 130);	
	/*points_panel = App->gui->CreateImage(fPoint(5, 70), App->gui->screen, { 62, 1588, 305, 112 }, true);*/
	points_image = App->gui->CreateImage(fPoint(280, 12), App->gui->screen, { 171, 871, 66, 61 }, true);
	points_label = App->gui->CreateLabel(fPoint(350, -30), App->gui->screen, p2SString(("%i"),score).GetString(), BLACK, "fonts/Final_Fantasy_font.ttf", 130);
	timer_label = App->gui->CreateLabel(fPoint(App->win->GetWindowWidth()/2, -28), App->gui->screen, str_time.GetString(), BLACK, "fonts/Final_Fantasy_font.ttf", 130);
}

void j1Scene::DestroyHUD()
{
	heart_1->to_delete = true;
	App->gui->DeleteElement(heart_1);
	heart_1 = nullptr;

	heart_2->to_delete = true;
	App->gui->DeleteElement(heart_2);
	heart_2 = nullptr;

	heart_3->to_delete = true;
	App->gui->DeleteElement(heart_3);
	heart_3 = nullptr;

	coin->to_delete = true;
	App->gui->DeleteElement(coin);
	coin = nullptr;

	coins_label->to_delete = true;
	App->gui->DeleteElement(coins_label);
	coins_label = nullptr;

	points_image->to_delete = true;
	App->gui->DeleteElement(points_image);
	points_image = nullptr;

	points_label->to_delete = true;
	App->gui->DeleteElement(points_label);
	points_label = nullptr;

	timer_label->to_delete = true;
	App->gui->DeleteElement(timer_label);
	timer_label = nullptr;
}

void j1Scene::TimerManage()
{
	if (timer_label)
	{
		str_time.create("%.2i:%.2i", (SDL_GetTicks() - start_time) / 60000, (SDL_GetTicks() - start_time) / 1000 % 60);
		timer_label->SetText(str_time.GetString());
	}
	
}

void j1Scene::ScoreManage()
{
	int time = (SDL_GetTicks() - start_time) / 1000 % 60;
	if ( time - minus_time >= 0)
	{
		minus_time += 10;
		score -= 10;
	}
	points_label->SetText(p2SString(("%i"), score).GetString());
}
