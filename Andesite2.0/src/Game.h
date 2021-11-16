#pragma once
#include "SDL_image.h"
#include "Input/Command.h"
#include "Input/InputManager.h"
#include "Graphics/TextureManager.h"
#include "Map/GameMap.h"
#include "Map/BackgroundLayer.h"
#include "Objects/Objects.h"

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
		void CreateGameMap(); // Creates game objects and sets camera target
		void CleanGameMap(); // Deletes game objects 
		void Clean(); // Free all dynamically allocated memory in game

		inline bool IsRunning() { return isRunning; }
		inline void SetIsRunning(bool status) { isRunning = status; };
		inline SDL_Renderer* GetRenderer() { return renderer; }
		inline GameMap* GetMap() { return gameMap; }
		inline Player* GetPlayer() { return player; }

		int currentGameState;

	private:
		Game();
		void CreateEnemies(); // Creates all enemy objects
		void LoadTextures(); // Loads all texture files

		bool isRunning;
		SDL_Window* window; 
		SDL_Renderer* renderer;
		Player* player;
		Properties* playerProperties;
		static Game* gameInstance;
		GameMap* gameMap;
		std::vector<BackgroundLayer*> parallaxBackground;
		std::vector<Golem*> golems;
		std::vector<Rock*> rocks;
		std::vector<Bat*> bats;
};