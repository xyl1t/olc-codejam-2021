#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "box2d/box2d.h"
#include "body.hpp"
#include <vector>
#include "agent.hpp"
// #include "olcPixelGameEngine.h"

class Enemy : public Agent {
public:
	Enemy(b2Body* body = nullptr);
	Enemy(b2Body* body, float posX, float posY);
	
	void Update(float fElapsedTime) override;
	// void Render(olc::PixelGameEngine& pge);
};

#endif
