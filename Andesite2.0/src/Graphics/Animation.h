#pragma once

class Animation {
public:
	Animation() = default;
	void Update(); 
	void Draw(float x, float y, int spriteWidth, int spriteHeight); 
	void SetProperties(std::string id, int row, int frameCnt, int animateSpeed, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	int spriteRow;
	int spriteFrame;
	int frameCount;
	int animationSpeed;
	std::string textureID;
	SDL_RendererFlip rendererFlip;
};