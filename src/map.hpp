#ifndef MAP_HPP
#define MAP_HPP
#include "olcPixelGameEngine.h"
#include "box2d/box2d.h"
#include <tuple>

enum class BodyType {
	PLAYER = 0,
	FLOOR = 160,
	WALL_BASIC     = 128,
	WALL_VARIANT_1 = 129,
	WALL_VARIANT_2 = 129,
	WALL_VARIANT_3 = 130,
	DOOR_LOCKED   = 192,
	DOOR_UNLOCKED = 193,
	DOOR_OPEN     = 194,
};

// Wall, floor etc
struct StaticBody {
	b2World& world;
	bool isSolid;
	BodyType type;
	
	StaticBody(b2World& world, bool isSolid, BodyType type);
};

// Player, enemy etc
struct DynamicBody {
	
};

// NOTE: may be used for multiple levels
class Map {
public: // TODO: change this to private later?
	std::vector<std::vector<char>> data; 
	std::vector<std::vector<StaticBody>> map;
	int width;
	int height;
	b2World world;
	
	olc::vf2d spawnPoint;
	olc::vf2d camera;
	
	Map();
	
public:	
	inline char GetRaw(int x, int y) { return data[x][y]; }
	inline StaticBody& Get(int x, int y) { return map[x][y]; }
	inline int GetSpriteIndex(int x_in_map, int y_in_map) {
		return (int)Get(x_in_map, y_in_map).type;
	}
	
	void Load(std::vector<std::vector<char>> data);
	
};

#endif
