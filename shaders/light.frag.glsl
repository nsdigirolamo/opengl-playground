#version 330 core
out vec4 fragmentColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shine;
};

uniform Material lightMaterial;

void main()
{
	fragmentColor = vec4(lightMaterial.specular, 1.0);
}