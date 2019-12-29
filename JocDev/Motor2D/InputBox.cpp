#include "InputBox.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "SDL_ttf/include/SDL_ttf.h"
#include "j1Input.h"

InputBox::InputBox(const int& pos_x, const int& pos_y, const char* txt, const Color& c, const char* path_font, const SDL_Rect& rect, bool drawable, UI* parent) : UI(pos_x, pos_y, parent, 0, 0, drawable)
{
	font = App->fonts->Load(path_font, 15);
	ui_type = UI::Type::INPUT_BOX;
	text.create(txt);
	box = rect;
	SetColor(c);
	texture = App->fonts->Print(text.GetString(), color, font);
	App->fonts->CalcSize(txt, position.w, position.h, font);
	cursor = new Animation();
	cursor->PushBack({ 103,1345, 2,15 });
	cursor->PushBack({ 0,0,0,0 });
	cursor->speed = 2.0F;
	uint width_ = 0u;
	App->tex->GetSize(texture, width_, HEIGHT);

}

InputBox::~InputBox()
{
	SDL_StopTextInput();
	if (cursor)
	{
		delete cursor;
		cursor = nullptr;
	}
}

void InputBox::CleanUp()
{
	text.Clear();
	delete cursor;
	cursor = nullptr;
	App->tex->UnLoad(texture);
	App->fonts->DeleteFonts(font);
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
