#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "agent.hpp"

class Enemy : public Agent {
public:
	Enemy(float posX, float posY);
	Enemy(float posX, float posY, b2BodyDef bodyDef, b2FixtureDef fixtureDef);
	
	void Update(float fElapsedTime) override;
	// void Draw(Game& game, const olc::vf2d& pos) override; 
};

#endif
