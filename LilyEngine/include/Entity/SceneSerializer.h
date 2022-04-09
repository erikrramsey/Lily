#pragma once

#include "json.hpp"
#include "Scene.h"

#include <fstream>

namespace Lily {

class Scene;
class SceneSerializer {
public:
    SceneSerializer();

    static void serialize(Scene* scene, const std::string& path);
    static void deserialize(Scene* scene, const std::string& path);
};

}