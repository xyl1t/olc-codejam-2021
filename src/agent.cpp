#include "agent.hpp"
#include "common.hpp"

Agent::Agent(b2Body* body, BodyType type, SpriteID spriteID) : body(body), type(type), spriteID(spriteID) {
}

Agent::Agent(b2Body* body, BodyType type, SpriteID spriteID, float posX, float posY) : body(body), type(type), spriteID(spriteID) {
	if (body) {
		body->SetTransform({posX, posY}, body->GetAngle());
	}
}
Agent::~Agent() {
	
}

void Agent::Update(float fElapsedTime) {
	// NOTE: selects roation sprite 	
	float angle = normalizeAngle(body->GetAngle());
	float bodyAngle = normalizeAngle(angle + M_PI*2/8 - (M_PI*2/8)/2);
	float val = bodyAngle / (M_PI*2);
	int rot = val * 8;
	spriteID = SpriteID(int(this->type)+rot); // TODO: the sprite id should also be affected by damage!
	
	directionAngle = normalizeAngle(directionAngle);
	float topAngle = normalizeAngle(directionAngle + M_PI*2/8 - (M_PI*2/8)/2);
	val = topAngle / (M_PI*2);
	rot = val * 8;
	topSpriteID = SpriteID(int(this->type) + 8 + rot); // TODO: the sprite id should also be affected by damage!

	
	// NOTE: turn player in the direction of his velocity
	b2Vec2 vel = body->GetLinearVelocity();
	if (vel.Length() > 1) {
		float desired = normalizeAngle(std::atan2f(vel.y, vel.x));
		float smooth{};
		
		if (desired > M_PI + M_PI_2 && angle < M_PI_2) {
			float offset = M_PI*2 - desired;
			angle += offset;
			desired = 0;
			smooth = lerp(angle, desired, 0.15f);
			smooth -= offset;
		} else if (angle > M_PI + M_PI_2 && desired < M_PI) {
			float offset = M_PI*2 - angle;
			angle = 0;
			desired += offset;
			smooth = lerp(angle, desired, 0.15f);
			smooth -= offset;
		} else {
			smooth = lerp(angle, desired, 0.15f);
		}
		
		body->SetTransform(body->GetPosition(), smooth);
	}
	
}