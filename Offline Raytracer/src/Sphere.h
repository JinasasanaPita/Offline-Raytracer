#pragma once

#include <smath.h>

#include "Hittable.h"
#include "Material.h"

class Sphere : public Hittable
{
public:
	Sphere(SMath::Vector3 _center, float _radius, std::shared_ptr<Material> _mat) :
		center(_center), radius(_radius), mat(_mat) {}

	bool Hit(const Ray& r, Interval ray_t, HitRecord& hitRecord)
		const override
	{
		SMath::Vector3 oc = r.origin() - center;
		double a = r.direction().SquareMagnitude();
		double half_b = SMath::Vector3::Dot(oc, r.direction());
		double c = oc.SquareMagnitude() - radius * radius;

		double discriminant = half_b * half_b - a * c;
		if (discriminant < 0) return false;
		double sqrt_discriminant = sqrt(discriminant);

		// Find the nearest root that lies in the acceptable range
		double root = (-half_b - sqrt_discriminant) / a;
		if (!ray_t.Surrounds(root))
		{
			root = (-half_b + sqrt_discriminant) / a;
			if (!ray_t.Surrounds(root))
				return false;
		}

		hitRecord.t = root;
		hitRecord.hitPoint = r.at(hitRecord.t);
		SMath::Vector3 outwardNormal = (hitRecord.hitPoint - center) / radius;
		hitRecord.SetFaceNormal(r, outwardNormal);
		hitRecord.mat = mat;

		return true;
	}

private:
	SMath::Vector3 center;
	double radius;
	std::shared_ptr<Material> mat;
};