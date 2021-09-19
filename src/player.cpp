#include "player.hpp"

Player::Player() : Agent() {
	type = BodyType::PLAYER;
	spriteID = SpriteID::PLAYER_RIGHT;
}
Player::Player(b2World& world, float posX, float posY, b2Shape* shape) : Agent(world, BodyType::PLAYER, SpriteID::PLAYER_RIGHT, posX, posY, shape) {
}

void Player::Update(float fElapsedTime) {
	Agent::Update(fElapsedTime);
}