#pragma once
#include <World.h>
#include <Camera.h>
#include <ImageIOInterface.h>
#include <SceneParser.h>

namespace Engine {
	void renderToFile(std::string scenePath, std::string outputPath);
}