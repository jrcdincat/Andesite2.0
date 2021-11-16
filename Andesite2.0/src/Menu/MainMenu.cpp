#include "../pch.h"
#include "MainMenu.h"
#include "../Game.h"
#include "../Graphics/TextureManager.h"
#include "../Constants.h"
using namespace constants;

MainMenu* MainMenu::mainMenuInstance = nullptr;

MainMenu::MainMenu()
{
	// Create play button 
	int x = SCREEN_WIDTH / 2 - 350 / 2 - 15;
	int y = SCREEN_HEIGHT / 2;
	playButton = new Button(x, y, 350, 125);
}

MainMenu::~MainMenu()
{
	delete playButton;
}

void MainMenu::HandleEvents()
{
	// Start game if play button is pressed
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
