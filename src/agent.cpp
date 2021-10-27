#include "agent.hpp"
#include "common.hpp"
#include "game.hpp"
#include "projectile.hpp"
#include <memory>

Agent::Agent(BodyType type, SpriteID spriteID, float posX, float posY) : Body(true, true, type, spriteID, posX, posY) {
	fixtureDef.density = 1.0f;
	fixtureDef.restitution = 0.125f;
	fixtureDef.shape = Body::GetCircleShape();
	bodyDef.linearDamping = 10;
	bodyDef.angularDamping = 7;
}
Agent::Agent(BodyType type, SpriteID spriteID, float posX, float posY, b2BodyDef bodyDef, b2FixtureDef fixtureDef) : Body(true, true, type, spriteID, posX, posY, bodyDef, fixtureDef) {
	
}
Agent::~Agent() { }

void Agent::Update(float fElapsedTime) {
	Body::Update(fElapsedTime);
	
	// NOTE: set top sprite based on direction
	directionAngle = normalizeAngle(directionAngle);
	float topAngle = normalizeAngle(directionAngle + M_PI*2/8 - (M_PI*2/8)/2);
	float val = topAngle / (M_PI*2);
	int rot = val * 8;
	int damageSpriteOffset = (3-health) * 32;
	topSpriteID = SpriteID(int(this->type) + 8 + rot + damageSpriteOffset); 
	spriteID = SpriteID(int(spriteID) + damageSpriteOffset);
}
void Agent::Draw(Game& game, const olc::vf2d& pos) {
	Body::Draw(game, pos);
	game.DrawSprite(topSpriteID, pos);
}

void Agent::Attack() {
	if (map) {
		b2Vec2 directionVec = {std::cos(directionAngle), std::sin(directionAngle)};
		b2Vec2 forceVec = {directionVec.x*100000, directionVec.y*100000};
		b2CircleShape circle;
		circle.m_radius = 0.125f;
		
		std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(GetX() + directionVec.x*0.7f, GetY() + directionVec.y*0.7f);
		projectile->bodyDef.angle = directionAngle;
		map->AddBody(projectile);
		projectile->body->ApplyForceToCenter(forceVec, true);
	}
}
