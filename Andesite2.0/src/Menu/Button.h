#pragma once

class Button
{
	public:
		Button(int x, int y, int w, int h);
		~Button() = default;

		bool HandleEvent();
		void Render();

	private:
		SDL_Rect rect;
};