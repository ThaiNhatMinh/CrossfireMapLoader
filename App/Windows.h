#pragma once
#include "stdafx.h"
extern void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
extern void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
extern void mouse_callback(GLFWwindow* window, double xpos, double ypos);
extern void scroll_callback(GLFWwindow*, double, double);


class Windows: public ISubSystem
{
private:
	GLuint m_iWidth;
	GLuint m_iHeight;
	vec2 m_Pos;
	GLFWmonitor* m_pMonitor;
	GLFWwindow* m_pWindow;
	string m_WindowTitle;
	GLboolean m_bIsFullscreen;
	GLuint m_iScreenWidth;
	GLuint m_iScreenHeight;

private:
	// Read config from XMLElement
	// return init pos window
	vec2 ReadConfig(tinyxml2::XMLElement* pData);
public:
	Windows();
	~Windows();
	virtual void Init(Context* c);
	virtual void ShutDown();
	void SetSize(int W, int H);
	void SetPos(vec2 pos);
	void ShowWindows();
	void HideWindows();
	void EnableFullScreen(bool enable);
	void SwitchMode();
	int ShouldClose();
	void SetMouse(int t);
	vec2 GetWindowSize(){ return vec2(m_iWidth, m_iHeight); };
	GLFWwindow* Window() {		return m_pWindow;	};
	HWND GetHandle() { return glfwGetWin32Window(m_pWindow); }
};

