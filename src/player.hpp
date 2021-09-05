#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "box2d/box2d.h"
#include "body.hpp"
#include <vector>
#include "agent.hpp"
// #include "olcPixelGameEngine.h"

class Player : public Agent {
public:
	std::vector<void*> weapons;

	Player(b2Body* body = nullptr);
	Player(b2Body* body, float posX, float posY);
	
	void Update(float fElapsedTime) override;
	// void Render(olc::PixelGameEngine& pge);
};

#endif
