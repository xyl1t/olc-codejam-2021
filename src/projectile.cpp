#include "projectile.hpp"

Projectile::Projectile(b2World& world, float posX, float posY, b2Shape* shape) : Body(world, true, true, BodyType::PROJECTILE, SpriteID::PROJECTILE_RIGHT, posX, posY, shape) {
}

void Projectile::Update(float fElapsedTime) {
	Body::Update(fElapsedTime);
}
