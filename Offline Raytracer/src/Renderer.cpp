#include "Renderer.h"

void Renderer::RenderImage()
{
	Scene scene;
	scene.add(std::make_shared<Sphere>(SMath::Vector3(0, -100.5, -1), 100));
	scene.add(std::make_shared<Sphere>(SMath::Vector3(0, 0, -1), 0.5));
	scene.add(std::make_shared<Sphere>(SMath::Vector3(-1.2, 0, -1), 0.5));
	scene.add(std::make_shared<Sphere>(SMath::Vector3( 1.2, 0, -1), 0.5));

	std::shared_ptr<Material> groundMat =
		std::make_shared<Lambertian>(SMath::Vector3(0.5, 0.5, 0.5));
	std::shared_ptr<Material> centerSphereMat =
		std::make_shared<Lambertian>(SMath::Vector3(0.5, 0.5, 0.5));
	std::shared_ptr<Material> leftSphereMat =
		std::make_shared<Dielectric>(1.5);
	std::shared_ptr<Material> rightSphereMat =
		std::make_shared<Metal>(SMath::Vector3(0.8, 0.6, 0.3), 0.0);

	scene.objects[0]->mat = groundMat;
	scene.objects[1]->mat = centerSphereMat;
	scene.objects[2]->mat = leftSphereMat;
	scene.objects[3]->mat = rightSphereMat;

	camera.Render(scene);
}