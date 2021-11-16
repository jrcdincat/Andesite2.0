#pragma once
#include "../Math/Vector2D.h"
#include "../Constants.h"
using namespace constants;

class Camera {
	public:
		static Camera* GetInstance() {
			return instance = (instance != nullptr) ? instance : new Camera(); 
		}
		~Camera() = default;

		void Update(float deltaTime); 

		inline SDL_Rect GetViewBox() { return viewBox; }
		inline Vector2D GetPosition() { return position; }
		inline void SetTarget(Vector2D* targetPoint) { pointTarget = targetPoint; }
		inline Vector2D GetPointTarget() { return Vector2D(pointTarget->x, pointTarget->y); }
		inline void SetMapSize(int width, int height) { MAP_WIDTH = width; MAP_HEIGHT = height; }

	private:
		Camera();
		static Camera* instance;
	
		int MAP_WIDTH, MAP_HEIGHT;
		Vector2D* pointTarget;
		Vector2D position;
		SDL_Rect viewBox;
};