#pragma once
#include <iostream>
#include <SMath.h>
#include <limits>

#include "lodepng.h"
#include "Scene.h"
#include "Sphere.h"
#include "Camera.h"

class Renderer
{
public:
	void RenderImage();

private:
	Camera camera;
};