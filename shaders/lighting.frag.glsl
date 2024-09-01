#version 330 core
out vec4 fragmentColor;

#define POINT_LIGHT_COUNT 4

struct Material {
    sampler2D diffuse;
    sampler2D specular;
	sampler2D emissive;
    float     shine;
};

struct SunLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 calcSunLight (SunLight light, vec3 normal, vec3 viewDirection);
vec3 calcPointLight (PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);
vec3 calcSpotLight (SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);

in vec3 fragmentPosition;
in vec3 surfaceNormal;
in vec2 uvCoordinate;

uniform vec3 viewPosition;

uniform Material material;

uniform SunLight sunLight;
uniform PointLight pointLights [POINT_LIGHT_COUNT];
uniform SpotLight spotLight;

void main()
{
	vec3 viewDirection = normalize(viewPosition - fragmentPosition);

	vec3 light = calcSunLight(sunLight, surfaceNormal, viewDirection);

	for (int i = 0; i < POINT_LIGHT_COUNT; ++i)
	{
		light += calcPointLight(pointLights[i], surfaceNormal, fragmentPosition, viewDirection);
	}

	light += calcSpotLight(spotLight, surfaceNormal, fragmentPosition, viewDirection);

	fragmentColor = vec4(light, 1.0);
}

vec3 calcSunLight (SunLight light, vec3 surfaceNormal, vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.direction);
	float diffuseStrength = max(dot(surfaceNormal, lightDirection), 0.0);
	vec3 reflectDirection = reflect(-lightDirection, surfaceNormal);
	float specularStrength = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shine);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, uvCoordinate));
	vec3 diffuse = light.diffuse * diffuseStrength * vec3(texture(material.diffuse, uvCoordinate));
	vec3 specular = light.specular * specularStrength * vec3(texture(material.specular, uvCoordinate));

	return (ambient + diffuse + specular);
}

vec3 calcPointLight (PointLight light, vec3 surfaceNormal, vec3 fragmentPosition, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - fragmentPosition);
	float diffuseStrength = max(dot(surfaceNormal, lightDirection), 0.0);
	vec3 reflectDirection = reflect(-lightDirection, surfaceNormal);
	float specularStrength = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shine);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, uvCoordinate));
	vec3 diffuse = light.diffuse * diffuseStrength * vec3(texture(material.diffuse, uvCoordinate));
	vec3 specular = light.specular * specularStrength * vec3(texture(material.specular, uvCoordinate));

	float distance = length(light.position - fragmentPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	return attenuation * (ambient + diffuse + specular);
}

vec3 calcSpotLight (SpotLight light, vec3 surfaceNormal, vec3 fragmentPosition, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - fragmentPosition);
	float diffuseStrength = max(dot(surfaceNormal, lightDirection), 0.0);
	vec3 reflectDirection = reflect(-lightDirection, surfaceNormal);
	float specularStrength = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shine);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, uvCoordinate));
	vec3 diffuse = light.diffuse * diffuseStrength * vec3(texture(material.diffuse, uvCoordinate));
	vec3 specular = light.specular * specularStrength * vec3(texture(material.specular, uvCoordinate));

	float distance = length(light.position - fragmentPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	return attenuation * intensity * (ambient + diffuse + specular);
}
