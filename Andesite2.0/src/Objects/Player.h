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
	virtual void UpdateAnimationState();
	virtual inline int GetCurrentState() { return currentState; }
	virtual void Idle();
	virtual void Die();
	b2Body* GetPlayerBody() { return physicsBody; }

	bool isMoveRight = false;
	bool isMoveLeft = false;
	bool isJump = false;
	bool isEscape = false;

private: 
	void UpdateMovement();
	void RunRight();
	void RunLeft();
	void SetVelocityMoveRight();
	void SetVelocityMoveLeft();
	void Jump();
	void Fall();
	void Escape();
};