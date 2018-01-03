#pragma once


class WorldTreeNode;
class WorldTree;
// This links a WorldTreeObj to a node.
class WTObjLink
{
public:
	WTObjLink() = default;
	~WTObjLink() {};
	LTLink          m_Link;
	WorldTreeNode   *m_pNode;
};

class WorldTreeObj
{
public:

	WorldTreeObj(WTObjType objType);
	virtual ~WorldTreeObj();

	inline WTObjType GetObjType() {
		return m_ObjType;
	}

	inline bool IsInWorldTree() {
		return m_WTFrameCode != FRAMECODE_NOTINTREE;
	}

	// This is called before the object is added to the world tree.
	// It gives objects a chance to put themselves on specific nodes rather than
	// filtering it down like normal.
	// If you return LTTRUE, then it assumes you added yourself.
	virtual bool InsertSpecial(WorldTree *pTree) { return LTFALSE; }

	// Unlink everything from the world tree.
	void RemoveFromWorldTree();

	// Get the bounding box of the object.
	const glm::vec3& GetBBoxMin() const { return m_MinBox; }
	const glm::vec3& GetBBoxMax() const { return m_MaxBox; }

	// Updates the extents of the bounding box based upon a position and dimensions
	void UpdateBBox(const glm::vec3& vPos, const glm::vec3& vDims)
	{
		m_MinBox = vPos - vDims;
		m_MaxBox = vPos + vDims;
	}

public:

	//the min and max extents of the bounding box of this object
	glm::vec3		m_MinBox;
	glm::vec3		m_MaxBox;

	WTObjLink       m_Links[MAX_OBJ_NODE_LINKS];

	// Tells what kind of object this is.
	WTObjType       m_ObjType;

	// Used in conjunction with WorldTree::m_CurFrameCode.
	// Set to FRAMECODE_NOTINTREE if the object is not in the WorldTree.
	uint32          m_WTFrameCode;
};



class StaticLight : public WorldTreeObj
{
public:
	StaticLight() : WorldTreeObj(WTObj_Light), m_eAttenuation(eAttenuation_Quartic),
		m_FOV(-1.0f), m_fConvertToAmbient(0.0f), m_pLightGroupColor(NULL), m_nLightGroupID(0) {}

	glm::vec3				m_Pos;				// Position of the light
	float					m_Radius;			// Maximum radius of the light
	glm::vec3				m_Color;			// 0-255
	glm::vec3				m_Dir;				// Normalized direction vector for directional lights
	float					m_FOV;				// cos(fov/2)  -1 for omnidirectional lights
	glm::vec3				m_AttCoefs;			// Light Attenuation Co-Eff (A, B, C - in inverse quad equasion)...
	float					m_fConvertToAmbient;// The amount of light to convert to ambient, ie if .2, 20% of light will be removed from directional and put into ambient
	uint32					m_Flags;			// Flags for the light. Currently only indicates if it should cast shadows
	const glm::vec3*			m_pLightGroupColor;	// A pointer to the lightgroup color that this light belongs to. If it is NULL, the light doesn't belong to a light group
	uint32					m_nLightGroupID;	// The ID of the lightgroup that this light belongs to
	ELightAttenuationType	m_eAttenuation;		// The attenuation model of this light
};

class WorldTreeNode
{
private:
	std::vector<std::vector<std::unique_ptr<WorldTreeNode>>> m_Children;
	std::vector<std::unique_ptr<WorldTreeNode>> m_ChildrenA;
	glm::vec3 m_BBoxMax;
	glm::vec3 m_BBoxMin;
	// Centerpoint of the bounding box in the X and Z dimensions
	float					m_fCenterX;
	float					m_fCenterZ;
	// The smallest of the X and Z dimensions.
		float					m_fSmallestDim;
		// All the objects sitting on this node.
		CheapLTLink				m_Objects[NUM_NODEOBJ_ARRAYS];
		// How many objects are on or below this node? Used to stop recursion early.
		uint32					m_nObjectsOnOrBelow;
public:
	WorldTreeNode* m_pParent;
	WorldTreeNode();
	~WorldTreeNode();

	void SetBBox(glm::vec3 min, glm::vec3 max);
	bool Load(FILE* pFile, uint8 &curByte, uint8 &curBit);

	bool Subdivide();
	void TermChildren();
	void Term();
	void Clear();

	//gets the smallest of the X and Z dimensions (Y is essentially irrelevant)
	float					GetSmallestDim() const { return m_fSmallestDim; }
	//determines if this node in the world tree has any children
	bool					HasChildren() const { return m_Children.empty(); }

	// Adds an object to the specified list.
	void					AddObjectToList(WTObjLink *pLink, NodeObjArray iArray);


	//accesses the specified world node
	WorldTreeNode*			GetChild(uint32 nX, uint32 nZ) { return m_Children[nX][nZ].get(); }
	const WorldTreeNode*	GetChild(uint32 nX, uint32 nZ) const { return m_Children[nX][nZ].get(); }

	WorldTreeNode*			GetChild(uint32 nNode) { return m_Children[nNode/2][nNode%2].get(); }
	const WorldTreeNode*	GetChild(uint32 nNode) const { return m_Children[nNode / 2][nNode % 2].get(); }

	//accessors for bounding box information
	const glm::vec3&			GetBBoxMin() const { return m_BBoxMin; }
	const glm::vec3&			GetBBoxMax() const { return m_BBoxMax; }
	float					GetCenterX() const { return m_fCenterX; }
	float					GetCenterZ() const { return m_fCenterZ; }

};

