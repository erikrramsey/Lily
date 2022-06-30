//
// Created by Erik on 6/30/2022.
//
#pragma once

#include "GraphicsHeaders.h"
#include "Shader.h"

namespace Lily {
class Material {
public:
    Material() {
        m_shader = nullptr;
    }
    Shader* m_shader;
    Texture albedo;
    Texture normal;
};
}
