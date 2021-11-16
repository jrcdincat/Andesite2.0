#pragma once
#include "Actor.h"
#include "../Graphics/Animation.h"
#include "../Physics/Physics.h"
#include "../Objects/Player.h"

class Golem : public Actor {
public:
	Golem() = default;
	Golem(Properties* properties);
	~Golem();

	virtual void Draw();
	virtual void Update(float dt);
	virtual void Clean();

	inline void SetMovementBoundaries(float leftXBoundary, float rightXBoundary) { 
		movementBoundaryLeft = leftXBoundary; 
		movementBoundaryRight = rightXBoundary; 
	}

	void Idle();
	void Die();
	virtual void UpdateAnimationState();
	virtual inline int GetCurrentState() { return currentState; }

private:
	void MoveRight();
	void MoveLeft();
	void FollowPlayerWhenInRange();
	
	Player* playerInstance;
	b2Body* playerBody;
	float movementBoundaryLeft;
	float movementBoundaryRight;
	bool isCharge;
};
