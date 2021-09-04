#ifndef AGENT_HPP
#define AGENT_HPP
#include "box2d/box2d.h"
#include "body.hpp"
#include <vector>
// #include "olcPixelGameEngine.h"

class Agent {
public:
	b2Body* body{};
	BodyType type;
	SpriteID spriteID;
	SpriteID topSpriteID;
	float directionAngle{};
	float health;
	float battery;
	void* currentWeapon;

	Agent(b2Body* body, BodyType type, SpriteID spriteID);
	Agent(b2Body* body, BodyType type, SpriteID spriteID, float posX, float posY);
	virtual ~Agent();
	
	virtual void Update(float fElpasedTime);
	// void Render(olc::PixelGameEngine& pge);
};

#endif
