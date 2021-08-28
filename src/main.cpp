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

#if 0
clang++ -arch x86_64 -std=c++17 -mmacosx-version-min=10.15 -Wall -framework OpenGL -framework GLUT -framework Carbon -lpng main.cpp -o test 
clang++ main.cpp -o pge -I/Users/maratisaw/Downloads/soloud20200207/include -L/Users/maratisaw/Downloads/soloud20200207/lib -Bstatic -static-libgcc -static-libstdc++ -lX11 -lGL -lpthread -lpng -lstdc++fs -lsoloud_static -lSDL2 -ldl
em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 main.cpp -o pge.html
clang++ -arch x86_64 -std=c++17 main.cpp -I./include -L./lib -framework OpenGL -framework GLUT -framework Carbon -lpng -lSDL2 -lsoloud -o pge
clang++ -arch x86_64 -std=c++17 main.cpp -Iinclude -Llib -framework OpenGL -framework GLUT -framework Carbon -lpng -lSDL2 -lSDL2_mixer -lsoloud_static -ldl -o pge

em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 -s USE_SDL_MIXER=2 main.cpp -o pge.html --preload-file ./assets
em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 -s USE_SDL_MIXER=2 main.cpp soloud.o -o pge.html --preload-file ./assets

em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 -s USE_SDL_MIXER=2 -Iinclude main.cpp soloud.o -o pge.html --preload-file ./assets
clang++ -arch x86_64 -std=c++17 main.cpp -Iinclude -Llib -framework OpenGL -framework GLUT -framework Carbon -lpng -lSDL2 -lSDL2_mixer -lsoloud_static -ldl -o pge
#endif

// Override base class with your custom functionality
class Test : public olc::PixelGameEngine {
public:
	Test()
	{
		// Name your application
		sAppName = "Test";
	}

	olc::Renderable gfx;
	olc::Renderable texture;
	olc::Renderable normalMap;
	uint32_t* data;

	olcPGEX_AudioListener AL;
	olcPGEX_AudioSource AS_Test;
	
	std::vector<olc::vf2d> expl_location;
	std::vector<olcPGEX_Animator2D> expl_anim;
	
	cdr::Renderer* renderer;

public:
	bool OnUserCreate() override {
		// Called once at the start, so create things here
		gfx.Load("./assets/gfx/animationTiles.png");
		texture.Load("./assets/gfx/boxtexture.png");
		// normalMap.Load("./assets/gfx/boxnormal.png");
		// normalMap.Load("./assets/gfx/simple_normalmap.png");
		normalMap.Load("./assets/gfx/normalMap.png");
		
		data = (uint32_t*)gfx.Sprite()->GetData();
		
		// Initialize audio
		AL.AudioSystemInit();
		AS_Test.AL = &AL;
		AS_Test.LoadAudioSample(0, "./assets/sfx/test.wav");
		
		renderer = new cdr::Renderer((uint32_t*)(GetDrawTarget()->GetData()), GetDrawTargetWidth(), GetDrawTargetHeight());
		
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
		tem::vec3 light(GetMouseX() - offset.x, GetMouseY() - offset.y, 1);

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
				tem::vec3 lightRay = tem::vec3(light.x - x, y - light.y, 60);
				
				lightRay.normalize();
				normals.normalize();
				
				float v = lightRay.x * normals.x + lightRay.y * normals.y + lightRay.z * normals.z;
				if (v > 0) 
					Draw({x + offset.x, y + offset.y}, olc::Pixel(v * 255, v * 255, v * 255));
					// Draw({x + offset.x, y + offset.y}, texture.Sprite()->GetPixel(x, y) * v);
			}
		}
		
		return true;
	}
};

#if 0
vec2 pixel = gl_FragCoord.xy / res.xy;
vec4 color = vec4(1.0);//solid white
//Distance of the current pixel from the light position
float dist = distance(gl_FragCoord.xy,light.xy);
vec3 NormalVector = texture2D(norm,pixel).xyz;
vec3 LightVector = vec3(light.x - gl_FragCoord.x,light.y - gl_FragCoord.y,60.0);
NormalVector.x -= 0.5;
NormalVector.y -= 0.5;
NormalVector = normalize(NormalVector);
LightVector = normalize(LightVector);


if(light.z * res.x > dist){
	float diffuse = max(dot( NormalVector, LightVector ),0.0);
	float distanceFactor = (1.0 - dist/(light.z * res.x));
	gl_FragColor = color * diffuse * distanceFactor;
} else {
	gl_FragColor = vec4(0.0);
}
#endif

int main(int argc, char** argv) {

	Test demo;
	if (demo.Construct(600, 400, 2, 2))
		demo.Start();
	
	return 0;
}
