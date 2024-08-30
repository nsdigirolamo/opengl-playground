#version 330 core
out vec4 fragmentColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shine;
}; 

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 fragmentPosition;
in vec3 surfaceNormal;
in vec2 uvCoordinate;

uniform vec3 lightPosition;
uniform vec3 viewPosition;

uniform Material objectMaterial;
uniform Light light;

void main()
{
	vec3 ambient = light.ambient * texture(objectMaterial.diffuse, uvCoordinate).rgb;

	vec3 lightDirection = normalize(lightPosition - fragmentPosition);
	float diffuseStrength = max(dot(surfaceNormal, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * diffuseStrength * texture(objectMaterial.diffuse, uvCoordinate).rgb;

	vec3 viewDirection = normalize(viewPosition - fragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, surfaceNormal);
	float specularStrength = pow(max(dot(viewDirection, reflectDirection), 0.0), objectMaterial.shine);
	vec3 specular = light.specular * specularStrength * texture(objectMaterial.specular, uvCoordinate).rgb;

	fragmentColor = vec4(ambient + diffuse + specular, 1.0);
}