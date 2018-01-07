#pragma once
#include "stdafx.h"

struct DefaultVertex
{
	vec3 pos;
	vec3 normal;
	vec2 uv;
};

class Mesh: public IMesh 
{
public:
	vector<DefaultVertex>	m_Vertexs;
	vector<unsigned int>	m_Indices;
public:
	Mesh();
	~Mesh();

	virtual void Init();
	virtual void Shutdown();
};

struct imguiVertex
{
	vec2 pos;
	vec2 uv;
	vec4 color;
};

class imguiMesh : public IMesh
{
public:
	vector<DefaultVertex>	m_Vertexs;
	vector<unsigned int>	m_Indices;
public:
	imguiMesh();
	~imguiMesh();

	virtual void Init();
	virtual void Shutdown();
};