#ifndef BODY_HPP
#define BODY_HPP

#include "box2d/box2d.h"
#include <tuple>
#include <unordered_map>

enum class SpriteID {
	PLAYER_RIGHT 		= 0,
	PLAYER_DOWN_RIGHT 	= 1,
	PLAYER_DOWN 		= 2,
	PLAYER_DOWN_LEFT 	= 3,
	PLAYER_LEFT 		= 4,
	PLAYER_UP_LEFT 		= 5,
	PLAYER_UP 			= 6,
	PLAYER_UP_RIGHT 	= 7,
	PLAYER_TOP_RIGHT 		= 128+0,
	PLAYER_TOP_DOWN_RIGHT 	= 128+1,
	PLAYER_TOP_DOWN 		= 128+2,
	PLAYER_TOP_DOWN_LEFT 	= 128+3,
	PLAYER_TOP_LEFT 		= 128+4,
	PLAYER_TOP_UP_LEFT 		= 128+5,
	PLAYER_TOP_UP 			= 128+6,
	PLAYER_TOP_UP_RIGHT 	= 128+7,
	
	ENEMY_RIGHT 	=  8,
	ENEMY_DOWN_RIGHT=  9,
	ENEMY_DOWN 		= 10,
	ENEMY_DOWN_LEFT = 11,
	ENEMY_LEFT 		= 12,
	ENEMY_UP_LEFT 	= 13,
	ENEMY_UP 		= 14,
	ENEMY_UP_RIGHT 	= 15,
	ENEMY_TOP_RIGHT 	 = 128+ 8,
	ENEMY_TOP_DOWN_RIGHT = 128+ 9,
	ENEMY_TOP_DOWN 		 = 128+10,
	ENEMY_TOP_DOWN_LEFT  = 128+11,
	ENEMY_TOP_LEFT 		 = 128+12,
	ENEMY_TOP_UP_LEFT 	 = 128+13,
	ENEMY_TOP_UP 		 = 128+14,
	ENEMY_TOP_UP_RIGHT 	 = 128+15,

	FLOOR = 736,
	WALL_BASIC     = 704,
	WALL_VARIANT_1 = 704+1,
	WALL_VARIANT_2 = 704+2,
	WALL_VARIANT_3 = 704+3,
	DOOR_LOCKED   = 768,
	DOOR_UNLOCKED = 768+1,
	DOOR_OPEN     = 768+2,
	
	SPACE_1 = 800,
	SPACE_2 = 801,
	SPACE_3 = 802,
	SPACE_4 = 803,
	
	CEIL_0  = 832,
	CEIL_1  = 833,
	CEIL_2  = 834,
	CEIL_3  = 835,
	CEIL_4  = 836,
	CEIL_5  = 837,
	CEIL_6  = 838,
	CEIL_7  = 839,
	CEIL_8  = 864,
	CEIL_9  = 864,
	CEIL_10 = 864,
	CEIL_11 = 864,
	CEIL_12 = 864,
	CEIL_13 = 864,
	CEIL_14 = 864,
	CEIL_15 = 864,
	CEIL_16 = 896,
	CEIL_17 = 896,
	CEIL_18 = 896,
	CEIL_19 = 896,
	CEIL_20 = 896,
	CEIL_21 = 896,
	CEIL_22 = 896,
	CEIL_23 = 896,
	CEIL_24 = 928,
	CEIL_25 = 928,
	CEIL_26 = 928,
	CEIL_27 = 928,
	CEIL_28 = 928,
	CEIL_29 = 928,
	CEIL_30 = 928,
	CEIL_31 = 928,
	CEIL_32 = 960,
	CEIL_33 = 960,
	CEIL_34 = 960,
	CEIL_35 = 960,
	CEIL_36 = 960,
	CEIL_37 = 960,
	CEIL_38 = 960,
	CEIL_39 = 960,
	CEIL_40 = 992,
	CEIL_41 = 992,
	CEIL_42 = 992,
	CEIL_43 = 992,
	CEIL_44 = 992,
	CEIL_45 = 992,
	CEIL_46 = 992,
	CEIL_47 = 992,
	
	AIR = 1020,
	NA = 1023
};
const inline std::unordered_map<int, int> ceilTile { {2, 1}, {8, 2}, {10, 3}, {11, 4}, {16, 5}, {18, 6}, {22, 7}, {24, 8}, {26, 9}, {27, 10}, {30, 11}, {31, 12}, {64, 13}, {66, 14}, {72, 15}, {74, 16}, {75, 17}, {80, 18}, {82, 19}, {86, 20}, {88, 21}, {90, 22}, {91, 23}, {94, 24}, {95, 25}, {104, 26}, {106, 27}, {107, 28}, {120, 29}, {122, 30}, {123, 31}, {126, 32}, {127, 33}, {208, 34}, {210, 35}, {214, 36}, {216, 37}, {218, 38}, {219, 39}, {222, 40}, {223, 41}, {248, 42}, {250, 43}, {251, 44}, {254, 45}, {255, 46}, {0, 47} };

enum class BodyType {
	PLAYER = (int)SpriteID::PLAYER_RIGHT,
	ENEMY = (int)SpriteID::ENEMY_RIGHT,
	BOSS,
	FLOOR = (int)SpriteID::FLOOR,
	WALL = (int)SpriteID::WALL_BASIC,
	CEIL = (int)SpriteID::CEIL_0,
	DOOR_LOCKED = (int)SpriteID::DOOR_LOCKED,
	DOOR_UNLOCKED = (int)SpriteID::DOOR_UNLOCKED,
	DOOR_OPEN,
	AIR = (int)SpriteID::AIR,
	SPACE = (int)SpriteID::SPACE_1,
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
	
	Body(const Body& other);
	Body& operator=(const Body& other);
	Body(const Body&& other);
	Body& operator=(const Body&& other);
	
};

#endif