#version 330 core
out vec4 fragmentColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shine;
};

in vec3 fragmentPosition;
in vec3 surfaceNormal;

uniform vec3 lightPosition;
uniform vec3 viewPosition;

uniform Material objectMaterial;
uniform Material lightMaterial;

void main()
{
	vec3 ambient = lightMaterial.ambient * objectMaterial.ambient;

	vec3 lightDirection = normalize(lightPosition - fragmentPosition);
	float diffuseStrength = max(dot(surfaceNormal, lightDirection), 0.0);
	vec3 diffuse = lightMaterial.diffuse * diffuseStrength * objectMaterial.diffuse;

	vec3 viewDirection = normalize(viewPosition - fragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, surfaceNormal);
	float specularStrength = pow(max(dot(viewDirection, reflectDirection), 0.0), objectMaterial.shine);
	vec3 specular = lightMaterial.specular * specularStrength * objectMaterial.specular;

	fragmentColor = vec4(ambient + diffuse + specular, 1.0);
}