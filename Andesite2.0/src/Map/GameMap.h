#pragma once
#include <vector>
#include "Layer.h"

class GameMap {
public: 
	GameMap() = default;
	~GameMap();
	void Render();
	void Update(); 

	inline std::vector<Layer*> GetMapLayers() { return mapLayers; }
private:
	friend class MapParser;
	std::vector<Layer*> mapLayers;
};