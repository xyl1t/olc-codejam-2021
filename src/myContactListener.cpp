#include "myContactListener.hpp"

// TODO: add custom function to body, like OnContact(...)
void MyContactListener::BeginContact(b2Contact* contact) {
	// check if fixture A was a Agent
	Agent* agent{};
	Projectile* projectile{};
	
	uintptr_t bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (bodyUserData) {
		Body* body = reinterpret_cast<Body*>(bodyUserData);
		if (body->type == BodyType::PROJECTILE) {
			projectile = dynamic_cast<Projectile*>(body);
			body = reinterpret_cast<Body*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
			if (body->isDynamic && (body->type == BodyType::ENEMY || body->type == BodyType::PLAYER || body->type == BodyType::BOSS)) {
				agent = dynamic_cast<Agent*>(body);
			}
		}
	}
	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	if (bodyUserData) {
		Body* body = reinterpret_cast<Body*>(bodyUserData);
		if (body->type == BodyType::PROJECTILE) {
			projectile = dynamic_cast<Projectile*>(body);
			body = reinterpret_cast<Body*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
			if (body->isDynamic && (body->type == BodyType::ENEMY || body->type == BodyType::PLAYER || body->type == BodyType::BOSS)) {
				agent = dynamic_cast<Agent*>(body);
			}
		}
	}
	if (agent && projectile) {
		projectile->alive = false;
		if (agent->health > 0) {
			agent->health -= 1;
		}
	}

	// //check if fixture B was a Agent
	// bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	// if (bodyUserData)
	// 	reinterpret_cast<Agent*>(bodyUserData)->startContact();

}
void MyContactListener::EndContact(b2Contact* contact) {
}