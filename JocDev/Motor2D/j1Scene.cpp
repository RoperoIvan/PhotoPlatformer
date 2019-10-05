#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1FadetoBlack.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "Player.h"
#include "j1Collisions.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load("Level1.tmx");
	
	App->entityManager->player->data.tiled.texture = App->tex->Load(App->entityManager->player->data.tiled.image_path.GetString());

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate(float dt)
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	DebugKeys();

	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate(float dt)
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::DebugKeys()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->map->Load("Level1.tmx");

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->map->Load("Level2.tmx");

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		App->entityManager->player->position = scene_spawn;

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		App->collisions->debug = !App->collisions->debug;

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		App->collisions->GodMode();
	}
}