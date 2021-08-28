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

class Test : public olc::PixelGameEngine {
public:
	Test() : gravity(0.0f, 10.f), world(gravity)
	{
		sAppName = "Test";
	}

	olc::Renderable gfx;
	olc::Renderable texture;
	olc::Renderable crate;
	olc::Renderable normalMap;
	uint32_t* data;

	olcPGEX_AudioListener AL;
	olcPGEX_AudioSource AS_Test;
	
	std::vector<olc::vf2d> expl_location;
	std::vector<olcPGEX_Animator2D> expl_anim;
	
	std::unique_ptr<cdr::Renderer> renderer;

	b2Vec2 gravity;
	b2World world;
	b2Body* player;
	static constexpr float SCALE = 30.f;
	
public:
	bool OnUserCreate() override {
		// Called once at the start, so create things here
		gfx.Load("./assets/gfx/animationTiles.png");
		texture.Load("./assets/gfx/boxtexture.png");
		crate.Load("assets/gfx/crate.png");
		// normalMap.Load("./assets/gfx/boxnormal.png");
		// normalMap.Load("./assets/gfx/simple_normalmap.png");
		normalMap.Load("./assets/gfx/normalMap.png");
		
		data = (uint32_t*)gfx.Sprite()->GetData();
		
		// Initialize audio
		AL.AudioSystemInit();
		AS_Test.AL = &AL;
		AS_Test.LoadAudioSample(0, "./assets/sfx/test.wav");
		
		renderer = std::make_unique<cdr::Renderer>(cdr::Renderer((uint32_t*)(GetDrawTarget()->GetData()), GetDrawTargetWidth(), GetDrawTargetHeight()));
		
		b2BodyDef groundDef;
		groundDef.position.Set(600.f/2/SCALE, 400.f/SCALE);
		b2Body* groundBody = world.CreateBody(&groundDef);
		b2PolygonShape rectangle;
		rectangle.SetAsBox(256.f/SCALE, 32.f/SCALE);
		groundBody->CreateFixture(&rectangle, 1);
		b2BodyUserData& userDataFloor = groundBody->GetUserData();
		userDataFloor.pointer = 0xf100;
		
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(480.f/2/SCALE, 0.f/SCALE);
		player = world.CreateBody(&bodyDef);
		rectangle.SetAsBox(32.f/SCALE, 32.f/SCALE);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &rectangle;
		fixtureDef.density = 1.f;
		fixtureDef.friction = 0.3f;
		player->CreateFixture(&fixtureDef);
		b2BodyUserData& userData = player->GetUserData();
		userData.pointer = 0xc12a1e;
		
		for (int i = 0; i < 10; i++) {
			b2BodyDef basic;
			basic.position.Set((200 + i%5 * 32)/SCALE, (100 + i/5 * 32)/SCALE);
			basic.type = b2_dynamicBody;
			b2Body* b = world.CreateBody(&basic);
			b2PolygonShape basicRectangle;
			basicRectangle.SetAsBox(16.f/SCALE, 16.f/SCALE);
			b->CreateFixture(&basicRectangle, 1);
			b2BodyUserData& basicUserData = b->GetUserData();
			basicUserData.pointer = 0xba51c;
		}
		
		return true;
	}
	
	bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::BLACK);
		
		static float counter = 0;
		bool allow = false;
		counter += fElapsedTime;
		if (counter > .1f) {
			allow = true;
			counter -= .1f;
		}
		if (GetMouse(olc::Mouse::LEFT).bHeld) {
			expl_anim.push_back(olcPGEX_Animator2D());
			expl_anim.back().AddAnimation("explosion", 0.2f, 15, gfx.Decal(), { 0, 0 }, { 8*3, 8*3 }, {0, 0}, { -(8*3)/2.f, -(8*3)/2.f });
			expl_anim.back().Play("explosion", true);
			expl_location.push_back(olc::vf2d(GetMouseX(), GetMouseY()));
			
			if (allow) {
				AS_Test.Play();
			}
		}
			
		for (int i = 0; i < expl_location.size(); i++) { 
			if (expl_anim[i].GetAnim("explosion")->bIsPlaying) {
				expl_anim[i].DrawAnimationFrame(expl_location[i]);
				expl_anim[i].UpdateAnimations(fElapsedTime);
			} else {
				expl_anim.erase(expl_anim.begin() + i);
				expl_location.erase(expl_location.begin() + i);
			}
		}
		
		// renderer->FillTriangle(0xffff0000, 0xff00ff00, 0xff0000ff, 10, 10, 100, 200, GetMouseX(), GetMouseY());
		
		olc::Pixel normalPixel = normalMap.Sprite()->GetPixel(GetMouseX(), GetMouseY());
		olc::vi2d offset{GetDrawTargetWidth() / 2 - normalMap.Sprite()->width/2, GetDrawTargetHeight() / 2 - normalMap.Sprite()->height/2};
		tem::vec3 normals = {normalPixel.r / 255.f, normalPixel.g / 255.f, normalPixel.b / 255.f};
		tem::vec3 light(player->GetPosition().x*SCALE - offset.x, player->GetPosition().y*SCALE - offset.y, 1);

		// DrawDecal({0,0}, normalMap.Decal());
		std::string text = std::to_string(normalPixel.r) + ", " + std::to_string(normalPixel.g) + ", " + std::to_string(normalPixel.b);
		DrawString({0, 0}, text);
		text = std::to_string(normals.x) + ", " + std::to_string(normals.y) + ", " + std::to_string(normals.z);
		DrawString({0, 0+16}, text);
		text = std::to_string(GetMouseX()) + ", " + std::to_string(GetMouseY());
		DrawString({0, 0+32}, text);

		for (int y = 0; y < normalMap.Sprite()->height; y++) {
			for (int x = 0; x < normalMap.Sprite()->width; x++) {
				normalPixel = normalMap.Sprite()->GetPixel(x, y);
				normals = {normalPixel.r / 255.f - 0.5f, normalPixel.g / 255.f - 0.5f, normalPixel.b / 255.f};
				tem::vec3 lightRay = tem::vec3(light.x - x, y - light.y, 50);
				
				lightRay.normalize();
				normals.normalize();
				
				float v = std::max(0.f, lightRay.x * normals.x + lightRay.y * normals.y + lightRay.z * normals.z);
				Draw({x + offset.x, y + offset.y}, olc::Pixel(v * 255, v * 255, v * 255));
					// Draw({x + offset.x, y + offset.y}, texture.Sprite()->GetPixel(x, y) * v);
			}
		}
		
		
		bool onGround = false;
		b2Vec2 playerPos = player->GetPosition();
		playerPos.y += 40.f/SCALE;
		for (b2Body* it = world.GetBodyList(); it != 0; it = it->GetNext()) {
			for (b2Fixture* f = it->GetFixtureList(); f != 0; f = f->GetNext()) {
				if (f->TestPoint(playerPos)) {
					onGround = true;
				}
			}
		}
		
		b2Vec2 playerVel = player->GetLinearVelocity();
		if (GetKey(olc::Key::W).bHeld) {
			if (onGround)
				player->ApplyForceToCenter(b2Vec2(0, -700), true);
		}
		if (GetKey(olc::Key::A).bHeld) {
			if (playerVel.x > -20)
				player->ApplyForceToCenter(b2Vec2(-100, 0), true);
		}
		if (GetKey(olc::Key::D).bHeld) {
			if (playerVel.x < 20)
				player->ApplyForceToCenter(b2Vec2(100, 0), true);
		}
		
		for (b2Body* it = world.GetBodyList(); it != 0; it = it->GetNext()) {
			b2Vec2 pos = it->GetPosition();
			float angle = it->GetAngle();
			if (it->GetUserData().pointer == 0xc12a1e) {
				olc::vf2d scale = {64.f / crate.Sprite()->width, 64.f / crate.Sprite()->height};
				DrawRotatedDecal({pos.x*SCALE, pos.y*SCALE}, crate.Decal(), angle, {crate.Sprite()->width/2.f, crate.Sprite()->height/2.f}, scale);
			} else if (it->GetUserData().pointer == 0xf100) {
				olc::vf2d scale = {512.f / crate.Sprite()->width, 64.f / crate.Sprite()->height};
				DrawRotatedDecal({pos.x*SCALE, pos.y*SCALE}, crate.Decal(), angle, {crate.Sprite()->width/2.f, crate.Sprite()->height/2.f}, scale);
			} else if (it->GetUserData().pointer == 0xba51c) {
				olc::vf2d scale = {32.f / crate.Sprite()->width, 32.f / crate.Sprite()->height};
				DrawRotatedDecal({pos.x*SCALE, pos.y*SCALE}, crate.Decal(), angle, {crate.Sprite()->width/2.f, crate.Sprite()->height/2.f}, scale);
			}
		}
		
		world.Step(fElapsedTime, 8, 3);
		
		return true;
	}
};

int main(int argc, char** argv) {

	Test demo;
	if (demo.Construct(640, 480, 2, 2))
		demo.Start();
	
	return 0;
}
