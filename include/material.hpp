#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "glad/glad.h"
#include "glm/glm.hpp"

struct Material {
    GLuint diffuse;
    glm::vec3 specular;
    float shine;
};

#endif