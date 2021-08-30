#include "assets.hpp"

Assets::Assets() : spriteWidth(), spriteHeight(), spriteAtlasWidth(), spriteAtlasHeight(), spriteAtlas() {	
}

void Assets::Load(const std::string& spriteAtlas, int spriteWidth, int spriteHeight) {
	this->spriteAtlas.Load(spriteAtlas);
	this->spriteWidth = spriteWidth;
	this->spriteHeight = spriteHeight;
	this->spriteAtlasWidth = this->spriteAtlas.Sprite()->width;
	this->spriteAtlasHeight = this->spriteAtlas.Sprite()->height;
}
