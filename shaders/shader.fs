#version 330 core
out vec4 FragColor;

in vec3 Norm;

void main()
{
	vec3 crossed = cross(vec3(1.0, 0.0, 0.0), Norm);
	FragColor = vec4(length(crossed), 0.0, 0.0, 1.0);
}