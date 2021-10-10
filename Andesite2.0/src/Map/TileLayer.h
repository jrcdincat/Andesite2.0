#pragma once

#include <string>; 
#include <vector>;
#include "Layer.h"
#include "../Math/Vector2D.h"

struct TileObject {
	int id;
	int typeID;
	int tileSetGID;
	int imageWidth;
	int imageHeight;
	int collisionWidth;
	int collisionHeight;
	int x, y;
	std::string source;
};

struct TileSet {
	int firstID, lastID; 
	int numRow, numCol;
	int numTiles;
    int tileSize;
	std::string name;
	std::string source;
	std::vector<TileObject> tileObjects;
};

using TileSetList = std::vector<TileSet>;
using TileMap = std::vector<std::vector<int> >;

class TileLayer: public Layer {
public: 
	TileLayer(bool isObjLayer, int tSize, int rowCnt, int colCnt, TileMap tMap, TileSetList tSets);

	virtual void Render(); 
	virtual void Update();
	inline TileMap GetTileMap() { return tileMap; }

	std::vector<TileObject> objects;

private: 
	int tileSize; 
	int numRow, numCol;
	bool isObjectLayer;
	TileMap tileMap; 
	TileSetList tileSets;
};