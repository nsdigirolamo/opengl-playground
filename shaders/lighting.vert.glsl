#version 330 core
layout (location = 0) in vec3 positionAttribute;
layout (location = 1) in vec3 surfaceNormalAttribute;
layout (location = 2) in vec2 uvCoordinateAttribute;

out vec3 fragmentPosition;
out vec3 surfaceNormal;
out vec2 uvCoordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fragmentPosition = vec3(model * vec4(positionAttribute, 1.0));
	surfaceNormal = normalize(mat3(transpose(inverse(model))) * surfaceNormalAttribute);
	uvCoordinate = uvCoordinateAttribute;
	gl_Position = projection * view * vec4(fragmentPosition, 1.0);
}
