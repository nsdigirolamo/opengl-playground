#version 330 core
out vec4 fragmentColor;

in vec3 vertexPosition;
in vec3 vertexSurfaceNormal;
in vec3 vertexColor;

uniform vec3 lightColor;
uniform vec3 lightPosition;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDirection = normalize(lightPosition - vertexPosition);
	float diffuseStrength = max(dot(vertexSurfaceNormal, lightDirection), 0.0);
	vec3 diffuse = diffuseStrength * lightColor;

	vec3 result = (diffuse + ambient) * vertexColor;
	fragmentColor = vec4(result, 1.0);
}