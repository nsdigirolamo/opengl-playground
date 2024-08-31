#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "glad/glad.h"

struct Material {
    GLuint diffuse;
    GLuint specular;
    GLuint emissive;
    float shine;
};

#endif