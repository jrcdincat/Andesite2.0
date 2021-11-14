#pragma once

class Animation {
public:
	Animation();
	void Update(); 
	void Draw(float x, float y, int spriteWidth, int spriteHeight); 
	void SetProperties(std::string id,bool repeat, int row, int frameCnt, int animateSpeed, SDL_RendererFlip flip = SDL_FLIP_NONE);
	inline bool IsEnded() { return isEnded; }
	inline std::string GetTextureID() { return textureID; }

private:
	int spriteRow;
	int currentFrame;
	int frameCount;
	int animationSpeed;
	bool isEnded;
	bool isRepeated;
	SDL_RendererFlip rendererFlip;
	std::string textureID;
};