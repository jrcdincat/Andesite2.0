#include "../pch.h"
#include "GameMap.h"

GameMap::~GameMap() {
	for (unsigned int i = 0; i < mapLayers.size(); i++) {
		delete mapLayers[i];
	}
}

void GameMap::Render() {
	for (unsigned int i = 0; i < mapLayers.size(); i++) {
		if (mapLayers[i] == nullptr)
			break;
		mapLayers[i]->Render();
	}
}

void GameMap::Update() {
	for (unsigned int i = 0; i < mapLayers.size(); i++) {
		if (mapLayers[i] == nullptr)
			break;
		mapLayers[i]->Update();
	}
}