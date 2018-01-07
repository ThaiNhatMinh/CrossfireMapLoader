#include "stdafx.h"



vec2 Windows::ReadConfig(tinyxml2::XMLElement * pWindows)
{
	if (!pWindows) return vec2(0,0);

	m_WindowTitle = pWindows->Attribute("name");
	
	m_iWidth = pWindows->DoubleAttribute("sizex", 800);
	m_iHeight = pWindows->DoubleAttribute("sizey", 600);
	vec2 pos;
	pos.x = pWindows->DoubleAttribute("posx", 0);
	pos.y = pWindows->DoubleAttribute("posy", 0);

	return pos;
	//m_pWindows = std::make_unique<Windows>(m_WindowName, size.x, size.y);
	//m_pWindows->InitWindow();
	//m_pWindows->SetPos(pos);
}

Windows::Windows()
{
	
	
	/*int	 count;
	const GLFWvidmode* mode = glfwGetVideoModes(m_pMonitor, &count);
	for (int i = 0; i < count; i++)
	{
		cout << mode[i].width << ' ' << mode[i].height << endl;
	}*/

}


Windows::~Windows()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void Windows::Init(Context* c)
{

	vec2 pos = ReadConfig(c->GetElement("Window"));
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWmonitor* m_pMonitor = glfwGetPrimaryMonitor();

	m_iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	m_iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	m_pWindow = glfwCreateWindow(m_iWidth, m_iHeight, m_WindowTitle.c_str(), nullptr, nullptr);
	if (!m_pWindow)
	{
		E_ERROR("Can't create Window.");
		return ;
	}
	
	glfwSetWindowUserPointer(m_pWindow, c);
	glfwMakeContextCurrent(m_pWindow);
	//glfwSwapInterval(1);
	glfwSetCursorPos(m_pWindow, m_iWidth / 2, m_iHeight / 2);
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	
	SetPos(pos);

	HideWindows();
	c->m_pWindows = std::unique_ptr<Windows>(this);
	
	return ;
}

void Windows::ShutDown()
{

}

void Windows::SetSize(int W, int H)
{
	m_iWidth = W;
	m_iHeight = H;
	if (m_pWindow) glfwSetWindowSize(m_pWindow, W, H);
}

void Windows::SetPos(vec2 pos)
{
	m_Pos = pos;
	if (pos.x == -1 || pos.y == -1)
	{
		m_Pos.x = m_iScreenWidth / 2 - m_iWidth / 2;
		m_Pos.y = m_iScreenHeight / 2 - m_iHeight / 2;
	}


	if(m_pWindow) glfwSetWindowPos(m_pWindow, m_Pos.x, m_Pos.y);
	
}

void Windows::ShowWindows()
{
	glfwShowWindow(m_pWindow);
}

void Windows::HideWindows()
{
	glfwHideWindow(m_pWindow);
}

void Windows::EnableFullScreen(bool enable)
{
	if (m_bIsFullscreen) // current full screen
	{
		if (!enable) // set to windowed mode
		{
			int xpos = m_iScreenWidth / 2 - m_iWidth / 2;
			int ypos = m_iScreenHeight / 2 - m_iHeight / 2;
			glfwSetWindowMonitor(m_pWindow, NULL, xpos , ypos, m_iWidth, m_iHeight, GLFW_DONT_CARE);
			m_bIsFullscreen = false;
		}
	}
	else if (!m_bIsFullscreen) // current windowed mode
	{
		if (enable) // set to full screen mode
		{
			GLFWmonitor* m_pMonitor = glfwGetPrimaryMonitor();
			glfwSetWindowMonitor(m_pWindow, m_pMonitor, 0, 0, m_iWidth, m_iHeight, GLFW_DONT_CARE);
			m_bIsFullscreen = true;
		}
	}
}

void Windows::SwitchMode()
{
	if (m_bIsFullscreen) EnableFullScreen(false);
	else EnableFullScreen(true);
}

int Windows::ShouldClose()
{
	return glfwWindowShouldClose(m_pWindow);
}

void Windows::SetMouse(int t)
{
	// Hide OS mouse cursor if ImGui is drawing it
	
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, t);
}
