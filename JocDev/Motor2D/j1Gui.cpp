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
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());
	CreateScreen();

	return true;
}

// Update all guis
bool j1Gui::PreUpdate(float dt)
{
	p2List_item<UI*>* item = objects.start;
	for (; item; item = item->next) {
			CheckMouse(item->data);
	}

	if (App->input->GetMouseButtonDown(1) == j1KeyState::KEY_DOWN && selected == nullptr)
		Select();


	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate(float dt)
{
	for (int i = 0; i < objects.count(); ++i) {
		objects[i]->PostUpdate();
	}

	p2List_item<UI*>* item = objects.start;
	for (; item; item = item->next) {
		if(item->data->drawable)
			item->data->Draw();
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	p2List_item<UI*>* item;
	for (item = objects.start; item; item = item->next) {
		RELEASE(item->data);
	}
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

Label* j1Gui::CreateLabel(const fPoint & pos, UI* parent,const char* text, const Color& c,const char* font,const uint& size, uint32 wrap_length)
{
	Label* ret = nullptr;
	ret = new Label(pos.x, pos.y, text, c,font, size, parent, true, wrap_length);
	objects.add(ret);
	return ret;
}

Slider * j1Gui::CreateSlider(const fPoint & pos, const SDL_Rect & slider_rect, Slider_TYPE type, UI * parent)
{
	Slider* ret = nullptr;
	ret = new Slider(pos.x, pos.y, slider_rect, parent, type);
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
	p2List_item<UI*>* item = objects.start;
	for (; item; item = item->next)
	{
		if (item->data->m_state == UI::MouseState::PUSH && item->data->ui_type == UI::Type::BUTTON)
		{
			dynamic_cast<Button*>(item->data)->ClickLogic();
			break;
		}
		if (item->data->m_state == UI::MouseState::PUSH && item->data->ui_type == UI::Type::CHECK_BOX)
		{
			dynamic_cast<CheckBox*>(item->data)->ClickLogic();
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

void j1Gui::DeleteElement(UI* ui) //TODO: Change this atrocity
{
	p2List_item<UI*>* item = objects.start;
	while (item != nullptr)
	{
		if (item->data->to_delete == true && item->data != nullptr)
		{
			objects.del(item);
			break;
		}
		item = item->next;

	}
}

void j1Gui::SetPosition(UI*ui, const int& x, const int& y)
{
	ui->position.x = x;
	ui->position.y = y;
}

iPoint UI::GetGlobalPosition() const
{
	return draw_offset;
}

// class Gui ---------------------------------------------------

bool UI::Draw()
{

	draw_offset.x = position.x;
	draw_offset.y = position.y;

	if (parent != nullptr) {
		for (UI* p = parent; p; p = p->parent) {
			draw_offset.x += p->position.x;
			draw_offset.y += p->position.y;
		}
	}
	
	/*if (App->gui->ui_debug)
		DebugDraw();*/

	InnerDraw();

	return true;
}
void Image::InnerDraw()
{
	if (drawable)
	{
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &dimension, true, SDL_FLIP_NONE, 0.0F, 255,
			true, false, { parent->GetGlobalPosition().x, parent->GetGlobalPosition().y, parent->position.w,parent->position.h });
	}
	
}
void Button::InnerDraw()
{
	switch (m_state) {
	case UI::MouseState::ONHOVER:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &hover, true,SDL_FLIP_NONE ,0.0f, 255, true);
		break;
	case UI::MouseState::IDLE:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &idle, true,SDL_FLIP_NONE, 0.0f, 255, true);
		break;
	case UI::MouseState::PUSH:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &push, true,SDL_FLIP_NONE, 0.0f, 255, true);
		break;
	case UI::MouseState::REPEAT:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &push, true, SDL_FLIP_NONE, 0.0f, 255, true);
		break;
	}
}

void Button::ClickLogic()
{	
	switch (type)
	{
	case Button_Type::Go_to_Menu:
		App->scene->DestroyPauseMenu();
		App->current_level = 0;
		App->fade->StartfadetoBlack();
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
		App->fade->StartfadetoBlack();
		App->main_menu->DestroyMainMenu();
		break;
	case Button_Type::Continue:
		App->main_menu->DestroyMainMenu();
		App->LoadGame();
		break;
	case Button_Type::Resume:
		(App->Pause()) ? App->scene->CreatePauseMenu() : App->scene->DestroyPauseMenu();
		break;
	case Button_Type::Slider:
		this->position.x = App->input->GetMousePosition().x;
		/*this->position.y = ;*/
			App->audio->SetVolume(dynamic_cast<Slider*>(parent)->GetSliderValue());		
		break;
	case Button_Type::Return:
		if (App->scene->active == true)
		{
			App->scene->DestroySettingsMenu();
			App->scene->CreatePauseMenu();
		}
		else if(App->main_menu->active == true)
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
		ShellExecuteA(NULL, "open", "https://github.com/RoperoIvan/PhotoPlatformer", NULL, NULL, SW_SHOWNORMAL); //TODO: LINK THE REAL WEBPAGE
		break;
	case Button_Type::Command:		
		ConsoleCommand* command;
		command = App->console->LookForCommand(App->console->console_input->GetText().GetString());
		
		if(command)
			App->console->ExecuteCommand(command);
		break;
	case Button_Type::No_button:
		break;
	default:
		break;
	}
}

void Label::InnerDraw()
{
	if(drawable)
		App->render->Blit(texture, position.x, position.y, 0, false ,SDL_FLIP_NONE, 0.0f, 255, true);

}

void Label::SetColor(Color c)
{
	switch (c) {
	case RED:
		color = { 255,0,0,color.a };
		break;
	case GREEN:
		color = { 0,255,0,color.a };
		break;
	case BLUE:
		color = { 0,0,255,color.a };
		break;
	case YELLOW:
		color = { 255,255,0,color.a };
		break;
	case GREY:
		color = { 150,150,150,color.a };
		break;
	case BLACK:
		color = { 0,0,0,color.a };
		break;
	case WHITE:
		color = { 255,255,255,color.a };
		break;
	default:
		color = { 255,255,255,color.a };
		break;
	}
	texture = App->fonts->Print(text.GetString(), color, font);
}

void Label::SetColor(SDL_Color c)
{
	color = c;
	texture = App->fonts->Print(text.GetString(), color, font);
}

void Label::SetText(const char * txt)
{
	text.create(txt);
	App->tex->UnLoad(texture);
	texture = App->fonts->Print(text.GetString(), color, font);
	App->fonts->CalcSize(text.GetString(), position.w, position.h, font);
}

void CheckBox::InnerDraw()
{
	if (drawable)
	{
		if (is_active)
		{
			switch (m_state)
			{
			case UI::MouseState::ONHOVER:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &active_hover, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			case UI::MouseState::IDLE:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &active_idle, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			case UI::MouseState::PUSH:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &active_push, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			case UI::MouseState::REPEAT:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &active_push, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			}
		}
			
			
		else
		{
			switch (m_state)
			{
			case UI::MouseState::ONHOVER:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &disactive_hover, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			case UI::MouseState::IDLE:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &disactive_idle, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			case UI::MouseState::PUSH:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &disactive_push, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			case UI::MouseState::REPEAT:
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &disactive_push, true, SDL_FLIP_NONE, 0.0f, 255, true);
				break;
			}
		}
	}
}

void CheckBox::ClickLogic()
{
	switch (type)
	{
	case CheckBox_Type::No_checkbox:
			break;
	case CheckBox_Type::Fullscreen:
		App->win->fullscreen = !App->win->fullscreen;
		if(App->win->fullscreen)
			SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		else
			SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_SHOWN);
		break;
	}
	is_active = !is_active;
}

void Slider::AddTargets(UI * target)
{
	control.add(target);
}

void Slider::AddThumb(Button * thumb_b)
{
	if (thumb == nullptr) {
		thumb = thumb_b;
	}
	else {
		thumb->to_delete = true;
	}
}

void Slider::SetSliderValueStart(float slider_value)
{
	if (slider_value >= 0.0f && slider_value <= 1.0f) {
		value = slider_value;
	}
	else {
		value = 0;
	}

	//App->gui->SetPosition(thumb, position.x * value, position.y * value);
}

void Slider::InnerDraw()
{
	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &image, false, SDL_FLIP_NONE, 0.0f, 255, true);
}

void Slider::PostUpdate()
{
	//if (thumb->position.x < 0)
	//	thumb->position.x = 0;

	///*if (thumb->position.y < 0)
	//	thumb->position.y = 0;*/

	//if (thumb->position.x > position.w - thumb->position.w)
	//	thumb->position.x = position.w - thumb->position.w;

	//if (thumb->position.y > position.h - thumb->position.h)
	//	thumb->position.y = position.h - thumb->position.h;

	if (type == Slider_TYPE::X)
		value = (float)(thumb->position.x + thumb->position.w / 2) / (float)position.w;
	else
		value = (float)(thumb->position.y + thumb->position.h / 2) / (float)position.h;
}

float Slider::GetSliderValue() const
{
	return value;
}

Button * Slider::GetSliderButton() const
{
	return thumb;
}

void InputBox::InnerDraw()
{
	if (text.Length() <= MAX_CHARACTERS) {
		SDL_StartTextInput();
		if ((char*)App->input->input_text.GetString()[0] != '\0') {
			p2SString t = App->input->input_text;
			App->input->input_text.Clear();
			AddText(t.GetString());
		}
	}
	else
		App->input->input_text.Clear();

	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN && text.GetCapacity() != 0)
	{
		DeleteText();
	}
	//Input Box blit
	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &box, true, SDL_FLIP_NONE, 0.0F, 255,
		true, false, { parent->GetGlobalPosition().x, parent->GetGlobalPosition().y, parent->position.w,parent->position.h });

	uint width_ = 0u, height_ = 0u;
	App->tex->GetSize(texture, width_, height_);
	//Text blit
	if (texture != nullptr)
		App->render->Blit(texture, position.x + 10, position.y, 0, false, SDL_FLIP_NONE, 0.0f, 255, true);
	//Cursor blit
	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x + width_ + 10, position.y + 5, &cursor->GetCurrentFrame(), false, SDL_FLIP_NONE, 0.0F);
}

void InputBox::SetText(const char* txt)
{
	text.create(txt);
	App->tex->UnLoad(texture);
	texture = App->fonts->Print(text.GetString(), color, font);
	App->fonts->CalcSize(text.GetString(), position.w, position.h, font);
}

void InputBox::SetColor(const Color& c)
{
	switch (c) {
	case RED:
		color = { 255,0,0,color.a };
		break;
	case GREEN:
		color = { 0,255,0,color.a };
		break;
	case BLUE:
		color = { 0,0,255,color.a };
		break;
	case YELLOW:
		color = { 255,255,0,color.a };
		break;
	case GREY:
		color = { 150,150,150,color.a };
		break;
	case BLACK:
		color = { 0,0,0,color.a };
		break;
	case WHITE:
		color = { 255,255,255,color.a };
		break;
	default:
		color = { 255,255,255,color.a };
		break;
	}
	texture = App->fonts->Print(text.GetString(), color, font);
}

void InputBox::SetColor(const SDL_Color& c)
{
	color = c;
}

p2SString InputBox::GetText()
{
	return text;
}

void InputBox::ChangeFont(const char* f, const int& size)
{
	App->fonts->Load(f, size);
	texture = App->fonts->Print(text.GetString(), color, font);
}

void InputBox::AddText(const char* txt)
{
	if (text.Length() <= MAX_CHARACTERS) {
		text += txt;
		App->tex->UnLoad(texture);
		texture = App->fonts->Print(text.GetString(), color, font);
		App->fonts->CalcSize(text.GetString(), position.w, position.h, font);
	}
}

void InputBox::DeleteText()
{
	if (text.Length() > 0) {
		int i = 0;
		while (text.Getchar()[i] != '\0')//Remove last character
		{
			i++;
		}
		text.Getchar()[i - 1] = '\0';
		text.Length();
		App->tex->UnLoad(texture);
		texture = App->fonts->Print(text.GetString(), color, font);
	}
}
