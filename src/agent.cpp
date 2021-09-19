#include "agent.hpp"
#include "common.hpp"

Agent::Agent() : Body() { }
Agent::Agent(b2World& world, BodyType type, SpriteID spriteID, float posX, float posY, b2Shape* shape) : Body(world, true, true, type, spriteID, posX, posY, shape) { 
	this->body->GetFixtureList()[0].SetDensity(1.0f);
	this->body->GetFixtureList()[0].SetRestitution(0.125f);
	this->body->SetLinearDamping(10);
	this->body->SetAngularDamping(7);

}
Agent::~Agent() { }

void Agent::Update(float fElapsedTime) {
	Body::Update(fElapsedTime);
	
	// NOTE: set top sprite based on direction
	directionAngle = normalizeAngle(directionAngle);
	float topAngle = normalizeAngle(directionAngle + M_PI*2/8 - (M_PI*2/8)/2);
	float val = topAngle / (M_PI*2);
	int rot = val * 8;
	topSpriteID = SpriteID(int(this->type) + 8 + rot); // TODO: the sprite id should also be affected by damage!
}

void Agent::Attack() {
	
	// Agent projectile;
	// projectile.isDynamic = true;
	// projectile.isSolid = false;
	
	// b2CircleShape circle;
	// circle.m_radius = 0.125f;
	// b2BodyDef bodyDef;
	// bodyDef.position = body->GetPosition();
	// bodyDef.angle = body->GetAngle();
	// bodyDef.type = b2_dynamicBody;
	// projectile.body = body->GetWorld()->CreateBody(&bodyDef);
	// b2FixtureDef fixtureDef;
	// fixtureDef.shape = &circle;
	// fixtureDef.density = 1;
	// // fixtureDef.restitution = 0.125f;
	// // projectile.body->SetLinearDamping(10);
	// // projectile.body->SetAngularDamping(7);
	// projectile.body->CreateFixture(&fixtureDef);
	// projectile.body->GetUserData().pointer = reinterpret_cast<uintptr_t>(projectile);
	
}
