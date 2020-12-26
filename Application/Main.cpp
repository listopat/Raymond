#include <Engine.h>
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc != 3 || strcmp(argv[1], "--help") == 0) {
		std::cout << "Usage: " << argv[0] << " scenePath outputPath" <<std::endl;
		std::cout << "scenePath - path to the json file with the scene description" << std::endl;
		std::cout << "outputPath - path to the output image" << std::endl;
		return -1;
	}

	std::string scene = argv[1];
	std::string output = argv[2];
	Engine::renderToFile(scene, output);
}
