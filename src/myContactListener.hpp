#ifndef MY_CONTACT_LISTENER
#define MY_CONTACT_LISTENER

#include "box2d/box2d.h"
#include "agent.hpp"
#include "projectile.hpp"

class MyContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
};

#endif
