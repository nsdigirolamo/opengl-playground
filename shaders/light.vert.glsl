#version 330 core
layout (location = 0) in vec3 positionAttribute;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 translate;
uniform mat4 scale;

void main()
{
	gl_Position = projection * view * translate * model * scale * vec4(positionAttribute, 1.0);
}