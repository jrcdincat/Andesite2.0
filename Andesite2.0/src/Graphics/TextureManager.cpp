#include "../pch.h"
#include "SDL_image.h"
#include "TextureManager.h"

TextureManager* TextureManager::textureManagerInstance = nullptr;

TextureManager:: ~TextureManager() {
	
}
bool TextureManager::LoadTexture(std::string id, std::string filename) {
	SDL_Surface* surface = IMG_Load(filename.c_str());

	if (surface == nullptr) {
		SDL_Log("Failed to load texture: %s, %s", filename.c_str(), SDL_GetError());
		return false;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::GetInstance()->GetRenderer(), surface);
	if (texture == nullptr) {
		SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
		return false;
	}

	SDL_FreeSurface(surface);
	surface = nullptr;

	textureMap[id] = texture;
	return true;
}

void TextureManager::Draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip) {
	SDL_Rect srcRect = { 0,0, width, height };
	SDL_Rect dstRect = { x, y, width, height };
	SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), TextureManager::GetInstance()->textureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip) {
	SDL_Rect srcRect = { width * frame, height * row, width, height };
	SDL_Rect dstRect = { x, y, width, height };
	SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), TextureManager::GetInstance()->textureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawTile(std::string tileSetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip) {
	SDL_Rect srcRect = { tileSize * frame, tileSize * row, tileSize, tileSize };
	SDL_Rect dstRect = { x, y, tileSize, tileSize };
	SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), textureMap[tileSetID], &srcRect, &dstRect, 0, 0, flip);
}

void TextureManager::RemoveTexture(std::string id) {
	SDL_DestroyTexture(textureMap[id]);
	textureMap.erase(id);
}
void TextureManager::Clean() {
	std::map<std::string, SDL_Texture*>::iterator i;
	for (i = textureMap.begin(); i != textureMap.end(); i++) {
		SDL_DestroyTexture(i->second);
	}

	textureMap.clear();
	SDL_Log("Texture Map Cleaned Successfully");
}