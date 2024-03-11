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
	int imageWidth = 1920; 
	double fov = 20;

	SMath::Vector3 lookFrom = SMath::Vector3(13, 2, 3);
	SMath::Vector3 lookAt = SMath::Vector3(0, 0, 0);
	SMath::Vector3 v_up = SMath::Vector3(0, 1, 0);

	double defocusAngle = 0.6;
	double focusDistance = 10;
	
	void Render(const Scene& scene);

private:
	int imageHeight;
	SMath::Vector3 cameraCenter;
	SMath::Vector3 pixelStartLoc;
	SMath::Vector3 pixelDelta_u;
	SMath::Vector3 pixelDelta_v;
	SMath::Vector3 u, v, w;
	SMath::Vector3 defocusDisk_u;
	SMath::Vector3 defocusDisk_v;

	void Initialize();
	void GenerateImageBuffer();
	SMath::Vector3 GetRayColor(const Ray& r, const Scene& scene, int depth);
	double LinearToGamma(double linearComponent);
	void EncodeImageToPNG(const char* filename);
	void WriteImageBuffer(SMath::Vector3 color, int pixelX, int pixelY);
	SMath::Vector3 DefocusDiskSample() const;

	std::vector<unsigned char> imageBuffer;
	unsigned int numSamples = 1024;
};