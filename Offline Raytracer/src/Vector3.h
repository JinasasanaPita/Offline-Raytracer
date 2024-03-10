#pragma once

#include <smath.h>

#include "Utils.h"


class Vector3
{
public:

	static SMath::Vector3 Random()
	{
		return SMath::Vector3(SMath::Random::UniformFloat(), 
			SMath::Random::UniformFloat(), SMath::Random::UniformFloat());
	}

	static SMath::Vector3 Random(double min, double max)
	{
		return SMath::Vector3(RandomDouble(min, max), RandomDouble(min, max), 
			RandomDouble(min, max));
	}

	static bool NearZero(SMath::Vector3 vec)
	{
		double s = 1e-8;
		return (vec.x < s) && (vec.y < s) && (vec.z < s);
	}

	static SMath::Vector3 Reflect(const SMath::Vector3& vec, const SMath::Vector3& normal)
	{
		return vec - normal * 2 * SMath::Vector3::Dot(vec, normal);
	}
};

inline SMath::Vector3 RandomInUnitSphere()
{
	while (true)
	{
		SMath::Vector3 p = Vector3::Random(-1, 1);
		if (p.SquareMagnitude() < 1)
			return p;
	}
}

inline SMath::Vector3 RandomUnitVector()
{
	return RandomInUnitSphere().Normalized();
}

inline SMath::Vector3 FindHemisphereOrientation(const SMath::Vector3 normal)
{
	SMath::Vector3 onUnitSphere = RandomUnitVector();
	if (SMath::Vector3::Dot(onUnitSphere, normal) > 0.0)
		return onUnitSphere;
	else
		return -onUnitSphere;
}

inline SMath::Vector3 Refract(const SMath::Vector3& uv, const SMath::Vector3& normal,
	double etai_over_etat)
{
	double cosTheta = fmin(SMath::Vector3::Dot(-uv, normal), 1.0);
	SMath::Vector3 r_out_perpendicular = (uv + normal * cosTheta) * etai_over_etat;
	SMath::Vector3 r_out_parallel = normal * -sqrt(fabs(1.0 - 
		r_out_perpendicular.SquareMagnitude()));
	return r_out_perpendicular + r_out_parallel;
}