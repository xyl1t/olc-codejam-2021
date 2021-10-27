#include "enemy.hpp"
#include "map.hpp"
#include "player.hpp"
#include <memory>

Enemy::Enemy(float posX, float posY) : Agent(BodyType::ENEMY, SpriteID::ENEMY_RIGHT, posX, posY) {
}
Enemy::Enemy(float posX, float posY, b2BodyDef bodyDef, b2FixtureDef fixtureDef) : Agent(BodyType::ENEMY, SpriteID::ENEMY_RIGHT, posX, posY, bodyDef, fixtureDef) {
}

void Enemy::Update(float fElapsedTime) {
	Agent::Update(fElapsedTime);
	if (map) {
		if (auto player = map->player.lock()) {
			this->directionAngle = std::atan2(player->GetY() - this->GetY(), player->GetX() - this->GetX());
		}
	}
}
