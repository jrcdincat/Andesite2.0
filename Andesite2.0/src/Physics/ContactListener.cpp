#include "../pch.h"
#include "ContactListener.h"
#include "Physics.h"

void ContactListener::BeginContact(b2Contact* contact) {
	//check if fixture A was the foot sensor
	Physics::FixtureUserData* fixtureUserData = reinterpret_cast<Physics::FixtureUserData*>(contact->GetFixtureA()->GetUserData().pointer);
	if (fixtureUserData->type == 1)
		Physics::GetInstance()->numFootContacts++;
	//check if fixture B was the foot sensor
	fixtureUserData = reinterpret_cast<Physics::FixtureUserData*>(contact->GetFixtureB()->GetUserData().pointer);
	if (fixtureUserData->type == 1)
		Physics::GetInstance()->numFootContacts++;
}

void ContactListener::EndContact(b2Contact* contact) {
	//check if fixture A was the foot sensor
	Physics::FixtureUserData* fixtureUserData = reinterpret_cast<Physics::FixtureUserData*>(contact->GetFixtureA()->GetUserData().pointer);
	if (fixtureUserData->type == 1)
		Physics::GetInstance()->numFootContacts--;
	//check if fixture B was the foot sensor
	fixtureUserData = reinterpret_cast<Physics::FixtureUserData*>(contact->GetFixtureB()->GetUserData().pointer);
	if (fixtureUserData->type == 1)
		Physics::GetInstance()->numFootContacts--;
}