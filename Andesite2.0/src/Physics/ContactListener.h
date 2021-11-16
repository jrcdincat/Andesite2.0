#pragma once
#include <box2d.h>

class ContactListener : public b2ContactListener
{
	public: 
		ContactListener() = default;

		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
};