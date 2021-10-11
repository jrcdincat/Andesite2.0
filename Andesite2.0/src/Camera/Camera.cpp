#include "../pch.h"
#include "Camera.h"

Camera* Camera::instance = nullptr;

Camera::Camera() {
	viewBox = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	pointTarget = nullptr;
}

Camera::~Camera() {
}

void Camera::Update(float deltaTime) {
	if (pointTarget == nullptr)
		return;
	
	// Camera moves after player moves half way across screen
	viewBox.x = pointTarget->x - SCREEN_WIDTH / 2;
	viewBox.y = pointTarget->y - SCREEN_HEIGHT / 2;

	// Ensure minimum value x and y is 0
	if (viewBox.x < 0)
	{
		viewBox.x = 0;
	}
	if (viewBox.y < 0) {
		viewBox.y = 0;
	}

	if (viewBox.x > (2 * SCREEN_WIDTH - viewBox.w)) {
		viewBox.x = (2 * SCREEN_WIDTH - viewBox.w);
	}
	if (viewBox.y > (SCREEN_HEIGHT - viewBox.h)) {
		viewBox.y = (SCREEN_HEIGHT - viewBox.h);
	}

	position = Vector2D(viewBox.x, viewBox.y);
}