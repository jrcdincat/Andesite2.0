#pragma once
#include "Actor.h"
#include "../Graphics/Animation.h"
#include "../Physics/Physics.h"

class Player : public Actor {
public: 
	Player() = default;
	Player(Properties* properties);
	~Player();

	virtual void Draw();
	virtual void Update(float dt);
	virtual void Clean();

	void Idle();

	bool isMoveRight = false;
	bool isMoveLeft = false;
	bool isJump = false;
	bool isEscape = false;

private: 
	int row, frame, frameCount; 
	int animationSpeed;
	Animation* animation;
	b2Body* physicsBody;
	int collisionWidth, collisionHeight;

	void UpdateMovement();
	void MoveRight();
	void MoveLeft();
	void Jump();
	void Fall();
	void Escape();
};