#pragma once


class Context
{
public:
	std::unique_ptr<Windows>			m_pWindows;
	std::unique_ptr<OpenGLRenderer>		m_pRenderer;
	std::unique_ptr<SystemUI>			m_pSystemUI;
	std::unique_ptr<Resources>			m_pResources;
	std::unique_ptr<GameTimer>			m_pTimer;
	std::unique_ptr<DirectInput>		m_pInput;

	Context();
	~Context();

	tinyxml2::XMLElement* GetElement(const char* p);
private:
	const char*				ConfigFile = "Assets//config.xml";
	tinyxml2::XMLDocument	ConfigData;
};