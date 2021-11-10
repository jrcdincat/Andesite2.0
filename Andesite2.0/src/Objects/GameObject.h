#pragma once
#include "IObject.h"
#include "../Math/Vector2D.h"
#include "../box2d/box2d.h"
#include "../Graphics/Animation.h"

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
		float x = properties->position.x + properties->width / 2;
		float y = properties->position.y + properties->height / 2;
		origin = new Vector2D(x, y);
	};
	virtual ~GameObject() {
		delete origin;
	};

	inline Vector2D* GetOrigin() { return origin; }

	virtual void Draw() = 0;
	virtual void Update(float dt) = 0;
	virtual void Clean() = 0;
	
protected:
	Vector2D* origin;
	Vector2D position; 
	b2Body* physicsBody;
	int collisionWidth, collisionHeight;
	
	Animation * animation;
	std::string textureID;
	int width, height;
	int row, frameCount;
	int animationSpeed;
	int currentState;
	int previousState;
	SDL_RendererFlip renderFlip;
	SDL_RendererFlip flipSprite;
	SDL_RendererFlip previousFlipSprite;
};