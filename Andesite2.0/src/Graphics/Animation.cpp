#include "../pch.h"
#include "Animation.h"
#include "TextureManager.h"

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