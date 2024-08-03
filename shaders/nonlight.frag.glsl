#version 330 core
out vec4 fragmentColor;

in vec3 vertexPosition;
in vec3 vertexSurfaceNormal;
in vec3 vertexColor;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDirection = normalize(lightPosition - vertexPosition);
	float diffuseStrength = max(dot(vertexSurfaceNormal, lightDirection), 0.0);
	vec3 diffuse = diffuseStrength * lightColor;

	float specularStrength = 0.5;
	vec3 viewDirection = normalize(viewPosition - vertexPosition);
	vec3 reflectDirection = reflect(-lightDirection, vertexSurfaceNormal);
	float highlight = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	vec3 specular = diffuse == 0 ? vec3(0.0f) : specularStrength * highlight * lightColor;

	vec3 result = (diffuse + ambient + specular) * vertexColor;
	fragmentColor = vec4(result, 1.0);
}