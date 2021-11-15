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

	inline void SetDetectRange(float range) { detectRange = range; }
	void Fall();
	void Explode();
	virtual void UpdateAnimationState();
	virtual inline int GetCurrentState() { return currentState; }

private:
	void MoveRight();
	void MoveLeft();
	void FallWhenPlayerInRange();

	Player* playerInstance;
	b2Body* playerBody;
	bool isExplode;
	bool isNotVisible;
	float detectRange;
};