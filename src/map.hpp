#ifndef MAP_HPP
#define MAP_HPP
#include "olcPixelGameEngine.h"
#include "box2d/box2d.h"
#include <tuple>
#include "body.hpp"
#include <string>
#include "player.hpp"
#include "enemy.hpp"

// NOTE: may be used for multiple levels
class Map {
public: // TODO: change this to private later?
	std::vector<std::string> data; 
	std::vector<std::vector<std::vector<Body>>> map;
	
	std::vector<Enemy*> enemies;
	
	int width;
	int height;
	b2World world;
	olc::vf2d camera;
	b2Body* playerBody;
	olc::vf2d spawnPoint;
	

public:	
	Map();

	inline char GetRaw(int x, int y) { return data[x][y]; }
	inline Body& Get(int x, int y, int layer = 0) { return map[x][y][layer]; }
	inline int GetSpriteIndex(int x_in_map, int y_in_map, int layer = 0) {
		return (int)Get(x_in_map, y_in_map, layer).spriteID;
	}
	
	void Load(std::vector<std::string> data);
	
};

#endif
