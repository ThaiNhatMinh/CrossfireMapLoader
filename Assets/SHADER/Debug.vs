#version 140

in vec3 position;
in vec3 color;
uniform mat4 MVP;

out vec3 vColor;
void main()
{
	vColor = color;
	gl_Position = MVP * vec4(position,1.0f);

}