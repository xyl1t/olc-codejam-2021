#ifndef MAP_HPP
#define MAP_HPP

#include "olcPixelGameEngine.h"
#include "box2d/box2d.h"
#include <tuple>
#include "body.hpp"
#include <string>
#include "player.hpp"
#include "enemy.hpp"
#include "agent.hpp"
#include "projectile.hpp"
#include <memory>
#include "myContactListener.hpp"

// NOTE: may be used for multiple levels
// TODO: is a smart pointer really necessary?
// TODO: add operator[] overload for simpler map access 
class Map {
public: // TODO: change this to private later?
	std::vector<std::vector<std::vector<std::shared_ptr<Body>>>> map;
	
	int width;
	int height;
	b2World world;
	olc::vf2d camera;
	std::weak_ptr<Player> player{};
	MyContactListener myContactListener;

public:	
	Map();
	Map(int width, int height);
	
	// CHECK: all this needs thorough testing!!!
	void AddBody(std::shared_ptr<Body> body);
	std::shared_ptr<Body> RemoveBody(std::weak_ptr<Body> body) { if (auto ptr = body.lock()) return RemoveBody(ptr.get()); else return nullptr; }
	std::shared_ptr<Body> RemoveBody(Body* body);
	std::shared_ptr<Body> RemoveBody(int layer, int x, int y);
	bool ReaddBody(std::weak_ptr<Body> body) { if (auto ptr = body.lock()) return ReaddBody(ptr.get()); else return false; }
	bool ReaddBody(Body* body);
	void Load(std::vector<std::string> data, std::shared_ptr<Player> player);
	
	void Update(float fElapsedTime);
	void Draw(Game& game, const olc::vf2d& pos);
	void Draw(Game& game);
		
	inline Body& Get(int x, int y, int layer = 0) { return *map[layer][x][y]; }
	inline int GetSpriteIndex(int x_in_map, int y_in_map, int layer = 0) {
		return (int)Get(x_in_map, y_in_map, layer).spriteID;
	}

private:
	void AddBodyOnlyToWorld(Body& body);
	void RemoveBodyOnlyFromWorld(Body& body);
};

#endif
