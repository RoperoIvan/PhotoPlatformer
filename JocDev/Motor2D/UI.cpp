#include "UI.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"

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

	if (App->gui->debug_ui)
		DebugDraw();

	return true;
}

void UI::DebugDraw()
{
	App->render->DrawQuad({ position.x,position.y,position.w,position.h }, 255U, 0U, 0U, 255U, false, false);
}
