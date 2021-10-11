#include "../pch.h"
#include "TileLayer.h"
#include "../Graphics/TextureManager.h"

TileLayer::TileLayer(bool isObjLayer, int tSize, int rowCnt, int colCnt, TileMap tMap, TileSetList tSets): 
	isObjectLayer(isObjLayer), tileSize(tSize), numRow(rowCnt), numCol(colCnt), tileMap(tMap), tileSets(tSets) {
	mapTileLayerOffset = 256;

	for (unsigned int i = 0; i < tileSets.size(); i++) {
		TileSet tileSet = tileSets[i];
		if (tileSet.tileObjects.size() > 0)
		{
			for (int i = 0; i < tileSet.tileObjects.size(); i++)
			{
				TextureManager::GetInstance()->LoadTexture(std::to_string(tileSet.tileObjects[i].id), "src/assets/" + tileSet.tileObjects[i].source);
			}
		}
		else
		{
			TextureManager::GetInstance()->LoadTexture(tileSet.name, "src/assets/" + tileSets[i].source);
		}
	}
}


void TileLayer::Render() {
	if (isObjectLayer)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			int setGID = objects[i].tileSetGID;
			// Determine which tile set the object is in
			int index = 0;
			if (tileSets.size() > 1) {
				for (int j = 0; j < tileSets.size(); j++) {
					if (setGID >= tileSets[j].firstID && setGID <= tileSets[j].lastID) {
						index = j;
						break;
					}
				}
			}

			TileSet tSet = tileSets[index]; // set object is located in
			TextureManager::GetInstance()->DrawStaticTileObject(objects[i].imageWidth, objects[i].imageHeight, objects[i].x, objects[i].y, objects[i].typeID);
		}
	}
	else
	{
		for (int row = 0; row < numRow; row++) {
			for (int col = 0; col < numCol; col++) {
				int tileID = tileMap[row][col];
				if (tileID == 0) {
					continue;
				}

				// Determine which tile set the tile is in
				int index = 0;
				if (tileSets.size() > 1) {
					for (int i = 0; i < tileSets.size(); i++) {
						if (tileID >= tileSets[i].firstID && tileID <= tileSets[i].lastID) {
							// TileID with respect to each tile set having a first ID as 1
							tileID = tileID + tileSets[i].numTiles - tileSets[i].lastID;
							index = i;
							break;
						}
					}
				}

				TileSet tSet = tileSets[index]; // set tile is located in
				int tRow = tileID / tSet.numCol; // row tile is in
				int tCol = tileID - tRow * tSet.numCol - 1; // column tile is in

				// Ensure tile in last column is checked
				if (tileID % tSet.numCol == 0) {
					tRow--;
					tCol = tSet.numCol - 1;
				}

				TextureManager::GetInstance()->DrawTile(tSet.name, tSet.tileSize, col * tSet.tileSize, row * tSet.tileSize + mapTileLayerOffset, tRow, tCol);
			}
		}
	}
}
void TileLayer::Update() {

}