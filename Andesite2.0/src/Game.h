#pragma once
#include "SDL_image.h"
#include "Objects/Player.h"
#include "Input/Command.h"
#include "Input/InputManager.h"
#include "Graphics/TextureManager.h"
#include "Map/GameMap.h"
#include "Map/BackgroundLayer.h"

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
	void CleanGameMap();
	void CreateGameMap();

	inline bool IsRunning() { return isRunning; }
	inline void SetIsRunning(bool status) { isRunning = status; };
	inline SDL_Renderer* GetRenderer() { return renderer; }
	inline GameMap* GetMap() { return gameMap; }
	inline Player* GetPlayer() { return player; }

	int currentGameState;

private:
	Game();
	void CreateEnemies();
	void LoadTextures();

	bool isRunning;
	SDL_Window* window; 
	SDL_Renderer* renderer;
	Player* player;
	Properties* playerProperties;
	Properties* enemyProperties;
	static Game* gameInstance;
	GameMap* gameMap;
	std::vector<BackgroundLayer*> parallaxBackground;
};