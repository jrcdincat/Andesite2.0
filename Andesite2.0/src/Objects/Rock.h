#pragma once
#pragma once
#include "Actor.h"
#include "../Graphics/Animation.h"
#include "../Physics/Physics.h"
#include "../Objects/Player.h"

class Rock : public Actor {
public:
	Rock() = default;
	Rock(Properties* properties);
	~Rock();

	virtual void Draw();
	virtual void Update(float dt);
	virtual void Clean();

	void Fall();
	void Explode();
	virtual void UpdateAnimationState();
	virtual inline int GetCurrentState() { return currentState; }

private:
	void MoveRight();
	void MoveLeft();
	void RandomSpawnInRangeOfPlayer();

	Vector2D movementBoundaryLeft;
	Vector2D movementBoundaryRight;
	Player* playerInstance;
	b2Body* playerBody;
	bool isExplode;

	float detectRange;
};