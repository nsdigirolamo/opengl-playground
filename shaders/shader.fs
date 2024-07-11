#version 330 core
out vec4 FragColor;

in vec3 Norm;

void main()
{
	vec3 newNorm = Norm;
	if (Norm.x < 0)
		newNorm.x = Norm.x * -1;
	if (Norm.y < 0)
		newNorm.y = Norm.y * -1;
	if (Norm.z < 0)
		newNorm.z = Norm.z * -1;
	FragColor = vec4(newNorm, 1.0);
}