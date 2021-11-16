#pragma once
#include <string>
#include "SDL.h"
#include "Layer.h"

class BackgroundLayer : public Layer {
	public: 
		BackgroundLayer(std::string id, int x, int y, float ratio = 1.0f, float scaleX = 1.0f, float scaleY = 1.0f);
		~BackgroundLayer() = default;

		virtual void Render();

		void QueryImage(std::string id);
		inline void SetScrollRatio(float ratio) { scrollRatio = ratio; }
		inline void SetOffset(int x, int y) { xOffset = x; yOffset = y; }
		inline void SetRepeated(bool isRepeat) { isRepeated = isRepeat; }

	private:
		bool isRepeated;
		float scrollRatio;
		std::string textureID;
		float xScale, yScale;
		int xOffset, yOffset;
		int imageWidth, imageHeight;
		SDL_RendererFlip flip;
};