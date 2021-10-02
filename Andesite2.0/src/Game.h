#pragma once
#include "SDL_image.h"
#include "Objects/Player.h"
#include "Input/Command.h"
#include "Input/InputManager.h"
#include "Graphics/TextureManager.h"
#include "Map/GameMap.h"

class Game
{
public: 
	static Game* GetInstance() {
		return gameInstance = ( gameInstance != nullptr ) ? gameInstance : new Game();
	}
	~Game(); 

	bool Init(const char* TITLE, int xpos, int ypos, int w, int h, bool fullscreen);

	void HandleEvent();
	void Update();
	void Render();
	void Clean();

	inline bool IsRunning() { return isRunning; }
	inline SDL_Renderer* GetRenderer() { return renderer; }
	inline GameMap* GetMap() { return gameMap; }

private:
	Game();
	bool isRunning;
	SDL_Window* window; 
	SDL_Renderer* renderer;
	Player* player;
	Properties* playerProperties;
	Command* command;
	const Uint8* keyState;
	static Game* gameInstance;
	GameMap* gameMap;
};