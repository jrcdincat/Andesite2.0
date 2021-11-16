#pragma once
#include "Actor.h"
#include "../Graphics/Animation.h"
#include "../Physics/Physics.h"

class Player : public Actor {
	public: 
		Player(Properties* properties);
		~Player();

		virtual void Draw();
		virtual void Update(float dt);
		virtual void UpdateAnimationState();
		virtual inline int GetCurrentState() { return currentState; }

		void Idle();
		void Die();
		void Win();
		b2Body* GetPlayerBody() { return physicsBody; }

		// Player input flag variables
		bool isMoveRight = false;
		bool isMoveLeft = false;
		bool isJump = false;
		bool isEscape = false;

	private: 
		void UpdateMovement();
		void SetVelocityMoveRight();
		void SetVelocityMoveLeft();
		void RunRight();
		void RunLeft();
		void Jump();
		void Fall();
};