#version 330 core
out vec4 fragmentColor;

in vec3 vertexNormal;
in vec3 vertexColor;

void main()
{
	float ambientStrength = 0.1;
	fragmentColor = vec4(vertexColor * ambientStrength, 1.0);
}