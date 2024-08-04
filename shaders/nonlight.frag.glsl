#version 330 core
out vec4 fragmentColor;

in vec3 fragmentPosition;
in vec3 surfaceNormal;

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDirection = normalize(lightPosition - fragmentPosition);
	float diffuseStrength = max(dot(surfaceNormal, lightDirection), 0.0);
	vec3 diffuse = diffuseStrength * lightColor;

	float specularStrength = 0.5;
	vec3 viewDirection = normalize(viewPosition - fragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, surfaceNormal);
	float highlight = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	vec3 specular = specularStrength * highlight * lightColor;

	vec3 result = (diffuse + ambient + specular) * objectColor;
	fragmentColor = vec4(result, 1.0);
}