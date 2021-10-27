#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP
#include "body.hpp"

class Projectile : public Body {
public:
	Projectile(float posX, float posY);
	Projectile(float posX, float posY, b2BodyDef bodyDef, b2FixtureDef fixtureDef);
	~Projectile();
	
	void Update(float fElapsedTime) override;
	void Draw(Game& game, const olc::vf2d& pos) override;
	
	float time{0.f};
};

#endif
