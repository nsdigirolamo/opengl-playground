#version 330 core
layout (location = 0) in vec3 positionAttribute;
layout (location = 1) in vec3 surfaceNormalAttribute;

out vec3 fragmentPosition;
out vec3 surfaceNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 translate;

void main()
{
	fragmentPosition = vec3(model * vec4(positionAttribute, 1.0));
	surfaceNormal = mat3(transpose(inverse(model))) * surfaceNormalAttribute;
	gl_Position = projection * view * translate * vec4(fragmentPosition, 1.0);
}