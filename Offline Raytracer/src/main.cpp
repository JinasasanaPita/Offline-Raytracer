#include <iostream>
#include <memory>
#include <smath.h>

#include "lodepng.h"

#include "Renderer.h"

int main()
{
	std::unique_ptr<Renderer> renderer(new Renderer());
	renderer->RenderImage();
}