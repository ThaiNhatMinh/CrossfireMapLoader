#pragma once
#include "stdafx.h"

enum ShaderAttribute
{
	SHADER_POSITION_ATTRIBUTE,
	SHADER_NORMAL_ATTRIBUTE,
	SHADER_TANGENT_ATTRIBUTE,
	SHADER_BINORMAL_ATTRIBUTE,
	SHADER_TEXCOORD_ATTRIBUTE,
	SHADER_TEXCOORD1_ATTRIBUTE,
	SHADER_COLOR_ATTRIBUTE,
	SHADER_BLEND1_ATTRIBUTE,
	SHADER_BLEND2_ATTRIBUTE,
	SHADER_BLEND3_ATTRIBUTE,
	SHADER_BLEND4_ATTRIBUTE
};
// base shader class
class Shader: public GLBO
{
public:
	GLuint m_iProgramID;
	GLuint m_iVertexShader;
	GLuint m_iFragmentShader;
	map<string, GLuint> m_UniformLists;
	
public:

public:
	Shader() {};
	Shader(const char* vertexShaderFile, const char* fragmentShaderFile);
	virtual void Init() {};
	virtual void Shutdown();
	virtual ~Shader();
	virtual void LinkShader();
	virtual void SetupRender(Camera* cam, void*)=0;

	bool Load(const char* vertexShaderFile, const char* fragmentShaderFile);
	inline void Use() { glUseProgram(m_iProgramID); };
	inline void UnUse() { glUseProgram(0); };

	inline GLuint GetAttribLocation(const char* name) { return glGetAttribLocation(m_iProgramID, name); };
	inline GLuint GetUniformLocation(const char* name) { return glGetUniformLocation(m_iProgramID, name); };


	inline void SetUniform(const char* name, float val) { glUniform1f(m_UniformLists[name], val); }
	inline void SetUniform(const char* name, float x, float y) { glUniform2f(m_UniformLists[name], x, y); }
	inline void SetUniform(const char* name, float x, float y, float z) { glUniform3f(m_UniformLists[name], x, y, z); }
	inline void SetUniform(const char* name, vec3 val) { glUniform3f(m_UniformLists[name], val.x, val.y, val.z); }
	inline void SetUniform(const char* name, int val) { glUniform1i(m_UniformLists[name], val); }
	inline void SetUniformMatrix(GLuint location, const float* data) { glUniformMatrix4fv(location, 1, GL_FALSE, data); }
	inline void SetUniformMatrix(const char* name, const float* data) { glUniformMatrix4fv(m_UniformLists[name], 1, GL_FALSE, data); }


	
};

