#include "UI.h"

UI::UI(const int pos_x, const int pos_y, UI* parent, const int& width, const int& height, bool drawable) : position({ pos_x, pos_y, width, height }), parent(parent), drawable(drawable)
{
}

UI::~UI()
{
}

iPoint UI::GetGlobalPosition() const
{
	return draw_offset;
}


bool UI::Draw()
{

	draw_offset.x = position.x;
	draw_offset.y = position.y;

	InnerDraw();

	return true;
}