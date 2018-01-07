#include "stdafx.h"
#include "ImGuiShader.h"

void ImGuiShader::LinkShader()
{
	glBindAttribLocation(m_iProgramID, SHADER_POSITION_ATTRIBUTE, "Position");
	glBindAttribLocation(m_iProgramID, SHADER_TEXCOORD_ATTRIBUTE, "UV");
	glBindAttribLocation(m_iProgramID, SHADER_COLOR_ATTRIBUTE, "Color");
	Shader::LinkShader();
}
