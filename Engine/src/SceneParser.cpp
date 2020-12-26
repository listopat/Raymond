#include <SceneParser.h>
#include <ImageIOInterface.h>
#include <Shapes/Sphere.h>
#include <Shapes/Plane.h>
#include <Shapes/Cube.h>
#include <Texture/UVImage.h>
#include <Texture/Patterns/Checkers.h>
#include <Texture/Patterns/Ring.h>
#include <Texture/Patterns/Stripe.h>
#include <Texture/Patterns/Gradient.h>
#include <map>
#include <iostream>
#include <stdexcept>

Matrix getTransformMatrix(nlohmann::json json) {
	Matrix result = Matrix::getIdentity4x4();

	if (json.find("scaling") != json.end())
	{
		Matrix scaling = Matrix::scaling(json["scaling"][0], json["scaling"][1], json["scaling"][2]);
		result = scaling * result;
	}

	if (json.find("rotation") != json.end())
	{
		Matrix rotationX = Matrix::rotationX(json["rotation"][0]);
		Matrix rotationY = Matrix::rotationY(json["rotation"][1]);
		Matrix rotationZ = Matrix::rotationZ(json["rotation"][2]);
		result = rotationZ * rotationY * rotationX * result;
	}

	if (json.find("translation") != json.end())
	{
		Matrix translation = Matrix::translation(json["translation"][0], json["translation"][1], json["translation"][2]);
		result = translation * result;
	}

	return result;
}

std::shared_ptr<Texture> getTexture(nlohmann::json json) {
	std::shared_ptr<UVMapping> mapping;
	if (json["mapping"].get<std::string>().compare("sphere") == 0) 
	{
		mapping = SphericalMap::createSphericalMap();
	}
	else if (json["mapping"].get<std::string>().compare("plane") == 0) 
	{
		mapping = PlanarMap::createPlanarMap();
	}
	else if (json["mapping"].get<std::string>().compare("cubic") == 0)
	{
		mapping = CubicMap::createCubicMap();
	}
	else 
	{
		throw std::invalid_argument("Missing mapping");
	}

	std::shared_ptr<Texture> result;
	if (json["pattern"].get<std::string>().compare("image") == 0) 
	{
		std::shared_ptr<Canvas> canvas = std::make_shared<Canvas>(ImageIOInterface::canvasFromImage(json["imagePath"]));
		std::shared_ptr<UVImage> image = UVImage::createUVImage(canvas);
		result = Texture::createTexture(image, mapping);
	}
	else if (json["pattern"].get<std::string>().compare("stripe") == 0)
	{
		Color color1 = Color(json["color1"][0], json["color1"][1], json["color1"][2]);
		Color color2 = Color(json["color2"][0], json["color2"][1], json["color2"][2]);
		std::shared_ptr<Stripe> stripe = Stripe::createStripe(color1, color2);
		result = Texture::createTexture(stripe, mapping);
	}
	else if (json["pattern"].get<std::string>().compare("gradient") == 0)
	{
		Color color1 = Color(json["color1"][0], json["color1"][1], json["color1"][2]);
		Color color2 = Color(json["color2"][0], json["color2"][1], json["color2"][2]);
		std::shared_ptr<Gradient> gradient = Gradient::createGradient(color1, color2);
		result = Texture::createTexture(gradient, mapping);
	}
	else if (json["pattern"].get<std::string>().compare("ring") == 0)
	{
		Color color1 = Color(json["color1"][0], json["color1"][1], json["color1"][2]);
		Color color2 = Color(json["color2"][0], json["color2"][1], json["color2"][2]);
		std::shared_ptr<Ring> ring = Ring::createRing(color1, color2);
		result = Texture::createTexture(ring, mapping);
	}
	else if (json["pattern"].get<std::string>().compare("checkers") == 0)
	{
		Color color1 = Color(json["color1"][0], json["color1"][1], json["color1"][2]);
		Color color2 = Color(json["color2"][0], json["color2"][1], json["color2"][2]);
		std::shared_ptr<Checkers> checkers = Checkers::createCheckers(json["width"], json["height"], color1, color2);
		result = Texture::createTexture(checkers, mapping);
	}
	else 
	{
		throw std::invalid_argument("Missing pattern");
	}
	result->setTransform(getTransformMatrix(json));
	return result;
}

Material getMaterial(nlohmann::json json, std::map<std::string, std::shared_ptr<Texture>> textureMap) {
	Color color;
	if (json.find("color") != json.end()) 
	{
		color = Color(json["color"][0], json["color"][1], json["color"][2]);
	}

	Material result(color, json["ambient"], json["diffuse"], json["specular"], json["shininess"], json["reflective"], json["transparency"], json["refractiveIndex"]);
	if (json.find("texture") != json.end())
	{
		std::shared_ptr<Texture> tex = textureMap[json["texture"]];
		result.texture = tex;
	}
	return result;
}

Camera SceneParser::getCameraFromSceneJSON(nlohmann::json sceneJson)
{
	nlohmann::json cameraJson = sceneJson["camera"];
	const int height = cameraJson["height"];
	const int width = cameraJson["width"];
	const double fov = cameraJson["fov"];
	
	Camera camera = Camera::makeCamera(height, width, fov);
	const Tuple from = Tuple::point(cameraJson["from"][0], cameraJson["from"][1], cameraJson["from"][2]);
	const Tuple to = Tuple::point(cameraJson["to"][0], cameraJson["to"][1], cameraJson["to"][2]);
	const Tuple up = Tuple::point(cameraJson["up"][0], cameraJson["up"][1], cameraJson["up"][2]);
	camera.setTransform(Matrix::viewTransform(from, to, up));

	return camera;
}


World SceneParser::getWorldFromSceneJSON(nlohmann::json sceneJson)
{
	World world = World();

	nlohmann::json lights = sceneJson["lights"];

	for (nlohmann::json light : lights["pointLights"]) 
	{
		Tuple position = Tuple::point(light["position"][0], light["position"][1], light["position"][2]);
		Color intensity = Color(light["intensity"][0], light["intensity"][1], light["intensity"][2]);
		PointLight pointLight = PointLight(position, intensity);
		world.addLight(pointLight);
	}

	for (nlohmann::json light : lights["areaLights"])
	{
		Tuple corner = Tuple::point(light["corner"][0], light["corner"][1], light["corner"][2]);
		Color intensity = Color(light["intensity"][0], light["intensity"][1], light["intensity"][2]);
		int usteps = light["usteps"];
		int vsteps = light["vsteps"];
		Tuple uvec = Tuple::vector(light["uvec"][0], light["uvec"][1], light["uvec"][2]);
		Tuple vvec = Tuple::vector(light["vvec"][0], light["vvec"][1], light["vvec"][2]);
		bool jitter = light["jitter"];

		AreaLight areaLight = AreaLight(intensity, corner, uvec, usteps, vvec, vsteps, jitter);

		world.addLight(areaLight);
	}

	std::map<std::string, std::shared_ptr<Texture>> textureMap;
	nlohmann::json textures = sceneJson["textures"];
	for (nlohmann::json texture : textures) 
	{
		textureMap.insert(std::make_pair(texture["name"], getTexture(texture)));
	}

	std::map<std::string, Material> materialMap;
	nlohmann::json materials = sceneJson["materials"];
	for (nlohmann::json material : materials) 
	{
		materialMap.insert(std::make_pair(material["name"], getMaterial(material, textureMap)));
	}

	nlohmann::json shapes = sceneJson["shapes"];
	for (nlohmann::json sphereJson : shapes["spheres"]) 
	{
		std::shared_ptr<Sphere> sphere = Sphere::createSphere();

		sphere->setTransform(getTransformMatrix(sphereJson));
		sphere->setMaterial(materialMap[sphereJson["material"]]);
		world.addObject(sphere);
	}

	for (nlohmann::json planeJson : shapes["planes"]) 
	{
		std::shared_ptr<Plane> plane = Plane::createPlane();

		plane->setTransform(getTransformMatrix(planeJson));
		plane->setMaterial(materialMap[planeJson["material"]]);
		world.addObject(plane);
	}

	for (nlohmann::json cubeJson : shapes["cubes"])
	{
		std::shared_ptr<Cube> cube = Cube::createCube();

		cube->setTransform(getTransformMatrix(cubeJson));
		cube->setMaterial(materialMap[cubeJson["material"]]);
		world.addObject(cube);
	}

	return world;
}