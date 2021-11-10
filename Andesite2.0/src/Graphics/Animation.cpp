#include "../pch.h"
#include "Animation.h"
#include "TextureManager.h"

Animation::Animation() {
	spriteRow = 0;
	currentFrame = 0;
	frameCount = 0;
	animationSpeed = 0;
	rendererFlip = SDL_FLIP_NONE;
	isEnded = true;
	isRepeated = false;
}

void Animation::Update() {
	if (isRepeated || !isEnded)
	{
		isEnded = false;
		currentFrame = (SDL_GetTicks() / animationSpeed) % frameCount;
	}

	if (!isRepeated && currentFrame == frameCount - 1)
	{
		isEnded = true;
		currentFrame = frameCount - 1;
	}
}

void Animation::Draw(float x, float y, int spriteWidth, int spriteHeight) {
	TextureManager::GetInstance()->DrawFrame(textureID, x, y, spriteWidth, spriteHeight, spriteRow, currentFrame, rendererFlip );
}

void Animation::SetProperties(std::string id, bool repeat, int row, int frameCnt, int animateSpeed, SDL_RendererFlip flip) {
	textureID = id;
	spriteRow = row;
	frameCount= frameCnt;
	animationSpeed = animateSpeed;
	rendererFlip = flip;
	isRepeated = repeat;
}