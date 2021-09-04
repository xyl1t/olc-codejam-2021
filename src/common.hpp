#ifndef COMMON_HPP
#define COMMON_HPP
#include <cmath>

inline const float WorldScale = 16;
inline float lerp(float a, float b, float t) {
	return a + t * (b - a);
}
inline float normalizeAngle(float angle) {
	angle = std::fmodf(angle, M_PI*2);
	if (angle < 0) angle += M_PI*2;
	return angle;
}

#endif