#version 330 core
out vec4 fragmentColor;

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

void main()
{
	fragmentColor = vec4(light.specular, 1.0);
}