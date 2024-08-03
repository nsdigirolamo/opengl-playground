#version 330 core
layout (location = 0) in vec3 positionAttribute;
layout (location = 1) in vec3 surfaceNormalAttribute;
layout (location = 2) in vec3 colorAttribute;

out vec3 vertexPosition;
out vec3 vertexSurfaceNormal;
out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 translate;

void main()
{
	vertexPosition = vec3(model * vec4(positionAttribute, 1.0));
	gl_Position = projection * view * translate * vec4(vertexPosition, 1.0);
	vertexSurfaceNormal = surfaceNormalAttribute;
	vertexColor = colorAttribute;
}