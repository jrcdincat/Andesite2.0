#include "../pch.h"
#include "MainMenu.h"
#include "../Game.h"
#include "../Input/InputManager.h"
#include "../Graphics/TextureManager.h"
#include "../Constants.h"
using namespace constants;

MainMenu* MainMenu::mainMenuInstance = nullptr;

MainMenu::MainMenu()
{
	playButton = new Button(SCREEN_WIDTH / 2 - 350 / 2 - 15, SCREEN_HEIGHT / 2, 350, 125);
}

MainMenu::~MainMenu()
{
	delete playButton;
}

void MainMenu::HandleEvents()
{
	if (playButton->HandleEvent())
	{
		Game::GetInstance()->currentGameState = IN_GAME;
		Game::GetInstance()->CreateGameMap();
	}
}

void MainMenu::Render()
{
	TextureManager::GetInstance()->Draw("menu_background", 0, 0, 1080, 760, 1, 1, 0);
	playButton->Render();
}
