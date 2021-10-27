#include "game.hpp"
#include "generation.hpp"
#include <chrono>

#define USE_DECALS

Game::Game() {
	sAppName = "Game";
}

bool Game::OnUserCreate() {
	assets.Load("../assets/gfx/tileset.png", 16, 16);
	
	player = std::make_shared<Player>(0, 0);

	bool proceduralMap = false;
	 
	if (proceduralMap) {
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		srand(millis);
		Generation gen(128, 128, 4, 4, 4, 4);
		if(gen.Start(30, 10, 16, 10, 16) == -1) {
			std::cout << "ERROR: The rooms can't fit into the map, decrease the minimum rooms size or the amount" << std::endl;
			return false;
		}
		else {
			for(int j = 0; j < gen.HEIGHT; j++)
				std::cout << gen.map[j] << std::endl;
			map.Load(gen.map, player);
		}
	} else {
		map.Load(mapData, player); 
	}
	
	return true;
}

bool Game::OnUserUpdate(float fElapsedTime) {
	
	/* INPUT ************************/
	// TODO: move this to player class
	// what? why?
	// hmmm
	
	if ((GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::W).bHeld) && player->body->GetLinearVelocity().y > -5) {
		player->body->ApplyForceToCenter(b2Vec2(0, -10000*fElapsedTime), true);
	}
	if ((GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::S).bHeld) && player->body->GetLinearVelocity().y < 5) {
		player->body->ApplyForceToCenter(b2Vec2(0, 10000*fElapsedTime), true);
	}
	if ((GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::A).bHeld) && player->body->GetLinearVelocity().x > -5) {
		player->body->ApplyForceToCenter(b2Vec2(-10000*fElapsedTime, 0), true);
	}
	if ((GetKey(olc::Key::RIGHT).bHeld || GetKey(olc::Key::D).bHeld) && player->body->GetLinearVelocity().x < 5) {
		player->body->ApplyForceToCenter(b2Vec2(10000*fElapsedTime, 0), true);
	}
	
	if (GetMouse(olc::Mouse::LEFT).bPressed || GetMouse(olc::Mouse::RIGHT).bHeld) {
		player->Attack();
	}
	
	if (GetKey(olc::Key::F3).bPressed) {
		debugView = !debugView;
	}
	
	
	// /* GAME LOGIC *******************/
	
	// map.camera.x = player->body->GetPosition().x;
	// map.camera.y = player->body->GetPosition().y;
	map.camera.x = lerp(map.camera.x, player->body->GetPosition().x, 5.f * fElapsedTime);
	map.camera.y = lerp(map.camera.y, player->body->GetPosition().y, 5.f * fElapsedTime);
	
	float visibleTilesX = std::min(ScreenWidth()  / constants::windowScale / assets.spriteWidth,  (float)map.width);
	float visibleTilesY = std::min(ScreenHeight() / constants::windowScale / assets.spriteHeight, (float)map.height);
	float offsetX = map.camera.x - visibleTilesX/2.f + 0.5f;
	float offsetY = map.camera.y - visibleTilesY/2.f + 0.5f;
	offsetX = std::max(std::min(offsetX, map.width - visibleTilesX), 0.f);
	offsetY = std::max(std::min(offsetY, map.height - visibleTilesY), 0.f);
	
	olc::vf2d playerSpritePos = {
		(float(player->body->GetPosition().x) * assets.spriteWidth  - offsetX * assets.spriteWidth) * constants::windowScale,
		(float(player->body->GetPosition().y) * assets.spriteHeight - offsetY * assets.spriteWidth) * constants::windowScale
	};
	player->directionAngle = std::atan2(GetMouseY() - playerSpritePos.y - assets.spriteWidth/2*constants::windowScale, GetMouseX() - playerSpritePos.x - assets.spriteHeight/2*constants::windowScale);
	
	map.Update(fElapsedTime);
	
	/* RENDER ***********************/
#ifndef USE_DECALS
	Clear(olc::Pixel(0x3C, 0x3B, 0x40));
#endif
	
	map.Draw(*this, {offsetX, offsetY});

	if (debugView) {
		static olc::Sprite* s = new olc::Sprite(GetDrawTargetWidth(), GetDrawTargetHeight());
		static olc::Decal* d{nullptr};
		
		SetDrawTarget(s);
		Clear({0, 0, 0, 0});
		
		for (b2Body* it = map.world.GetBodyList(); it != nullptr; it = it->GetNext()) {
			b2Vec2 v = it->GetPosition();
			b2Shape* shape = it->GetFixtureList()[0].GetShape();
			
			olc::vf2d tileLocation {
				((float)v.x * assets.spriteWidth  - offsetX * assets.spriteWidth ) * constants::windowScale, 
				((float)v.y * assets.spriteHeight - offsetY * assets.spriteHeight) * constants::windowScale
			};
			
			SetPixelMode(olc::Pixel::Mode::ALPHA);
			if (shape->GetType() == b2Shape::Type::e_polygon) {
				DrawRect(
					tileLocation,
					{int(assets.spriteWidth*constants::windowScale), int(assets.spriteHeight*constants::windowScale)},
					{128, 128, 255, 255}
				);
			} else if (shape->GetType() == b2Shape::Type::e_circle) {
				DrawCircle(
					{int(tileLocation.x + assets.spriteWidth/2*constants::windowScale), int(tileLocation.y + assets.spriteWidth/2*constants::windowScale)}, 
					shape->m_radius * assets.spriteWidth*constants::windowScale,
					{128, 255, 128, 255}
				);
			}
			// Draws the angle the dynamic body looks at
			if (it->GetType() == b2_dynamicBody) {
				tileLocation.x += assets.spriteWidth /2.f*constants::windowScale;
				tileLocation.y += assets.spriteHeight/2.f*constants::windowScale;
				float lookDirX = std::cos(it->GetAngle())*16*constants::windowScale;
				float lookDirY = std::sin(it->GetAngle())*16*constants::windowScale;
				DrawLineDecal(
					tileLocation, 
					{tileLocation.x + lookDirX, tileLocation.y + lookDirY}, 
					{255,255,255,64});
			}
			
			SetPixelMode(olc::Pixel::Mode::NORMAL);
		}
		
		SetDrawTarget((uint8_t)0);
		
		delete d;
		d = new olc::Decal(s);
		
		DrawDecal({0, 0}, d);
	}
	
	return true;
}

void Game::DrawSprite(SpriteID spriteID, const olc::vf2d& pos) {
#ifdef USE_DECALS
	DrawPartialDecal(
		pos,
		{assets.spriteWidth*constants::windowScale, assets.spriteHeight*constants::windowScale},
		assets.spriteAtlas.Decal(),
		assets.GetSprite(spriteID),
		{(float)assets.spriteWidth, (float)assets.spriteHeight}
	);
#else
	DrawPartialSprite(
		pos,
		assets.spriteAtlas.Sprite(),
		assets.GetSprite(spriteID), 
		{assets.spriteWidth, assets.spriteHeight},
		constants::windowScale
	);
#endif
}


/*
RENDERING ONLY VISIBLE TILES
NOTICE ONLY RENDERING FIRST LAYER
NOT OPTIMAL - PROBLEMS WITH RENDERING AGENTS, DOESN'T GIVE ANY PERFORMANCE BOOST...
for (int layer = 0; layer < 1; layer++) {
	// for (int x = 0; x < map.map[layer].size(); x++) {
	// 	for (int y = 0; y < map.map[layer][x].size(); y++) {	
	for (int x = 0; x < visibleTilesX+1; x++) {
		for (int y = 0; y < visibleTilesY+1; y++) {	
			if (x + offsetX < 0 || y + offsetY < 0 || x + offsetX >= map.width || y + offsetY >= map.height) continue;

			if (!map.map[layer][x+offsetX][y+offsetY]) continue;
			Body& body = *(map.map[layer][x+offsetX][y+offsetY]);
			
			olc::vf2d tileLocation;
			if (layer != 1) {
				tileLocation = {
					((float)x * assets.spriteWidth  - tileOffsetX) * constants::windowScale, 
					((float)y * assets.spriteHeight - tileOffsetY) * constants::windowScale
				};
			} else {
				tileLocation = {
					((float)body.body->GetPosition().x * assets.spriteWidth - tileOffsetX) * constants::windowScale, 
					((float)body.body->GetPosition().y * assets.spriteHeight - tileOffsetY) * constants::windowScale
				};
			}
			
			// if (tileLocation.x + assets.spriteWidth < 0 || tileLocation.y + assets.spriteHeight < 0 || tileLocation.x > GetDrawTargetWidth() || tileLocation.y > GetDrawTargetHeight()) continue;
			
			body.Draw(*this, tileLocation);
		}
	}
}
*/