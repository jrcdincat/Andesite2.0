#pragma once
#include "Actor.h"
#include "../Graphics/Animation.h"
#include "../Physics/Physics.h"

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
	void Escape();

private: 
	int row, frame, frameCount; 
	int animationSpeed;
	Animation* animation;
	b2Body* physicsBody;
	int collisionWidth, collisionHeight;
};