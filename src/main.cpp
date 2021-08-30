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

class Test : public olc::PixelGameEngine {
public:
	Test() {
		sAppName = "Game";
	}

	const float SCALE = 1.f;
	
	std::vector<std::vector<char>> mapData {
		{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', 'P', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
	};
	Map map;
	
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
		static float playerX{};
		static float playerY{};
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
		
		map.camera.x = playerX;
		map.camera.y = playerY;
		
		/* GAME LOGIC *******************/

		
		/* RENDER ***********************/
		float visibleTilesX = std::min(ScreenWidth() / assets.spriteWidth, map.width);
		float visibleTilesY = std::min(ScreenHeight() / assets.spriteHeight, map.height);
		float offsetX = map.camera.x - visibleTilesX/2.f;
		float offsetY = map.camera.y - visibleTilesY/2.f;
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
					(float)i * assets.spriteWidth + offsetX - tileOffsetX, 
					(float)j * assets.spriteHeight + offsetY - tileOffsetY
				};
				
				DrawPartialDecal(
					tileLocation, 
					{(float)assets.spriteWidth, (float)assets.spriteHeight},
					assets.spriteAtlas.Decal(),
					{(float)spriteX, (float)spriteY}, 
					{(float)assets.spriteWidth * SCALE, (float)assets.spriteHeight * SCALE}
				);
			}
		}
		
		DrawPartialDecal(
			{
				(float)playerX * assets.spriteWidth - offsetX * assets.spriteWidth, 
				(float)playerY * assets.spriteHeight - offsetY * assets.spriteWidth
			}, 
			{(float)assets.spriteWidth * SCALE, (float)assets.spriteHeight * SCALE},
			assets.spriteAtlas.Decal(),
			{(float)0, (float)0}, 
			{(float)assets.spriteWidth * SCALE, (float)assets.spriteHeight * SCALE}
		);
	
		return true;
	}
};

int main(int argc, char** argv) {

	Test demo;
	if (demo.Construct(200, 150, 4, 4))
		demo.Start();
	
	return 0;
}
