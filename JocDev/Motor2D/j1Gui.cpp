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
bool j1Gui::PreUpdate()
{
	p2List_item<UI*>* item = objects.start;
	for (; item; item = item->next) {
		CheckMouse((Button*)item->data);
	}
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

Button * j1Gui::CreateButton(const fPoint & pos, const SDL_Rect & idle, const SDL_Rect & hover, const SDL_Rect & push)
{
	Button* ret = nullptr;
	ret = new Button(pos, idle, hover, push);
	objects.add(ret);
	return ret;
}

void j1Gui::Draw() {

}

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

// class Gui ---------------------------------------------------

bool Image::Draw()
{
	bool ret = false;

	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &dimension, SDL_FLIP_NONE, 0.0F);

	return ret;
}

bool Button::Draw()
{
	bool ret = false;
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