#include "body.hpp"
#include "common.hpp"
#include "map.hpp"
#include "game.hpp"

Body::Body(bool isSolid, bool isDynamic, BodyType type, SpriteID spriteID, float posX, float posY) : isSolid(isSolid), isDynamic(isDynamic), type(type), spriteID(spriteID), body(nullptr), bodyDef(), fixtureDef() {
	bodyDef.position.x = posX;
	bodyDef.position.y = posY;
	if (isDynamic)
		bodyDef.type = b2_dynamicBody;
	if (!fixtureDef.shape)
		fixtureDef.shape = Body::GetRectangleShape();
	fixtureDef.density = 1;
}
Body::Body(bool isSolid, bool isDynamic, BodyType type, SpriteID spriteID, float posX, float posY, b2BodyDef bodyDef, b2FixtureDef fixtureDef) : isSolid(isSolid), isDynamic(isDynamic), type(type), spriteID(spriteID), body(nullptr), bodyDef(bodyDef), fixtureDef(fixtureDef) {
	bodyDef.position.x = posX;
	bodyDef.position.y = posY;
}
Body::~Body() {
}

Body::Body(const Body& other) : map(nullptr), body(nullptr), isSolid(other.isSolid), isDynamic(other.isDynamic), type(other.type), spriteID(other.spriteID), bodyDef(other.bodyDef), fixtureDef(other.fixtureDef) {
}
Body& Body::operator=(const Body& other) {
	this->isSolid = other.isSolid;
	this->isDynamic = other.isDynamic;
	this->type = other.type;
	this->spriteID = other.spriteID;
	this->bodyDef = other.bodyDef;
	this->fixtureDef = other.fixtureDef;
	if (this->map && this->body) {
		this->map->ReaddBody(this);
	}
	return *this;
}
Body::Body(const Body&& other) : map(std::move(other.map)), body(std::move(other.body)), isSolid(other.isSolid), isDynamic(other.isDynamic), type(other.type), spriteID(other.spriteID), bodyDef(other.bodyDef), fixtureDef(other.fixtureDef) {
}
Body& Body::operator=(const Body&& other) {
	map = std::move(other.map);
	body = std::move(other.body);
	this->isSolid = other.isSolid;
	this->isDynamic = other.isDynamic;
	this->type = other.type;
	this->spriteID = other.spriteID;
	this->bodyDef = other.bodyDef;
	this->fixtureDef = other.fixtureDef;
	return *this;
}

void Body::Update(float fElapsedTime) {
	// NOTE: selects roation sprite 	
	float angle = (body ? normalizeAngle(body->GetAngle()) : 0);
	float bodyAngle = normalizeAngle(angle + M_PI*2/8 - (M_PI*2/8)/2);
	float val = bodyAngle / (M_PI*2);
	int rot = val * 8;
	spriteID = SpriteID(int(this->type)+rot);
	
	// NOTE: turn agent in the direction of his velocity
	const b2Vec2& vel = (body ? body->GetLinearVelocity() : b2Vec2());
	if (vel.Length() > 0.1f) {
		float desired = normalizeAngle(std::atan2(vel.y, vel.x));
		float smooth{};
		
		if (desired > M_PI + M_PI_2 && angle < M_PI_2) {
			float offset = M_PI*2 - desired;
			angle += offset;
			desired = 0;
			smooth = lerp(angle, desired, 0.50f);
			smooth -= offset;
		} else if (angle > M_PI + M_PI_2 && desired < M_PI) {
			float offset = M_PI*2 - angle;
			angle = 0;
			desired += offset;
			smooth = lerp(angle, desired, 0.50f);
			smooth -= offset;
		} else {
			smooth = lerp(angle, desired, 0.50f);
		}
		
		if (body)
			body->SetTransform(body->GetPosition(), smooth);
	}
}

void Body::Draw(Game& game, const olc::vf2d& pos) {
	game.DrawSprite(spriteID, pos);
}
