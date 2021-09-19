#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "box2d/box2d.h"
#include "body.hpp"
#include <vector>
#include "agent.hpp"
// #include "olcPixelGameEngine.h"

class Enemy : public Agent {
public:
	Enemy();
	Enemy(b2World& world, float posX = 0.f, float posY = 0.f, b2Shape* shape = nullptr);
	
	void Update(float fElapsedTime) override;
	// void Render(olc::PixelGameEngine& pge);
};

#endif
