#pragma once

#include <smath.h>
#include "Ray.h"
#include "Vector3.h"

class HitRecord;

class Material
{
public:
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& r_in, const HitRecord& hitRecord, 
		SMath::Vector3& attenuation, Ray& scattered) const = 0;
	/*Material() : albedo(SMath::Vector3(1.0, 1.0, 1.0)) {}

	SMath::Vector3 albedo;*/
};

class Lambertian : public Material
{
public:
	Lambertian(const SMath::Vector3& _albedo) : albedo(_albedo) {}

	bool Scatter(const Ray& r_in, const HitRecord& hitRecord,
		SMath::Vector3& attenuation, Ray& scattered) const override
	{
		// Cosine weighted
		SMath::Vector3 scatterDirection = 
			hitRecord.normal + RandomUnitVector();
		

		scattered = Ray(hitRecord.hitPoint, scatterDirection);
		double cosTheta = SMath::Vector3::Dot(scatterDirection,
			hitRecord.normal);
		SMath::Vector3 brdf = albedo / pi;
		double cosineSamplingPDF = cosTheta / pi;
		
		attenuation = brdf * cosTheta / cosineSamplingPDF;

		return true;
	}

private:
	SMath::Vector3 albedo;
};

class Metal : public Material
{
public:
	Metal(const SMath::Vector3& _albedo, double _fuzz) 
		: albedo(_albedo), fuzz(_fuzz) {}

	bool Scatter(const Ray& r_in, const HitRecord& hitRecord,
		SMath::Vector3& attenuation, Ray& scattered) const override
	{
		SMath::Vector3 reflected = Vector3::Reflect(r_in.direction().Normalized(), hitRecord.normal);
		scattered = Ray(hitRecord.hitPoint, reflected + RandomUnitVector() * fuzz);
		attenuation = albedo;
		return true;
	}

private:
	SMath::Vector3 albedo;
	double fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric(double _ior) : ior(_ior) {}

	bool Scatter(const Ray& r_in, const HitRecord& hitRecord,
		SMath::Vector3& attenuation, Ray& scattered) const override
	{
		attenuation = SMath::Vector3(1.0, 1.0, 1.0);
		double refractionRatio = hitRecord.isFrontFacing ? (1.0 / ior) : ior;

		SMath::Vector3 unitDirection = r_in.direction().Normalized();
		double cosTheta = fmin(SMath::Vector3::Dot(-unitDirection, hitRecord.normal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool rayCannotRefract = refractionRatio * sinTheta > 1.0;
		SMath::Vector3 direction;

		if (rayCannotRefract)
			direction = Vector3::Reflect(unitDirection, hitRecord.normal);
		else
			direction = Refract(unitDirection, hitRecord.normal, refractionRatio);

		scattered = Ray(hitRecord.hitPoint, direction);
		return true;
	}

private:
	double ior;
};