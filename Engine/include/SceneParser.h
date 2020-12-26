#pragma once
#include <World.h>
#include <Camera.h>
#include <json.hpp>

namespace SceneParser {
	Camera getCameraFromSceneJSON(nlohmann::json sceneJson);
	World getWorldFromSceneJSON(nlohmann::json sceneJson);
}