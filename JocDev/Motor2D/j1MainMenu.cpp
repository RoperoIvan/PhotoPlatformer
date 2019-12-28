#include "j1App.h"
#include "j1MainMenu.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1FadetoBlack.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Audio.h"
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
	if (slider_volume)
	{
		float vol = slider_volume->slider_value;
		int final_vol = (int)(vol * 180);
		App->audio->SetVolume(final_vol);
	}
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
	//input_console = App->gui->CreateInputBox(fPoint((panel->position.w / 2), (panel->position.h / 2)), "", App->gui->screen, BLACK, "fonts/open_sans/OpenSans-Bold.ttf", { 950,955,239,53 }, true);
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
	volume_level = App->gui->CreateImage(fPoint((settings_panel->position.w / 2), (settings_panel->position.h / 2) - 250), settings_panel, {193, 423, 469, 10}, true);
	slider_volume = App->gui->CreateSlider(fPoint((settings_panel->position.w / 2), (settings_panel->position.h / 2) - 250), { 77, 400, 30, 45 }, Slider_TYPE::X, settings_panel);

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
	credits_panel = App->gui->CreateImage(fPoint(0, 0), App->gui->screen, { 1704, 57, 1024, 768 }, true);
	App->gui->SetPosition(credits_panel, (App->win->GetWindowWidth() - credits_panel->position.w) / 2, (App->win->GetWindowHeight() - credits_panel->position.h) / 2);;
	authors_label = App->gui->CreateLabel(fPoint(80, 50), credits_panel, "Authors", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 90);
	didac_button = App->gui->CreateButton(fPoint(100, 150), credits_panel, { 24, 1904, 167, 82 }, { 200, 1904, 168, 82 }, { 376, 1904, 168, 82 }, UI::Button_Type::Profile_Didac);
	didac_label = App->gui->CreateLabel(fPoint(130, 170), didac_button, "Didac Llop", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 40);
	ivan_button = App->gui->CreateButton(fPoint(100, 250), credits_panel, { 24, 1904, 167, 82 }, { 200, 1904, 168, 82 }, { 376, 1904, 168, 82 }, UI::Button_Type::Profile_Ivan);
	ivan_label = App->gui->CreateLabel(fPoint(120, 270), didac_button, "Ivan Ropero", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 40);
	p2SString license;
	license.create("MIT License\nCopyright(c) 2019\n\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the ''Software''), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions : \n\nThe above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. \n\nTHE SOFTWARE IS PROVIDED ''AS IS'', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
	license_text_label = App->gui->CreateLabel(fPoint(80, 400), credits_panel, license.GetString(), BLACK, "fonts/open_sans/OpenSans-Bold.ttf", 12, 900U);
	license_label = App->gui->CreateLabel(fPoint(80, 340), credits_panel, "License", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 40);
	page_button = App->gui->CreateButton(fPoint(650, 150), credits_panel, { 77, 1158, 87, 87 }, { 171, 1158, 87, 87 }, { 265, 1158, 87, 87 }, UI::Button_Type::Webpage);
	page_label = App->gui->CreateLabel(fPoint(500, 150), credits_panel, "Website", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 70);
	to_main_menu_button = App->gui->CreateButton(fPoint((credits_panel->position.w / 2) - 100, (credits_panel->position.h / 2) + 260), credits_panel, { 24, 1904, 167, 82 }, { 200, 1904, 168, 82 }, { 376, 1904, 168, 82 }, UI::Button_Type::Return_MainMenu);
	to_main_menu_label = App->gui->CreateLabel(fPoint((credits_panel->position.w / 2) - 60, (credits_panel->position.h / 2) + 275), to_main_menu_button, "Return", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 50);
}

void j1MainMenu::DestroyCreditsMenu()
{
	credits_panel->to_delete = true;
	App->gui->DeleteElement(credits_panel);
	credits_panel = nullptr;

	authors_label->to_delete = true;
	App->gui->DeleteElement(authors_label);
	authors_label = nullptr;

	didac_button->to_delete = true;
	App->gui->DeleteElement(didac_button);
	didac_button = nullptr;

	didac_label->to_delete = true;
	App->gui->DeleteElement(didac_label);
	didac_label = nullptr;

	ivan_button->to_delete = true;
	App->gui->DeleteElement(ivan_button);
	ivan_button = nullptr;

	ivan_label->to_delete = true;
	App->gui->DeleteElement(ivan_label);
	ivan_label = nullptr;

	license_text_label->to_delete = true;
	App->gui->DeleteElement(license_text_label);
	license_text_label = nullptr;

	license_label->to_delete = true;
	App->gui->DeleteElement(license_label);
	license_label = nullptr;

	page_button->to_delete = true;
	App->gui->DeleteElement(page_button);
	page_button = nullptr;

	page_label->to_delete = true;
	App->gui->DeleteElement(page_label);
	page_label = nullptr;

	to_main_menu_button->to_delete = true;
	App->gui->DeleteElement(to_main_menu_button);
	to_main_menu_button = nullptr;

	to_main_menu_label->to_delete = true;
	App->gui->DeleteElement(to_main_menu_label);
	to_main_menu_label = nullptr;
}
