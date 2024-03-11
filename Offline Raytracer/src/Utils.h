#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <random>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double DegreesToRadians(double degrees)
{
	return degrees * pi / 180.0;
}

inline double RandomDouble()
{
	return SMath::Random::UniformFloat();
}

inline double RandomDouble(double min, double max)
{
	return min + (max - min) * RandomDouble();
}