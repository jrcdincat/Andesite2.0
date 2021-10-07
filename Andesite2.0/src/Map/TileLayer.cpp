#include "../pch.h"
#include "TileLayer.h"
#include "../Graphics/TextureManager.h"

TileLayer::TileLayer(int tSize, int rowCnt, int colCnt, TileMap tMap, TileSetList tSets): 
	tileSize(tSize), numRow(rowCnt), numCol(colCnt), tileMap(tMap), tileSets(tSets) {

	for (unsigned int i = 0; i < tileSets.size(); i++) {
		TextureManager::GetInstance()->LoadTexture(tileSets[i].name, "src/assets/" + tileSets[i].source);
	}
}

void TileLayer::Render() {
	for (unsigned int row = 0; row < numRow; row++) {
		for (unsigned int col = 0; col < numCol; col++) {
			int tileID = tileMap[row][col];
			if (tileID == 0) {
				continue;
			}

			// Determine which tile set the tile is in
			int index = 0; 
			if (tileSets.size() > 1) {
				for (unsigned int i = 0; i < tileSets.size(); i++) {
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

			TextureManager::GetInstance()->DrawTile(tSet.name, tSet.tileSize, col * tSet.tileSize, row * tSet.tileSize, tRow, tCol);
		}
	}
}
void TileLayer::Update() {

}