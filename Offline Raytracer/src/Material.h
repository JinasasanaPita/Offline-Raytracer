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

class OrenNayer : public Material
{
public:
	OrenNayer(const SMath::Vector3& _albedo, const double _roughness) :
		albedo(_albedo), roughness(_roughness) {}

	bool Scatter(const Ray& r_in, const HitRecord& hitRecord,
		SMath::Vector3& attenuation, Ray& scattered) const override
	{
		SMath::Vector3 scatterDirection =
			hitRecord.normal + RandomUnitVector();

		scattered = Ray(hitRecord.hitPoint, scatterDirection);

		SMath::Vector3 L1;
		SMath::Vector3 L2;
		SMath::Vector3 C1;
		SMath::Vector3 C2;
		SMath::Vector3 C3;

		double a = std::max(SMath::Vector3::Angle(scatterDirection, hitRecord.normal),
			SMath::Vector3::Angle(r_in.direction(), hitRecord.normal));
		double b = std::min(SMath::Vector3::Angle(scatterDirection, hitRecord.normal),
			SMath::Vector3::Angle(r_in.direction(), hitRecord.normal));

		double roughnessSquared = pow(roughness, 2);
		double cosTheta = SMath::Vector3::Dot(scatterDirection,
			hitRecord.normal);

		C1 = 1 - 0.5 * (roughnessSquared / (roughnessSquared + 0.33));
		// C2: Might be handles wrongly, missing if flux_i - flux_r < 0
		C2 = 0.45 * (roughnessSquared / (roughnessSquared + 0.09)) * sin(a);
		C3 = 0.125 * (roughnessSquared / (roughnessSquared + 0.09)) *
			pow(((4 * a * b) / pow(pi, 2)), 2);

		// L1: missing E_0 term
		// L1 = (albedo / pi) * cosTheta * (C1 + C2 * )
	}

private:
	SMath::Vector3 albedo;
	double roughness;
};

class Metal : public Material
{
public:
	Metal(const SMath::Vector3& _albedo, double _fuzz) 
		: albedo(_albedo), fuzz(_fuzz) {}

	bool Scatter(const Ray& r_in, const HitRecord& hitRecord,
		SMath::Vector3& attenuation, Ray& scattered) const override
	{
		SMath::Vector3 reflected = Vector3::Reflect(r_in.direction().Normalized(), 
			hitRecord.normal);
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

		if (rayCannotRefract || Reflectance(cosTheta, refractionRatio) > RandomDouble())
			direction = Vector3::Reflect(unitDirection, hitRecord.normal);
		else
			direction = Refract(unitDirection, hitRecord.normal, refractionRatio);

		scattered = Ray(hitRecord.hitPoint, direction);
		return true;
	}

private:
	double ior;

	static double Reflectance(double cosine, double refractiveIndex)
	{
		// Schlicks approximation
		double r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};