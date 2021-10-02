#pragma once
#include "SDL_image.h";
#include "../Math/Vector2D.h";

class Entity {
public:
	Entity(Vector2D posParam, SDL_Texture* textureParam);
	~Entity();
	Vector2D& GetPos();
	SDL_Texture* GetTexture();
	SDL_Rect GetCurrentFrame();

private: 
	Vector2D position;
	SDL_Rect currentFrame;
	SDL_Texture* texture;
};