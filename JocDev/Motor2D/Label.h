#ifndef _LABEL_H
#define _LABEL_H

#include "UI.h"
#include "j1Textures.h"


class Label :public UI {
public:
	Label(int pos_x, int pos_y, const char* txt, const Color& c, const char* path_font, const uint& size, UI* parent, bool drawable = true, uint32 wrap_length = 0U);
	~Label() {}
	void CleanUp();
	void InnerDraw();
	void SetColor(Color color);
	void SetColor(SDL_Color color);
	void SetText(const char* text);

	_TTF_Font* font;
	SDL_Texture* texture;
	p2SString		text;
	uint			size;
	SDL_Color		color = { 255, 255, 255, 255 };
};

#endif

