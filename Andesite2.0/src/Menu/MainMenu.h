#pragma once
#include "Button.h"

class MainMenu
{
	public:
		static MainMenu* GetInstance() {
			return mainMenuInstance = (mainMenuInstance != nullptr) ? mainMenuInstance : new MainMenu();
		}
		~MainMenu();

		void HandleEvents();
		void Render();

	private:
		MainMenu();

		static MainMenu* mainMenuInstance;
		Button* playButton;
};