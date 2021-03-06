#pragma once
#include "../Game.h"
#include <map>

class TextureManager {
	public: 
		static TextureManager* GetInstance() { 
			return textureManagerInstance = (textureManagerInstance != nullptr) ? textureManagerInstance : new TextureManager(); 
		}
		~TextureManager();
		
		bool LoadTexture(std::string id, std::string filename);
		void Draw(std::string id, int x, int y, int width, int height, float scaleX, float scaleY, float speedRatio,SDL_RendererFlip flip = SDL_FLIP_NONE);
		void DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void DrawTile(std::string tileSetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void DrawStaticTileObject(int imgWidth, int imgHeight, int x, int y, int typeID, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void DrawRect(int x, int y, int w, int h);

		SDL_Texture* GetTexture(std::string id) { return textureMap[id]; }
		void RemoveTexture(std::string id);

	private: 
		TextureManager() = default;
		static TextureManager* textureManagerInstance;
		std::map<std::string, SDL_Texture*> textureMap;
};