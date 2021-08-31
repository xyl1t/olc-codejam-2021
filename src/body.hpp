#ifndef BODY_HPP
#define BODY_HPP

#include "box2d/box2d.h"
#include <tuple>

enum class SpriteID {
	PLAYER_RIGHT 		= 0,
	PLAYER_LEFT 		= 1,
	PLAYER_BOTTOM 		= 2,
	PLAYER_TOP 			= 3,
	PLAYER_BOTTOM_LEFT 	= 4,
	PLAYER_BOTTOM_RIGHT = 5,
	PLAYER_TOP_LEFT 	= 6,
	PLAYER_TOP_RIGHT 	= 7,

	FLOOR = 160,
	WALL_BASIC     = 128,
	WALL_VARIANT_1 = 129,
	WALL_VARIANT_2 = 129,
	WALL_VARIANT_3 = 130,
	DOOR_LOCKED   = 192,
	DOOR_UNLOCKED = 193,
	DOOR_OPEN     = 194,
};

enum class BodyType {
	PLAYER,
	ENEMY,
	BOSS,
	FLOOR,
	WALL,
	DOOR_LOCKED,
	DOOR_UNLOCKED,
	DOOR_OPEN,
};

struct Body {
	b2Body* body{};
	b2World& world;
	bool isSolid;
	bool isDynamic;
	BodyType type;
	SpriteID spriteID;
	
	Body(b2World& world, bool isSolid, bool isDynamic, BodyType type, SpriteID spriteID, int posX = 0, int posY = 0);
	Body(b2World& world, bool isSolid, bool isDynamic, BodyType type, SpriteID spriteID, const b2Shape& shape, int posX = 0, int posY = 0);
};

#endif