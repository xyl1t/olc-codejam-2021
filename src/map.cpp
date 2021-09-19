#include "map.hpp"

Map::Map() : data(), map(), width(), height(), world(b2Vec2(0, 0)) {
	
}

void Map::Load(std::vector<std::string> data) {
	this->data = data;
	
	auto createAgent = [this](b2Body*& agentBody, int x, int y) {
		b2CircleShape circle;
		circle.m_radius = 0.49f;
		b2BodyDef bodyDef;
		bodyDef.position.x = x;
		bodyDef.position.y = y;
		bodyDef.type = b2_dynamicBody;
		agentBody = world.CreateBody(&bodyDef);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circle;
		fixtureDef.density = 1;
		fixtureDef.restitution = 0.125f;
		agentBody->SetLinearDamping(10);
		agentBody->SetAngularDamping(7);
		agentBody->CreateFixture(&fixtureDef);
		agentBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(agentBody);
	};
	
	b2PolygonShape rect;
	rect.SetAsBox(0.5f, 0.5f);
	
	b2CircleShape circle;
	circle.m_radius = 0.49f;
	
	for (int y = 0; y < data[0].size(); y++) {
		std::vector<std::vector<Body>> row;
		for (int x = 0; x < data.size(); x++) {
			char d = data[x][y];
			
			const auto& it = charToBodyType.find(d);
			if (it != charToBodyType.end()) {
				
				BodyType bt = charToBodyType.at(d);
				switch (bt) {
					
					case BodyType::DOOR_OPEN: // DOOR
					// 	row.push_back({Body{this->world, false, false, BodyType::DOOR_OPEN, SpriteID::DOOR_UNLOCKED, y, x, &rect}}); 
					// break;
					case BodyType::FLOOR:
						row.push_back({Body{this->world, false, false, BodyType::FLOOR, SpriteID::FLOOR, y, x, &rect}}); 
					break;
					
					case BodyType::WALL: // WALL
						row.push_back({Body{this->world, true, false, BodyType::WALL,  SpriteID::WALL_BASIC, y, x, &rect}}); 
					break;
					
					case BodyType::PLAYER: { // PLAYER
						row.push_back({Body{this->world, false, false, BodyType::FLOOR, SpriteID::FLOOR, y, x, &rect}}); 
						createAgent(playerBody, y, x);
						spawnPoint = { (float)y, (float)x };
					} break;
					case BodyType::ENEMY: { // ENEMY
						row.push_back({Body{this->world, false, false, BodyType::FLOOR, SpriteID::FLOOR, y, x, &rect}}); 
						Enemy e(this->world, y, x, &circle);
						enemies.push_back(e);
					} break;
					
					case BodyType::SPACE: {
						int localY = x % 2;
						int localX = y % 2;
						int id = localX + localY * 2;
						row.push_back({Body{this->world, false, false, BodyType::SPACE, (SpriteID)((int)SpriteID::SPACE_1 + id), y, x, &rect}}); 
					} break;
					
					case BodyType::AIR: { // EMPTY
						row.push_back({Body{this->world, false, false, BodyType::AIR, SpriteID::AIR, y, x, &rect}}); 
					} break;
					
					default:
						row.push_back({Body{this->world, false, false, BodyType::AIR, SpriteID::AIR, y, x, &rect}}); 
				}
			} else {
				row.push_back({Body{this->world, false, false, BodyType::AIR, SpriteID::NA, y, x, &rect}}); 
			}
		}
		map.push_back(row);
	}
	width = map.size();
	height = map[0].size();
	
	auto SetTile = [this](const Body& b, int x, int y, int layer = 0) {
		if (!(x >= 0 && y >= 0 && x < width && y < height)) return;
			
		if (layer >= map[x][y].size()) {
			map[x][y].insert(map[x][y].begin() + layer, b);
		} else {
			map[x][y][layer] = b;
		}
	};
	
	#define IsWall(x, y) (x >= 0 && y >= 0 && x < width && y < height && Get((x), (y)).type == BodyType::WALL)
	for (int y = 0; y < height; y++) 
	for (int x = 0; x < width; x++) {
		SetTile(Body{this->world, false, false, BodyType::AIR, SpriteID::AIR, x, y}, x, y, 1);
		if (Get(x, y).type == BodyType::WALL) {
			
			bool topLeft = IsWall(x-1, y-1);
			bool top = IsWall(x, y-1);
			bool topRight = IsWall(x+1, y-1);
			bool left = IsWall(x-1, y);
			bool right = IsWall(x+1, y);
			bool bottomLeft = IsWall(x-1, y+1);
			bool bottom = IsWall(x, y+1);
			bool bottomRight = IsWall(x+1, y+1);
			
			int tileValue = 
				(topLeft * top * left)       *  1 + top    * 2 +        (topRight * top * right) *   4 +
				                        left *  8 +                                        right *  16 +
				(bottomLeft * bottom * left) * 32 + bottom * 64 + (bottomRight * bottom * right) * 128
			;
			
			int tileIdx = ceilTile.at(tileValue);
			
			int tileX = tileIdx % 8;
			int tileY = tileIdx / 8;
			
			tileY = tileY * 32 + (int)SpriteID::CEIL_0;
			
			int newTileIdx = tileX + tileY;
			
			SetTile(Body{this->world, false, false, BodyType::CEIL, (SpriteID)newTileIdx, x, y}, x, y-1, 1);
		}
	}
	#undef IsWall
}
