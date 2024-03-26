#pragma once
#include <optional>

class Integrator
{
public:
	// Primitive aggregate;
	// std::vector<Light> lights;
	// std::vector<Light> infiniteLights;

	virtual void Render() = 0;

	// pstd::optional<ShapeIntersection> Intersect(const Ray &ray, float tMax = Infinity) const;
	

protected:
	/* Integrator(Primitive aggregate, std::vector<Light> lights)
		: aggregate(aggregate), lights(lights) {}*/
};

class ImageTileIntegrator : public Integrator
{
public:
	/* ImageTileIntegrator(Camera camera, Sampler sampler, Primitive aggregate, 
		std::vector<Light> lights) : Integrator(aggregate, lights), camera(camera), 
		samplerPrototype(sampler) {}*/

	void Render();

protected:
	// Camera camera;
	// Sampler samplerPrototype;
};