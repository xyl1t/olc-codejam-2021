#include "projectile.hpp"
#include "game.hpp"

Projectile::Projectile(float posX, float posY) : Body(true, true, BodyType::PROJECTILE, SpriteID::PROJECTILE_RIGHT, posX, posY) {
	// HACK: calling new for shape because otherwise the fixture.shape pointer will be pointing to a variable that lives on the stack but will be destoried after this function
	b2CircleShape* circle = new b2CircleShape();
	circle->m_radius = 0.125f;

	fixtureDef.shape = circle;
	fixtureDef.restitution = 0.5f;
	fixtureDef.density = 1;
	bodyDef.linearDamping = 3;
	bodyDef.angularDamping = 7;	
}
Projectile::Projectile(float posX, float posY, b2BodyDef bodyDef, b2FixtureDef fixtureDef) : Body(true, true, BodyType::PROJECTILE, SpriteID::PROJECTILE_RIGHT, posX, posY, bodyDef, fixtureDef) {
}
Projectile::~Projectile() {
	delete fixtureDef.shape;
}

void Projectile::Update(float fElapsedTime) {
	Body::Update(fElapsedTime);
	
	// NOTE: THIS IS TEMP, JUST FOR TESTING
	time += fElapsedTime;
	if (time >= 7) {
		alive = false;
	}
}

void Projectile::Draw(Game& game, const olc::vf2d& pos) {
	game.SetPixelMode(olc::Pixel::Mode::ALPHA);
	Body::Draw(game, pos);
	game.SetPixelMode(olc::Pixel::Mode::NORMAL);
}
