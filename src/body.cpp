#include "body.hpp"
#include "common.hpp"

static void createWorldBody(Body& body, b2World& world, int x = 0, int y = 0, const b2Shape* shape = nullptr) {
	if (body.isSolid) {
	// if (body.isDynamic) {
		b2BodyDef bodyDef;
		bodyDef.position.x = x;
		bodyDef.position.y = y;
		if (body.isDynamic)
			bodyDef.type = b2_dynamicBody;
		body.body = world.CreateBody(&bodyDef);
		b2FixtureDef fixtureDef;
		if (shape) {
			fixtureDef.shape = shape;
		} else {
			b2PolygonShape rectangle;
			rectangle.SetAsBox(0.5f, 0.5f);
			fixtureDef.shape = &rectangle;
		}
		fixtureDef.density = 1;
		body.body->CreateFixture(&fixtureDef);
	}
}

Body::Body() : body(nullptr), isSolid(false), isDynamic(false), type(BodyType::NA), spriteID(SpriteID::NA) { }
Body::Body(b2World& world, bool isSolid, bool isDynamic, BodyType type, SpriteID spriteID, int posX, int posY, b2Shape* shape) : isSolid(isSolid), isDynamic(isDynamic), type(type), spriteID(spriteID) {
	createWorldBody(*this, world, posX, posY, shape);
}
Body::~Body() {
}

Body::Body(const Body& other) : body(other.body), isSolid(other.isSolid), isDynamic(other.isDynamic), type(other.type), spriteID(other.spriteID) {
	// if (other.isSolid)
	// 	createWorldBody(*this, *other.body->GetWorld(), other.body->GetPosition().x, other.body->GetPosition().y, other.body->GetFixtureList()[0].GetShape());
}
Body& Body::operator=(const Body& other) {
	this->body = other.body;
	this->isSolid = other.isSolid;
	this->isDynamic = other.isDynamic;
	this->type = other.type;
	this->spriteID = other.spriteID;
	// if (other.isSolid)
	// 	createWorldBody(*this, *other.body->GetWorld(), other.body->GetPosition().x, other.body->GetPosition().y, other.body->GetFixtureList()[0].GetShape());
	return *this;
}
Body::Body(const Body&& other) : body(std::move(other.body)), isSolid(other.isSolid), isDynamic(other.isDynamic), type(other.type), spriteID(other.spriteID) {
}
Body& Body::operator=(const Body&& other) {
	body = std::move(other.body);
	this->isSolid = other.isSolid;
	this->isDynamic = other.isDynamic;
	this->type = other.type;
	this->spriteID = other.spriteID;
	return *this;
}


void Body::Update(float fElapsedTime) {
	// NOTE: selects roation sprite 	
	float angle = (body ? normalizeAngle(body->GetAngle()) : 0);
	float bodyAngle = normalizeAngle(angle + M_PI*2/8 - (M_PI*2/8)/2);
	float val = bodyAngle / (M_PI*2);
	int rot = val * 8;
	spriteID = SpriteID(int(this->type)+rot); // TODO: the sprite id should also be affected by damage!
	
	// NOTE: turn agent in the direction of his velocity
	const b2Vec2& vel = (body ? body->GetLinearVelocity() : b2Vec2());
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
		
		if (body)
			body->SetTransform(body->GetPosition(), smooth);
	}
	
}