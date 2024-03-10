#pragma once

#include "Hittable.h"

#include <memory>
#include <vector>

class Scene : public Hittable
{
public:
	std::vector<std::shared_ptr<Hittable>> objects;

	Scene() {}
	Scene(std::shared_ptr<Hittable> object) { add(object); }

	void clear() { objects.clear(); }

	void add(std::shared_ptr<Hittable> object)
	{
		objects.push_back(object);
	}

	bool Hit(const Ray& r, Interval ray_t, HitRecord& hitRecord) const override
	{
		HitRecord tempRec;
		bool hitAnything = false;
		double closestSoFar = ray_t.max;

		for (const auto& object : objects)
		{
			if (object->Hit(r, Interval(ray_t.min, closestSoFar), tempRec))
			{
				hitAnything = true;
				closestSoFar = tempRec.t;
				hitRecord = tempRec;
				hitRecord.mat = object->mat;
			}
		}

		return hitAnything;
	}
};