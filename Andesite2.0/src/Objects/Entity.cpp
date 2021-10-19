#include "../pch.h"
#include "Entity.h"

Entity::Entity(Vector2D posParam, SDL_Texture* textureParam)
	:position(posParam), texture(textureParam) {
	// Remove hardcode later
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 32; // 32 pixels
	currentFrame.h = 32;
}
Entity::~Entity() {

}

Vector2D& Entity::GetPos() {
	return position;
}

SDL_Texture* Entity::GetTexture() {
	return texture;
}

SDL_Rect Entity::GetCurrentFrame() {
	return currentFrame;
}