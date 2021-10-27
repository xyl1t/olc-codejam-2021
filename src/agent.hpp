#ifndef AGENT_HPP
#define AGENT_HPP
#include "body.hpp"

// NOTE: add health to constructor
class Agent : public Body {
public:
	SpriteID topSpriteID;
	float directionAngle{};
	float health{3}; // NOTE: temporary
	float battery;
	void* currentWeapon;

	Agent(BodyType type, SpriteID spriteID, float posX, float posY);
	Agent(BodyType type, SpriteID spriteID, float posX, float posY, b2BodyDef bodyDef, b2FixtureDef fixtureDef);
	virtual ~Agent();
	
	void Update(float fElpasedTime) override;
	void Draw(Game& game, const olc::vf2d& pos) override; 
	
	virtual void Attack();
};

#endif
