#include "enemy.hpp"

Enemy::Enemy() : Agent() { }
Enemy::Enemy(b2World& world, float posX, float posY, b2Shape* shape) : Agent(world, BodyType::ENEMY, SpriteID::ENEMY_RIGHT, posX, posY, shape) {
}

void Enemy::Update(float fElapsedTime) {
	Agent::Update(fElapsedTime);
}
