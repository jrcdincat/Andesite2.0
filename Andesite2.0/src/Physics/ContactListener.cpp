#include "../pch.h"
#include "ContactListener.h"
#include "Physics.h"
#include "../Constants.h"
#include "../Objects/Objects.h"
using namespace constants;

void ContactListener::BeginContact(b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	//check if fixture A was the foot sensor
	Physics::FixtureUserData* fixtureUserData = reinterpret_cast<Physics::FixtureUserData*>(fixtureA->GetUserData().pointer);
	if (fixtureUserData->type == USER_TYPE_PLAYER_FEET)
		Physics::GetInstance()->numFootContacts++;
	//check if fixture B was the foot sensor
	fixtureUserData = reinterpret_cast<Physics::FixtureUserData*>(fixtureB->GetUserData().pointer);
	if (fixtureUserData->type == USER_TYPE_PLAYER_FEET)
		Physics::GetInstance()->numFootContacts++;

	int contactDef = fixtureA->GetFilterData().categoryBits | fixtureB->GetFilterData().categoryBits;
	switch (contactDef)
	{
		// Enemy head is hit 
		case PLAYER_FEET | ENEMY_HEAD:
			if (fixtureA->GetFilterData().categoryBits == ENEMY_HEAD)
			{
				Golem* golem = (Golem*)reinterpret_cast<Physics::FixtureUserData*>(fixtureA->GetUserData().pointer)->object;
				if (golem != nullptr)
				{
					golem->Die();
				}
			}
			else
			{
				Golem* golem = (Golem*)reinterpret_cast<Physics::FixtureUserData*>(fixtureB->GetUserData().pointer)->object;
				if (golem != nullptr)
				{
					golem->Die();
				}
			}
			break;

		// Player hit
		case PLAYER | ENEMY:
			if (fixtureA->GetFilterData().categoryBits == PLAYER)
			{
				Player* player = (Player*)reinterpret_cast<Physics::FixtureUserData*>(fixtureA->GetUserData().pointer)->object;
				if (player != nullptr)
				{
					player->Hit();
				}
			}
			else
			{
				Player* player = (Player*)reinterpret_cast<Physics::FixtureUserData*>(fixtureB->GetUserData().pointer)->object;
				if (player != nullptr)
				{
					player->Hit();
				}
			}
	}
}

void ContactListener::EndContact(b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	//check if fixture A was the foot sensor
	Physics::FixtureUserData* fixtureUserData = reinterpret_cast<Physics::FixtureUserData*>(fixtureA->GetUserData().pointer);
	if (fixtureUserData->type == USER_TYPE_PLAYER_FEET)
		Physics::GetInstance()->numFootContacts--;
	//check if fixture B was the foot sensor
	fixtureUserData = reinterpret_cast<Physics::FixtureUserData*>(fixtureB->GetUserData().pointer);
	if (fixtureUserData->type == USER_TYPE_PLAYER_FEET)
		Physics::GetInstance()->numFootContacts--;
}