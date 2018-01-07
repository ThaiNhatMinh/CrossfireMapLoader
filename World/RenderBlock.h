#pragma once

class RenderWorld;

// Standard renderblock base vertex type. Note that the format for this needs to match byte
//for byte the format of the vertex on disk. This makes it a bit more fragile, but saves
//several million calls to read at load time which is a significant gain. If this cannot
//be done, the loading code can be modified to use a custom read operator.
struct SRBVertex
{
	glm::vec3 m_vPos;
	float m_fU0, m_fV0;
	float m_fU1, m_fV1;
	uint32 m_nColor;
	glm::vec3 m_vNormal;
	glm::vec3 m_vTangent;
	glm::vec3 m_vBinormal;
};


// Geometry-only n-gon w/ a pre-stored plane and bounds
struct SRBGeometryPoly
{
	SRBGeometryPoly() {}
	~SRBGeometryPoly() {}
	SRBGeometryPoly(const SRBGeometryPoly &sOther) :
		m_aVerts(sOther.m_aVerts),
		m_cPlane(sOther.m_cPlane),
		m_ePlaneCorner(sOther.m_ePlaneCorner),
		m_vMin(sOther.m_vMin),
		m_vMax(sOther.m_vMax)
	{}
	SRBGeometryPoly &operator=(const SRBGeometryPoly &sOther) {
		m_aVerts = sOther.m_aVerts;
		m_cPlane = sOther.m_cPlane;
		m_ePlaneCorner = sOther.m_ePlaneCorner;
		m_vMin = sOther.m_vMin;
		m_vMax = sOther.m_vMax;
		return *this;
	}
	typedef std::vector<glm::vec3> TVertList;
	TVertList m_aVerts;
	LTPlane m_cPlane;
	EAABBCorner m_ePlaneCorner;
	glm::vec3 m_vMin, m_vMax;
};

// Occluder polygon
struct SRBOccluder : public SRBGeometryPoly
{
	SRBOccluder() : SRBGeometryPoly(), m_nID(0), m_bEnabled(true) {}
	SRBOccluder(const SRBOccluder &sOther) :
		SRBGeometryPoly(sOther),
		m_nID(sOther.m_nID),
		m_bEnabled(sOther.m_bEnabled)
	{}
	SRBOccluder &operator=(const SRBOccluder &sOther) {
		static_cast<SRBGeometryPoly&>(*this) = sOther;
		m_nID = sOther.m_nID;
		m_bEnabled = sOther.m_bEnabled;
		return *this;
	}
	uint32 m_nID;
	bool m_bEnabled;
};

// Light group data for the render block
struct SRBLightGroup
{
	struct SSubLM
	{
		bool Load(FILE* cStream);

		uint32 m_nLeft, m_nTop;
		uint32 m_nWidth, m_nHeight;

		typedef std::vector<uint8> TDataList;
		TDataList m_aData;
	};

	SRBLightGroup() {}
	~SRBLightGroup();
	SRBLightGroup(const SRBLightGroup &sOther);
	SRBLightGroup &operator=(const SRBLightGroup &sOther);

	uint32 m_nID;
	glm::vec3 m_vColor;
	typedef std::vector<uint8> TVertexIntensityData;
	TVertexIntensityData m_aVertexIntensities;
	typedef std::vector<SSubLM> TSubLMList;
	typedef std::vector<TSubLMList*> TSectionLMList;
	TSectionLMList m_aSectionLMs;

	// Clear the section lightmap list
	void ClearSectionLMList();
};

class Camera;
class MapMesh;
class Shader;
class OpenGLRenderer;
class RenderWorld;
class RenderBlock
{
private:
	RenderWorld* m_pWorld;

	// Bounds of the render block
	glm::vec3 m_vBoundsMin, m_vBoundsMax;
	glm::vec3 m_vCenter, m_vHalfDims;

	typedef std::vector<CRBSection> TSectionList;
	TSectionList m_aSections;

	typedef std::vector<SRBGeometryPoly> TGeometryPolyList;
	TGeometryPolyList m_aSkyPortals;
	typedef std::vector<SRBOccluder> TOccluderList;
	TOccluderList m_aOccluders;

	typedef std::vector<SRBLightGroup> TLightGroupList;
	TLightGroupList m_aLightGroups;

	// The raw rendering data
	uint32 m_nTriCount;
	uint16 *m_aIndices;
	uint32 m_nVertexCount;
	SRBVertex *m_aVertices;
	std::unique_ptr<MapMesh> Mesh;
	// Are the shaders bound?
	bool m_bShadersBound;
	// Are the shaders dirty?
	bool m_bShadersDirty;

	// Our children
	enum { k_NumChildren = 2 };
	enum { k_InvalidChild = 0xFFFFFFFF };
	RenderBlock* m_aChildren[k_NumChildren];

	Shader* m_pShader;
	vector<vec3> color;
public:
	RenderBlock();
	~RenderBlock();


	bool Load(FILE* pFile);

	void Render(OpenGLRenderer* r,Camera* cam);
	//void RenderSection(int i, OpenGLRenderer* r, Camera* cam);
	void SetWorld(RenderWorld *pWorld);
	RenderWorld *GetWorld() const { return m_pWorld; }
	bool IsSpriteTexture(const char *pFilename);
	SharedTexture *LoadSpriteData(CRBSection &cSection, uint32 nTextureIndex, const char *pFilename);
	ERenderShader TranslatePCShaderCode(uint32 nCode);

	RenderBlock *GetChild(uint32 nIndex) const { return m_aChildren[nIndex]; }

	void FixupChildren(std::vector<std::unique_ptr<RenderBlock>>& pBase);

	// Is the provided point inside the rendering block?
	inline bool IsPtInside(const vec3 &vTestPt) const {
		return (vTestPt.x >= m_vBoundsMin.x) && (vTestPt.x <= m_vBoundsMax.x) &&
			(vTestPt.y >= m_vBoundsMin.y) && (vTestPt.y <= m_vBoundsMax.y) &&
			(vTestPt.z >= m_vBoundsMin.z) && (vTestPt.z <= m_vBoundsMax.z);
	}

};

