#include "stdafx.h"
#include "RenderWorld.h"


RenderWorld::RenderWorld(Context* p) :m_Context(p)
{
	numdraw = 0;
}


RenderWorld::~RenderWorld()
{
}

bool RenderWorld::Load(FILE * pFile)
{
	STREAM_READ(m_nRenderBlockCount);

	m_pRenderBlocks.resize(m_nRenderBlockCount);

	for (uint32 nCurRenderBlock = 0; nCurRenderBlock < m_nRenderBlockCount; ++nCurRenderBlock)
	{
		m_pRenderBlocks[nCurRenderBlock] = std::make_unique<RenderBlock>();
		m_pRenderBlocks[nCurRenderBlock]->SetWorld(this);
	}

	bool bResult = true;

	// Load the blocks
	for (uint32 nReadLoop = 0; nReadLoop < m_nRenderBlockCount; ++nReadLoop)
	{
		if (!m_pRenderBlocks[nReadLoop]->Load(pFile))
		{
			// Cut off the array and remember that we failed
			m_nRenderBlockCount = nReadLoop;
			bResult = false;
			break;
		}
	}

	// Fix-up their child pointers
	for (uint32 nFixupLoop = 0; nFixupLoop < m_nRenderBlockCount; ++nFixupLoop)
	{
		m_pRenderBlocks[nFixupLoop]->FixupChildren(m_pRenderBlocks);
	}

	m_bBlocksDirty = true;

	// Load the worldmodels
	uint32 nNumWorldModels;
	STREAM_READ(nNumWorldModels);

	m_aWorldModels.clear();

	for (uint32 nCurWorldModel = 0; nCurWorldModel < nNumWorldModels; ++nCurWorldModel)
	{
		char sWMName[MAX_WORLDNAME_LEN + 1]; uint16 len;
		STREAM_READ(len);
		fread(sWMName, len, 1, pFile);
		sWMName[len] = 0;

		RenderWorld *pNewWorldModel;
		pNewWorldModel = new RenderWorld(m_Context);

		if (!pNewWorldModel->Load(pFile))
		{
			assert(!"Error loading world model");
			return false;
		}

		uint32 nNameHash = GetWMNameHash(sWMName);
		TWorldModelMap::iterator iFindWMNameHash = m_aWorldModels.find(nNameHash);
		if (iFindWMNameHash != m_aWorldModels.end())
		{
			assert(!"World model name hash conflict detected!");
			printf("Error loading world model %s due to name hash conflict", sWMName);
			delete pNewWorldModel;
			continue;
		}

		m_aWorldModels[nNameHash] = pNewWorldModel;
	}

	return bResult;

}


void RenderWorld::Render(Camera * cam)
{
	/*int numdraw = 0;
	RenderBlock* pInsideBlock = m_pRenderBlocks[0].get();
	if (pInsideBlock->IsPtInside(cam->GetPosition()))
	{
		while (pInsideBlock)
		{
			pInsideBlock->Render(m_Context->m_pRenderer.get(), cam);
			numdraw++;
			RenderBlock *pChild = LTNULL;
			uint32 nChildIndex = 0;
			for (; !pChild && (nChildIndex < 2); ++nChildIndex)
			{
				pChild = pInsideBlock->GetChild(nChildIndex);
				if (pChild && !pChild->IsPtInside(cam->GetPosition()))
					pChild = LTNULL;
			}
			pInsideBlock = pChild;
		}
	}
	else
	{
		for (uint32 nRBDrawLoop = 0; nRBDrawLoop < m_nRenderBlockCount; ++nRBDrawLoop)
		{
			m_pRenderBlocks[nRBDrawLoop]->Render(m_Context->m_pRenderer.get(), cam);
		}
	}
	ImGui::Text("Num Draw: %d", numdraw);
	*/

	int c = numdraw > m_nRenderBlockCount?m_nRenderBlockCount : numdraw;
	for (uint32 nRBDrawLoop = 0; nRBDrawLoop < c; ++nRBDrawLoop)
	{
		m_pRenderBlocks[nRBDrawLoop]->Render(m_Context->m_pRenderer.get(), cam);
	}

	
}

uint32 RenderWorld::GetWMNameHash(const char *pName)
{
	return st_GetHashCode(pName);
}

Shader * RenderWorld::GetShader()
{
	return m_Context->m_pResources->GetShader("MapShader");
}
