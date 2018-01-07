#pragma once


class ImGuiShader : public Shader
{
public:
	ImGuiShader() {};
	ImGuiShader(const char* vertexShaderFile, const char* fragmentShaderFile) :Shader(vertexShaderFile, fragmentShaderFile) {};
	~ImGuiShader() {  };

	virtual void SetupRender(Camera*, void*) {};
	virtual void LinkShader();
};