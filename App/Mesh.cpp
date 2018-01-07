#include "stdafx.h"


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
	
}
void Mesh::Shutdown()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::Init()
{
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertexs.size()*sizeof(DefaultVertex), &m_Vertexs[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

	size_t stride = sizeof(DefaultVertex);
	

	size_t offset=0;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 3 * sizeof(float);



	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 3 * sizeof(float);




	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 2 * sizeof(float);


	glBindVertexArray(0);
	
	NumIndices = m_Indices.size();
	Topology = GL_TRIANGLES;
}

imguiMesh::imguiMesh()
{
}

imguiMesh::~imguiMesh()
{
	
}

void imguiMesh::Shutdown()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void imguiMesh::Init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

	// position
	glEnableVertexAttribArray(SHADER_POSITION_ATTRIBUTE);
	glVertexAttribPointer(SHADER_POSITION_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
	// uv
	glEnableVertexAttribArray(SHADER_TEXCOORD_ATTRIBUTE);
	glVertexAttribPointer(SHADER_TEXCOORD_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
	//color
	glEnableVertexAttribArray(SHADER_COLOR_ATTRIBUTE);
	glVertexAttribPointer(SHADER_COLOR_ATTRIBUTE, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

	Topology = GL_TRIANGLES;
}
