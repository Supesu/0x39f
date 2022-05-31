#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <Core/Model/Model.hpp>

struct Renderable
{
    std::shared_ptr<eve::Model> model;
    glm::vec3 color;
};
