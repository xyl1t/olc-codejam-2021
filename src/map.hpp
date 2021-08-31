#ifndef MAP_HPP
#define MAP_HPP
#include "olcPixelGameEngine.h"
#include "box2d/box2d.h"
#include <tuple>
#include "body.hpp"

// NOTE: may be used for multiple levels
class Map {
public: // TODO: change this to private later?
	std::vector<std::vector<char>> data; 
	std::vector<std::vector<Body>> map;
	int width;
	int height;
	b2World world;
	b2Body* playerBody;
	
	olc::vf2d spawnPoint;
	olc::vf2d camera;
	
	Map();
	
public:	
	inline char GetRaw(int x, int y) { return data[x][y]; }
	inline Body& Get(int x, int y) { return map[x][y]; }
	inline int GetSpriteIndex(int x_in_map, int y_in_map) {
		return (int)Get(x_in_map, y_in_map).spriteID;
	}
	
	void Load(std::vector<std::vector<char>> data);
	
};

#endif
