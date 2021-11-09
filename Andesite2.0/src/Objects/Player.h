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

	virtual void Idle();
	virtual void Die();
	void Hit();
	void UpdateAnimationState();
	inline int GetPlayerState() { return currentState; }

	bool isMoveRight = false;
	bool isMoveLeft = false;
	bool isJump = false;
	bool isEscape = false;

private: 
	int row, frame, frameCount; 
	int animationSpeed;
	Animation* animation;
	b2Body* physicsBody;
	int health;
	int collisionWidth, collisionHeight;
	int currentState;
	int previousState;
	SDL_RendererFlip flipSprite;
	SDL_RendererFlip previousFlipSprite;

	void UpdateMovement();
	void RunRight();
	void RunLeft();
	void SetVelocityMoveRight();
	void SetVelocityMoveLeft();
	void Jump();
	void Fall();
	void Escape();

};