#pragma once
#include "../Math/Vector2D.h"
#include "../Constants.h"
using namespace constants;

class Camera {
public:
	~Camera();
	inline static Camera* GetInstance() { return instance = (instance != nullptr) ? instance : new Camera(); }
	inline SDL_Rect GetViewBox() { return viewBox; }
	inline Vector2D GetPosition() { return position; }
	inline void SetTarget(Vector2D* targetPoint) { pointTarget = targetPoint; }
	inline void SetMapSize(int width, int height) { MAP_WIDTH = width; MAP_HEIGHT = height; }
	void Update(float deltaTime); 

private:
	Camera();
	
	int MAP_WIDTH, MAP_HEIGHT;
	Vector2D* pointTarget;
	Vector2D position;
	SDL_Rect viewBox;

	static Camera* instance;
};