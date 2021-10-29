#include "../pch.h"
#include "MapParser.h"

MapParser* MapParser::instance = nullptr;

bool MapParser::Load() {
	return (Parse("map", "src/assets/map.tmx"));		
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

	// Loop through all xml tile sets inside the .tmx file and add them into a TileSetList
	TileSetList tileSets;
	for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
		if (element->Value() == std::string("tileset")) {
			tileSets.push_back(ParseTileSet(element));
		}
	}
	
	GameMap* gameMap = new GameMap();
	for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
		TileLayer* tileLayer;

		if (element->Value() == std::string("layer")) {
			tileLayer = ParseTileLayer(element, tileSets, tileSize, numRow, numCol);
			gameMap->mapLayers.push_back(tileLayer);
		}
		else if (element->Value() == std::string("objectgroup"))
		{
			tileLayer = ParseStaticObjectCollisionLayer(element, tileSets, tileSize, numRow, numCol);
			std::cout << "obj:" << std::endl;
			gameMap->mapLayers.push_back(tileLayer);
		}
	}

	gameMaps[id] = gameMap;
	
	return true;
}

TileSet MapParser::ParseTileSet(TiXmlElement* xmlTileSet) {
	TileSet tileSet;

	// Tileset information
	tileSet.name = xmlTileSet->Attribute("name");
	xmlTileSet->Attribute("firstgid", &tileSet.firstID);
	xmlTileSet->Attribute("tilecount", &tileSet.numTiles);
	tileSet.lastID = (tileSet.firstID + tileSet.numTiles) - 1;
	xmlTileSet->Attribute("columns", &tileSet.numCol);
	tileSet.numRow = tileSet.numCol > 0 ? tileSet.numTiles / tileSet.numCol : 0;
	xmlTileSet->Attribute("tilewidth", &tileSet.tileSize);

	// Tileset with 0 columns is a collection of images with object groups
	if (tileSet.numCol == 0)
	{
		// Get all tile objects
		for (TiXmlElement* element = xmlTileSet->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
		{
			if (element->Value() == std::string("tile"))
			{
				TileObject newTileObject;
				element->Attribute("id", &newTileObject.id);

				for (TiXmlElement* nextElement = element->FirstChildElement(); nextElement != nullptr; nextElement = nextElement->NextSiblingElement())
				{
					if (nextElement->Value() == std::string("image"))
					{
						newTileObject.source = nextElement->Attribute("source");
						nextElement->Attribute("width", &newTileObject.imageWidth);
						nextElement->Attribute("height", &newTileObject.imageHeight);
					}
					else if (nextElement->Value() == std::string("objectgroup"))
					{
						TiXmlElement* objectElement = nextElement->FirstChildElement();
						objectElement->Attribute("width", &newTileObject.collisionWidth);
						objectElement->Attribute("height", &newTileObject.collisionHeight);
						objectElement->Attribute("x", &newTileObject.x);
						objectElement->Attribute("y", &newTileObject.y);
					}
				}
				tileSet.tileObjects.push_back(newTileObject);
			}
		}
	}
	else
	{
		// Set image source of Tileset based on single image
		TiXmlElement* image = xmlTileSet->FirstChildElement();
		tileSet.source = image->Attribute("source");
	}

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
	return new TileLayer(false, tileSize, numRow, numCol, tileMap, tileSets);
}

TileLayer* MapParser::ParseStaticObjectCollisionLayer(TiXmlElement* xmlLayer, TileSetList tileSets, int tileSize, int numRow, int numCol)
{
	TileMap tileMap(numRow, std::vector<int>(numCol, 0));
	TileLayer* tileLayer = new TileLayer(true, tileSize, numRow, numCol, tileMap, tileSets);

	for (TiXmlElement* element = xmlLayer->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
		if (element->Value() == std::string("object"))
		{
			TileObject object;
			element->Attribute("id", &object.id);
			element->Attribute("type", &object.typeID);
			element->Attribute("x", &object.x);
			element->Attribute("y", &object.y);
			element->Attribute("width", &object.imageWidth);
			element->Attribute("height", &object.imageHeight);
			object.tileSetGID = tileSets[1].firstID;

			for (int i = 0; i < tileSets[1].tileObjects.size(); i++)
			{
				if (tileSets[1].tileObjects[i].id == object.typeID)
				{
					object.collisionWidth = tileSets[1].tileObjects[i].collisionWidth;
					object.collisionHeight = tileSets[1].tileObjects[i].collisionHeight;
					break;
				}
			}

			object.physicsBody = Physics::GetInstance()->AddRect(object.x + (object.collisionWidth / 2), object.y + object.collisionHeight, object.collisionWidth, object.collisionHeight, false);
			
			tileLayer->objects.push_back(object);
		}
	}

	return tileLayer;
}
