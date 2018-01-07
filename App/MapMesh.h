#pragma once

class MapMesh :	public IMesh
{
public:
	MapMesh();
	~MapMesh();

public:
	vector<SRBVertex>	m_Vertexs;
	vector<unsigned int>	m_Indices;

	virtual void Init();
	virtual void Shutdown();
};

