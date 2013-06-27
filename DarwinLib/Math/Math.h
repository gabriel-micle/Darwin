
#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <cassert>
#include <algorithm>

#include "Vector.h"
#include "Matrix.h"

#define M_EPS	1e-6f


inline double log2 (const double x) { 
	return log(x) / M_LN2; 
}

inline float log2f (const float x) {
	return static_cast<float>( log2( static_cast<double>(x) ) );
}

inline float log2 (const float x) { 
	return log2f(x); 
}


inline double degrees (const double radians) {
	return (radians * 180.0 / M_PI);
}

inline float degreesf (const float radians) {
	return static_cast<float>( degrees( static_cast<double>(radians) ) );
}

inline float degrees (const float radians) {
	return degreesf(radians); 
}


inline double radians (const double degrees) {
	return (degrees * M_PI / 180.0);
}

inline float radiansf (const float degrees) {
	return static_cast<float>( radians( static_cast<double>(degrees) ) );
}

inline float radians (const float degrees) {
	return radiansf(degrees);
}
