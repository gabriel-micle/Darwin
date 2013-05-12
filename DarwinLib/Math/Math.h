
#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <cassert>

#include "Vector.h"
#include "Matrix.h"


inline double log2 (const double x) { return log(x) / M_LN2; }

inline float log2f (const float x) { return (float) (log2((double) x)); }

inline float log2 (const float x) { return log2f(x); }
