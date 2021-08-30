#ifndef ASSETS_HPP
#define ASSETS_HPP

#include "olcPixelGameEngine.h"
#include <string>

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
	
};

#endif