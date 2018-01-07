#include "stdafx.h"


Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	
	Load(vertexShaderFile, fragmentShaderFile);

}

void Shader::LinkShader()
{
	GLint check;
	char infoLog[512];
	glLinkProgram(m_iProgramID);

	glGetProgramiv(m_iProgramID, GL_LINK_STATUS, &check);
	if (check == GL_FALSE)
	{
		glGetProgramInfoLog(m_iProgramID, 512, NULL, infoLog);
		E_ERROR( "Program Shader link error -> " + string(infoLog));
	}

	GLint nUniform, maxLen;
	glGetProgramiv(m_iProgramID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
	glGetProgramiv(m_iProgramID, GL_ACTIVE_UNIFORMS, &nUniform);
	GLsizei writen;
	GLenum type;
	GLint size;
	GLint location;
	char* name = new char[maxLen];
	for (int i = 0; i < nUniform; i++)
	{
		
		glGetActiveUniform(m_iProgramID, i, maxLen, &writen, &size, &type, name);
		location = glGetUniformLocation(m_iProgramID, name);
		m_UniformLists.insert({ string(name),location });
#if 0
		cout << location << " " << name << endl;
#endif
	}
	delete[] name;
	 
}


void Shader::Shutdown()
{
	glDeleteProgram(m_iProgramID);

	glDeleteShader(m_iFragmentShader);
	glDeleteShader(m_iVertexShader);
}

Shader::~Shader()
{
	

}

bool Shader::Load(const char * vertexShaderFile, const char * fragmentShaderFile)
{
	char sourceVertex[10000];
	char sourceFragment[10000];
	GLint check;
	char infoLog[512];
//	GLuint vertexShader, fragmentShader;


	FILE* pvFile = fopen(vertexShaderFile, "rt");
	if (!pvFile)
	{
		E_ERROR( "Can't open File: " + string(vertexShaderFile));
		return false;
	}
	if (fseek(pvFile, 0L, SEEK_END) == 0)
	{
		long bufsize = ftell(pvFile);
		fseek(pvFile, 0L, SEEK_SET);

		size_t newLen = fread(sourceVertex, sizeof(char), bufsize, pvFile);
		sourceVertex[newLen] = '\0'; /* Just to be safe. */
	}
	fclose(pvFile);
	FILE* pfFile = fopen(fragmentShaderFile, "rt");
	if (!pfFile)
	{
		E_ERROR( "Can't open File: " + string(fragmentShaderFile));
		return false;
	}
	if (fseek(pfFile, 0L, SEEK_END) == 0)
	{
		long bufsize = ftell(pfFile);
		fseek(pfFile, 0L, SEEK_SET);

		size_t newLen = fread(sourceFragment, sizeof(char), bufsize, pfFile);
		sourceFragment[newLen] = '\0'; /* Just to be safe. */
	}
	fclose(pfFile);
	m_iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* pvStr = sourceVertex;
	glShaderSource(m_iVertexShader, 1, &pvStr, NULL);
	glCompileShader(m_iVertexShader);


	glGetShaderiv(m_iVertexShader, GL_COMPILE_STATUS, &check);
	if (check == GL_FALSE)
	{
		glGetShaderInfoLog(m_iVertexShader, 512, NULL, infoLog);
		E_ERROR("FILE: " + string(vertexShaderFile));
		E_ERROR( "Vertex shader compile error -> " + string(infoLog));

	}

	m_iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* pfStr = sourceFragment;
	glShaderSource(m_iFragmentShader, 1, &pfStr, NULL);
	glCompileShader(m_iFragmentShader);


	glGetShaderiv(m_iFragmentShader, GL_COMPILE_STATUS, &check);
	if (check == GL_FALSE)
	{
		glGetShaderInfoLog(m_iFragmentShader, 512, NULL, infoLog);
		E_ERROR("FILE: " + string(fragmentShaderFile));
		E_ERROR( "Fragment shader compile error -> " + string(infoLog));

	}

	m_iProgramID = glCreateProgram();
	glAttachShader(m_iProgramID, m_iVertexShader);
	glAttachShader(m_iProgramID, m_iFragmentShader);

	return true;
}
