#include <vector>
#include <string>
#include <iostream>

#include <Windows.h>
#include <cassert>

#include "options.h"

std::vector<std::string>GetCommandLineArguments(char* argv[]);
void InitPBRT(const PBRTOptions& opt);
void CleanupPBRT();

int main(int argc, char* argv[])
{
	// Convert command-line arguments to a vector of strings
	std::vector<std::string> args = GetCommandLineArguments(argv);

	// Declare variables for parsed command line
	PBRTOptions options;
	std::vector<std::string> filenames;

	// Process command-line arguments
	// 
	// Initialize pbrt
	InitPBRT(options);
	// Parse provided scene description files
	// BasicScene scene;
	// BasicSceneBuilder builder(&scene);
	// ParseFiles(&builder, filenames);
	
	// Render the scene
	/*
	if (Options->useGPU || Options->wavefront)
		RenderWavefront(scene)
	else
		RenderCPU(scene)
	*/
	
	// Clean up after rendering the scene
	CleanupPBRT();
	std::cout << "PBRT" << std::endl;

	return 0;
	
}

std::vector<std::string>GetCommandLineArguments(char* argv[])
{
	std::vector<std::string> argStrings;
	
//#ifdef PBRT_IS_WINDOWS
//	
//
//#endif

#define CHECK(x) assert(x)

	// Handle UTF16-encoded argument on Windows
	int argc;
	LPWSTR* argvw = CommandLineToArgvW(GetCommandLineW(), &argc);
	CHECK(argv != nullptr);
	for (int i = 1; i < argc; ++i)
		argStrings.push_back(argv[i]);

	return argStrings;
}

void InitPBRT(const PBRTOptions& opt)
{

}

void CleanupPBRT()
{

}