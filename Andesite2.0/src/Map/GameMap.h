#pragma once
#include <vector>
#include "Layer.h"

class GameMap {
	public: 
		GameMap() = default;
		~GameMap();
		void Render();

		inline std::vector<Layer*> GetMapLayers() { return mapLayers; }
		inline int GetMapWidth() { return MAP_WIDTH; }
		inline int GetMapHeight() { return MAP_HEIGHT; }

	private:
		friend class MapParser;
		std::vector<Layer*> mapLayers;
		int MAP_WIDTH, MAP_HEIGHT;
};