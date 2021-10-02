#pragma once

#include <map>
#include <string>
#include "tinyxml.h"
#include "GameMap.h"
#include "TileLayer.h"


class MapParser {
public:
	inline static MapParser* GetInstance() { return instance = (instance != nullptr) ? instance : new MapParser(); }
	inline GameMap* GetMap(std::string id) { return gameMaps[id]; }

	bool Load();
	void Clean();

private:
	MapParser() = default;
	bool Parse(std::string id, std::string src);
	/// <summary>
	/// Parse through xml tile set inside of the .tmx file to store inside an instance of the TileSet class
	/// </summary>
	/// <param name="xmlTileSet"></param>
	/// <returns></returns>
	TileSet ParseTileSet(TiXmlElement* xmlTileSet );
	
	/// <summary>
	/// Parse through xml tile layer data inside of the .tmx file to store inside the TileLayer class
	/// </summary>
	/// <param name="xmlLayer"></param>
	/// <param name="tileSets"></param>
	/// <param name="tileSize"></param>
	/// <param name="numRow"></param>
	/// <param name="numCol"></param>
	/// <returns></returns>
	TileLayer* ParseTileLayer(TiXmlElement* xmlLayer, TileSetList tileSets, int tileSize, int numRow, int numCol);

	static MapParser* instance;
	std::map<std::string, GameMap*> gameMaps;
};