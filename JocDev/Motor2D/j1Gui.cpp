#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1MainMenu.h"
#include "j1Console.h"
#include "j1FadetoBlack.h"
#include "j1Scene.h";
#include "j1Audio.h"
#include "j1Gui.h"
#include "Button.h"
#include "Label.h"
#include "Image.h"
#include "CheckBox.h"
#include "Slider.h"
#include "InputBox.h"
#include "j1Console.h"
#include "Brofiler/Brofiler.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	App->console->SetLog("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());
	button_sfx = App->audio->LoadFx("audio/fx/click_fx.wav");
	CreateScreen();

	return true;
}

// Update all guis
bool j1Gui::PreUpdate(float dt)
{
	BROFILER_CATEGORY("GUI PreUpdate", Profiler::Color::Gray);

	for (int i = 0; i < objects.count(); ++i)
	{
		if (objects[i] == nullptr)
			continue;

		CheckMouse(objects[i]);
	}

	if (App->input->GetMouseButtonDown(1) == j1KeyState::KEY_DOWN && selected == nullptr)
		Select();

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		debug_ui = !debug_ui;

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate(float dt)
{
	BROFILER_CATEGORY("GUI PostUpdate", Profiler::Color::BlanchedAlmond);

	for (int i = 0; i < objects.count(); ++i) 
	{
		if (objects[i] == nullptr)
			continue;
		objects[i]->PostUpdate();
	}

	p2List_item<UI*>* item = objects.start;
	for (; item != nullptr; item = item->next)
	{
		if (item->data == nullptr)
			continue;
		item->data->Draw();

		if (item->data->to_delete)
		{
			item->data->CleanUp();
			objects.del(item);
			item->data = nullptr;
			/*break;*/
		}
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	for (int i = 0; i < objects.count(); ++i)
	{
		if (objects[i] == nullptr)
			continue;

		objects[i]->CleanUp();
	}
	objects.clear();
	App->tex->UnLoad(atlas);
	return true;
}

void j1Gui::CreateScreen()
{
	if (objects.find(screen) == -1)
		screen = CreateImage(fPoint(0, 0), nullptr, { 0,0,(int)App->win->GetWindowWidth(),(int)App->win->GetWindowHeight() }, false);
}

Button * j1Gui::CreateButton(const fPoint & pos, UI* parent,const SDL_Rect & idle, const SDL_Rect & hover, const SDL_Rect & push, const UI::Button_Type& type)
{
	Button* ret = nullptr;
	ret = new Button(pos.x, pos.y, parent, idle, hover, push, type);
	objects.add(ret);
	return ret;
}

Image* j1Gui::CreateImage(const fPoint & pos, UI* parent, const SDL_Rect & rect, bool drawable)
{
	Image* ret = nullptr;
	ret = new Image(pos.x, pos.y, rect, parent, drawable);
	objects.add(ret);
	return ret;
}

Label* j1Gui::CreateLabel(const fPoint & pos, UI* parent,const char* text, const Color& c,const char* font,const uint& size, uint32 wrap_length, bool ui_element)
{
	Label* ret = nullptr;
	ret = new Label(pos.x, pos.y, text, c,font, size, parent, true, wrap_length);
	if(ui_element)
		objects.add(ret);

	return ret;
}

Slider * j1Gui::CreateSlider(const fPoint & pos, const SDL_Rect & slider_rect, Slider_TYPE type, int value_to_calculate, UI * parent)
{
	Slider* ret = nullptr;
	ret = new Slider(pos.x, pos.y, slider_rect, value_to_calculate, parent, type);
	objects.add(ret);
	return ret;
}

CheckBox * j1Gui::CreateCheckbox(const fPoint & pos, const bool & is_active, UI * parent, bool drawable, UI::CheckBox_Type type,const SDL_Rect & active_idle, const SDL_Rect & active_hover, const SDL_Rect & active_push, const SDL_Rect & disactive_idle, const SDL_Rect & disactive_hover, const SDL_Rect & disactive_push)
{
	CheckBox* ret = nullptr;
	ret = new CheckBox(pos.x, pos.y, is_active, parent, drawable, type, active_idle, active_hover, active_push, disactive_idle, disactive_hover, disactive_push);
	objects.add(ret);
	return ret;
}

InputBox* j1Gui::CreateInputBox(const fPoint& pos, const char* text, UI* parent, Color color, const char* font_path, const SDL_Rect& rect, bool drawable)
{
	InputBox* ret = nullptr;
	ret = new InputBox(pos.x, pos.y, text, color, font_path, rect, drawable, parent);
	objects.add(ret);
	return ret;
}

void j1Gui::CheckMouse(UI *b)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	if (x > b->position.x&&x<b->position.x + b->position.w &&
		y>b->position.y&&y < b->position.y + b->position.h) {
		if (App->input->GetMouseButtonDown(1)) {
			b->m_state = UI::MouseState::PUSH;
			//LOG("PUSH");
		}
		else {
			b->m_state = UI::MouseState::ONHOVER;
			//LOG("ONHOVER");
		}


	}
	else if (b->m_state != UI::MouseState::IDLE) {
		b->m_state = UI::MouseState::IDLE;
		//LOG("IDLE");
	}

}

bool j1Gui::Select() const
{
	const bool ret = true;
	for (int i = 0; i < objects.count(); ++i)
	{
		if (objects[i] == nullptr)
			continue;
		if (objects[i]->m_state == UI::MouseState::PUSH && objects[i]->ui_type == UI::Type::BUTTON)
		{
			dynamic_cast<Button*>(objects[i])->ClickLogic();
			break;
		}
		if (objects[i]->m_state == UI::MouseState::PUSH && objects[i]->ui_type == UI::Type::CHECK_BOX)
		{
			dynamic_cast<CheckBox*>(objects[i])->ClickLogic();
			break;
		}
	}
	return  ret;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

void j1Gui::SetPosition(UI*ui, const int& x, const int& y)
{
	ui->position.x = x;
	ui->position.y = y;
}