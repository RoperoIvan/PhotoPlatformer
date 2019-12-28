#ifndef _INPUTBOX_H
#define _INPUTBOX_H

#include "UI.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_keyboard.h"
#include "SDL/include/SDL_render.h"

#include "Animation.h"

#define MAX_CHARACTERS 10

class InputBox : public UI
{
public:
	InputBox(const int& pos_x, const int& pos_y, const char* txt, const Color& c, const char* path_font, const SDL_Rect& rect, bool drawable, UI* parent);
	~InputBox();

	void CleanUp();
	void InnerDraw();

	void SetText(const char* txt);

	void SetColor(const Color& c);

	void SetColor(const SDL_Color& c);

	p2SString GetText();

	void ChangeFont(const char* f, const int& size);

public:
	void AddText(const char* txt);
	void DeleteText();
	SDL_Rect box = { 0,0,0,0 };
	p2SString text;
	Animation* cursor = nullptr;
protected:
	SDL_Texture* texture = nullptr;
	SDL_Color color = { 255,255,255,255 };

	SDL_Rect input_box{ 0,0,0,0 };
	_TTF_Font* font;

	uint HEIGHT = 0u;
};

#endif

