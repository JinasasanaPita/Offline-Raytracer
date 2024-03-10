#pragma once

#include <smath.h>

class Ray
{
public:
	Ray() {};

	Ray(const SMath::Vector3& origin, const SMath::Vector3 direction) :
		_origin(origin), _direction(direction) {}

	SMath::Vector3 origin() const { return _origin; }
	SMath::Vector3 direction() const { return _direction; }

	SMath::Vector3 at(double t) const
	{
		return _origin + _direction * t;
	}

private:
	SMath::Vector3 _origin;
	SMath::Vector3 _direction;
};