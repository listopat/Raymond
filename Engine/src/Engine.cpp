#include <Engine.h>
#include <SceneParser.h>
#include <ImageIOInterface.h>

#include <fstream>

void Engine::renderToFile(std::string scenePath, std::string outputPath)
{
	std::ifstream ifs(scenePath);
	nlohmann::json sceneJson = nlohmann::json::parse(ifs);

	Camera camera = SceneParser::getCameraFromSceneJSON(sceneJson);
	World world = SceneParser::getWorldFromSceneJSON(sceneJson);

	Canvas image = camera.render(world);
	ImageIOInterface::saveToImage(image, outputPath);
}
