#include "stdafx.h"
#include "DebugShader.h"

void DebugShader::LinkShader()
{
	glBindAttribLocation(m_iProgramID, SHADER_POSITION_ATTRIBUTE, "position");
	glBindAttribLocation(m_iProgramID, SHADER_COLOR_ATTRIBUTE, "color");
	Shader::LinkShader();
}
