#version 330 core
layout (location = 0) in vec3 positionAttribute;
layout (location = 1) in vec3 surfaceNormalAttribute;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 translate;

void main()
{
	gl_Position = projection * view * translate * model * vec4(positionAttribute, 1.0);
}