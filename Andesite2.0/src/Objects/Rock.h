#pragma once
#include "Actor.h"
#include "../Graphics/Animation.h"
#include "../Physics/Physics.h"
#include "../Objects/Player.h"

class Rock : public Actor {
	public:
		Rock(Properties* properties);
		~Rock();

		virtual void Draw();
		virtual void Update(float dt);
		virtual void UpdateAnimationState();
		virtual inline int GetCurrentState() { return currentState; }

		void Fall();
		void Explode();
		inline void SetDetectRange(float range) { detectRange = range; }

	private:
		void FallWhenPlayerInRange();

		Player* playerInstance;
		b2Body* playerBody;
		bool isExplode;
		bool isNotVisible;
		float detectRange;
		int explosionWidth;
		int explosionHeight;
};