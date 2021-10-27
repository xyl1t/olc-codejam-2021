#ifndef COMMON_HPP
#define COMMON_HPP
#include <cmath>

inline const float WorldScale = 16; // what is this? I forgot
namespace constants {
inline const float windowScale = 1.f;
inline const int pixelScale = 3.f;
inline const float timestep = 1 / 240.f;	
}


inline float lerp(float a, float b, float t) {
	return a + t * (b - a);
}
inline float normalizeAngle(float angle) {
	angle = std::fmod(angle, M_PI*2);
	if (angle < 0) angle += M_PI*2;
	return angle;
}

#endif