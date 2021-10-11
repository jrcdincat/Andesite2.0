#include <iostream>
#include "pch.h"
#include <vector>
#include <SDL_image.h>
#include <box2d.h>

#include <map>
#include "Time/Timer.h"
#include "Game.h"

const int SCREENWIDTH = 640;
const int SCREENHEIGHT = 480;
const int XPOS = SDL_WINDOWPOS_UNDEFINED;
const int YPOS = SDL_WINDOWPOS_UNDEFINED;

 
int main(int argc, char*args[])
{
	bool fullscreen = true;

	// Create and Initialize Game
	Game::GetInstance()->Init("Andesite", XPOS, YPOS, SCREENWIDTH, SCREENHEIGHT, fullscreen);

	// Game Loop
	while (Game::GetInstance()->IsRunning() == true)
	{
		Game::GetInstance()->HandleEvent();
		Game::GetInstance()->Update();
		Game::GetInstance()->Render();
		Timer::GetInstance()->Tick();
	}

	delete Game::GetInstance();
	delete Timer::GetInstance();
	return 0;
}



// Add JSON file for Player data


//const float M2P = 20;
//const float P2M = 1 / M2P;
//bool fullscreen = false;
//SDL_Window* window;
//SDL_Renderer* renderer;
//b2World* physicsWorld;
//
//b2Body* addRect(int x, int y, int w, int h, bool dyn = true)
//{
//	b2BodyDef bodyDef;
//	bodyDef.position.Set(x , y );
//	if (dyn)
//	{
//		bodyDef.type = b2_dynamicBody;
//	}
//	b2Body* body = physicsWorld->CreateBody(&bodyDef);
//	b2PolygonShape shape;
//	shape.SetAsBox(w / 2,  h / 2);
//	b2FixtureDef fixtureDef;
//	fixtureDef.shape = &shape;
//	fixtureDef.density = 1.0;
//	body->CreateFixture(&fixtureDef);
//
//	return body;
//}
//
//class Box {
//	SDL_Rect rec
//
//		body = addRect(x, y, w, h, true);
//	}
//
//
//	void display() {
//		rect.x = body->GetPosition().x;
//		rect.y = body->GetPosition().y;
//		SDL_RenderDrawRect(renderer, &rect);
//		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//		SDL_RenderFillRect(renderer, &rect);
//	}
//};
//
//void Init(const char* TITLE, int xPos, int yPos, int w, int h, bool fullscreen)
//{
//	int isFullScreen = 0;
//	int imgTypeFlag = IMG_INIT_PNG;
//	physicsWorld = new b2World(b2Vec2(0.0, 9.8)); // Create box2d world with 9.8 gravity
//	addRect(SCREENWIDTH / 2, SCREENHEIGHT - 50, SCREENWIDTH, 30, false);
//
//	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
//	{
//		if (fullscreen)
//		{
//			isFullScreen = SDL_WINDOW_FULLSCREEN_DESKTOP;
//		}
//
//		// Create Window
//		window = SDL_CreateWindow(TITLE, xPos, yPos, w, h, isFullScreen);
//		if (!window)
//		{
//			SDL_Log("Failed to create window: %s", SDL_GetError());
//		}
//
//		// Create Renderer
//		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//		if (!renderer)
//		{
//			SDL_Log("Failed to create renderer: %s", SDL_GetError());
//		}
//
//
//		if (!(IMG_Init(imgTypeFlag) & imgTypeFlag))
//		{
//			SDL_Log("Failed to initialize SDL IMG: %s", IMG_GetError());
//		}
//	}
//	else
//	{
//		SDL_Log("ERROR::GAME::INITIALIZE: Unable to initialize SDL: %s", SDL_GetError());
//	}
//}
//
//SDL_Texture* LoadTexture(std::string id, std::string filename) {
//	SDL_Surface* surface = IMG_Load(filename.c_str());
//
//	if (surface == nullptr) {
//		SDL_Log("Failed to load texture: %s, %s", filename.c_str(), SDL_GetError());
//		return nullptr;
//	}
//
//	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
//	if (texture == nullptr) {
//		SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
//		return nullptr;
//	}
//
//	SDL_FreeSurface(surface);
//	surface = nullptr;
//
//	return texture;
//}
//
//void render(std::vector<Box*> boxes,float dt)
//{
//	int velocityIterations = 6;
//	int positionIterations = 2;
//
//	physicsWorld->Step(dt, velocityIterations, positionIterations);
//
//	// Render background
//	const char* img_path = "src\\assets\\images\\Background.png";
//	SDL_RenderClear(renderer);
//	SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
//	SDL_Surface* surface = IMG_Load(img_path);
//	SDL_Texture* background = SDL_CreateTextureFromSurface(renderer, surface);
//	SDL_RenderCopy(renderer, background, NULL, NULL);
//	
//	for (int i = 0; i < boxes.size(); i++)
//	{
//		boxes[i]->display();
//	}
//
//	SDL_FreeSurface(surface);
//	SDL_RenderPresent(renderer);
//}
//
//int main(int argc, char* args[])
//{
//	// Initialize Game
//	Init("Andesite", XPOS, YPOS, SCREENWIDTH, SCREENHEIGHT, fullscreen);
//	bool running = true;
//	Uint32 start;
//	SDL_Event event;
//	std::vector<Box*> boxes;
//	// Game Loop
//	while (running)
//	{
//		Timer::GetInstance()->Tick();
//		while (SDL_PollEvent(&event))
//		{
//			switch (event.type)
//			{
//				case SDL_QUIT:
//					running = false;
//					break;
//
//				case SDL_KEYDOWN:
//					switch (event.key.keysym.sym)
//					{
//						case SDLK_ESCAPE:
//							running = false;
//							break;
//					}
//					break;
//				case SDL_MOUSEBUTTONDOWN: 
//					// addRect(event.button.x, event.button.y, 20, 20, true);
//					boxes.push_back(new Box(event.button.x, event.button.y, 20, 20));
//					break;
//			default:
//				break;
//			}
//		}
//
//		render(boxes, Timer::GetInstance()->GetDeltaTime());
//	}
//	
//
//	// Delete
//	for (int i = 0; i < boxes.size(); i++)
//	{
//		delete boxes[i];
//	}
//	delete physicsWorld;
//	delete Timer::GetInstance();
//	return 0;
//}