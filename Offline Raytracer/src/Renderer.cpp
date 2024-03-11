#include "Renderer.h"

void Renderer::RenderImage()
{
	Scene scene;	

	/*std::shared_ptr<Material> groundMat =
		std::make_shared<Lambertian>(SMath::Vector3(0.5, 0.5, 0.5));
	std::shared_ptr<Material> centerSphereMat =
		std::make_shared<Lambertian>(SMath::Vector3(0.5, 0.5, 0.5));
	std::shared_ptr<Material> leftSphereMat =
		std::make_shared<Dielectric>(1.5);
	std::shared_ptr<Material> rightSphereMat =
		std::make_shared<Metal>(SMath::Vector3(0.8, 0.6, 0.3), 0.0);

	scene.add(std::make_shared<Sphere>(SMath::Vector3(0, -100.5, -1), 100, groundMat));
	scene.add(std::make_shared<Sphere>(SMath::Vector3(0, 0, -1), 0.5, centerSphereMat));
	scene.add(std::make_shared<Sphere>(SMath::Vector3(-1.2, 0, -1), 0.5, leftSphereMat));
	scene.add(std::make_shared<Sphere>(SMath::Vector3(-1.2, 0, -1), -0.4, leftSphereMat));
	scene.add(std::make_shared<Sphere>(SMath::Vector3(1.2, 0, -1), 0.5, rightSphereMat));*/

	/*scene.objects[0]->mat = groundMat;
	scene.objects[1]->mat = centerSphereMat;
	scene.objects[2]->mat = leftSphereMat;
	scene.objects[3]->mat = leftSphereMat;
	scene.objects[4]->mat = rightSphereMat;*/

	/*double R = cos(pi / 4);

	scene.add(std::make_shared<Sphere>(SMath::Vector3(R, 0, -1), R));
	scene.add(std::make_shared<Sphere>(SMath::Vector3(-R, 0, -1), R));

	std::shared_ptr<Material> blue =
		std::make_shared<Lambertian>(SMath::Vector3(0, 0, 1));
	std::shared_ptr<Material> red =
		std::make_shared<Lambertian>(SMath::Vector3(1, 0, 0));

	scene.objects[0]->mat = blue;
	scene.objects[1]->mat = red;*/

	std::shared_ptr<Material> groundMaterial =
		std::make_shared<Lambertian>(SMath::Vector3(0.5, 0.5, 0.5));
	scene.add(std::make_shared<Sphere>(SMath::Vector3(0, -1000, 0), 1000, groundMaterial));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			double chooseMaterial = RandomDouble();
			SMath::Vector3 center = 
				SMath::Vector3(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

			if ((center - SMath::Vector3(4, 0.2, 0)).Magnitude() > 0.9)
			{
				std::shared_ptr<Material> sphereMaterial;

				if (chooseMaterial < 0.8)
				{
					SMath::Vector3 albedo = Vector3::Random() * Vector3::Random();
					sphereMaterial = std::make_shared<Lambertian>(albedo);
					scene.add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else if (chooseMaterial < 0.95)
				{
					SMath::Vector3 albedo = Vector3::Random(0.5, 1);
					double fuzz = RandomDouble(0, 0.5);
					sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
					scene.add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else
				{
					sphereMaterial = std::make_shared<Dielectric>(1.5);
					scene.add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
			}
		}
	}

	std::shared_ptr<Material> material1 = std::make_shared<Dielectric>(1.5);
	scene.add(std::make_shared<Sphere>(SMath::Vector3(0, 1, 0), 1.0, material1));

	std::shared_ptr<Material> material2 =
		std::make_shared<Lambertian>(SMath::Vector3(0.4, 0.2, 0.1));
	scene.add(std::make_shared<Sphere>(SMath::Vector3(-4, 1, 0), 1.0, material2));

	std::shared_ptr<Material> material3 =
		std::make_shared<Metal>(SMath::Vector3(0.7, 0.6, 0.5), 0.0);
	scene.add(std::make_shared<Sphere>(SMath::Vector3(4, 1, 0), 1.0, material3));

	camera.Render(scene);
}