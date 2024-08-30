#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glad/glad.h"
#include "glm/glm.hpp"

struct Light {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

#endif