#include "enemy.hpp"

Enemy::Enemy(b2Body* body) : Agent(body, BodyType::ENEMY, SpriteID::ENEMY_RIGHT) {
}

Enemy::Enemy(b2Body* body, float posX, float posY) : Agent(body, BodyType::ENEMY, SpriteID::ENEMY_RIGHT, posX, posY) {
	if (body) {
		body->SetTransform({posX, posY}, body->GetAngle());
	}
}

void Enemy::Update(float fElapsedTime) {
	Agent::Update(fElapsedTime);
}