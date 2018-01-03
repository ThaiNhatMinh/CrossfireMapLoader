#include "stdafx.h"
#include "WorldBSP.h"


WorldBSP::WorldBSP()
{
}


WorldBSP::~WorldBSP()
{
}



//LOADING STRUCTURES
// These structures are in the form that commonly read elements are in on disk. Therefore
// an element can be loaded in a single read and then converted over to the internal
// format, instead of having to do a large number of read operations

struct SDiskSurface
{
	uint32	m_nFlags;
	uint16	m_nTexture;
	uint16	m_nTextureFlags;
};

struct SDiskPoly
{
	//this will calculate the disk size of a polygon with the specified number of vertices
	static uint32 CalcPolyReadSize(uint32 nNumVerts) { return sizeof(uint32) * (2 + nNumVerts); }

	uint32	m_nSurface;
	uint32	m_nPlane;
	uint32	m_nVerts[MAX_WORLDPOLY_VERTS];
};
static Node _g_InNode(NF_IN);
static Node _g_OutNode(NF_OUT);

Node *NODE_IN = &_g_InNode;
Node *NODE_OUT = &_g_OutNode;
Node* w_NodeForIndex(Node *pList, uint32 listSize, int index)
{
	if (index == -1)
	{
		return NODE_IN;
	}
	else if (index == -2)
	{
		return NODE_OUT;
	}
	else if (index >= (int)listSize)
	{
		return NULL;
	}
	else
	{
		return &pList[index];
	}
}

void w_SetPlaneTypes(Node *pNodes, uint32 nNodes, bool bUsePlaneTypes)
{
	uint32 i;
	Node *pNode;

	for (i = 0; i < nNodes; i++)
	{
		pNode = &pNodes[i];

		pNode->m_PlaneType = PLANE_GENERIC;
		if (bUsePlaneTypes)
		{
			if (pNode->GetPlane()->m_Normal.x > PLANE_EP)
				pNode->m_PlaneType = PLANE_POSX;
			else if (pNode->GetPlane()->m_Normal.x < -PLANE_EP)
				pNode->m_PlaneType = PLANE_NEGX;
			else if (pNode->GetPlane()->m_Normal.y > PLANE_EP)
				pNode->m_PlaneType = PLANE_POSY;
			else if (pNode->GetPlane()->m_Normal.y < -PLANE_EP)
				pNode->m_PlaneType = PLANE_NEGY;
			else if (pNode->GetPlane()->m_Normal.z > PLANE_EP)
				pNode->m_PlaneType = PLANE_POSZ;
			else if (pNode->GetPlane()->m_Normal.z < -PLANE_EP)
				pNode->m_PlaneType = PLANE_NEGZ;
		}
	}
}
ELoadWorldStatus WorldBSP::Load(FILE * pFile, bool bUsePlaneTypes)
{
	uint32 i, k;
	uint16 wLeaf;

	uint32 curVert, nLeafs;
	uint32 nPoints, nPolies, nVerts, totalVisListSize;
	uint32 poliesStartPos;
	uint32 poliesSize;

	uint32 curPos;
	WorldPoly *pPoly;
	uint32 nPlanes, nSurfaces;
	uint32 iPoly;

	Node *pNode;
	int j, nodeIndex;
	uint16 tempWord;
	//ConParse conParse;
	uint32 nSections;
	uint8 nVertices;
	uint32 nUserPortals;
	uint32 nLeafLists;

	uint32 nDWordWorldInfoFlags;
	STREAM_READ(nDWordWorldInfoFlags);

	//make sure we aren't truncating anything
	assert((nDWordWorldInfoFlags & 0xFFFF0000) == 0);
	m_WorldInfoFlags = nDWordWorldInfoFlags;

	uint16 len;
	STREAM_READ(len);
	fread(m_WorldName, len, 1, pFile);

	STREAM_READ(nPoints);
	STREAM_READ(nPlanes);
	STREAM_READ(nSurfaces);

	STREAM_READ(nUserPortals);
	STREAM_READ(nPolies);
	STREAM_READ(nLeafs);
	STREAM_READ(nVerts);
	STREAM_READ(totalVisListSize);
	STREAM_READ(nLeafLists);
	STREAM_READ(m_nNodes);

	STREAM_READ(m_MinBox);
	STREAM_READ(m_MaxBox);
	STREAM_READ(m_WorldTranslation);

	if (nUserPortals > 0)
		return LoadWorld_InvalidFile;

	// Read the texture list.
	uint32 nNamesLen;
	uint32 nTextures;

	STREAM_READ(nNamesLen);
	STREAM_READ(nTextures);

	curPos = 0;
	for (i = 0; i < nTextures; i++)
	{
		//m_TextureNames[i] = &m_TextureNameData[curPos];
		std::string name;
		for (;;)
		{
			char c;
			STREAM_READ(c);
			name.append(&c);
			if (c == 0)
			{
				m_TextureNames.push_back(name);
			}
			curPos++;
			
		}
	}

	// Figure out how much space to allocate for the polygon buffers.
	poliesSize = 0;
	poliesStartPos = ftell(pFile);
	// Read in the initial structure sizes so it can do structure alignment.    
	for (i = 0; i < nPolies; i++)
	{
		STREAM_READ(nVertices);

		poliesSize += WORLDPOLY_SIZE(nVertices);
		poliesSize = ALIGN_MEMORY(poliesSize);
	}

	fseek(pFile, poliesStartPos, SEEK_SET);

	// (Try to) allocate all the data.
	m_PolyDataSize = poliesSize;

	//char            *m_PolyData;        // Data blocks
	//m_PolyData = new char[poliesSize];
	m_Points = new Vertex[nPoints];
	m_Polies.resize(nPolies);
	for (size_t i = 0; i < m_Polies.size(); i++) m_Polies[i] = std::make_unique<WorldPoly>();

	m_Planes = new LTPlane[nPlanes];
	m_Surfaces = new Surface[nSurfaces];
	m_Nodes = new Node[m_nNodes];

	m_MemoryUse = poliesSize +
		sizeof(Vertex) * nPoints +
		sizeof(Node) * m_nNodes +
		sizeof(WorldPoly *) * nPolies +
		sizeof(LTPlane) * nPlanes +
		sizeof(Surface) * nSurfaces +
		nNamesLen +
		sizeof(char *) * nTextures +
		sizeof(WorldBSP);

	m_nPoints = nPoints;
	m_nPolies = nPolies;

	m_nPlanes = nPlanes;
	m_nSurfaces = nSurfaces;

	// Construct all the polygons.
	curVert = 0;
	curPos = 0;
	for (i = 0; i < nPolies; i++)
	{
		STREAM_READ(nVertices);

		//pPoly = (WorldPoly*)(&m_PolyData[curPos]);
		m_Polies[i]->SetIndex(i);
		m_Polies[i]->SetNumVertices(nVertices);

		curVert += nVertices;
		curPos += WORLDPOLY_SIZE(nVertices);
		curPos = ALIGN_MEMORY(curPos);
	}

	//pseudo load in the leaf lists
	for (i = 0; i < nLeafs; i++)
	{
		uint16 nNumLeafLists;
		STREAM_READ(nNumLeafLists);

		if (nNumLeafLists == 0xFFFF)
		{
			// This leaf uses the lists from another leaf.
			STREAM_READ(tempWord);
		}
		else
		{
			// This leaf has its own lists.
			for (k = 0; k < nNumLeafLists; k++)
			{
				//portal ID
				STREAM_READ(tempWord);

				//list size
				uint16 nListSize;
				STREAM_READ(nListSize);

				//skip over the list
				fseek(pFile,ftell(pFile) + nListSize, SEEK_SET);
			}
		}
	}

	// Read in the planes.
	fread(m_Planes, sizeof(LTPlane) * nPlanes,1,pFile);

	// Read in the surfaces.
	SDiskSurface DiskSurface;
	for (i = 0; i < nSurfaces; i++)
	{
		STREAM_READ(DiskSurface);

		m_Surfaces[i].m_pTexture = NULL;
		m_Surfaces[i].m_Flags = DiskSurface.m_nFlags;
		m_Surfaces[i].m_iTexture = DiskSurface.m_nTexture;
		m_Surfaces[i].m_TextureFlags = DiskSurface.m_nTextureFlags;
	}

	// Read in all the polies.
	SDiskPoly DiskPoly;
	for (i = 0; i < nPolies; i++)
	{
		pPoly = m_Polies[i].get();

		//read in our polygon from disk
		fread(&DiskPoly, SDiskPoly::CalcPolyReadSize(pPoly->GetNumVertices()),1,pFile);

		if (DiskPoly.m_nSurface >= m_nSurfaces)
			return LoadWorld_InvalidFile;

		//Get the plane of this polygon
		if (DiskPoly.m_nPlane >= m_nPlanes)
			return LoadWorld_InvalidFile;

		pPoly->SetSurface(&m_Surfaces[DiskPoly.m_nSurface]);
		pPoly->SetPlane(&m_Planes[DiskPoly.m_nPlane]);

		// Read in the list of indices.
		for (k = 0; k < pPoly->GetNumVertices(); k++)
		{
			if (DiskPoly.m_nVerts[k] >= m_nPoints)
				return LoadWorld_InvalidFile;

			pPoly->GetVertices()[k].m_Vertex = &m_Points[DiskPoly.m_nVerts[k]];
		}
	}

	// Read nodes.
	uint32 nNodeIndices[2];

	for (i = 0; i < m_nNodes; i++)
	{
		pNode = &m_Nodes[i];

		STREAM_READ(iPoly);
		if (iPoly >= m_nPolies)
		{
			return LoadWorld_InvalidFile;
		}

		pNode->m_pPoly = m_Polies[iPoly].get();
		pNode->m_Flags = 0;
		pNode->m_PlaneType = 0;

		STREAM_READ(wLeaf);
		STREAM_READ(nNodeIndices);

		for (j = 0; j < 2; j++)
		{
			pNode->m_Sides[j] = w_NodeForIndex(m_Nodes, m_nNodes, nNodeIndices[j]);
			if (!pNode->m_Sides[j])
			{
				return LoadWorld_InvalidFile;
			}
		}
	}

	// Classify its plane.
	w_SetPlaneTypes(m_Nodes, m_nNodes, bUsePlaneTypes);

	// Read m_Points.
	fread(m_Points, (nPoints * sizeof(glm::vec3)),1,pFile);

	// Root poly index..
	STREAM_READ(nodeIndex);
	m_RootNode = w_NodeForIndex(m_Nodes, m_nNodes, nodeIndex);
	if (!m_RootNode)
	{
		return LoadWorld_InvalidFile;
	}

	// Sections.
	STREAM_READ(nSections);

	//Note that this should always be 0, this should be removed later when the level
	//version is updated
	if (nSections > 0)
		return LoadWorld_InvalidFile;



	printf("m_MemoryUse: %d\n", m_MemoryUse);

	return LoadWorld_Ok;
}

void WorldBSP::CalcBoundingSpheres()
{
	uint32 i, j;
	WorldPoly *pPoly;
	float dist;

	glm::vec3 vMin;
	glm::vec3 vMax;

	for (i = 0; i < m_nPolies; i++)
	{
		pPoly = m_Polies[i].get();

		vMin = pPoly->GetVertex(0);
		vMax = vMin;

		for (j = 1; j < pPoly->GetNumVertices(); j++)
		{
			VEC_MIN(vMin, vMin, pPoly->GetVertex(j));
			VEC_MAX(vMax, vMax, pPoly->GetVertex(j));
		}
		pPoly->SetCenter((vMin + vMax) * 0.5f);

		float fRadiusSqr = 0.0f;
		for (j = 0; j < pPoly->GetNumVertices(); j++)
		{
			dist = glm::distance(pPoly->GetCenter(),pPoly->GetVertex(j));
			fRadiusSqr = LTMAX(fRadiusSqr, dist);
		}
		pPoly->SetRadius(sqrtf(fRadiusSqr) + 0.2f);
	}
}
