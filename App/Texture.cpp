#include "stdafx.h"

void Texture::Init()
{
	glGenTextures(1, &iIndex);
	glBindTexture(GL_TEXTURE_2D, iIndex);
}

void Texture::Shutdown()
{
	glDeleteTextures(1, &iIndex);
}
