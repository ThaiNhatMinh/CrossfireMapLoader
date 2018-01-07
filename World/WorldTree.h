#pragma once



class WorldTreeObj;
class WorldTreeNode;
class WorldTree
{
private:
	// Root of tree (depth value 0).		
	WorldTreeNode	m_RootNode;

	// Depth of tree that terrains are at.
	uint32			m_TerrainDepth;
public:
	WorldTree();
	~WorldTree();

	bool Load(FILE* pFile);
	// Add and remove objects from the tree.
	void            InsertObject(WorldTreeObj *pObj, NodeObjArray iArray = NOA_Objects);
	void            InsertObject2(WorldTreeObj *pObj, const glm::vec3& vMin, const glm::vec3& vMax, NodeObjArray iArray = NOA_Objects);

};

