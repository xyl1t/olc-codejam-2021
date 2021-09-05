#include "player.hpp"
#include "common.hpp"
#include <cmath>

Player::Player(b2Body* body) : Agent(body, BodyType::PLAYER, SpriteID::PLAYER_RIGHT) {
}

Player::Player(b2Body* body, float posX, float posY) : Agent(body, BodyType::PLAYER, SpriteID::PLAYER_RIGHT, posX, posY) {
	if (body) {
		body->SetTransform({posX, posY}, body->GetAngle());
	}
}

void Player::Update(float fElapsedTime) {
	Agent::Update(fElapsedTime);
}