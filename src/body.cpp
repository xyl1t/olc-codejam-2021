#include "body.hpp"

static void createWorldBody(Body& body, b2World& world, int x = 0, int y = 0) {
	if (body.isSolid) {
		b2PolygonShape rectangle;
		rectangle.SetAsBox(0.5f, 0.5f);
		b2BodyDef bodyDef;
		bodyDef.position.x = x;
		bodyDef.position.y = y;
		if (body.isDynamic)
			bodyDef.type = b2_dynamicBody;
		body.body = world.CreateBody(&bodyDef);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &rectangle;
		fixtureDef.density = 1;
		body.body->CreateFixture(&fixtureDef);
	}
}

Body::Body(b2World& world, bool isSolid, bool isDynamic, BodyType type, SpriteID spriteID, int posX, int posY) : world(world), isSolid(isSolid), isDynamic(isDynamic), type(type), spriteID(spriteID) {
	createWorldBody(*this, world, posX, posY);
}
Body::Body(b2World& world, bool isSolid, bool isDynamic, BodyType type, SpriteID spriteID, const b2Shape& shape, int posX, int posY) : world(world), isSolid(isSolid), isDynamic(isDynamic), type(type), spriteID(spriteID) {
	createWorldBody(*this, world, posX, posY);
}

Body::Body(const Body& other) : world(other.world), isSolid(other.isSolid), isDynamic(other.isDynamic), type(other.type), spriteID(other.spriteID) {
	createWorldBody(*this, world, (other.body ? other.body->GetPosition().x : 0), (other.body ? other.body->GetPosition().y : 0));
}
Body& Body::operator=(const Body& other) {
	this->world = other.world;
	this->isSolid = other.isSolid;
	this->isDynamic = other.isDynamic;
	this->type = other.type;
	this->spriteID = other.spriteID;
	createWorldBody(*this, world, (other.body ? other.body->GetPosition().x : 0), (other.body ? other.body->GetPosition().y : 0));
	return *this;
}
Body::Body(const Body&& other) : body(std::move(other.body)), world(other.world), isSolid(other.isSolid), isDynamic(other.isDynamic), type(other.type), spriteID(other.spriteID) {
}
Body& Body::operator=(const Body&& other) {
	body = std::move(other.body);
	this->world = other.world;
	this->isSolid = other.isSolid;
	this->isDynamic = other.isDynamic;
	this->type = other.type;
	this->spriteID = other.spriteID;
	return *this;
}
