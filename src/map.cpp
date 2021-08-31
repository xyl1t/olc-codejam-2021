#include "map.hpp"

Map::Map() : data(), map(), width(), height(), world(b2Vec2(0, 0)) {
	
}

void Map::Load(std::vector<std::vector<char>> data) {
	this->data = data;
	for (int y = 0; y < data[0].size(); y++) {
		std::vector<Body> row;
		for (int x = 0; x < data.size(); x++) {
			char d = data[x][y];
			switch (d) {
				case ' ': 
					row.push_back(Body{this->world, false, false, BodyType::FLOOR, SpriteID::FLOOR, y, x}); 
				break;
				case 'P': {
					row.push_back(Body{this->world, false, false, BodyType::FLOOR, SpriteID::FLOOR, y, x}); 
					b2CircleShape circle;
					circle.m_radius = 0.5f;
					b2BodyDef bodyDef;
					bodyDef.position.x = y;
					bodyDef.position.y = x;
					bodyDef.type = b2_dynamicBody;
					playerBody = world.CreateBody(&bodyDef);
					b2FixtureDef fixtureDef;
					fixtureDef.shape = &circle;
					fixtureDef.density = 1;
					playerBody->CreateFixture(&fixtureDef);

					spawnPoint = { (float)y, (float)x };
				} break;
				case '#': 
					row.push_back(Body{this->world, true, false, BodyType::WALL,  SpriteID::WALL_BASIC, y, x}); 
				break;
			}
		}
		map.push_back(row);
	}
	
	width = map.size();
	height = map[0].size();
}
