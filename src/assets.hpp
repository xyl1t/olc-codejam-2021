#ifndef ASSETS_HPP
#define ASSETS_HPP

#include "olcPixelGameEngine.h"
#include <string>
#include <utility>
#include "body.hpp"

class Assets {
public:
	int spriteWidth;
	int spriteHeight;
	int spriteAtlasWidth;
	int spriteAtlasHeight;
	olc::Renderable spriteAtlas;
	
public:
	Assets();
	void Load(const std::string& spriteAtlas, int spriteWidth, int spriteHeight);
	inline std::pair<int, int> GetSpriteXYFromIndex(int index) {
		return { 
			index % (spriteAtlasWidth / spriteWidth) * spriteWidth, 
			index / (spriteAtlasWidth / spriteWidth) * spriteWidth 
		};
	}
	inline olc::vf2d GetSprite(int index) {
		return { 
			float(index % (spriteAtlasWidth / spriteWidth) * spriteWidth), 
			float(index / (spriteAtlasWidth / spriteWidth) * spriteWidth) 
		};
	}
	inline olc::vf2d GetSprite(SpriteID spriteID) {
		return { 
			float((int)spriteID % (spriteAtlasWidth / spriteWidth) * spriteWidth), 
			float((int)spriteID / (spriteAtlasWidth / spriteWidth) * spriteWidth) 
		};
	}
};

#endif