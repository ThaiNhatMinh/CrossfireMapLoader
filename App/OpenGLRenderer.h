#pragma once
#include "stdafx.h"
#include <GLFW\glfw3.h>


class DebugData
{
public:
	glm::vec3 pos;
	glm::vec3 color;

};

class OpenGLRenderer :public ISubSystem
{

private:
	string			m_RenderName;
	vec4			m_ClearColor;
	vec4			m_Viewport;
	bool			m_HasInit;
	unsigned int	m_iClearFlag;
	GLenum			m_DrawMode;
	GLFWwindow*		m_glfwWindow;

	Shader*				m_pShader;
	unsigned int			VAO;
	unsigned int			VBO;
	vector<DebugData>	m_Lists;
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	virtual void Init(Context* c);
	virtual void ShutDown();

	virtual bool HasInit();
	virtual void ClearBuffer();
	virtual void ClearColor();
	virtual void Clear();

	virtual void Draw(GLint first, GLsizei count, GLsizei primcount = 0);
	virtual void DrawElement(GLsizei count, GLenum type, const GLvoid * indices, GLsizei primcount = 0);

	virtual void DrawLine(vec3 from, vec3 to, vec3 color = vec3(0.5f));
	virtual void DrawLineBox(vec3 min, vec3 max, vec3 color = vec3(0.5f));

	virtual void SetClearFlag(unsigned int flag);
	virtual void SetTexture(Texture* p, GLuint tex_unit = 0);
	virtual void SetViewport(int x, int y, int weight, int height);
	virtual void SetViewport(vec4 v);
	virtual void SetDrawMode(GLenum mode);
	virtual void SetVertexArrayBuffer(GLuint vao);
	virtual vec4 GetViewport();

	virtual void Render(Camera* cam);

	virtual void SwapBuffer();


	void SetDebugShader(Shader*);

private:
	

	bool ReadConfig(tinyxml2::XMLElement *pData);
};