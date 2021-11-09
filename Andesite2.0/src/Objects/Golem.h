#pragma once
#include "Actor.h"
#include "../Graphics/Animation.h"
#include "../Physics/Physics.h"

class Golem : public Actor {
public:
	Golem() = default;
	Golem(Properties* properties);
	~Golem();

	virtual void Draw();
	virtual void Update(float dt);
	virtual void Clean();

	virtual void Idle();
	virtual void Die();

private:
	int row, frameCount;
	int animationSpeed;
	Animation* animation;
	b2Body* physicsBody;
	int collisionWidth, collisionHeight;

	void MoveRight();
	void MoveLeft();
};