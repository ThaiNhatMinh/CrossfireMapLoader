#include "stdafx.h"

Context* ISubSystem::m_Context = nullptr;

Context::Context()
{
	
	int errorID = ConfigData.LoadFile(ConfigFile);
	if (errorID)
	{
		E_ERROR("Failed to load engine config file: " + string(ConfigFile));
	}
}

Context::~Context()
{
	E_WARNING("Context Release...");
}

tinyxml2::XMLElement * Context::GetElement(const char * p)
{
	tinyxml2::XMLElement * pData = nullptr;
	tinyxml2::XMLElement* pRoot = ConfigData.FirstChildElement("GameConfig");
	pData = pRoot->FirstChildElement(p);

	return pData;
}
