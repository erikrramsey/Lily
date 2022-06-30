#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#include <GL/glew.h>
// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/detail/type_float.hpp>


#define INVALID_UNIFORM_LOCATION 0x7fffffff

namespace Lily {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    struct Texture {
        unsigned int id = 0;
        std::string type = "";
        std::string path = "";
    };


}
