#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
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

	return true;
}

// Update all guis
bool j1Gui::PreUpdate(float dt)
{
	p2List_item<UI*>* item = objects.start;
	for (; item; item = item->next) {
		if (item->data->ui_type == UI::Type::BUTTON)
			CheckMouse((Button*)item->data);
	}
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate(float dt)
{
	p2List_item<UI*>* item = objects.start;
	for (; item; item = item->next) {
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

Button * j1Gui::CreateButton(const fPoint & pos, const SDL_Rect & idle, const SDL_Rect & hover, const SDL_Rect & push)
{
	Button* ret = nullptr;
	ret = new Button(pos, idle, hover, push);
	objects.add(ret);
	return ret;
}

Image* j1Gui::CreateImage(const fPoint & pos, const SDL_Rect & rect)
{
	Image* ret = nullptr;
	ret = new Image(pos, rect);
	objects.add(ret);
	return ret;
}

Label* j1Gui::CreateLabel(const fPoint & pos, const char* text, const char* font)
{
	Label* ret = nullptr;
	ret = new Label(pos, text, font);
	objects.add(ret);
	return ret;
}

//void j1Gui::Draw() {
//	LOG("GOL");
//}

void j1Gui::CheckMouse(Button *b)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	if (x > b->position.x&&x<b->position.x + b->idle.w &&
		y>b->position.y&&y < b->position.y + b->idle.h) {
		if (App->input->GetMouseButtonDown(1)) {
			b->mouse = Mouse::PUSH;
			LOG("PUSH");
		}
		else {
			b->mouse = Mouse::ONHOVER;
			LOG("ONHOVER");
		}


	}
	else if (b->mouse != Mouse::IDLE) {
		b->mouse = Mouse::IDLE;
		LOG("IDLE");
	}

}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

void j1Gui::DeleteElement(UI* ui)
{
	p2List_item<UI*> item = objects[objects.find(ui)];
	objects.del(&item);
}

// class Gui ---------------------------------------------------

bool Image::Draw()
{
	bool ret = true;

	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &dimension, SDL_FLIP_NONE, 0.0F);

	return ret;
}

bool Button::Draw()
{
	bool ret = true;
	switch (mouse) {
	case Mouse::ONHOVER:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &hover, SDL_FLIP_NONE ,0.0f);
		break;
	case Mouse::IDLE:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &idle, SDL_FLIP_NONE, 0.0f);
		break;
	case Mouse::PUSH:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &push, SDL_FLIP_NONE, 0.0f);
		break;
	}
	return ret;
}

bool Label::Draw()
{
	App->render->Blit(texture, position.x, position.y, 0, SDL_FLIP_NONE, 0.0f);
	return false;
}

bool CheckBox::Draw()
{
	bool ret = true;
	if(is_active)
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &active, SDL_FLIP_NONE, 0.0f);
	else
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &disactive, SDL_FLIP_NONE, 0.0f);
	return ret;
}
