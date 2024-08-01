#version 330 core
out vec4 fColor;

in vec3 norm;
in vec3 color;

void main()
{
	float ambientStrength = 0.1;
	fColor = vec4(color * ambientStrength, 1.0);
}