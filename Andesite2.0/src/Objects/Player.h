#pragma once
#include "Actor.h"
#include "../Graphics/Animation.h"

class Player : Actor {
public: 
	Player() = default;
	Player(Properties* properties);
	~Player();

	virtual void Draw();
	virtual void Update(float dt);
	virtual void Clean();

	void MoveRight();
	void MoveLeft();
	void Idle();

private: 
	int row, frame, frameCount; 
	int animationSpeed;
	Animation* animation;
};