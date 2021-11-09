#include "../pch.h"
#include "SDL_image.h"
#include "TextureManager.h"
#include "../Constants.h"
#include "../Camera/Camera.h"
using namespace constants;

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

void TextureManager::Draw(std::string id, int x, int y, int width, int height, float scaleX, float scaleY, float scrollRatio, SDL_RendererFlip flip) {
	SDL_Rect srcRect = { 0,0, width, height };
	Vector2D cam = Camera::GetInstance()->GetPosition() * scrollRatio;
	SDL_Rect dstRect = { x - cam.x, y - cam.y, width * scaleX, height * scaleY };
	SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), TextureManager::GetInstance()->textureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip) {
	SDL_Rect srcRect = { width * frame, height * row, width, height };
	Vector2D cam = Camera::GetInstance()->GetPosition();
	SDL_Rect dstRect = { x - cam.x, y - cam.y, width, height };
	SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), TextureManager::GetInstance()->textureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawTile(std::string tileSetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip) {
	SDL_Rect srcRect = { tileSize * frame, tileSize * row, tileSize, tileSize };
	Vector2D cam = Camera::GetInstance()->GetPosition();
	SDL_Rect dstRect = { x - cam.x, y - cam.y, tileSize, tileSize };
	SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), textureMap[tileSetID], &srcRect, &dstRect, 0, 0, flip);
}

void TextureManager::DrawStaticTileObject(int imgWidth, int imgHeight, int x, int y, int typeID, SDL_RendererFlip flip) {
	SDL_Rect srcRect = { 0, 0, imgWidth, imgHeight };
	Vector2D cam = Camera::GetInstance()->GetPosition();
	SDL_Rect dstRect = { x - cam.x, y - cam.y, imgWidth, imgHeight};
	SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), textureMap[std::to_string(typeID)], &srcRect, &dstRect, 0, 0, flip);
}

void TextureManager::DrawRect(int x, int y, int w, int h)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h, 
	SDL_SetRenderDrawBlendMode(Game::GetInstance()->GetRenderer(), SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(Game::GetInstance()->GetRenderer(), 0, 0, 0, 150);
	SDL_RenderFillRect(Game::GetInstance()->GetRenderer(), &rect);
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