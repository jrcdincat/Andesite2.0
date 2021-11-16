#pragma once

#include <map>
#include <string>
#include "tinyxml.h"
#include "GameMap.h"
#include "TileLayer.h"

class MapParser {
	public:
		~MapParser();
		inline static MapParser* GetInstance() { return instance = (instance != nullptr) ? instance : new MapParser(); }
		inline GameMap* GetMap(std::string id) { return gameMaps[id]; }

		/// Parse map.tmx to create map
		bool Load();

	private:
		MapParser() = default;
		bool Parse(std::string id, std::string src);

		/// Parse through xml tile set inside of the .tmx file to store inside an instance of the TileSet class
		TileSet ParseTileSet(TiXmlElement* xmlTileSet );

		/// Parse through xml tile layer data inside of the .tmx file to store inside the TileLayer class
		TileLayer* ParseTileLayer(TiXmlElement* xmlLayer, TileSetList tileSets, int tileSize, int numRow, int numCol);

		/// Parse through xml collision layer data inside of the .tmx file to store inside the TileLayer class. 
		TileLayer* ParseStaticObjectCollisionLayer(TiXmlElement* xmlLayer, TileSetList tileSets, int tileSize, int numRow, int numCol);

		static MapParser* instance;
		std::map<std::string, GameMap*> gameMaps;
};