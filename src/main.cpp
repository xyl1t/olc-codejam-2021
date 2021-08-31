#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define AUDIO_LISTENER_IMPLEMENTATION
#include "olcPGEX_AudioListener.h"
#define AUDIO_SOURCE_IMPLEMENTATION
#include "olcPGEX_AudioSource.h"
#include "olcPGEX_Animator2D.h"
#define CIDR_IMPLEMENTATION
#include "cidr.hpp"
#include "box2d/box2d.h"

#include <vector>
#include <algorithm>
#include <memory>

#include "map.hpp"
#include "assets.hpp"

/* TODO:
 * Collision callbacks (for shooting) http://www.iforce2d.net/b2dtut/collision-callbacks
 *
 */

static inline float lerp(float a, float b, float t) {
	return a + t * (b - a);
}

class Test : public olc::PixelGameEngine {
public:
	Test() {
		sAppName = "Game";
	}

	const float SCALE = 1.f;
	
	std::vector<std::vector<char>> mapData {
		{ '#', ' ', '#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
		{ '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', 'P', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', '#', '#', '#', ' ', '#', '#', '#', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', '#', '#', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
	};
	Map map; // TODO: make multiple maps (levels) and have a pointer to current one
	b2World* world;
	
	Assets assets;
	
public:
	bool OnUserCreate() override {
		
		map.Load(mapData);
		assets.Load("./assets/gfx/tileset.png", 16, 16);
		
		return true;
	}
	
	bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::Pixel(0xff3c3b40));
		
		/* INPUT ************************/
		// TODO: move this to player class
		static float playerX{map.spawnPoint.x};
		static float playerY{map.spawnPoint.y};
		if (GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::W).bHeld) {
			playerY -= 6 * fElapsedTime;
		}
		if (GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::S).bHeld) {
			playerY += 6 * fElapsedTime;
		}
		if (GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::A).bHeld) {
			playerX -= 6 * fElapsedTime;
		}
		if (GetKey(olc::Key::RIGHT).bHeld || GetKey(olc::Key::D).bHeld) {
			playerX += 6 * fElapsedTime;
		}
		
		map.camera.x = lerp(map.camera.x, playerX, 5.f * fElapsedTime);
		map.camera.y = lerp(map.camera.y, playerY, 5.f * fElapsedTime);
		
		/* GAME LOGIC *******************/

		
		/* RENDER ***********************/
		float visibleTilesX = std::min(ScreenWidth() / assets.spriteWidth / SCALE,   (float)map.width);
		float visibleTilesY = std::min(ScreenHeight() / assets.spriteHeight / SCALE, (float)map.height);
		float offsetX = map.camera.x - visibleTilesX/2.f + 0.5f;
		float offsetY = map.camera.y - visibleTilesY/2.f + 0.5f;
		offsetX = std::max(std::min(offsetX, map.width - visibleTilesX), 0.f);
		offsetY = std::max(std::min(offsetY, map.height - visibleTilesY), 0.f);
		
		float tileOffsetX = (offsetX - (int)offsetX) * assets.spriteWidth;
		float tileOffsetY = (offsetY - (int)offsetY) * assets.spriteHeight;
		
		for (int i = -1; i < visibleTilesX + 2; i++) {
			for (int j = -1; j < visibleTilesY + 2; j++) {
				if (i + offsetX < 0 || j + offsetY < 0 || i + offsetX >= map.width || j + offsetY >= map.height) continue;
		// for (int i = 0; i < visibleTilesX; i++) {
		// 	for (int j = 0; j < visibleTilesY; j++) {
				int index = map.GetSpriteIndex(i + offsetX, j + offsetY);
				int spriteX = index % (assets.spriteAtlasWidth / assets.spriteWidth) * assets.spriteWidth;
				int spriteY = index / (assets.spriteAtlasWidth / assets.spriteWidth) * assets.spriteHeight;
				
				olc::vf2d tileLocation {
					((float)i * assets.spriteWidth  + offsetX - tileOffsetX) * SCALE, 
					((float)j * assets.spriteHeight + offsetY - tileOffsetY) * SCALE
				};
				
				DrawPartialDecal(
					tileLocation, 
					{(float)assets.spriteWidth * SCALE, (float)assets.spriteHeight * SCALE},
					assets.spriteAtlas.Decal(),
					{(float)spriteX, (float)spriteY}, 
					{(float)assets.spriteWidth, (float)assets.spriteHeight}
				);
				// Body b = map.Get(i + offsetX, j + offsetY);
				// if(b.body) {
				// 	// b.body->GetPosition().x;
				// 	// b.body->GetPosition().y;
				// 	tileLocation = {
				// 		((float)b.body->GetPosition().x * assets.spriteWidth  + offsetX - tileOffsetX) * SCALE, 
				// 		((float)b.body->GetPosition().y * assets.spriteHeight + offsetY - tileOffsetY) * SCALE
				// 	};
				// 	FillRectDecal(
				// 		tileLocation,
				// 		{(float)(assets.spriteWidth * SCALE), (float)(assets.spriteHeight * SCALE)},
				// 		{255, 255, 255, 128}
				// 	);
				// }
				
			}
		}
		
		DrawPartialDecal(
			{
				((float)playerX * assets.spriteWidth  - offsetX * assets.spriteWidth) * SCALE,
				((float)playerY * assets.spriteHeight - offsetY * assets.spriteWidth) * SCALE
			}, 
			{(float)assets.spriteWidth * SCALE, (float)assets.spriteHeight * SCALE},
			assets.spriteAtlas.Decal(),
			{(float)0, (float)0}, 
			{(float)assets.spriteWidth, (float)assets.spriteHeight}
		);
	
		return true;
	}
};

int main(int argc, char** argv) {

	Test demo;
	if (demo.Construct(200*2, 150*2, 2, 2))
		demo.Start();
	
	return 0;
}
