#include <utility>
#include "map.hpp"
#include "game.hpp"

// NOTE: air and nullptr probably have the same effect: nothing
// NOTE: use layers to put agents on top of floor:
// layer 0: floor
// layer 1: agents
// layer 2: walls, ceilings, etc
// NOTE: map[layer][x][y]

Map::Map() : map(), width(), height(), world(b2Vec2(0, 0)) {
	world.SetContactListener(&myContactListener);
}

Map::Map(int width, int height) : map(), width(width), height(height), world(b2Vec2(0, 0)) {
	map.resize(3);
	for (int i = 0; i < 2; i++) {
		map[i+i].resize(width);
		for (int j = 0; j < width; j++)
			map[i+i][j].resize(height);
	}
	map[1].resize(1);
	world.SetContactListener(&myContactListener);
}

void Map::AddBodyOnlyToWorld(Body& body) {
	body.body = world.CreateBody(&body.bodyDef);
	body.body->CreateFixture(&body.fixtureDef);
	body.body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&body); // CHECK: I don't know if this works as intended
}
void Map::RemoveBodyOnlyFromWorld(Body& body) {
	world.DestroyBody(body.body);
	body.body = nullptr;
}

void Map::AddBody(std::shared_ptr<Body> body) {
	// NOTE: if the body is not an agent, it goes to layer 0
	// agents go to layer 1
	// layer 2 is used for bodies that are in layer 0, but also need layer 2
	
	if (body->isSolid) {
		body->body = world.CreateBody(&body->bodyDef);
		body->body->CreateFixture(&body->fixtureDef);
		body->body->GetUserData().pointer = reinterpret_cast<uintptr_t>(body.get());
	}
	body->map = this;
	if (body->isDynamic) {
		map[1][0].push_back(std::shared_ptr<Body>(body));
	} else {
		map[0][(int)body->GetX()][(int)body->GetY()] = body;
	}
}

std::shared_ptr<Body> Map::RemoveBody(Body* body) {
	if (body) {
		if (!body->isDynamic) {
			const b2Vec2& pos = {body->GetX(), body->GetY()};
			if (pos.x >= 0 && pos.y >= 0 && pos.x < width && pos.y < height) {
				for (int layer = 0; layer < 1; layer += 2) {
					if (map[layer][pos.x][pos.y].get() == body || map[layer][pos.x][pos.y]->body == body->body) {
						return RemoveBody(layer, pos.x, pos.y);
					}
				}
			}

			for (int layer = 0; layer < 1; layer += 2) 
			for (int y = 0; y < height; y++) 
			for (int x = 0; x < width; x++) {
				if (map[layer][x][y].get() == body || map[layer][x][y]->body == body->body) {
					return RemoveBody(layer, x, y);
				}
			}
			
		} else {
			for (int i = 0; i < map[1][0].size(); i++) {
				if (map[1][0][i].get() == body && map[1][0][i]->body == body->body) {
					return RemoveBody(1, 0, i);
				}
			}
		}
	}
	return nullptr;
}
std::shared_ptr<Body> Map::RemoveBody(int layer, int x, int y) {
	if (layer >= 0 && layer < 3 && x >= 0 && x < map[layer].size() && y >= 0&& y < map[layer][x].size()) {
		if (map[layer][x][y]->body) {
			world.DestroyBody(map[layer][x][y]->body);
			map[layer][x][y]->body = nullptr;
		}
		map[layer][x][y]->map = nullptr;
		map[layer][x][y] = nullptr;
		if (layer == 1) {
			map[layer][x].erase(map[layer][x].begin() + y);
		}
		return map[layer][x][y];
	}
	return nullptr;
}
bool Map::ReaddBody(Body* body) {
	bool bodyFound{false};
	
	if (body && body->isSolid) {
		if (!body->isDynamic) {
			if (body->GetX() >= 0 && body->GetX() < map[0].size() && body->GetY() >= 0&& body->GetY() < map[0][body->GetX()].size()) {
				if (map[0][body->GetX()][body->GetY()]->body == body->body) {
					bodyFound = true;
				}
			}
		} else {
			for (int i = 0; i < map[1][0].size() && !bodyFound; i++) {
				if (map[1][0][i].get() == body && map[1][0][i]->body == body->body) {
					bodyFound = true;
				}
			}
		}
	} 
	if (bodyFound) {
		RemoveBodyOnlyFromWorld(*body);
		AddBodyOnlyToWorld(*body);
	}
	
	return bodyFound;
}

void Map::Update(float fElapsedTime) {
	// NOTE: only updating DYNAMIC bodies!!
	// NOTE: only updating DYNAMIC bodies!!
	// NOTE: only updating DYNAMIC bodies!!
	
	for (int i = map[1][0].size()-1; i >= 0; i--) {
		if (map[1][0][i] && !(map[1][0][i]->alive)) {
			RemoveBody(1, 0, i);
		}
	}
	
	for (const auto agent : map[1][0]) {
		if (agent) {
			agent->Update(fElapsedTime);
		}
	}
	
	static float time = 0;
	static const float timestep = 1 / 240.f;
	
    for(time += fElapsedTime; time >= timestep; time -= timestep) {
		// std::cout << "step, ";
        world.Step(timestep, 8, 2);
	}
	// std::cout << "\n";
	
	// world.Step(fElapsedTime, 8, 2);
}
void Map::Draw(Game& game, const olc::vf2d& pos) {
	int visibleTilesX = std::min(int(game.ScreenWidth()  / constants::windowScale / game.assets.spriteWidth),  width);
	int visibleTilesY = std::min(int(game.ScreenHeight() / constants::windowScale / game.assets.spriteHeight), height);

	for (int layer = 0; layer < 3; layer++) {
		int x = 0;
		int end_x = map[layer].size();
		if (layer != 1) {
			x = pos.x;
			if (pos.x + visibleTilesX + 1 < map[layer].size()) {
				end_x = pos.x + visibleTilesX + 1;
			}
		}
		for (; x < end_x; x++) {
			int y = 0;
			int end_y = map[layer][x].size();
			if (layer != 1) {
				y = pos.y;
				if (pos.y + visibleTilesY + 2 < map[layer][x].size()) {
					end_y = pos.y + visibleTilesY + 2;
				}
			}
			for (; y < end_y; y++) {
				if (!map[layer][x][y]) continue;
				Body& body = *(map[layer][x][y]);
				
				olc::vf2d tileLocation {
					(body.GetX() * game.assets.spriteWidth - pos.x * game.assets.spriteWidth) * constants::windowScale, 
					(body.GetY() * game.assets.spriteHeight - pos.y * game.assets.spriteHeight) * constants::windowScale
				};
				
				if (tileLocation.x + game.assets.spriteWidth*constants::windowScale < 0 || tileLocation.y + game.assets.spriteHeight*constants::windowScale < 0 || tileLocation.x > game.GetDrawTargetWidth() || tileLocation.y > game.GetDrawTargetHeight()) {
					continue;
				}
				
				body.Draw(game, tileLocation);
			}
		}
	}
}
void Map::Draw(Game& game) {
	float visibleTilesX = std::min(game.ScreenWidth()  / constants::windowScale / game.assets.spriteWidth,  (float)width);
	float visibleTilesY = std::min(game.ScreenHeight() / constants::windowScale / game.assets.spriteHeight, (float)height);
	float offsetX = camera.x - visibleTilesX/2.f + 0.5f;
	float offsetY = camera.y - visibleTilesY/2.f + 0.5f;
	offsetX = std::max(std::min(offsetX, width - visibleTilesX), 0.f);
	offsetY = std::max(std::min(offsetY, height - visibleTilesY), 0.f);
	Draw(game, {offsetX, offsetY});
}




// CHECK: if player is nullptr
void Map::Load(std::vector<std::string> data, std::shared_ptr<Player> player) {
	this->player = player;
	
	width = data[0].size();
	height = data.size();
	
	for (int i = 0; i < 2; i++) {
		map[i+i].resize(width);
		for (int j = 0; j < width; j++)
			map[i+i][j].resize(height);
	}
	map[1].resize(1);
	
	b2PolygonShape rect;
	rect.SetAsBox(0.5f, 0.5f);
	b2CircleShape circle;
	circle.m_radius = 0.49f;
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			const auto& it = charToBodyType.find(data[y][x]);
			
			if (it != charToBodyType.end()) {
				BodyType bt = charToBodyType.at(data[y][x]);
				switch (bt) {
					
					case BodyType::DOOR_OPEN: // DOOR
					// 	row.push_back({Body{this->world, false, false, BodyType::DOOR_OPEN, SpriteID::DOOR_UNLOCKED, x, y, &rect}}); 
					// break;
					case BodyType::FLOOR:
			 			AddBody(std::make_shared<Body>(false, false, BodyType::FLOOR, SpriteID::FLOOR, x, y));
					break;
					
					case BodyType::WALL: // WALL
						AddBody(std::make_shared<Body>(true, false, BodyType::WALL, SpriteID::WALL_BASIC, x, y));
					break;
					
					case BodyType::PLAYER: { // PLAYER
			 			AddBody(std::make_shared<Body>(false, false, BodyType::FLOOR, SpriteID::FLOOR, x, y));
						player->SetX(x);
						player->SetY(y);
						AddBody(player);
					} break;

					case BodyType::ENEMY: { // ENEMY
			 			AddBody(std::make_shared<Body>(false, false, BodyType::FLOOR, SpriteID::FLOOR, x, y));
			 			AddBody(std::make_shared<Enemy>(x, y));
					} break;
					
					case BodyType::SPACE: {
						int localY = y % 2;
						int localX = x % 2;
						int id = localX + localY * 2;
						AddBody(std::make_shared<Body>(false, false, BodyType::SPACE, (SpriteID)((int)SpriteID::SPACE_1 + id), x, y));
					} break;
					
					case BodyType::AIR: { // EMPTY
						// NOTE: should be nullptr
					} break;
					
					default: {
						// NOTE: should be nullptr
					}
				}
			} else {
				// NOTE: should be nullptr
			}
		}
	}
	
	// auto createAgent = [this](b2Body*& agentBody, int x, int y) {
	// 	b2CircleShape circle;
	// 	circle.m_radius = 0.49f;
	// 	b2BodyDef bodyDef;
	// 	bodyDef.position.x = x;
	// 	bodyDef.position.y = y;
	// 	bodyDef.type = b2_dynamicBody;
	// 	agentBody = world.CreateBody(&bodyDef);
	// 	b2FixtureDef fixtureDef;
	// 	fixtureDef.shape = &circle;
	// 	fixtureDef.density = 1;
	// 	fixtureDef.restitution = 0.125f;
	// 	agentBody->SetLinearDamping(10);
	// 	agentBody->SetAngularDamping(7);
	// 	agentBody->CreateFixture(&fixtureDef);
	// 	agentBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(agentBody);
	// };
	
	// for (int x = 0; x < data[0].size(); x++) {
	// 	std::vector<std::vector<Body>> row;
	// 		for (int y = 0; y < data.size(); y++) {
	// 		char d{};
	// 		if (x < data[y].size()) 
	// 			d = data[y][x];
	// 		else 
	// 			d = '\0';
	
	// 		const auto& it = charToBodyType.find(d);
	// 		if (it != charToBodyType.end()) {
				
	// 			BodyType bt = charToBodyType.at(d);
	// 			switch (bt) {
					
	// 				case BodyType::DOOR_OPEN: // DOOR
	// 				// 	row.push_back({Body{this->world, false, false, BodyType::DOOR_OPEN, SpriteID::DOOR_UNLOCKED, x, y, &rect}}); 
	// 				// break;
	// 				case BodyType::FLOOR:
	// 					row.push_back({Body{*this, false, false, BodyType::FLOOR, SpriteID::FLOOR, x, y, &rect}}); 
	// 				break;
					
	// 				case BodyType::WALL: // WALL
	// 					row.push_back({Body{*this, true, false, BodyType::WALL,  SpriteID::WALL_BASIC, x, y, &rect}}); 
	// 				break;
					
	// 				case BodyType::PLAYER: { // PLAYER
	// 					row.push_back({Body{*this, false, false, BodyType::FLOOR, SpriteID::FLOOR, x, y, &rect}}); 
	// 					// createAgent(playerBody, x, y);
	// 					spawnPoint = { (float)x, (float)y };
	// 				} break;
	// 				case BodyType::ENEMY: { // ENEMY
	// 					row.push_back({Body{*this, false, false, BodyType::FLOOR, SpriteID::FLOOR, x, y, &rect}}); 
	// 					Enemy e(*this, x, y, &circle);
	// 					enemies.push_back(e);
	// 				} break;
					
	// 				case BodyType::SPACE: {
	// 					int localY = y % 2;
	// 					int localX = x % 2;
	// 					int id = localX + localY * 2;
	// 					row.push_back({Body{*this, false, false, BodyType::SPACE, (SpriteID)((int)SpriteID::SPACE_1 + id), x, y, &rect}}); 
	// 				} break;
					
	// 				case BodyType::AIR: { // EMPTY
	// 					row.push_back({Body{*this, false, false, BodyType::AIR, SpriteID::AIR, x, y, &rect}}); 
	// 				} break;
					
	// 				default:
	// 					row.push_back({Body{*this, false, false, BodyType::AIR, SpriteID::AIR, x, y, &rect}}); 
	// 			}
	// 		} else {
	// 			row.push_back({Body{*this, false, false, BodyType::AIR, SpriteID::NA, x, y, &rect}}); 
	// 		}
	// 	}
	// 	map.push_back(row);
	// }
	// width = map.size();
	// height = map[0].size();
	
	// auto SetTile = [this](const Body& b, int x, int y, int layer = 0) {
	// 	if (!(x >= 0 && y >= 0 && x < width && y < height)) return;
			
	// 	if (layer >= map[x][y].size()) {
	// 		map[x][y].insert(map[x][y].begin() + layer, b);
	// 	} else {
	// 		map[x][y][layer] = b;
	// 	}
	// };
	
	// #define IsWall(x, y) (x >= 0 && y >= 0 && x < width && y < height && Get((x), (y)).type == BodyType::WALL)
	// for (int y = 0; y < height; y++) 
	// for (int x = 0; x < width; x++) {
	// 	SetTile(Body{*this, false, false, BodyType::AIR, SpriteID::AIR, x, y}, x, y, 1);
	// 	if (Get(x, y).type == BodyType::WALL) {
			
	// 		bool topLeft = IsWall(x-1, y-1);
	// 		bool top = IsWall(x, y-1);
	// 		bool topRight = IsWall(x+1, y-1);
	// 		bool left = IsWall(x-1, y);
	// 		bool right = IsWall(x+1, y);
	// 		bool bottomLeft = IsWall(x-1, y+1);
	// 		bool bottom = IsWall(x, y+1);
	// 		bool bottomRight = IsWall(x+1, y+1);
			
	// 		int tileValue = 
	// 			(topLeft * top * left)       *  1 + top    * 2 +        (topRight * top * right) *   4 +
	// 			                        left *  8 +                                        right *  16 +
	// 			(bottomLeft * bottom * left) * 32 + bottom * 64 + (bottomRight * bottom * right) * 128;
			
	// 		int tileIdx = ceilTile.at(tileValue);
			
	// 		int tileX = tileIdx % 8;
	// 		int tileY = tileIdx / 8;
			
	// 		tileY = tileY * 32 + (int)SpriteID::CEIL_0;
			
	// 		int newTileIdx = tileX + tileY;
			
	// 		SetTile(Body{*this, false, false, BodyType::CEIL, (SpriteID)newTileIdx, x, y}, x, y-1, 1);
	// 	}
	// }
	// #undef IsWall
}

// void Map::CreateBody(Body* body, int x, int y, int layer, b2Shape* shape) {
	
// 	if (body->isSolid) {
// 		b2BodyDef bodyDef;
// 		bodyDef.position.x = x;
// 		bodyDef.position.y = y;
// 		if (body->isDynamic)
// 			bodyDef.type = b2_dynamicBody;
// 		body->body = world.CreateBody(&bodyDef);
// 		b2FixtureDef fixtureDef;
// 		if (shape) {
// 			fixtureDef.shape = shape;
// 		} else {
// 			b2PolygonShape rectangle;
// 			rectangle.SetAsBox(0.5f, 0.5f);
// 			fixtureDef.shape = &rectangle;
// 		}
// 		fixtureDef.density = 1;
// 		body->body->CreateFixture(&fixtureDef);
// 	}
	
// 	map[x][y][body->isDynamic] = std::shared_ptr<Body>{body};
	
// 	// // if (!(x >= 0 && y >= 0 && x < width && y < height)) return;

// 	// if (body->isSolid) {
// 	// // if (body->isDynamic) {
// 	// 	b2BodyDef bodyDef;
// 	// 	bodyDef.position.x = x;
// 	// 	bodyDef.position.y = y;
// 	// 	if (body->isDynamic)
// 	// 		bodyDef.type = b2_dynamicBody;
// 	// 	body->body = world.CreateBody(&bodyDef);
// 	// 	b2FixtureDef fixtureDef;
// 	// 	if (shape) {
// 	// 		fixtureDef.shape = shape;
// 	// 	} else {
// 	// 		b2PolygonShape rectangle;
// 	// 		rectangle.SetAsBox(0.5f, 0.5f);
// 	// 		fixtureDef.shape = &rectangle;
// 	// 	}
// 	// 	fixtureDef.density = 1;
// 	// 	body->body->CreateFixture(&fixtureDef);
// 	// }
	
// 	// if (layer >= map[x][y].size()) {
// 	// 	map[x][y].insert(map[x][y].begin() + layer, body);
// 	// } else {
// 	// 	map[x][y][layer] = body;
// 	// }
// }
