#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define AUDIO_LISTENER_IMPLEMENTATION
#include "olcPGEX_AudioListener.h"
#define AUDIO_SOURCE_IMPLEMENTATION
#include "olcPGEX_AudioSource.h"
#define PGEX_ANIMATOR_IMPLEMENTATION
#include "olcPGEX_Animator2D.h"
// #define CIDR_IMPLEMENTATION
// #include "cidr.hpp"

#include "game.hpp"
 
// NOTE: ctrl+j connects lines) 
/* TODO: 
 * Audio 
 * Are we leaking memory? 👀
 * weak_ptr this pointer for bodies?
 * get body from map using bodyIndex
 * add sleep to bodies, ie their update method will not be called if they are sleeping, they will unsleep only if *something* happens
 * the amount of bullets depends on fps because the faster the game updates the faster the player shoots therfore the more bullets come
 */

int main(int argc, char** argv) {
	Game game;
	if (game.Construct(400*constants::windowScale, 250*constants::windowScale, constants::pixelScale, constants::pixelScale, false, false))
		game.Start();
	
	return 0;
}
