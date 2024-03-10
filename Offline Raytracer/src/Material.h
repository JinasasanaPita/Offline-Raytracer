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
		// scatterDirection = RandomUnitVector();
		/*do
		{
			scatterDirection = RandomUnitVector();
		} while (SMath::Vector3::Dot(scatterDirection, hitRecord.normal) < 0);*/

		scattered = Ray(hitRecord.hitPoint, scatterDirection);
		double cosTheta = SMath::Vector3::Dot(scatterDirection,
			hitRecord.normal);
		SMath::Vector3 brdf = albedo / pi;
		double cosineSamplingPDF = cosTheta / pi;
		double hemisphereSamplingPDF = 1 / (2 * pi);

		// attenuation = albedo * abs(cos(SMath::Vector3::Angle(r_in.direction(), hitRecord.normal)));
		attenuation = brdf * cosTheta / cosineSamplingPDF;

		/*attenuation = albedo;*/
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