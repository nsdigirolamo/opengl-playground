#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "model.hpp"

struct Light {

	glm::vec3 position;
    glm::vec3 scale;
	Model* model;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

#endif