#include "j1App.h"
#include "j1MainMenu.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1FadetoBlack.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "p2Log.h"

j1MainMenu::j1MainMenu()
{
	name.create("main_menu");
}

j1MainMenu::~j1MainMenu()
{
}

bool j1MainMenu::Awake(pugi::xml_node & conf)
{
	LOG("Loading Main Menu");
	bool ret = true;

	return ret;
}

bool j1MainMenu::Start()
{
	bool ret = true;

	App->gui->CreateScreen();

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	CreateMainMenu();

	return ret;
}

bool j1MainMenu::PreUpdate(float dt)
{
	bool ret = true;

	return ret;
}

bool j1MainMenu::Update(float dt)
{
	bool ret = true;

	return ret;
}

bool j1MainMenu::PostUpdate(float dt)
{
	bool ret = true;

	return ret;
}

bool j1MainMenu::CleanUp()
{
	bool ret = true;

	return ret;
}

bool j1MainMenu::Load(pugi::xml_node &)
{
	bool ret = true;

	return ret;
}

bool j1MainMenu::Save(pugi::xml_node &) const
{
	bool ret = true;

	return ret;
}

void j1MainMenu::CreateMainMenu()
{
	panel = App->gui->CreateImage(fPoint(0,0), App->gui->screen, {2356, 861, 1024, 768}, true);
	play_button = App->gui->CreateButton(fPoint((panel->position.w / 2) + 230, (panel->position.h / 2) - 100), panel, { 24, 1904, 167, 82 }, { 200, 1904, 168, 82 }, { 376, 1904, 168, 82 }, UI::Button_Type::Play);
	play_label = App->gui->CreateLabel(fPoint((panel->position.w / 2) + 285, (panel->position.h / 2) - 85), play_button, "Play", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 50);
	continue_button = App->gui->CreateButton(fPoint((panel->position.w / 2) + 230, (panel->position.h / 2) - 10), panel, { 24, 1904, 167, 82 }, { 200, 1904, 168, 82 }, { 376, 1904, 168, 82 }, UI::Button_Type::Continue);
	continue_label = App->gui->CreateLabel(fPoint((panel->position.w / 2) + 262, (panel->position.h / 2) + 5), play_button, "Continue", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 50);
	main_settings_button = App->gui->CreateButton(fPoint((panel->position.w / 2) + 230, (panel->position.h / 2) + 80), panel, { 24, 1904, 167, 82 }, { 200, 1904, 168, 82 }, { 376, 1904, 168, 82 }, UI::Button_Type::Main_Settings_Menu);
	main_settings_label = App->gui->CreateLabel(fPoint((panel->position.w / 2) + 262, (panel->position.h / 2) + 95), play_button, "Settings", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 50);
	credits_button = App->gui->CreateButton(fPoint((panel->position.w / 2) + 230, (panel->position.h / 2) + 170), panel, { 24, 1904, 167, 82 }, { 200, 1904, 168, 82 }, { 376, 1904, 168, 82 }, UI::Button_Type::Credits);
	credits_label = App->gui->CreateLabel(fPoint((panel->position.w / 2) + 265, (panel->position.h / 2) + 185), play_button, "Credits", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 50);
	exit_button = App->gui->CreateButton(fPoint((panel->position.w / 2) + 230, (panel->position.h / 2) + 260), panel, { 24, 1904, 167, 82 }, { 200, 1904, 168, 82 }, { 376, 1904, 168, 82 }, UI::Button_Type::Exit);
	exit_label = App->gui->CreateLabel(fPoint((panel->position.w / 2) + 290, (panel->position.h / 2) + 275), play_button, "Exit", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 50);
}

void j1MainMenu::DestroyMainMenu()
{
	panel->to_delete = true;
	App->gui->DeleteElement(panel);
	panel = nullptr;

	play_button->to_delete = true;
	App->gui->DeleteElement(play_button);
	play_button = nullptr;

	play_label->to_delete = true;
	App->gui->DeleteElement(play_label);
	play_label = nullptr;

	continue_button->to_delete = true;
	App->gui->DeleteElement(continue_button);
	continue_button = nullptr;

	continue_label->to_delete = true;
	App->gui->DeleteElement(continue_label);
	continue_label = nullptr;

	main_settings_button->to_delete = true;
	App->gui->DeleteElement(main_settings_button);
	main_settings_button = nullptr;

	main_settings_label->to_delete = true;
	App->gui->DeleteElement(main_settings_label);
	main_settings_label = nullptr;

	credits_button->to_delete = true;
	App->gui->DeleteElement(credits_button);
	credits_button = nullptr;

	credits_label->to_delete = true;
	App->gui->DeleteElement(credits_label);
	credits_label = nullptr;

	exit_button->to_delete = true;
	App->gui->DeleteElement(exit_button);
	exit_button = nullptr;

	exit_label->to_delete = true;
	App->gui->DeleteElement(exit_label);
	exit_label = nullptr;
}

void j1MainMenu::CreateSettingsMenu()
{
	//Panel
	settings_panel = App->gui->CreateImage(fPoint(0, 0), App->gui->screen, { 1704, 57, 1024, 768 }, true);
	App->gui->SetPosition(settings_panel, (App->win->GetWindowWidth() - settings_panel->position.w) / 2, (App->win->GetWindowHeight() - settings_panel->position.h) / 2);
	//Slider
	/*volume_level = App->gui->CreateImage(fPoint(0, 0), settings_panel, {}, true);*/
	slider_volume = App->gui->CreateSlider(fPoint((settings_panel->position.w / 2) + 80, (settings_panel->position.h / 2) - 250), { 193, 423, 469, 10 }, Slider_TYPE::X, settings_panel);
	thumb_button = App->gui->CreateButton(fPoint(500, 200), slider_volume, { 77, 400, 30, 45 }, { 77, 400, 30, 45 }, { 77, 456, 30, 45 }, UI::Button_Type::Slider);
	slider_volume->AddThumb(thumb_button);

	//CheckBox
	fullscreen_checkbox = App->gui->CreateCheckbox(fPoint((settings_panel->position.w / 2) + 120, (settings_panel->position.h / 2) - 60), false, settings_panel, true, UI::CheckBox_Type::Fullscreen, { 829, 573, 177, 178 }, { 1026, 573, 176, 178 }, { 1223, 573, 176, 178 }, { 829, 334, 177, 178 }, { 1026, 334, 176, 178 }, { 1223, 334, 176, 178 });
	fullscreen_label = App->gui->CreateLabel(fPoint((settings_panel->position.w / 2) -100, (settings_panel->position.h / 2) - 10), fullscreen_checkbox, "Fullscreen", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 90);
	//Buttons	 
	to_main_menu_button = App->gui->CreateButton(fPoint((settings_panel->position.w / 2) - 100, (settings_panel->position.h / 2) + 180), settings_panel, { 24, 1904, 167, 82 }, { 200, 1904, 168, 82 }, { 376, 1904, 168, 82 }, UI::Button_Type::Return);
	to_main_menu_label = App->gui->CreateLabel(fPoint((settings_panel->position.w / 2) - 60, (settings_panel->position.h / 2) + 195), to_main_menu_button, "Return", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 50);
}

void j1MainMenu::DestroySettingsMenu()
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

	to_main_menu_button->to_delete = true;
	App->gui->DeleteElement(to_main_menu_button);
	to_main_menu_button = nullptr;

	to_main_menu_label->to_delete = true;
	App->gui->DeleteElement(to_main_menu_label);
	to_main_menu_label = nullptr;
}

void j1MainMenu::CreateCreditsMenu()
{
}

void j1MainMenu::DestroyCreditsMenu()
{
}
