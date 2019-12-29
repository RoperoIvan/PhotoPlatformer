#include "j1App.h"
#include "j1MainMenu.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1FadetoBlack.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "Button.h"
#include "Label.h"
#include "Image.h"
#include "CheckBox.h"
#include "Slider.h"
#include "InputBox.h"

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
	App->audio->PlayMusic("audio/music/bensound-retrosoul.ogg", 2.0);
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
		App->audio->AdjustSliderVolume(slider_volume);
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
	play_button->to_delete = true;
	play_label->to_delete = true;
	continue_button->to_delete = true;
	continue_label->to_delete = true;
	main_settings_button->to_delete = true;
	main_settings_label->to_delete = true;
	credits_button->to_delete = true;
	credits_label->to_delete = true;
	exit_button->to_delete = true;
	exit_label->to_delete = true;
}

void j1MainMenu::CreateSettingsMenu()
{
	//Panel
	settings_panel = App->gui->CreateImage(fPoint(0, 0), App->gui->screen, { 1704, 57, 1024, 768 }, true);
	App->gui->SetPosition(settings_panel, (App->win->GetWindowWidth() - settings_panel->position.w) / 2, (App->win->GetWindowHeight() - settings_panel->position.h) / 2);
	//Slider
	volume_level = App->gui->CreateImage(fPoint((settings_panel->position.w / 2) -40, (settings_panel->position.h / 2) - 250), settings_panel, {193, 423, 469, 10}, true);
	slider_volume = App->gui->CreateSlider(fPoint((settings_panel->position.w / 2) - 40, (settings_panel->position.h / 2) - 250), { 77, 400, 30, 45 }, Slider_TYPE::X,App->audio->GetVolume()*100/ 185, settings_panel);
	volume_label = App->gui->CreateLabel(fPoint((settings_panel->position.w / 2 - 260), (settings_panel->position.h / 2) - 270), settings_panel, "Music volume :", BLACK, "fonts/wolfsbane/wolfsbane2acad.ttf", 50);

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
	volume_level->to_delete = true;
	slider_volume->to_delete = true;
	volume_label->to_delete = true;
	fullscreen_checkbox->to_delete = true;
	fullscreen_label->to_delete = true;
	to_main_menu_button->to_delete = true;
	to_main_menu_label->to_delete = true;
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
	authors_label->to_delete = true;
	didac_button->to_delete = true;
	didac_label->to_delete = true;
	ivan_button->to_delete = true;
	ivan_label->to_delete = true;
	license_text_label->to_delete = true;
	license_label->to_delete = true;
	page_button->to_delete = true;
	page_label->to_delete = true;
	to_main_menu_button->to_delete = true;
	to_main_menu_label->to_delete = true;
}
