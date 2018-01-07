#include "stdafx.h"
#include "MapShader.h"





void MapShader::SetupRender(Camera *, void *)
{
	
}

void MapShader::LinkShader()
{
	glBindAttribLocation(m_iProgramID, SHADER_POSITION_ATTRIBUTE, "position");
	glBindAttribLocation(m_iProgramID, SHADER_TEXCOORD_ATTRIBUTE, "uv");
	glBindAttribLocation(m_iProgramID, SHADER_TEXCOORD1_ATTRIBUTE, "uv1");
	glBindAttribLocation(m_iProgramID, SHADER_COLOR_ATTRIBUTE, "color");
	glBindAttribLocation(m_iProgramID, SHADER_NORMAL_ATTRIBUTE, "normal");
	glBindAttribLocation(m_iProgramID, SHADER_TANGENT_ATTRIBUTE, "tangent");
	glBindAttribLocation(m_iProgramID, SHADER_BINORMAL_ATTRIBUTE, "binormal");
	Shader::LinkShader();

}
