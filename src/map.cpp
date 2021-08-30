#include "map.hpp"

StaticBody::StaticBody(b2World& world, bool isSolid, BodyType type) : world(world), isSolid(isSolid), type(type) {
}

Map::Map() : data(), map(), width(), height(), world(b2Vec2(0, 0)) {
}

void Map::Load(std::vector<std::vector<char>> data) {
	this->data = data;
	for (int i = 0; i < data[0].size(); i++) {
		std::vector<StaticBody> row;
		for (int j = 0; j < data.size(); j++) {
			char d = data[j][i];
			switch (d) {
				case ' ': row.push_back(StaticBody{this->world, false, BodyType::FLOOR}); break;
				case '#': row.push_back(StaticBody{this->world, true,  BodyType::WALL_BASIC}); break;
				case 'P': row.push_back(StaticBody{this->world, false, BodyType::FLOOR}); break;
			}
		}
		map.push_back(row);
	}
	
	width = map.size();
	height = map[0].size();
}
