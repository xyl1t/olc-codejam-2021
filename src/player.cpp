#include "player.hpp"

Player::Player(float posX, float posY) : Agent(BodyType::PLAYER, SpriteID::PLAYER_RIGHT, posX, posY) {
}

void Player::Update(float fElapsedTime) {
	Agent::Update(fElapsedTime);
}