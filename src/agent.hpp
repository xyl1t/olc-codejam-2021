#ifndef AGENT_HPP
#define AGENT_HPP
#include "box2d/box2d.h"
#include "body.hpp"
#include <vector>
// #include "olcPixelGameEngine.h"

class Agent : public Body {
public:
	bool alive{true};
	SpriteID topSpriteID;
	float directionAngle{};
	float health;
	float battery;
	void* currentWeapon;

	Agent();
	Agent(b2World& world, BodyType type, SpriteID spriteID, float posX = 0.f, float posY = 0.f, b2Shape* shape = nullptr);
	virtual ~Agent();
	
	void Update(float fElpasedTime) override;
	
	virtual void Attack();
	// void Render(olc::PixelGameEngine& pge);
};

#endif
