#pragma once

#include <smath.h>
#include "Ray.h"
#include "Interval.h"

class Material;

class HitRecord
{
public:
	SMath::Vector3 hitPoint;
	SMath::Vector3 normal;
	std::shared_ptr<Material> mat;

	double t;
	bool isFrontFacing;

	void SetFaceNormal(const Ray& r, const SMath::Vector3 outwardNormal)
	{
		isFrontFacing = SMath::Vector3::Dot(r.direction(), outwardNormal) < 0;
		normal = isFrontFacing ? outwardNormal : -outwardNormal;
	}
};

class Hittable
{
public:
	std::shared_ptr<Material> mat;

	virtual ~Hittable() = default;
	virtual bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;
};