#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "glm/glm.hpp"
#include "material.hpp"
#include "model.hpp"

struct Object {

    glm::vec3 position;
    glm::vec3 scale;
    Model* model;

    Material* material;
};

#endif
