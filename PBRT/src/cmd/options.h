#pragma once

#include <string>

// TO-DO: Complete the rendering coordinate system
enum class RenderingCoordinateSystem {};

struct BasicPBRTOptions
{
	int seed = 0;
	bool quiet = false;
	bool disablePixelJitter = false, disableWavelengthJitter = false;
	bool disableTextureFiltering = false;
	bool forceDiffuse = false;
	bool useGPU = false;
	bool wavefront = false;
	// RenderingCoordinateSystem renderingSpace = 
	//			RenderingCoordinateSystem::CameraWorld;
};

struct PBRTOptions : BasicPBRTOptions
{
	int nThreads = 0;
	// LogLevel logLevel = LogLevel::Error;
	std::string logFile;
	bool logUtilization = false;
	bool writePartialImages = false;
	bool recordPixelStatistics = false;
	bool printStatistics = false;
	// pstd::optional<int> pixelSamples;
	// pstd::optional<int> gpuDevice;
	std::string imageFile;
	std::string mseReferenceImage, mseReferenceOutput;
	std::string debugStart;
	std::string displayServer;
	// pstd::optional<Bounds2f> cropWindow;
	// pstd::optional<Bounds2i> pixelBounds;
	// pstd::optional<Point2i> pixelMaterial;
	float displacementEdgeScale = 1;

	std::string ToString() const;
};