#version 140


in vec3 vColor;

out vec4 result;

void main()
{
	result = vec4(vColor,1.0f);
}