#pragma once

#include <iostream>
#include <smath.h>
#include <algorithm>

#include "Hittable.h"
#include "Scene.h"
#include "lodepng.h"
#include "Utils.h"
#include "Vector3.h"
#include "Material.h"

class Camera
{
public:
	double aspectRatio = 16.0/9.0;
	int imageWidth = 1024; 
	
	void Render(const Scene& scene);

private:
	int imageHeight;
	SMath::Vector3 cameraCenter;
	SMath::Vector3 pixelStartLoc;
	SMath::Vector3 pixelDelta_u;
	SMath::Vector3 pixelDelta_v;

	void Initialize();
	void GenerateImageBuffer();
	SMath::Vector3 GetRayColor(const Ray& r, const Scene& scene, int depth);
	double LinearToGamma(double linearComponent);
	void EncodeImageToPNG(const char* filename);
	void WriteImageBuffer(SMath::Vector3 color, int pixelX, int pixelY);

	std::vector<unsigned char> imageBuffer;
	unsigned int numSamples = 256;
};