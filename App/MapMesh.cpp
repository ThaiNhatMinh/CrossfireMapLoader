#include "stdafx.h"
#include "MapMesh.h"


MapMesh::MapMesh()
{
}


MapMesh::~MapMesh()
{
}

void MapMesh::Init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertexs.size() * sizeof(SRBVertex), &m_Vertexs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

	size_t stride = sizeof(SRBVertex);


	size_t offset = 0;

	glEnableVertexAttribArray(SHADER_POSITION_ATTRIBUTE);
	glVertexAttribPointer(SHADER_POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 3 * sizeof(float);

	glEnableVertexAttribArray(SHADER_TEXCOORD_ATTRIBUTE);
	glVertexAttribPointer(SHADER_TEXCOORD_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 2 * sizeof(float);

	glEnableVertexAttribArray(SHADER_TEXCOORD1_ATTRIBUTE);
	glVertexAttribPointer(SHADER_TEXCOORD1_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 2 * sizeof(float);

	glEnableVertexAttribArray(SHADER_COLOR_ATTRIBUTE);
	glVertexAttribPointer(SHADER_COLOR_ATTRIBUTE, 1, GL_UNSIGNED_INT, GL_FALSE, stride, (GLvoid*)offset);
	offset += sizeof(int);

	glEnableVertexAttribArray(SHADER_NORMAL_ATTRIBUTE);
	glVertexAttribPointer(SHADER_NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 3 * sizeof(float);

	glEnableVertexAttribArray(SHADER_TANGENT_ATTRIBUTE);
	glVertexAttribPointer(SHADER_TANGENT_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 3 * sizeof(float);

	glEnableVertexAttribArray(SHADER_BINORMAL_ATTRIBUTE);
	glVertexAttribPointer(SHADER_BINORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 3 * sizeof(float);

	glBindVertexArray(0);

	NumIndices = m_Indices.size();
	Topology = GL_TRIANGLES;
}

void MapMesh::Shutdown()
{
}
