#include "integrator.h"

/*
Integrator::Integrator(Primitive aggregate, std::vector<Light> lights)
	: aggregate(aggregate), lights(lights)
{
	Bounds3f sceneBounds = aggregate ? aggregate.Bounds() : Bounds3f();
	for (Light &light : lights) 
	{
		light.Preprocess(sceneBounds);
		if (light.Type == LightType::Infinite)
			infiniteLights.push_back(light);
	}
}
*/