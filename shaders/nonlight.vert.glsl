#version 330 core
layout (location = 0) in vec3 posAttribute;
layout (location = 1) in vec3 normalAttribute;
layout (location = 2) in vec3 colorAttribute;

out vec3 vertexNormal;
out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(posAttribute, 1.0f);
	vertexNormal = normalAttribute;
	vertexColor = colorAttribute;
}