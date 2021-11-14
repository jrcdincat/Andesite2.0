#include "pch.h"
#include "Game.h"
#include "Map/MapParser.h"
#include "Time/Timer.h"
#include "Physics/Physics.h"
#include "Camera/Camera.h"
#include "Objects/Objects.h"

Game* Game::gameInstance = nullptr;
Golem* enemyGolem = nullptr;
Rock* rock;
Properties* rockProperties;

Game::Game()
{
	isRunning = false; 
	window = nullptr;
	renderer = nullptr;
	gameInstance = nullptr;
	player = nullptr;
	playerProperties = nullptr;
	gameMap = nullptr;
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
	}
	else 
	{
		SDL_Log("ERROR::GAME::INITIALIZE: Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}


	if (!MapParser::GetInstance()->Load()) {
		std::cout << "Failed to load map" << std::endl;
	}

	gameMap = MapParser::GetInstance()->GetMap("map");

	LoadTextures();

	// Set Map Background Layers
	parallaxBackground.push_back(new BackgroundLayer("background", 0, -200, 0.3, 0.75, 0.75));
	parallaxBackground.push_back(new BackgroundLayer("background", 2550, -200, 0.3, 0.75, 0.75));
	
	playerProperties = new Properties("player_idle", 1 * PIXEL_PER_METER, 21.5 * PIXEL_PER_METER, 300, 300);
	player = new Player(playerProperties);

	CreateEnemies();

	// Setup Camera
	Camera::GetInstance()->SetMapSize(gameMap->GetMapWidth(), gameMap->GetMapHeight());
	Camera::GetInstance()->SetTarget(player->GetOrigin());

	isRunning = true;
	return isRunning;
}

void Game::HandleEvent() {
	InputManager::GetInstance()->HandleEvent(player);
}



void Game::Update() {
	float deltaTime = Timer::GetInstance()->GetDeltaTime();
	player->Update(deltaTime);
	enemyGolem->Update(deltaTime);
	rock->Update(deltaTime);
	gameMap->Update();
	Camera::GetInstance()->Update(deltaTime);
}

void Game::Render() {
	const char* img_path = "src\\assets\\images\\Background.png";

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	for (auto backgroundLayer : parallaxBackground)
	{
		backgroundLayer->Render();
	}

	gameMap->Render();
	player->Draw();
	enemyGolem->Draw();
	rock->Draw();
	Physics::GetInstance()->Render();
	SDL_RenderPresent(renderer);
}

void Game::Clean() {
	delete player;
	delete enemyGolem;
	delete rock;

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
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

void Game::CreateEnemies()
{
	enemyProperties = new Properties("golem_idle", 17 * PIXEL_PER_METER, 21.5 * PIXEL_PER_METER, 75, 75);
	enemyGolem = new Golem(enemyProperties);

	rockProperties = new Properties("rock1", 1 * PIXEL_PER_METER, 0, 60, 50);
	rock = new Rock(rockProperties);
}

void Game::LoadTextures()
{
	// Load Background Texture
	TextureManager::GetInstance()->LoadTexture("background", "src/assets/images/Background.png");

	// Load Enemy Textures
	TextureManager::GetInstance()->LoadTexture("golem_idle", "src/assets/images/golem/Golem_Idle.png");
	TextureManager::GetInstance()->LoadTexture("golem_walking", "src/assets/images/golem/Golem_Walking.png");
	TextureManager::GetInstance()->LoadTexture("golem_dying", "src/assets/images/golem/Golem_Dying.png");

	// Load Rock Textures
	TextureManager::GetInstance()->LoadTexture("rock1", "src/assets/images/rocks/Rock1.png");
	TextureManager::GetInstance()->LoadTexture("explosion1", "src/assets/images/rocks/Explosion1.png");

	// Load Player Textures
	TextureManager::GetInstance()->LoadTexture("player_idle", "src/assets/images/hero/Sprites/Idle.png");
	TextureManager::GetInstance()->LoadTexture("player_run", "src/assets/images/hero/Sprites/Run.png");
	TextureManager::GetInstance()->LoadTexture("player_jump", "src/assets/images/hero/Sprites/Jump.png");
	TextureManager::GetInstance()->LoadTexture("player_fall", "src/assets/images/hero/Sprites/Fall.png");
	TextureManager::GetInstance()->LoadTexture("player_death", "src/assets/images/hero/Sprites/Death.png");
}