#include "stdafx.h"
#include "WorldTree.h"


class FilterObjInfo
{
public:
	WorldTreeObj		*m_pObj;
	NodeObjArray		m_iObjArray;
	glm::vec3			m_Min;
	glm::vec3			m_Max;
	float				m_MaxSize;
	uint32				m_iCurLink;
};

WorldTree::WorldTree()
{
}


WorldTree::~WorldTree()
{
}

bool WorldTree::Load(FILE * pFile)
{
	// Save root node dims.
	glm::vec3 m_BBoxMax;
	glm::vec3 m_BBoxMin;
	uint32 nTotalNodes;


	fread(&m_BBoxMin, sizeof(float) * 3, 1, pFile);
	fread(&m_BBoxMax, sizeof(float) * 3, 1, pFile);
	fread(&nTotalNodes, sizeof(uint32), 1, pFile);
	fread(&m_TerrainDepth, sizeof(uint32), 1, pFile);

	uint8 curByte = 0;
	uint8 curBit = 8;
	uint32 nCurrOffset = 0;

	m_RootNode.SetBBox(m_BBoxMin, m_BBoxMax);
	return m_RootNode.Load(pFile, curByte, curBit);
}

void WorldTree::InsertObject(WorldTreeObj *pObj, NodeObjArray iArray)
{
	InsertObject2(pObj, pObj->GetBBoxMin(), pObj->GetBBoxMax(), iArray);
}

static void FilterObj_R(WorldTreeNode *pNode, FilterObjInfo *pInfo);

void WorldTree::InsertObject2(WorldTreeObj *pObj,
	const glm::vec3& vMin,
	const glm::vec3& vMax,
	NodeObjArray iArray)
{
	FilterObjInfo foInfo;
	glm::vec3 vDiff;

	pObj->RemoveFromWorldTree();

	vDiff = vMax - vMin;

	foInfo.m_pObj = pObj;
	foInfo.m_iObjArray = iArray;
	foInfo.m_Min = vMin;
	foInfo.m_Max = vMax;
	foInfo.m_MaxSize = LTMAX(vDiff.x, vDiff.z);
	foInfo.m_iCurLink = 0;

	if (!pObj->InsertSpecial(this))
	{
		FilterObj_R(&m_RootNode, &foInfo);
	}
}

// Used by some of the recursive routines.
typedef void(*FilterFn_R)(WorldTreeNode *pNode, void *pData);
inline void FilterBox(const glm::vec3 *pMin, const glm::vec3 *pMax,
	WorldTreeNode *pNode, FilterFn_R fn, void *pFnData)
{
	if (pMin->x < pNode->GetCenterX())
	{
		if (pMin->z < pNode->GetCenterZ())
		{
			fn(pNode->GetChild(0, 0), pFnData);
		}

		if (pMax->z > pNode->GetCenterZ())
		{
			fn(pNode->GetChild(0, 1), pFnData);
		}
	}

	if (pMax->x > pNode->GetCenterX())
	{
		if (pMin->z < pNode->GetCenterZ())
		{
			fn(pNode->GetChild(1, 0), pFnData);
		}

		if (pMax->z > pNode->GetCenterZ())
		{
			fn(pNode->GetChild(1, 1), pFnData);
		}
	}
}


static void FilterObj_R(WorldTreeNode *pNode, FilterObjInfo *pInfo)
{
	if (pInfo->m_MaxSize >= (pNode->GetSmallestDim() * 0.5f) || !pNode->HasChildren())
	{
		// This shouldn't ever happen.  If it does, the object won't be
		// located correctly.
		if (pInfo->m_iCurLink >= MAX_OBJ_NODE_LINKS)
		{
			assert(false);
			return;
		}

		// Ok, it's likely to cover the space of all the nodes below us anyways, so add it
		// to this node and stop recursing.
		WTObjLink *pLink = &pInfo->m_pObj->m_Links[pInfo->m_iCurLink];
		assert(pLink->m_Link.IsTiedOff());

		pNode->AddObjectToList(pLink, pInfo->m_iObjArray);

		pInfo->m_iCurLink++;
	}
	else
	{
		FilterBox(&pInfo->m_Min, &pInfo->m_Max, pNode, (FilterFn_R)FilterObj_R, pInfo);
	}
}