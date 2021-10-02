#pragma once
// Followed TileMap Youtube tutorial by Madsycode 
#include "Layer.h"
#include <string>; 
#include <vector>;

struct TileSet {
	int firstID, lastID; 
	int numRow, numCol;
	int numTiles;
    int tileSize;
	std::string name;
	std::string source; 
};

using TileSetList = std::vector<TileSet>;
using TileMap = std::vector<std::vector<int> >;

class TileLayer: public Layer {
public: 
	TileLayer(int tSize, int rowCnt, int colCnt, TileMap tMap, TileSetList tSets);
	virtual void Render(); 
	virtual void Update();
	inline TileMap GetTileMap() { return tileMap; }

private: 
	int tileSize; 
	int numRow, numCol;
	TileMap tileMap; 
	TileSetList tileSets;
};