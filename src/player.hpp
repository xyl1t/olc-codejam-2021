#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "agent.hpp"

class Player : public Agent {
public:
	std::vector<void*> weapons;

	Player(float posX, float posY);
	
	void Update(float fElapsedTime) override;
	// void Draw(Game& game, const olc::vf2d& pos) override; 
};

#endif
