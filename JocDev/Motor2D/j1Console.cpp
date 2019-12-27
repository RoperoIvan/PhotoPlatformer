#include "j1Console.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1Collisions.h"
#include "j1FadetoBlack.h"
#include "j1Gui.h"

j1Console::j1Console() : j1Module()
{
	name.create("console");
	
}

j1Console::~j1Console()
{
}

bool j1Console::Start()
{
	bool ret = true;
	CreateCommand("map", CommandType::MAP);
	CreateCommand("fps", CommandType::FPS);
	CreateCommand("quit", CommandType::QUIT);
	CreateCommand("god", CommandType::GOD_MODE);
	CreateCommand("list", CommandType::LIST);

	return ret;
}

bool j1Console::Update(float dt)
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN)
	{
		has_console = !has_console;
		(has_console) ? CreateConsole() : DestroyConsole();
	}
	if (logs_label)
		logs_label->text = log_buff;

	return ret;
}

bool j1Console::CleanUp()
{
	p2List_item<ConsoleCommand*>* command = commands.start;
	while (command != NULL)
	{
		delete command->data;
		command->data = nullptr;
		command = command->next;
	}
	return true;
}

void j1Console::CreateConsole()
{
	App->gui->CreateScreen();
	console_panel = App->gui->CreateImage(fPoint(0, 0), App->gui->screen, {1212, 1904, 754, 548}, true);
	console_input = App->gui->CreateInputBox(fPoint(10, console_panel->position.h - 40), "", console_panel, BLACK, "fonts/open_sans/OpenSans-Bold.ttf", { 453, 2409, 717, 24}, true);
	//logs_label = App->gui->CreateLabel(fPoint(0, 0), console_panel, log_buff.GetString(), BLACK, "fonts/open_sans/OpenSans-Bold.ttf", 12, 700U);
	command_button = App->gui->CreateButton(fPoint(console_panel->position.w - 100, console_panel->position.h - 50), console_panel, {1098, 2357, 72, 39}, { 1006, 2357, 72, 39 }, { 916, 2357, 72, 39 }, UI::Button_Type::Command);
	command_label = App->gui->CreateLabel(fPoint(console_panel->position.w - 90, console_panel->position.h - 42), command_button, "Search", BLACK, "fonts/open_sans/OpenSans-Bold.ttf", 15);
}

void j1Console::DestroyConsole()
{
	console_panel->to_delete = true;
	App->gui->DeleteElement(console_panel);
	console_panel = nullptr;

	console_input->to_delete = true;
	App->gui->DeleteElement(console_input);
	console_input = nullptr;

	/*logs_label->to_delete = true;
	App->gui->DeleteElement(logs_label);
	logs_label = nullptr;
	log_buff.Clear();*/

	command_button->to_delete = true;
	App->gui->DeleteElement(command_button);
	command_button = nullptr;

	command_label->to_delete = true;
	App->gui->DeleteElement(command_label);
	command_label = nullptr;
	
}

void j1Console::HideConsole()
{
	console_panel->drawable = !console_panel->drawable;
	console_input->drawable = !console_input->drawable;
	logs_label->drawable = !logs_label->drawable;
}

void j1Console::GetLog(const char* log)
{
	log_buff += log;
}

ConsoleCommand* j1Console::CreateCommand(const char* name, CommandType t, uint argument)
{
	ConsoleCommand* ret = nullptr;
	ret = new ConsoleCommand(name, t, argument);
	commands.add(ret);
	return ret;
}

ConsoleCommand* j1Console::LookForCommand(const char* com)
{
	console_input->GetText().Clear();
	ConsoleCommand* ret = nullptr;
	p2SString c;
	c.create(com);
	for (p2List_item<ConsoleCommand*>* command = commands.start; command != nullptr; command = command->next)
	{
		if (c.Find(command->data->name.GetString()) >= 1)
		{
			ret = command->data;
			if (ret->type == CommandType::MAP || ret->type == CommandType::FPS) //We look for the map number	
					ret->argument = FromCharToInt(c);
			break;
		}
	}

	return ret;
}

void j1Console::ExecuteCommand(ConsoleCommand* com)
{
	switch (com->type)
	{
	case CommandType::NO_TYPE:
		break;
	case CommandType::GOD_MODE:
		if(App->entityManager->player)
			App->collisions->GodMode();
		(App->collisions->god_mode) ? LOG("GOD MODE: ON") : LOG("GOD MODE: OFF");
			
		break;
	case CommandType::LIST:
		GetLog("list: display all the commands god: activate god mode quit: exit the game map<number of map>: go to that specific map fps<number fps>: change the fps cap to that number");

		break;
	case CommandType::MAP:
		if(com->argument == App->current_level)
			LOG("You are already in %i map", com->argument);
		else if(com->argument > 2)
			LOG("The map %i doesn't exists", com->argument);
		else
		{
			LOG("Changing map to %i", com->argument);
			App->current_level = com->argument;
			App->fade->StartfadetoBlack();
		}		
		break;
	case CommandType::QUIT:
		App->exit = true;
		break;
	case CommandType::FPS:
		if(com->argument <= 0)
			LOG("FPS cap has to be greater than 0");
		else
		{
			App->SetFPScap(com->argument);
			LOG("FPS cap now is : %i", com->argument);
		}		
		break;
	}
}

int j1Console::FromCharToInt(p2SString c)
{
	char* nums = nullptr;
	int start = -1;
	int result = -1;
	int finish = -1;
	for (int i = 0; i < c.Length(); i++)
	{
		if (c.GetString()[i] == '>')
			finish = i - 1;
		if (c.GetString()[i] == '<')
		{
			start = i + 1;
		}
	}
	if (start >= 0 && finish >= 0)
	{	
		result = 0;
		for (int j = start; j <= finish; ++j)
		{
			char fps = c.GetString()[j];
			int sign = 1;
			if (fps == '-')
			{
				sign = -1;
				fps++;
			}
			int num = 0;
			num = ((fps)-'0') + num * 10;
			int pot = 1;
			for (int k = 0; k < finish - j; ++k)
			{
				pot *= 10;
			}
			result += num * pot;
		}
	}
		return result;
}