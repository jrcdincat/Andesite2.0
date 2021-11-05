#include "../pch.h"
#include "BackgroundLayer.h"
#include "../Graphics/TextureManager.h"
#include "../Camera/Camera.h"

BackgroundLayer::BackgroundLayer(std::string id, int x, int y, float ratio, float scaleX, float scaleY)
{
	textureID = id;
	xOffset = x;
	yOffset = y;
	scrollRatio = ratio;
	xScale = scaleX;
	yScale = scaleY;
	QueryImage(textureID);
}

void BackgroundLayer::QueryImage(std::string id)
{
	SDL_Texture* texture = TextureManager::GetInstance()->GetTexture(textureID);
	SDL_QueryTexture(texture, NULL, NULL, &imageWidth, &imageHeight);
}

void BackgroundLayer::Render()
{
	long playerPosition = Camera::GetInstance()->GetPointTarget().x * scrollRatio;
	long scaledImageXSize = imageWidth* xScale;
	long endOfBackgroundXPosition = xOffset + scaledImageXSize;

	// Reposition background based on player position
	if ( playerPosition > endOfBackgroundXPosition + 0.25 * endOfBackgroundXPosition)
	{
		xOffset += 2 * scaledImageXSize;
	}
	else if (playerPosition < xOffset - (scaledImageXSize * 0.75))
	{
		xOffset -= 2 * scaledImageXSize;
	}
	
	TextureManager::GetInstance()->Draw(
		textureID, 
		xOffset, 
		yOffset, 
		imageWidth, 
		imageHeight, 
		xScale, 
		yScale, 
		scrollRatio, 
		flip
	);
}

void BackgroundLayer::Update() 
{

}
