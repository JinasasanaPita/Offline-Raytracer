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

				SMath::Vector3 rayOrigin = (defocusAngle <= 0) ? cameraCenter : DefocusDiskSample();
				SMath::Vector3 rayDirection = pixelCenter - rayOrigin;
				Ray r(rayOrigin, rayDirection);
				pixelColor += GetRayColor(r, scene, 16);
			}

			pixelColor /= numSamples;
			WriteImageBuffer(pixelColor, i, j);
		}
	}

	EncodeImageToPNG("renders/final.png");
}

void Camera::Initialize()
{
	imageHeight = static_cast<int>(imageWidth / aspectRatio);
	// Check for invalid image height
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;
	GenerateImageBuffer();

	// double focalLength = (lookFrom - lookAt).Magnitude();
	double theta = DegreesToRadians(fov);
	double h = tan(theta / 2);
	double viewportHeight = 2.0 * h * focusDistance;
	double viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);

	cameraCenter = lookFrom;

	w = (lookFrom - lookAt).Normalized();
	u = SMath::Vector3::Cross(v_up, w).Normalized();
	v = SMath::Vector3::Cross(w, u);

	/*SMath::Vector3 viewport_u = SMath::Vector3(viewportWidth, 0, 0);
	SMath::Vector3 viewport_v = SMath::Vector3(0, -viewportHeight, 0);*/
	SMath::Vector3 viewport_u = u * viewportWidth;
	SMath::Vector3 viewport_v = -v * viewportHeight;

	pixelDelta_u = viewport_u / imageWidth;
	pixelDelta_v = viewport_v / imageHeight;

	/*SMath::Vector3 viewportUpperLeft = cameraCenter -
		SMath::Vector3(0, 0, focalLength) - viewport_u / 2 - viewport_v / 2;*/
	SMath::Vector3 viewportUpperLeft = cameraCenter - (w * focusDistance)
		- viewport_u / 2 - viewport_v / 2;
	pixelStartLoc = viewportUpperLeft + (pixelDelta_u + pixelDelta_v) * 0.5;

	double defocusRadius = focusDistance * tan(DegreesToRadians(defocusAngle / 2));
	defocusDisk_u = u * defocusRadius;
	defocusDisk_v = v * defocusRadius;
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
		Ray scattered;
		SMath::Vector3 attenuation;
		if (hitPoint.mat->Scatter(r, hitPoint, attenuation, scattered))
			return attenuation * GetRayColor(scattered, scene, depth - 1);
		return SMath::Vector3(0, 0, 0);
	}

	SMath::Vector3 unitDirection = r.direction().Normalized();
	double a = 0.5 * (unitDirection.y + 1.0);
	return (SMath::Vector3(1.0, 0.7, 0.5) * a + (1.0 - a));
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

SMath::Vector3 Camera:: DefocusDiskSample() const
{
	SMath::Vector3 p = RandomUnitDisk();
	return cameraCenter + (defocusDisk_u * p.x) + (defocusDisk_v * p.y);
}