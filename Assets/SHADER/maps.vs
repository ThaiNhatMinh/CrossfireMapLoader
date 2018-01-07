#version 140

in vec3 position;
in vec2 uv;
in vec2 uv1;
in int color;
in vec3 normal;
in vec3 tangent;
in vec3 binormal;

uniform mat4 Model;
uniform mat4 MVP;

out vec3 vNormal;
out vec2 vUV;
out vec3 vFragPos;

void main()
{
	gl_Position = MVP * vec4(position,1.0f);
	vNormal = mat3(Model) * normal;
	vUV = uv;
	vFragPos = vec3(Model* vec4(position,1.0f));
}