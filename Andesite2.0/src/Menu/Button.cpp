#include "../pch.h"
#include "Button.h"
#include "../Graphics/TextureManager.h"
#include "../Input/InputManager.h"
#include "../Game.h"
#include "../Constants.h"
using namespace constants;

Button::Button(int x, int y, int w, int h)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}

bool Button::HandleEvent()
{
	if (InputManager::GetInstance()->HandleButtonEvent(rect))
	{
		return true;
	}
	return false;
}

void Button::Render()
{
	TextureManager::GetInstance()->Draw("button", rect.x, rect.y, rect.w, rect.h, 1, 1, 0);
}