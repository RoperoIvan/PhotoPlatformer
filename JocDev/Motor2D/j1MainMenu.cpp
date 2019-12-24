#include "j1App.h"
#include "j1MainMenu.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1FadetoBlack.h"
#include "j1Gui.h"
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
	play_button = App->gui->CreateButton(fPoint((panel->position.w / 2) + 80, (panel->position.h / 2) - 250), panel, { 77, 109, 363, 178 }, { 462, 109, 362, 178 }, { 845, 109, 362, 178 }, UI::Button_Type::Play);
	play_label = App->gui->CreateLabel(fPoint((panel->position.w / 2) + 180, (panel->position.h / 2) - 220), play_button, "Play", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 100);

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
}
