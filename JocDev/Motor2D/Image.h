#ifndef _IMAGE_H
#define _IMAGE_H

#include "UI.h"

class Image :public UI {
public:
	Image(int pos_x, int pos_y, const SDL_Rect& r, UI* parent, bool drawable = true);
	~Image();
	void CleanUp();
	void InnerDraw();
	SDL_Rect dimension;
};

#endif