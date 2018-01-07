#pragma once
class MapShader:public Shader
{
public:
	MapShader() {};
	MapShader(const char* vertexShaderFile, const char* fragmentShaderFile) :Shader(vertexShaderFile, fragmentShaderFile) {};
	~MapShader() {  };

	virtual void SetupRender(Camera*, void*) ;
	virtual void LinkShader();
};

