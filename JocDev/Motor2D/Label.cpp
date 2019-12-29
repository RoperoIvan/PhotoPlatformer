#include "Label.h"
#include "j1App.h"
#include "j1Render.h"
#include "SDL_ttf/include/SDL_ttf.h"
#include "j1Fonts.h"


Label::Label(int pos_x, int pos_y, const char* txt, const Color& c, const char* path_font, const uint& size, UI* parent, bool drawable, uint32 wrap_length) : UI(pos_x, pos_y, parent, 0, 0, drawable) {
	font = App->fonts->Load(path_font, size);
	text.create(txt);
	SetColor(c);
	if (wrap_length == 0U)
		texture = App->fonts->Print(text.GetString(), color, font);
	else
		texture = App->fonts->PrintWrapped(text.GetString(), color, font, wrap_length);

	App->fonts->CalcSize(txt, position.w, position.h, font);
	ui_type = UI::Type::LABEL;
}

void Label::CleanUp()
{
	App->tex->UnLoad(texture);
	App->fonts->DeleteFonts(font);
	text.Clear();
}

void Label::InnerDraw()
{
	if (drawable)
		App->render->Blit(texture, position.x, position.y, 0, false, SDL_FLIP_NONE, 0.0f, 255, true);

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

void Label::SetText(const char* txt)
{
	text.create(txt);
	App->tex->UnLoad(texture);
	texture = App->fonts->Print(text.GetString(), color, font);
	App->fonts->CalcSize(text.GetString(), position.w, position.h, font);
}