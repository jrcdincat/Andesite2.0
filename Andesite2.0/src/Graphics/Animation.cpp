#include "../pch.h"
#include "Animation.h"
#include "TextureManager.h"

Animation::Animation() {
	spriteRow = 0;
	spriteFrame = 0;
	frameCount = 0;
	animationSpeed = 0;
	rendererFlip = SDL_FLIP_NONE;
}

void Animation::Update() {
	spriteFrame = (SDL_GetTicks() / animationSpeed) % frameCount;
}

void Animation::Draw(float x, float y, int spriteWidth, int spriteHeight) {
	TextureManager::GetInstance()->DrawFrame(textureID, x, y, spriteWidth, spriteHeight, spriteRow, spriteFrame, rendererFlip );
}

void Animation::SetProperties(std::string id, int row, int frameCnt, int animateSpeed, SDL_RendererFlip flip) {
	textureID = id;
	spriteRow = row;
	frameCount= frameCnt;
	animationSpeed = animateSpeed;
	rendererFlip = flip;
}