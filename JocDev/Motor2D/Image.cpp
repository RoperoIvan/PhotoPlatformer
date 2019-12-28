#include "Image.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"


Image::Image(int pos_x, int pos_y, const SDL_Rect& r, UI* parent, bool drawable) :UI(pos_x, pos_y, parent, r.w, r.h, drawable)
{
	dimension = r;
	ui_type = UI::Type::IMAGE;
}

Image::~Image()
{
}

void Image::CleanUp()
{

}
void Image::InnerDraw()
{
	if (drawable)
	{
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &dimension, true, SDL_FLIP_NONE, 0.0F, 255,
			true, false, { parent->GetGlobalPosition().x, parent->GetGlobalPosition().y, parent->position.w,parent->position.h });
	}

}


