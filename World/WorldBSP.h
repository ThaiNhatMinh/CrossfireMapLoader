#pragma once


enum ELoadWorldStatus {
	LoadWorld_Ok = 0,
	LoadWorld_InvalidVersion = 1,
	LoadWorld_InvalidFile = 2,
	LoadWorld_InvalidParams,
	LoadWorld_Error // Generic error
};

class WorldPoly;
class SPolyVertex;
class LTPlane;
class Node;
class Surface;
class Vertex;
#define WORLDPOLY_SIZE(n) 	sizeof(WorldPoly) + (((uint32)(n - 1))*sizeof(SPolyVertex))
#define ALIGNMENT_SIZE      4
#define ALIGN_MEMORY(n)     ((n%ALIGNMENT_SIZE) == 0) ? n : (n + ALIGNMENT_SIZE - (n % ALIGNMENT_SIZE))

#define PLANE_EP 			0.99999f


class WorldBSP
{
public:
	uint32 m_WorldInfoFlags;
	char m_WorldName[65];
	
	uint32			m_MemoryUse;		// Total memory usage.

	LTPlane         *m_Planes;			// Planes.
	uint32			m_nPlanes;

	Node			*m_Nodes;			// Nodes.
	uint32			m_nNodes;

	Surface			*m_Surfaces;		// Surfaces.
	uint32			m_nSurfaces;

	Node            *m_RootNode;		// Root node of the tree.

	std::vector<std::unique_ptr<WorldPoly>>		m_Polies;			// Polies.
	uint32			m_nPolies;

	Vertex			*m_Points;			// Vertices.
	uint32			m_nPoints;

	std::vector<std::string> m_TextureNames;

	glm::vec3        m_MinBox, m_MaxBox; // Bounding box on the whole WorldBsp.

	glm::vec3        m_WorldTranslation; // Centering translation for WorldModels, so they can always
										// be treated like they're at the origin.

										// Index, used for poly references.  The main world always has an index
										// of 0xFFFF.  The WorldModels are indexed into m_WorldModelArray.
	uint16			m_Index;

	
	uint32          m_PolyDataSize;
public:
	WorldBSP();
	~WorldBSP();
	ELoadWorldStatus Load(FILE*pFile, bool t);

	void CalcBoundingSpheres();
};

