#include "body.hpp"

Body::Body(b2World& world, bool isSolid, bool isDynamic, BodyType type, SpriteID spriteID, int posX, int posY) : world(world), isSolid(isSolid), isDynamic(isDynamic), type(type), spriteID(spriteID) {
	if (isSolid) {
		b2PolygonShape rectangle;
		rectangle.SetAsBox(0.5f, 0.5f);
		b2BodyDef bodyDef;
		bodyDef.position.x = posX;
		bodyDef.position.y = posY;
		if (isDynamic)
			bodyDef.type = b2_dynamicBody;
		body = world.CreateBody(&bodyDef);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &rectangle;
		fixtureDef.density = 1;
		body->CreateFixture(&fixtureDef);
	}
}
Body::Body(b2World& world, bool isSolid, bool isDynamic, BodyType type, SpriteID spriteID, const b2Shape& shape, int posX, int posY) : world(world), isSolid(isSolid), isDynamic(isDynamic), type(type), spriteID(spriteID) {
	if (isSolid) {
		b2BodyDef bodyDef;
		bodyDef.position.x = posX;
		bodyDef.position.y = posY;
		if (isDynamic)
			bodyDef.type = b2_dynamicBody;
		body = world.CreateBody(&bodyDef);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1;
		body->CreateFixture(&fixtureDef);
	}
}
