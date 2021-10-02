#include "../pch.h"
#include "MapParser.h"

MapParser* MapParser::instance = nullptr;

bool MapParser::Load() {
	return (Parse("map", "src/assets/map/map.tmx"));		
}

void MapParser::Clean() {
	std::map <std::string, GameMap*>::iterator it;
	for (it = gameMaps.begin(); it != gameMaps.end(); it++) {
		delete it->second;
		it->second = nullptr;
	}

	gameMaps.clear();
}

bool MapParser::Parse(std::string id, std::string src) {
	TiXmlDocument xml;
	xml.LoadFile(src);

	if (xml.Error()) {
		std::cerr << "Failed to load: " << src << std::endl;
		return false;
	}

	TiXmlElement* root = xml.RootElement();
	int numRow, numCol, tileSize = 0;
	root->Attribute("width", &numCol);
	root->Attribute("height", &numRow);
	root->Attribute("tilewidth", &tileSize);

	// Loop through all xml tile sets inside the .tmx file and ad them into a TileSetList
	TileSetList tileSets;
	for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
		if (element->Value() == std::string("tileset")) {
			tileSets.push_back(ParseTileSet(element));
		}
	}

	GameMap* gameMap = new GameMap();
	for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
		if (element->Value() == std::string("layer")) {
			TileLayer* tileLayer = ParseTileLayer(element, tileSets, tileSize, numRow, numCol);
			gameMap->mapLayers.push_back(tileLayer);
		}
	}

	gameMaps[id] = gameMap;
	
	return true;
}

TileSet MapParser::ParseTileSet(TiXmlElement* xmlTileSet) {
	TileSet tileSet;
	tileSet.name = xmlTileSet->Attribute("name");
	xmlTileSet->Attribute("firstgid", &tileSet.firstID);
	xmlTileSet->Attribute("tilecount", &tileSet.numTiles);
	tileSet.lastID = (tileSet.firstID + tileSet.numTiles) - 1;
	xmlTileSet->Attribute("columns", &tileSet.numCol);
	tileSet.numRow = tileSet.numTiles / tileSet.numCol;
	xmlTileSet->Attribute("tilewidth", &tileSet.tileSize);

	TiXmlElement* image = xmlTileSet->FirstChildElement();
	tileSet.source = image->Attribute("source");
	return tileSet;
}

TileLayer* MapParser::ParseTileLayer(TiXmlElement* xmlLayer, TileSetList tileSets, int tileSize, int numRow, int numCol) {
	TiXmlElement* data = nullptr;
	for (TiXmlElement* element = xmlLayer->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
		if (element->Value() == std::string("data")) {
			data = element;
			break;
		}
	}

	if (data == nullptr) {
		return nullptr;
	}

	// Parse through layer matrix of tile map (csv data)
	std::string matrix(data->GetText());
	std::istringstream iss(matrix);
	std::string id;

	// Create tile map 2D vector
	TileMap tileMap(numRow, std::vector<int>(numCol, 0));

	for (int row = 0; row < numRow; row++) {
		for (int col = 0; col < numCol; col++) {
			std::getline(iss, id, ',');
			std::stringstream convertor(id);
			convertor >> tileMap[row][col];
			
			// Stop at end of stream
			if (!iss.good()) {
				break;
			}
		}
	}
	return new TileLayer(tileSize, numRow, numCol, tileMap, tileSets);
}