#include "Button.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1MainMenu.h"
#include "j1FadetoBlack.h"
#include "j1Input.h"
#include "j1Console.h"
#include "InputBox.h"
#include "j1Audio.h"


Button::Button(int pos_x, int pos_y, UI* parent, const SDL_Rect& idle, const SDL_Rect& hover, const SDL_Rect& push, const UI::Button_Type& type, bool drawable)
	:UI(pos_x, pos_y, parent, idle.w, idle.h, drawable), idle(idle), hover(hover), push(push), type(type)
{
	ui_type = UI::Type::BUTTON;
}

Button::~Button()
{
}

void Button::CleanUp()
{
}
void Button::InnerDraw()
{
	switch (m_state) {
	case UI::MouseState::ONHOVER:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &hover, true, SDL_FLIP_NONE, 0.0f, 255, true);
		break;
	case UI::MouseState::IDLE:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &idle, true, SDL_FLIP_NONE, 0.0f, 255, true);
		break;
	case UI::MouseState::PUSH:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &push, true, SDL_FLIP_NONE, 0.0f, 255, true);
		break;
	case UI::MouseState::REPEAT:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &push, true, SDL_FLIP_NONE, 0.0f, 255, true);
		break;
	}
}

void Button::ClickLogic()
{
	App->audio->PlayFx(App->gui->button_sfx);
	switch (type)
	{
	case Button_Type::Go_to_Menu:
		App->scene->DestroyPauseMenu();
		App->current_level = 0;
		App->fade->StartfadetoBlack();
		App->Pause();
		break;
	case Button_Type::Settings_Menu:
		App->scene->DestroyPauseMenu();
		App->scene->CreateSettingsMenu();
		break;
	case Button_Type::Exit:
		App->exit = true;
		break;
	case Button_Type::Credits:
		App->main_menu->DestroyMainMenu();
		App->main_menu->CreateCreditsMenu();
		break;
	case Button_Type::Play:
		App->current_level = 1;
		App->main_menu->DestroyMainMenu();
		App->fade->StartfadetoBlack();

		break;
	case Button_Type::Continue:
		App->main_menu->DestroyMainMenu();
		App->LoadGame();
		break;
	case Button_Type::Resume:
		(App->Pause()) ? App->scene->CreatePauseMenu() : App->scene->DestroyPauseMenu();
		break;
	case Button_Type::Slider_Up:
			
		break;
	case Button_Type::Slider_Down:
		
		break;
	case Button_Type::Return:
		if (App->scene->active == true)
		{
			App->scene->DestroySettingsMenu();
			App->scene->CreatePauseMenu();
		}
		else if (App->main_menu->active == true)
		{
			App->main_menu->DestroySettingsMenu();
			App->main_menu->CreateMainMenu();
		}
		break;
	case Button_Type::Main_Settings_Menu:
		App->main_menu->DestroyMainMenu();
		App->main_menu->CreateSettingsMenu();
		break;
	case Button_Type::Return_MainMenu:
		App->main_menu->DestroyCreditsMenu();
		App->main_menu->CreateMainMenu();
		break;
	case Button_Type::Profile_Didac:
		ShellExecuteA(NULL, "open", "https://github.com/didaclis", NULL, NULL, SW_SHOWNORMAL);
		break;
	case Button_Type::Profile_Ivan:
		ShellExecuteA(NULL, "open", "https://github.com/RoperoIvan", NULL, NULL, SW_SHOWNORMAL);
		break;
	case Button_Type::Webpage:
		ShellExecuteA(NULL, "open", "https://roperoivan.github.io/PhotoPlatformer/", NULL, NULL, SW_SHOWNORMAL);
		break;
	case Button_Type::Command:
		ConsoleCommand* command;
		command = App->console->LookForCommand(App->console->console_input->GetText().GetString());

		if (command)
			App->console->ExecuteCommand(command);
		break;
	case Button_Type::No_button:
		break;
	default:
		break;
	}
}