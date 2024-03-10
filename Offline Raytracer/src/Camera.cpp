#include "Camera.h"

void Camera::Render(const Scene& scene)
{
	Initialize();

	for (int j = 0; j < imageHeight; ++j)
	{
		std::clog << "\nPercentage complete: " << ((float)j / (float)imageHeight) << "%" << std::flush;
		for (int i = 0; i < imageWidth; ++i)
		{
			SMath::Vector3 pixelColor;

			for (int k = 0; k < numSamples; ++k)
			{
				//MSAA
				SMath::Vector3 pixelCenter = pixelStartLoc + (pixelDelta_u * i) + (pixelDelta_v * j)
					+ pixelDelta_u * (((float)rand() / RAND_MAX) - 0.5)
					+ pixelDelta_v * (((float)rand() / RAND_MAX) - 0.5);

				SMath::Vector3 rayDirection = pixelCenter - cameraCenter;
				Ray r(cameraCenter, rayDirection);
				pixelColor += GetRayColor(r, scene, 16);
			}

			pixelColor /= numSamples;
			WriteImageBuffer(pixelColor, i, j);
		}
	}

	EncodeImageToPNG("renders/image13.png");
}

void Camera::Initialize()
{
	imageHeight = static_cast<int>(imageWidth / aspectRatio);
	// Check for invalid image height
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;
	GenerateImageBuffer();

	double focalLength = 1.0;
	double viewportHeight = 2.0;
	double viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);
	cameraCenter = SMath::Vector3(0, 0, 0);

	SMath::Vector3 viewport_u = SMath::Vector3(viewportWidth, 0, 0);
	SMath::Vector3 viewport_v = SMath::Vector3(0, -viewportHeight, 0);

	pixelDelta_u = viewport_u / imageWidth;
	pixelDelta_v = viewport_v / imageHeight;

	SMath::Vector3 viewportUpperLeft = cameraCenter -
		SMath::Vector3(0, 0, focalLength) - viewport_u / 2 - viewport_v / 2;
	pixelStartLoc = viewportUpperLeft + (pixelDelta_u + pixelDelta_v) * 0.5;
}

SMath::Vector3 Camera::GetRayColor(const Ray& r, const Scene& scene, int depth)
{
	HitRecord hitPoint;
	SMath::Vector3 randomVec;

	do
		randomVec = SMath::Vector3(SMath::Vector3(rand(), rand(), rand()) / RAND_MAX
			- SMath::Vector3(0.5, 0.5, 0.5)).Normalized();
	while (randomVec.Magnitude() > 1);

	if (depth <= 0)
		return SMath::Vector3(0, 0, 0);

	if (scene.Hit(r, Interval(SMath::Epsilon, infinity), hitPoint))
	{
		// Default
		/*Ray r_reflect(hitPoint.hitPoint + hitPoint.normal * 0.01,
			(hitPoint.normal + randomVec).Normalized());*/

		// Diffused
		/*SMath::Vector3 direction = RandomUnitVector(hitPoint.normal);
		return (hitPoint.mat.albedo *
			GetRayColor(Ray(hitPoint.hitPoint, direction), scene, depth - 1)) *
			SMath::Vector3::Dot(hitPoint.normal, direction);*/

		// Lambert's Diffused
		/*SMath::Vector3 direction = hitPoint.normal + RandomUnitVector();*/

		Ray scattered;
		SMath::Vector3 attenuation;
		if (hitPoint.mat->Scatter(r, hitPoint, attenuation, scattered))
			return attenuation * GetRayColor(scattered, scene, depth - 1);
		return SMath::Vector3(0, 0, 0);

		/*return (SMath::Vector3(0.5, 0.5, 0.5) *
			GetRayColor(Ray(hitPoint.hitPoint, direction), scene, depth - 1));*/
	}

	SMath::Vector3 unitDirection = r.direction().Normalized();
	double a = 0.5 * (unitDirection.y + 1.0);
	return (SMath::Vector3(0.5, 0.7, 1.0) * a + (1.0 - a));
	/*return SMath::Vector3(0.5, 0.7, 1.0);*/
}

void Camera::GenerateImageBuffer()
{
	imageBuffer.resize(imageWidth * imageHeight * 4);
}

double Camera::LinearToGamma(double linearComponent)
{
	return sqrt(linearComponent);
}

void Camera::EncodeImageToPNG(const char* filename)
{
	// Encode the image pixel buffer to an image in PNG format
	unsigned error = lodepng::encode(filename, imageBuffer, imageWidth, imageHeight);
	if (error)
		std::cout << "Encoder error" << error << ": " << lodepng_error_text(error) << std::endl;
}

void Camera::WriteImageBuffer(SMath::Vector3 color, int pixelX, int pixelY)
{
	color.x = fmax(0, fmin(color.x, 1));
	color.y = fmax(0, fmin(color.y, 1));
	color.z = fmax(0, fmin(color.z, 1));

	color.x = LinearToGamma(color.x);
	color.y = LinearToGamma(color.y);
	color.z = LinearToGamma(color.z);

	imageBuffer[4 * imageWidth * pixelY + 4 * pixelX + 0] = static_cast<int>(255.99 * color.x);
	imageBuffer[4 * imageWidth * pixelY + 4 * pixelX + 1] = static_cast<int>(255.99 * color.y);
	imageBuffer[4 * imageWidth * pixelY + 4 * pixelX + 2] = static_cast<int>(255.99 * color.z);
	imageBuffer[4 * imageWidth * pixelY + 4 * pixelX + 3] = 255;
}