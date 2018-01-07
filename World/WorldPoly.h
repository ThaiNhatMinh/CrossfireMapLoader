#pragma once
class WorldPoly
{
private:
	//This value holds the number of vertices in its lower 8 bits, and its index in its
	//upper 24 bits. This is done because the index is rarely set/read, and the number,
	//and the number of vertices is fairly frequently read
	uint32			m_nIndexAndNumVerts;
	Surface			*m_pSurface;    // This polygon's surface.
	LTPlane			*m_pPlane;		// This polygon's plane.

									//NOTE: THIS MUST COME LAST, memory is overllocated so that the vertex list
									//begins and runs past the end of the class. Any variables after this one will
									//be stomped over with vertex data
	std::vector<SPolyVertex>		m_Vertices;
	int				m_Index;
	int				m_NumVertices;

	glm::vec3		m_vCenter;		// Center of the polygon
	float			m_fRadius;		// Radius of the sphere that contains the polygon

public:
	WorldPoly();
	~WorldPoly();

	uint32 GetIndex() const							{ return m_Index; }
	void SetIndex(uint32 index)						{ m_Index=index; }
	
	uint32 GetNumVertices() const					{ return m_NumVertices; }
	void SetNumVertices(uint32 nVertices)			{ m_NumVertices = nVertices; m_Vertices.resize(nVertices); }

	const Surface *GetSurface() const				{ return m_pSurface; }
	Surface *GetSurface()							{ return m_pSurface; }
	void SetSurface(Surface *pSurface)				{ m_pSurface = pSurface; }

	LTPlane* GetPlane()								{ return m_pPlane; }
	const LTPlane* GetPlane() const					{ return m_pPlane; }
	void SetPlane(LTPlane *pPlane)					{ m_pPlane = pPlane; }

	// Get a vertex's position.
	glm::vec3& GetVertex(uint32 i)					{ assert(i < GetNumVertices()); return m_Vertices[i].m_Vertex->m_Vec; }
	const glm::vec3& GetVertex(uint32 i) const		{ assert(i < GetNumVertices()); return m_Vertices[i].m_Vertex->m_Vec; }

	const SPolyVertex *GetVertices() const			{ return &m_Vertices[0]; }
	SPolyVertex *GetVertices()						{ return &m_Vertices[0]; }

	const glm::vec3 &GetCenter() const				{ return m_vCenter; }
	glm::vec3 &GetCenter()							{ return m_vCenter; }
	void SetCenter(const glm::vec3 &vCenter)		{ m_vCenter = vCenter; }

	float GetRadius() const							{ return m_fRadius; }
	void SetRadius(float fRadius)					{ m_fRadius = fRadius; }

};

