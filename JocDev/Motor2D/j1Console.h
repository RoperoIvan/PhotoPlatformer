#ifndef _J1CONSOLE_H
#define _J1CONSOLE_H

#include "j1Module.h"
#include "p2SString.h"
#include "p2List.h"
#include "p2Log.h"

class Button;
class Label;
class Image;
class InputBox;

enum class CommandType
{
	NO_TYPE,
	QUIT,
	GOD_MODE,
	LIST,
	MAP,
	FPS,
};
class ConsoleCommand
{
public:
	ConsoleCommand(const char* name, CommandType type, uint argument = 0) : name(name), type(type), argument(argument)
	{
	}
	~ConsoleCommand()
	{
	}

public:
	p2SString name;
	j1Module* callback;
	uint argument;
	CommandType type;
};

class j1Console : public j1Module
{
public:
	j1Console();
	~j1Console();

	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void CreateConsole();
	void DestroyConsole();

	void SetLog(p2SString log);
	ConsoleCommand* CreateCommand(const char* name, CommandType t, uint argument = 0);
	ConsoleCommand* LookForCommand(const char*);
	void ExecuteCommand(ConsoleCommand* com);

	int FromCharToInt(p2SString ch_i);

public:
	InputBox* console_input = nullptr;
	Image* console_panel = nullptr;
	Button* command_button = nullptr;
	Label* command_label = nullptr;
	bool has_console = false;
	p2List<p2SString> log_buffers;
	p2List<Label*> logs_labels;
	p2List<ConsoleCommand*> commands;
};




#endif
