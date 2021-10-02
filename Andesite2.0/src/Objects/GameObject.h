#pragma once
#include "IObject.h"
#include "../Math/Vector2D.h"

struct Properties {
	Properties(std::string texID, int x, int y, int w, int h, SDL_RendererFlip flip = SDL_FLIP_NONE) :textureID(texID), width(w), height(h), renderFlip(flip) {
		position.x = x;
		position.y = y;
	};

	Vector2D position; 
	int width, height; 
	std::string textureID;
	SDL_RendererFlip renderFlip;
};

class GameObject : public IObject {
public: 
	GameObject() = default;
	GameObject(Properties* properties)
		:textureID(properties->textureID), 
		position(properties->position),
		width(properties->width), height(properties->height), renderFlip(properties->renderFlip) { 
	};
	virtual ~GameObject() {};

	virtual void Draw() = 0;
	virtual void Update(float dt) = 0;
	virtual void Clean() = 0;
	
protected:
	Vector2D position; 
	int width, height; 
	std::string textureID; 
	SDL_RendererFlip renderFlip; 
};