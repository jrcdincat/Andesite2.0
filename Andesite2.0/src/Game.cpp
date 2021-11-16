#include "pch.h"
#include "Game.h"
#include "Map/MapParser.h"
#include "Time/Timer.h"
#include "Physics/Physics.h"
#include "Camera/Camera.h"
#include "Objects/Objects.h"
#include "Menu/Button.h"
#include "Audio/AudioManager.h"
#include <fstream>

Game* Game::gameInstance = nullptr;

Game::Game()
{
	isRunning = false; 
	window = nullptr;
	renderer = nullptr;
	gameInstance = nullptr;
	player = nullptr;
	playerProperties = nullptr;
	gameMap = nullptr;
	currentGameState = IN_MAIN_MENU;
}
Game::~Game()
{
	Clean();
}

bool Game::Init(const char* TITLE, int xPos, int yPos, int w, int h, bool fullscreen)
{
	int isFullScreen = 0;
	int imgTypeFlag = IMG_INIT_PNG;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		if (fullscreen)
		{
			isFullScreen = SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		// Create Window
		window = SDL_CreateWindow(TITLE, xPos, yPos, w, h, isFullScreen);
		if (!window)
		{
			SDL_Log("Failed to create window: %s", SDL_GetError());
			return false; 
		}

		// Create Renderer
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (!renderer)
		{
			SDL_Log("Failed to create renderer: %s", SDL_GetError());
			return false; 
		}

		if (!(IMG_Init(imgTypeFlag) & imgTypeFlag))
		{
			SDL_Log("Failed to initialize SDL IMG: %s", IMG_GetError());
			return false;
		}

		// Initialize SDL Audio Mixer
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			SDL_Log("Failed to initialize Mixer: %s", Mix_GetError());
			return false;
		}
		
		// Load all audio files
		if (!AudioManager::GetInstance()->LoadAudio())
		{
			return false;
		}
	}
	else 
	{
		SDL_Log("ERROR::GAME::INITIALIZE: Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Set window icon
	SDL_Surface* icon = IMG_Load("src/assets/images/lava_icon.png");
	SDL_SetWindowIcon(window, icon);


	// Parse through map.tmx data to create game map
	if (!MapParser::GetInstance()->Load()) {
		std::cout << "Failed to load map" << std::endl;
	}
	gameMap = MapParser::GetInstance()->GetMap("map");

	// Load all texture files
	LoadTextures();

	// Set Map Background Layers
	parallaxBackground.push_back(new BackgroundLayer("background", 0, -200, 0.3, 0.75, 0.75));
	parallaxBackground.push_back(new BackgroundLayer("background", 2550, -200, 0.3, 0.75, 0.75));
	
	// Initialize game camera and set map size
	Camera::GetInstance()->SetMapSize(gameMap->GetMapWidth(), gameMap->GetMapHeight());

	// Initialize and set in-game music
	AudioManager::GetInstance()->PlayMusic();

	isRunning = true;
	return isRunning;
}

void Game::HandleEvent() {
	switch (currentGameState)
	{
		case IN_MAIN_MENU:
			MainMenu::GetInstance()->HandleEvents();
			break;

		case IN_GAME:
			InputManager::GetInstance()->HandleEvent(player);
			break;
	}
}

void Game::Update() {
	switch (currentGameState)
	{
		case IN_GAME:
			float deltaTime = Timer::GetInstance()->GetDeltaTime();

			player->Update(deltaTime);

			for (Golem* golem : golems)
			{
				golem->Update(deltaTime);
			}

			for (Rock* rock : rocks)
			{
				rock->Update(deltaTime);
			}

			for (Bat* bat : bats)
			{
				bat->Update(deltaTime);
			}

			gameMap->Update();
			Camera::GetInstance()->Update(deltaTime);
			break;
	}
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	switch (currentGameState)
	{
		case IN_MAIN_MENU:
			MainMenu::GetInstance()->Render();
			if (Game::GetInstance()->GetPlayer() != nullptr && Game::GetInstance()->currentGameState == IN_MAIN_MENU)
			{
				Game::GetInstance()->CleanGameMap();
			}
			break;

		case IN_GAME:
			for (auto backgroundLayer : parallaxBackground)
			{
				backgroundLayer->Render();
			}

			gameMap->Render();

			TextureManager::GetInstance()->DrawFrame("cave_background", 7120, 397, 1000, 450, 0, 0);
			TextureManager::GetInstance()->DrawFrame("controls", 50, 620, 200, 100, 0, 0);

			player->Draw();

			for (Golem* golem : golems)
			{
				golem->Draw();
			}

			for (Rock* rock : rocks)
			{
				rock->Draw();
			}

			for (Bat* bat : bats)
			{
				bat->Draw();
			}

			TextureManager::GetInstance()->DrawFrame("cave_foreground", 7120, 397, 1000, 450, 0, 0);

			if (player->GetCurrentState() == PlayerState::WIN)
			{
				TextureManager::GetInstance()->Draw("win", 150, 30, 500, 400, 1.5, 1.5, 0);
			}

			Physics::GetInstance()->Render();
			break;
	}

	SDL_RenderPresent(renderer);
}

void Game::CreateGameMap()
{
	// Initialize Player
	playerProperties = new Properties(
		"player_idle", 
		PLAYER_X_START_POS * PIXEL_PER_METER,
		PLAYER_Y_START_POS * PIXEL_PER_METER,
		300,
		300
	);
	player = new Player(playerProperties);

	CreateEnemies();

	// Setup Camera
	Camera::GetInstance()->SetTarget(player->GetOrigin());
}

void Game::CleanGameMap()
{
	delete player;
	player = nullptr;

	for (Golem* golem : golems)
	{
		delete golem;
		golem = nullptr;
	}
	golems.clear();

	for (Rock* rock : rocks)
	{
		delete rock;
		rock = nullptr;
	}
	rocks.clear();

	for (Bat* bat : bats)
	{
		delete bat;
		bat = nullptr;
	}
	bats.clear();
}

void Game::Clean() {

	CleanGameMap();

	for (auto backgroundLayer : parallaxBackground)
	{
		delete backgroundLayer;
	}

	delete Camera::GetInstance();
	delete InputManager::GetInstance();
	MapParser::GetInstance()->Clean();
	delete MapParser::GetInstance();
	TextureManager::GetInstance()->Clean();
	delete TextureManager::GetInstance();
	delete Physics::GetInstance();
	delete MainMenu::GetInstance();
	delete AudioManager::GetInstance();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

void Game::CreateEnemies()
{
	float x, y, leftXBoundary, rightXBoundary, range;

	std::ifstream inGolemFile("src/assets/spawn_locations/golem_locations.txt");
	Golem* golem = nullptr;
	Properties* golemProperties = nullptr;
	// Try and read first line of file
	if (!(inGolemFile >> x >> y >> leftXBoundary >> rightXBoundary))
	{
		SDL_Log("ERROR: Failed to parse golem_locations.txt");
	}
	// Parse other lines to create golem enemies
	while (inGolemFile >> x >> y >> leftXBoundary >> rightXBoundary)
	{
		golemProperties = new Properties("golem_idle", x * PIXEL_PER_METER, y * PIXEL_PER_METER, 75, 75);
		golem = new Golem(golemProperties);
		golem->SetMovementBoundaries(leftXBoundary, rightXBoundary);
		golems.push_back(golem);
	}

	std::ifstream inRockFile("src/assets/spawn_locations/rock_locations.txt");
	Rock* rock = nullptr;
	Properties* rockProperties = nullptr;
	// Try and read first line of file
	if (!(inRockFile >> x >> y >> range))
	{
		SDL_Log("ERROR: Failed to parse rock_locations.txt");
	}
	// Parse other lines to create rocks
	while (inRockFile >> x >> y >> range)
	{
		rockProperties = new Properties("rock1", x * PIXEL_PER_METER, y, 60, 50);	
		rock = new Rock(rockProperties);
		rock->SetDetectRange(range);
		rocks.push_back(rock);
	}

	std::ifstream inBatFile("src/assets/spawn_locations/bat_locations.txt");
	Bat* bat = nullptr;
	Properties* batProperties = nullptr;
	// Try and read first line of file
	if (!(inBatFile >> x >> y >> leftXBoundary >> rightXBoundary))
	{
		SDL_Log("ERROR: Failed to parse bat_locations.txt");
	}
	// Parse other lines to create bats
	while (inBatFile >> x >> y >> leftXBoundary >> rightXBoundary)
	{
		batProperties = new Properties("bat_idle", x * PIXEL_PER_METER, y * PIXEL_PER_METER, 102, 80);
		bat = new Bat(batProperties);
		bat->SetMovementBoundaries(leftXBoundary, rightXBoundary);
		bats.push_back(bat);
	}
}

void Game::LoadTextures()
{
	// Load Background Texture
	TextureManager::GetInstance()->LoadTexture("background", "src/assets/images/Background.png");

	// Load Enemy Textures
	TextureManager::GetInstance()->LoadTexture("golem_idle", "src/assets/images/golem/Golem_Idle.png");
	TextureManager::GetInstance()->LoadTexture("golem_walking", "src/assets/images/golem/Golem_Walking.png");
	TextureManager::GetInstance()->LoadTexture("golem_dying", "src/assets/images/golem/Golem_Dying.png");
	TextureManager::GetInstance()->LoadTexture("bat_idle", "src/assets/images/bat/Bat_Idle.png");
	TextureManager::GetInstance()->LoadTexture("bat_flying", "src/assets/images/bat/Bat_Fly.png");
	TextureManager::GetInstance()->LoadTexture("bat_dying", "src/assets/images/bat/Bat_Die.png");

	// Load Rock Textures
	TextureManager::GetInstance()->LoadTexture("rock1", "src/assets/images/rocks/Rock1.png");
	TextureManager::GetInstance()->LoadTexture("explosion1", "src/assets/images/rocks/Explosion1.png");

	// Load Player Textures
	TextureManager::GetInstance()->LoadTexture("player_idle", "src/assets/images/hero/Sprites/Idle.png");
	TextureManager::GetInstance()->LoadTexture("player_run", "src/assets/images/hero/Sprites/Run.png");
	TextureManager::GetInstance()->LoadTexture("player_jump", "src/assets/images/hero/Sprites/Jump.png");
	TextureManager::GetInstance()->LoadTexture("player_fall", "src/assets/images/hero/Sprites/Fall.png");
	TextureManager::GetInstance()->LoadTexture("player_death", "src/assets/images/hero/Sprites/Death.png");

	// Load Cave Textures
	TextureManager::GetInstance()->LoadTexture("cave_background", "src/assets/images/cave/cave_background.png");
	TextureManager::GetInstance()->LoadTexture("cave_foreground", "src/assets/images/cave/cave_foreground.png");

	// Load Menu Textures
	TextureManager::GetInstance()->LoadTexture("button", "src/assets/images/buttons/play_button.png");
	TextureManager::GetInstance()->LoadTexture("menu_background", "src/assets/images/buttons/menu_background.png");

	// Load Text Textures
	TextureManager::GetInstance()->LoadTexture("win", "src/assets/images/text/win.png");
	TextureManager::GetInstance()->LoadTexture("controls", "src/assets/images/text/controls.png");
}
