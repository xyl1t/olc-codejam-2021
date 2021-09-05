#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define AUDIO_LISTENER_IMPLEMENTATION
#include "olcPGEX_AudioListener.h"
#define AUDIO_SOURCE_IMPLEMENTATION
#include "olcPGEX_AudioSource.h"
#include "olcPGEX_Animator2D.h"
// #define CIDR_IMPLEMENTATION
// #include "cidr.hpp"
#include "box2d/box2d.h"

#include <vector>
#include <algorithm>
#include <memory>
#include <string>

#include "map.hpp"
#include "assets.hpp"
#include "player.hpp"
#include "common.hpp"

/* TODO:
 * Collision callbacks (for shooting) http://www.iforce2d.net/b2dtut/collision-callbacks
 *
 */

class Test : public olc::PixelGameEngine {
public:
	Test() {
		sAppName = "Game";
	}

	const float SCALE = 1.f;
	
	std::vector<std::string> mapData {
		{"----------------------------------------------"},
		{"----------------------------------------------"},
		{"----------------------------------------------"},
		{"----------------------------------------------"},
		{"----------------------------------------------"},
		{"--------############################----------"},
		{"--------#..........................#----------"},
		{"--------#........P.............##.##----------"},
		{"--------#......................#...#----------"},
		{"--------#......................#...#----------"},
		{"--------#.........#.#..........#...#----------"},
		{"--------#.........#.#..........#...#----------"},
		{"--------#........##.##.........##.####--------"},
		{"--------#.........###..........#.....#--------"},
		{"--------#......................#.....#--------"},
		{"--------#......................#.....#--------"},
		{"--------#......................#.....#--------"},
		{"--------#......................#.....#--------"},
		{"--------#......................#.....#--------"},
		{"--------#......................#.....#--------"},
		{"--------#..........#############.....#--------"},
		{"--------#####.######.................#--------"},
		{"--------#..........#.................#--------"},
		{"--------#....................#########--------"},
		{"--------#..........#.........#----------------"},
		{"--------##########.###########----------------"},
		{"-------------#.....#--------------------------"},
		{"-------------#.....#--------------------------"},
		{"-------------#.....#--------------------------"},
		{"-------------#######--------------------------"},
		{"----------------------------------------------"},
		{"----------------------------------------------"},
		{"----------------------------------------------"},
		{"----------------------------------------------"},
	};
	Map map; // TODO: make multiple maps (levels) and have a pointer to current one
	b2World* world;
	Player player;
	
	Assets assets;
	
public:
	bool OnUserCreate() override {
		map.Load(mapData);
		assets.Load("../assets/gfx/tileset.png", 16, 16);
		player.body = map.playerBody;
		
		return true;
	}
	
	bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::Pixel(0x3C, 0x3B, 0x40));
		
		/* INPUT ************************/
		// TODO: move this to player class
		
		if ((GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::W).bHeld) && player.body->GetLinearVelocity().y > -5) {
			// player.body->SetLinearVelocity({0, player.body->GetLinearVelocity().y});
			player.body->ApplyForceToCenter(b2Vec2(0, -70), true);
			// player.spriteID = SpriteID::PLAYER_UP;
		}
		if ((GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::S).bHeld) && player.body->GetLinearVelocity().y < 5) {
			// player.body->SetLinearVelocity({0, player.body->GetLinearVelocity().y});
			player.body->ApplyForceToCenter(b2Vec2(0, 70), true);
			// player.spriteID = SpriteID::PLAYER_DOWN;
		}
		if ((GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::A).bHeld) && player.body->GetLinearVelocity().x > -5) {
			// player.body->SetLinearVelocity({player.body->GetLinearVelocity().x, 0});
			player.body->ApplyForceToCenter(b2Vec2(-70, 0), true);
			// player.spriteID = SpriteID::PLAYER_LEFT;
		}
		if ((GetKey(olc::Key::RIGHT).bHeld || GetKey(olc::Key::D).bHeld) && player.body->GetLinearVelocity().x < 5) {
			// player.body->SetLinearVelocity({player.body->GetLinearVelocity().x, 0});
			player.body->ApplyForceToCenter(b2Vec2(70, 0), true);
			// player.spriteID = SpriteID::PLAYER_RIGHT;
		}
		// if ((GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::W).bHeld) && (GetKey(olc::Key::RIGHT).bHeld || GetKey(olc::Key::D).bHeld)) {
		// 	player.spriteID = SpriteID::PLAYER_UP_RIGHT;
		// }
		// if ((GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::W).bHeld) && (GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::A).bHeld)) {
		// 	player.spriteID = SpriteID::PLAYER_UP_LEFT;
		// }
		// if ((GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::S).bHeld) && (GetKey(olc::Key::RIGHT).bHeld || GetKey(olc::Key::D).bHeld)) {
		// 	player.spriteID = SpriteID::PLAYER_DOWN_RIGHT;
		// }
		// if ((GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::S).bHeld) && (GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::A).bHeld)) {
		// 	player.spriteID = SpriteID::PLAYER_DOWN_LEFT;
		// }
		
		
		// map.camera.x = player.body->GetPosition().x;
		// map.camera.y = player.body->GetPosition().y;
		map.camera.x = lerp(map.camera.x, player.body->GetPosition().x, 5.f * fElapsedTime);
		map.camera.y = lerp(map.camera.y, player.body->GetPosition().y, 5.f * fElapsedTime);
		
		/* GAME LOGIC *******************/
		player.Update(fElapsedTime);
				
		/* RENDER ***********************/
		float visibleTilesX = std::min(ScreenWidth()  / SCALE / assets.spriteWidth,  (float)map.width);
		float visibleTilesY = std::min(ScreenHeight() / SCALE / assets.spriteHeight, (float)map.height);
		float offsetX = map.camera.x - visibleTilesX/2.f + 0.5f;
		float offsetY = map.camera.y - visibleTilesY/2.f + 0.5f;
		offsetX = std::max(std::min(offsetX, map.width - visibleTilesX), 0.f);
		offsetY = std::max(std::min(offsetY, map.height - visibleTilesY), 0.f);
		
		float tileOffsetX = (offsetX - (int)offsetX) * assets.spriteWidth;
		float tileOffsetY = (offsetY - (int)offsetY) * assets.spriteHeight;
		
		for (int i = -1; i < visibleTilesX + 2; i++) {
			for (int j = -1; j < visibleTilesY + 2; j++) {
				if (i + offsetX < 0 || j + offsetY < 0 || i + offsetX >= map.width || j + offsetY >= map.height) continue;
				int index = map.GetSpriteIndex(i + offsetX, j + offsetY);
				auto [spriteX, spriteY] = assets.GetSpriteXYFromIndex(index);
				
				olc::vf2d tileLocation {
					((float)i * assets.spriteWidth  - tileOffsetX) * SCALE, 
					((float)j * assets.spriteHeight - tileOffsetY) * SCALE
				};
				
				DrawPartialDecal(
					tileLocation,
					{(float)assets.spriteWidth * SCALE, (float)assets.spriteHeight * SCALE},
					assets.spriteAtlas.Decal(),
					{(float)spriteX, (float)spriteY}, 
					{(float)assets.spriteWidth, (float)assets.spriteHeight}
				);
			}
		}
		
		
		olc::vf2d playerSpritePos = {
			(float(player.body->GetPosition().x) * assets.spriteWidth  - offsetX * assets.spriteWidth) * SCALE,
			(float(player.body->GetPosition().y) * assets.spriteHeight - offsetY * assets.spriteWidth) * SCALE
		};
		float mouseAngle = std::atan2f(GetMouseY() - playerSpritePos.y, GetMouseX() - playerSpritePos.x);
		player.directionAngle = mouseAngle;
		// float playerAngle = mouseAngle + M_PI*2/8 - (M_PI*2/8)/2;
		// if (playerAngle < 0) playerAngle = playerAngle + M_PI*2;
		// if (playerAngle > M_PI*2) playerAngle -= M_PI * 2; 
		// float val = playerAngle / (M_PI*2);
		// int playerSpriteIdx = val * 8;
		// auto [playerSpriteX, playerSpriteY] = assets.GetSpriteXYFromIndex(playerSpriteIdx);
		DrawPartialDecal(
			playerSpritePos,
			{assets.spriteWidth*SCALE, assets.spriteHeight*SCALE},
			assets.spriteAtlas.Decal(),
			assets.GetSprite(player.spriteID),
			{(float)assets.spriteWidth, (float)assets.spriteHeight}
		);
		DrawPartialDecal(
			playerSpritePos,
			{assets.spriteWidth*SCALE, assets.spriteHeight*SCALE},
			assets.spriteAtlas.Decal(),
			assets.GetSprite(player.topSpriteID),
			{(float)assets.spriteWidth, (float)assets.spriteHeight}
		);
		
		// float desired = mouseAngle;
		// float smooth = lerp(player.body->GetAngle(), desired, 0.05);
		// DrawString({0,0}, std::to_string(player.body->GetAngle()));
		// player.body->ApplyTorque((mouseAngle - player.body->GetAngle()) * 10, true);
		// player.body->ApplyAngularImpulse((mouseAngle - player.body->GetAngle()), true);
		// player.body->SetTransform(player.body->GetPosition(), smooth);
		// DrawPartialRotatedDecal(
		// 	playerSpritePos,
		// 	assets.spriteAtlas.Decal(),
		// 	player.body->GetAngle(),
		// 	{assets.spriteWidth/2.f, assets.spriteHeight/2.f},
		// 	{0,0},
		// 	{(float)assets.spriteWidth, (float)assets.spriteHeight},
		// 	{SCALE, SCALE}
		// );
		
		playerSpritePos.x += assets.spriteWidth /2.f;
		playerSpritePos.y += assets.spriteHeight/2.f;
		float testX = std::cos(player.body->GetAngle())*16*SCALE;
		float testY = std::sin(player.body->GetAngle())*16*SCALE;
		// DrawLineDecal(playerSpritePos, {playerSpritePos.x + testX, playerSpritePos.y + testY});
		
		// DrawCircle(
		// 	{
		// 		(int)(player.body->GetPosition().x * assets.spriteWidth  +assets.spriteWidth /2 - offsetX * assets.spriteWidth * SCALE),
		// 		(int)(player.body->GetPosition().y * assets.spriteHeight +assets.spriteHeight/2 - offsetY * assets.spriteWidth * SCALE)
		// 	},
		// 	player.body->GetFixtureList()[0].GetShape()->m_radius * assets.spriteWidth
		// );
		
		for (int i = -1; i < visibleTilesX + 2; i++) {
			for (int j = -1; j < visibleTilesY + 2; j++) {
				if (i + offsetX < 0 || j + offsetY < 0 || i + offsetX >= map.width || j + offsetY >= map.height) continue;
				const auto& b = map.Get(i + offsetX, j + offsetY, 1);
				
				olc::vf2d tileLocation {
					((float)i * assets.spriteWidth  - tileOffsetX) * SCALE, 
					((float)j * assets.spriteHeight - tileOffsetY) * SCALE
				};
				
				DrawPartialDecal(
					tileLocation,
					{(float)assets.spriteWidth * SCALE, (float)assets.spriteHeight * SCALE},
					assets.spriteAtlas.Decal(),
					assets.GetSprite(b.spriteID), 
					{(float)assets.spriteWidth, (float)assets.spriteHeight}
				);
			}
		}
		
		map.world.Step(fElapsedTime, 16, 4);
	
		return true;
	}
};

int main(int argc, char** argv) {

	Test demo;
	if (demo.Construct(300*1, 200*1, 3, 3, false, false))
		demo.Start();
	
	return 0;
}
