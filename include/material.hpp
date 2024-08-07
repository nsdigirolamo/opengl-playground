#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "glm/glm.hpp"

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shine;
};

#endif