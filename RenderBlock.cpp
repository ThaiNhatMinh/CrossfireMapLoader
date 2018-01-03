#include "stdafx.h"
#include "RenderBlock.h"

// Shader types we're going to get from the processor
enum EPCShaderType {
	ePCShader_None = 0, // No shading
	ePCShader_Gouraud = 1, // Textured and vertex-lit
	ePCShader_Lightmap = 2, // Base lightmap
	ePCShader_Lightmap_Texture = 4, // Texturing pass of lightmapping
	ePCShader_Skypan = 5, // Skypan
	ePCShader_SkyPortal = 6,
	ePCShader_Occluder = 7,
	ePCShader_DualTexture = 8,	// Gouraud shaded dual texture
	ePCShader_Lightmap_DualTexture = 9, //Texture stage of lightmap shaded dual texture
	ePCShader_Unknown = 10 // Unknown - draw something to make it obvious there's a problem
};


RenderBlock::RenderBlock()
{
}


RenderBlock::~RenderBlock()
{
}

bool RenderBlock::IsSpriteTexture(const char *pFilename)
{
	uint32 nTexNameLen = strlen(pFilename);

	if (nTexNameLen < 4)
		return false;

	return _stricmp(&pFilename[nTexNameLen - 4], ".spr") == 0;
}

uint32 LoadSprite(const char *pFilename, Sprite **ppSprite) { return 0; };

void spr_InitTracker(SpriteTracker *pTracker, Sprite *pSprite)
{
	pTracker->m_pSprite = pSprite;
	pTracker->m_pCurAnim = &pSprite->m_Anims[0];

	if (pTracker->m_pCurAnim->m_nFrames > 0)
		pTracker->m_pCurFrame = &pTracker->m_pCurAnim->m_Frames[0];
	else
		pTracker->m_pCurFrame = LTNULL;

	pTracker->m_MsCurTime = 0;
	pTracker->m_Flags = SC_PLAY | SC_LOOP;
}

EAABBCorner GetAABBPlaneCorner(const glm::vec3 &vNormal)
{
	static glm::vec3 aCornerDir[8] = {
		glm::vec3(-1.0f, 1.0f, -1.0f), // Near top-left
		glm::vec3(1.0f, 1.0f, -1.0f), // Near top-right
		glm::vec3(-1.0f, -1.0f, -1.0f), // Near bottom-left
		glm::vec3(1.0f, -1.0f, -1.0f), // Near bottom-right
		glm::vec3(-1.0f, 1.0f, 1.0f), // Far top-left
		glm::vec3(1.0f, 1.0f, 1.0f), // Far top-right
		glm::vec3(-1.0f, -1.0f, 1.0f), // Far bottom-left
		glm::vec3(1.0f, -1.0f, 1.0f), // Far bottom-right
	};

	uint32 nBestCorner = 0;
	float fBestCornerDot = -1.0f;
	for (uint32 nCornerLoop = 0; nCornerLoop < 8; ++nCornerLoop)
	{
		float fCornerDot = glm::dot(vNormal,aCornerDir[nCornerLoop]);
		// We're looking for the most positive corner dot product..
		if (fCornerDot > fBestCornerDot)
		{
			nBestCorner = nCornerLoop;
			fBestCornerDot = fCornerDot;
		}
	}

	return (EAABBCorner)nBestCorner;
}

ERenderShader RenderBlock::TranslatePCShaderCode(uint32 nCode)
{
	switch ((EPCShaderType)nCode)
	{
	case ePCShader_None:
		return eShader_Invalid;
	case ePCShader_Gouraud:
		return eShader_Gouraud;
	case ePCShader_Lightmap:
		return eShader_Lightmap;
	case ePCShader_Lightmap_Texture:
		return eShader_Lightmap_Texture;
	case ePCShader_DualTexture:
		return eShader_Gouraud_DualTexture;
	case ePCShader_Lightmap_DualTexture:
		return eShader_Lightmap_DualTexture;
	default:
		return eShader_Invalid;
	}
}

SharedTexture *RenderBlock::LoadSpriteData(CRBSection &cSection, uint32 nTextureIndex, const char *pFilename)
{
	SharedTexture *pResult = nullptr;

	// We do indeed have a sprite, so make us a sprite object
	CRBSection::CSpriteData* pNewSprite;
	pNewSprite = new CRBSection::CSpriteData;

	if (!pNewSprite)
		return NULL;

	

	// Now fill it in appropriately
	uint32 Result = LoadSprite(pFilename, &pNewSprite->m_pSprite);
	if (Result != LT_OK)
	{
		delete pNewSprite;
		return LTNULL;
	}

	// Now initialize our tracker
	spr_InitTracker(&pNewSprite->m_SpriteTracker, pNewSprite->m_pSprite);
	if (pNewSprite->m_SpriteTracker.m_pCurFrame)
		pResult = pNewSprite->m_SpriteTracker.m_pCurFrame->m_pTex;

	// Assign this sprite to the section
	cSection.m_pSpriteData[nTextureIndex] = pNewSprite;

	return pResult;
}


bool RenderBlock::Load(FILE * pFile)
{
	fread(&m_vCenter, sizeof(float) * 3, 1, pFile);
	fread(&m_vHalfDims, sizeof(float) * 3, 1, pFile);

	m_vBoundsMin = m_vCenter - m_vHalfDims;
	m_vBoundsMax = m_vCenter + m_vHalfDims;

	// Read in the section list
	uint32 nSectionCount;
	uint32 nIndexOffset = 0;
	STREAM_READ(nSectionCount);

	if (nSectionCount)
	{
		for (uint32 nSectionLoop = 0; nSectionLoop < nSectionCount; ++nSectionLoop)
		{
			// Read the data
			char sTextureName[CRBSection::kNumTextures][260 + 1];
			uint32 nCurrTex;
			for (nCurrTex = 0; nCurrTex < CRBSection::kNumTextures; nCurrTex++)
			{
				uint16 len;
				STREAM_READ(len);
				fread(sTextureName, len, 1, pFile);
				sTextureName[nCurrTex][len] = 0;
			}
			uint8 nShaderCode;
			STREAM_READ(nShaderCode);
			uint32 nTriCount;
			STREAM_READ(nTriCount);
			assert(nTriCount != 0);

			char sTextureEffect[260 + 1];
			uint16 len;
			STREAM_READ(len);
			fread(sTextureEffect, len, 1, pFile);
			sTextureEffect[len] = 0;

			// Set up the section
			m_aSections.push_back(CRBSection());
			CRBSection &cCurSection = m_aSections[nSectionLoop];
			cCurSection.m_nStartIndex = nIndexOffset;
			cCurSection.m_nTriCount = nTriCount;
			nIndexOffset += nTriCount * 3;

			cCurSection.m_nIndex = nSectionLoop;
			// Determine if the texture is a texture or actually a sprite
			for (nCurrTex = 0; nCurrTex < CRBSection::kNumTextures; nCurrTex++)
			{
				// The texture for this section
				SharedTexture* pSectionTexture = NULL;

				if (IsSpriteTexture(sTextureName[nCurrTex]))
				{
					pSectionTexture = LoadSpriteData(cCurSection, nCurrTex, sTextureName[nCurrTex]);
				}
				else if (sTextureName[nCurrTex][0])
				{
					//pSectionTexture = g_pStruct->GetSharedTexture(sTextureName[nCurrTex]);
					E_DEBUG
				}
				cCurSection.m_pTexture[nCurrTex] = pSectionTexture;

				// Load the texture
				// Don't try to load lightmap textures
				// NYI
				if (cCurSection.m_pTexture[nCurrTex])
				{
					//cCurSection.m_pTexture[nCurrTex]->SetRefCount(cCurSection.m_pTexture[nCurrTex]->GetRefCount() + 1);
					E_DEBUG
				}
			}

			// Translate the shader code
			cCurSection.m_eShader = TranslatePCShaderCode(nShaderCode);

			if ((cCurSection.m_eShader != eShader_Gouraud) &&
				(cCurSection.m_eShader != eShader_Lightmap) &&
				(cCurSection.m_pTexture[0] == NULL))
			{
				printf("Renderer: Error loading texture %s", sTextureName[0]);
			}

			// Handle the dual texture cases
			if (((cCurSection.m_eShader == eShader_Gouraud_DualTexture) || (cCurSection.m_eShader == eShader_Lightmap_DualTexture)) &&
				(cCurSection.m_pTexture[1] == NULL))
			{
				printf("Renderer: Error loading texture %s", sTextureName[1]);
			}

			// Load the lightmap
			STREAM_READ(cCurSection.m_nLightmapWidth);
			STREAM_READ(cCurSection.m_nLightmapHeight);
			STREAM_READ(cCurSection.m_nLightmapSize);

			if (cCurSection.m_nLightmapSize)
			{
				//LT_MEM_TRACK_ALLOC(cCurSection.m_pLightmapData = new uint8[cCurSection.m_nLightmapSize], LT_MEM_TYPE_RENDER_LIGHTMAP);
				cCurSection.m_pLightmapData = new uint8[cCurSection.m_nLightmapSize];
				fread(cCurSection.m_pLightmapData, cCurSection.m_nLightmapSize, 1, pFile);
			}

			// Load the texture effect
			if (sTextureEffect[0] != '\0')
			{
				//cCurSection.m_pTextureEffect = CTextureScriptMgr::GetSingleton().GetInstance(sTextureEffect);
				E_DEBUG
			}
		}


	}

	// Get the vertex data
	STREAM_READ(m_nVertexCount);
	if (m_nVertexCount)
	{
		m_aVertices = new SRBVertex[m_nVertexCount];

		//read in all the vertices in a single read, this makes the structure layout dependant
		//upon the file format, but saves millions of reads.
		fread(m_aVertices, sizeof(SRBVertex) * m_nVertexCount,1,pFile);

	}

	// Get the triangle count
	STREAM_READ(m_nTriCount);
	if (m_nTriCount)
	{
		m_aIndices = new uint16[m_nTriCount * 3];

		// Tracking for the active section vertex span information..
		TSectionList::iterator iCurSection = m_aSections.begin();
		uint32 nSectionLeft = iCurSection->m_nTriCount;
		uint32 nMinVertex = m_nVertexCount;
		uint32 nMaxVertex = 0;
		uint32 nSectionIndex = 0;

		// Read in the indices
		uint32 nIndexOffset = 0;
		for (uint32 nTriLoop = 0; nTriLoop < m_nTriCount; ++nTriLoop)
		{
			uint32 nIndex0, nIndex1, nIndex2;
			fread(&nIndex0, sizeof(uint32), 1, pFile);
			fread(&nIndex1, sizeof(uint32), 1, pFile);
			fread(&nIndex2, sizeof(uint32), 1, pFile);

			assert("Invalid index found" &&
				(nIndex0 < m_nVertexCount) &&
				(nIndex1 < m_nVertexCount) &&
				(nIndex2 < m_nVertexCount));

			m_aIndices[nIndexOffset] = (uint16)nIndex0;
			++nIndexOffset;
			m_aIndices[nIndexOffset] = (uint16)nIndex1;
			++nIndexOffset;
			m_aIndices[nIndexOffset] = (uint16)nIndex2;
			++nIndexOffset;

			// skip the poly index (for now)
			uint32 nPolyIndex;
			STREAM_READ(nPolyIndex);

			// Track the min/max vertex accessing
			nMinVertex = LTMIN(nMinVertex, nIndex0);
			nMinVertex = LTMIN(nMinVertex, nIndex1);
			nMinVertex = LTMIN(nMinVertex, nIndex2);
			nMaxVertex = LTMAX(nMaxVertex, nIndex0);
			nMaxVertex = LTMAX(nMaxVertex, nIndex1);
			nMaxVertex = LTMAX(nMaxVertex, nIndex2);

			--nSectionLeft;
			// Have we completed a section?
			if (!nSectionLeft)
			{
				// Update the current one
				iCurSection->m_nStartVertex = nMinVertex;
				iCurSection->m_nVertexCount = (nMaxVertex - nMinVertex) + 1;
				// Start the next one
				++iCurSection;
				nMinVertex = m_nVertexCount;
				nMaxVertex = 0;
				if (iCurSection != m_aSections.end())
					nSectionLeft = iCurSection->m_nTriCount;
			}
		}
	}

	// Read the sky portals
	uint32 nSkyPortalCount;
	STREAM_READ(nSkyPortalCount);

	m_aSkyPortals.reserve(nSkyPortalCount);
	for (; nSkyPortalCount; --nSkyPortalCount)
	{
		SRBGeometryPoly cPoly;
		uint8 nVertCount;
		STREAM_READ(nVertCount);
		assert("Invalid geometry poly found" && nVertCount > 2);

		cPoly.m_vMin = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
		cPoly.m_vMax = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		cPoly.m_aVerts.reserve(nVertCount);
		for (; nVertCount; --nVertCount)
		{
			glm::vec3 vPos;
			fread(&vPos,sizeof(float)*3,1,pFile);
			cPoly.m_aVerts.push_back(vPos);

			VEC_MIN(cPoly.m_vMin, cPoly.m_vMin, vPos);
			VEC_MAX(cPoly.m_vMax, cPoly.m_vMax, vPos);
		}
		fread(&cPoly.m_cPlane.m_Normal, sizeof(float) * 3, 1, pFile);
		fread(&cPoly.m_cPlane.m_Dist, sizeof(float), 1, pFile);

		cPoly.m_ePlaneCorner = GetAABBPlaneCorner(cPoly.m_cPlane.m_Normal);

		m_aSkyPortals.push_back(cPoly);
	}

	// Read the occluders
	uint32 nOccluderCount;
	STREAM_READ(nOccluderCount);

	m_aOccluders.reserve(nOccluderCount);

	for (; nOccluderCount; --nOccluderCount)
	{
		SRBOccluder cPoly;
		uint8 nVertCount;
		STREAM_READ(nVertCount);
		assert("Invalid geometry poly found" && nVertCount > 2);

		cPoly.m_vMin = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
		cPoly.m_vMax = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		cPoly.m_aVerts.reserve(nVertCount);
		for (; nVertCount; --nVertCount)
		{
			glm::vec3 vPos;
			fread(&vPos, sizeof(float) * 3, 1, pFile);
			cPoly.m_aVerts.push_back(vPos);

			VEC_MIN(cPoly.m_vMin, cPoly.m_vMin, vPos);
			VEC_MAX(cPoly.m_vMax, cPoly.m_vMax, vPos);
		}
		fread(&cPoly.m_cPlane.m_Normal, sizeof(float) * 3, 1, pFile);
		fread(&cPoly.m_cPlane.m_Dist, sizeof(float), 1, pFile);

		cPoly.m_ePlaneCorner = GetAABBPlaneCorner(cPoly.m_cPlane.m_Normal);

		fread(&cPoly.m_nID, sizeof(float), 1, pFile);

		m_aOccluders.push_back(cPoly);
	}

	// Read the light groups
	uint32 nLightGroupCount;
	STREAM_READ(nLightGroupCount);

	m_aLightGroups.reserve(nLightGroupCount);
	
	for (; nLightGroupCount; --nLightGroupCount)
	{
		SRBLightGroup cLightGroup;
		uint16 nLength;
		STREAM_READ(nLength);

		cLightGroup.m_nID = 0;
		for (; nLength; --nLength)
		{
			uint8 nNextChar;
			STREAM_READ(nNextChar);

			cLightGroup.m_nID *= 31;
			cLightGroup.m_nID += (uint32)nNextChar;
		}

		fread(&cLightGroup.m_vColor, sizeof(float) * 3, 1, pFile);

		uint32 nDataLength;
		STREAM_READ(nDataLength);

		cLightGroup.m_aVertexIntensities.resize(nDataLength, 0);
		fread(static_cast<void*>(&(*cLightGroup.m_aVertexIntensities.begin())), nDataLength,1,pFile);

		// Validate the vertex intensity stream here instead of at binding time
		// NYI

		// Read in the section sub-lightmap list
		cLightGroup.ClearSectionLMList();

		uint32 nSectionLMSize;
		STREAM_READ(nSectionLMSize);
		cLightGroup.m_aSectionLMs.resize(nSectionLMSize);
		SRBLightGroup::TSectionLMList::iterator iCurSection = cLightGroup.m_aSectionLMs.begin();
		for (; iCurSection != cLightGroup.m_aSectionLMs.end(); ++iCurSection)
		{
			uint32 nSubLMSize;
			STREAM_READ(nSubLMSize);

			if (!nSubLMSize)
				continue;

			*iCurSection = new SRBLightGroup::TSubLMList;
			(*iCurSection)->resize(nSubLMSize);
			SRBLightGroup::TSubLMList::iterator iCurSubLM = (*iCurSection)->begin();
			for (; iCurSubLM != (*iCurSection)->end(); ++iCurSubLM)
			{
				iCurSubLM->Load(pFile);
			}
		}

		m_aLightGroups.push_back(cLightGroup);
	}

	// Read the child flags
	uint8 nChildFlags = 0;
	STREAM_READ(nChildFlags);


	// Read the children
	uint8 nMask = 1;
	for (uint32 nChildReadLoop = 0; nChildReadLoop < k_NumChildren; ++nChildReadLoop)
	{
		uint32 nIndex;
		STREAM_READ(nIndex);

		if (nChildFlags & nMask)
			m_aChildren[nChildReadLoop] = (reinterpret_cast<RenderBlock*>(nIndex));
		else
			m_aChildren[nChildReadLoop] = (reinterpret_cast<RenderBlock*>(k_InvalidChild));
		nMask <<= 1;
	}

	return true;
}


void RenderBlock::FixupChildren(std::vector<std::unique_ptr<RenderBlock>>& pBase)
{
	for (uint32 nChildLoop = 0; nChildLoop < k_NumChildren; ++nChildLoop)
	{
		uint nIndex = reinterpret_cast<uint>(m_aChildren[nChildLoop]);
		if (nIndex == (uint)k_InvalidChild)
			m_aChildren[nChildLoop] = 0;
		else
			m_aChildren[nChildLoop] = pBase[nIndex].get();
	}
}

bool SRBLightGroup::SSubLM::Load(FILE *pFile)
{
	STREAM_READ(m_nLeft);
	STREAM_READ(m_nTop);
	STREAM_READ(m_nWidth);
	STREAM_READ(m_nHeight);
	uint32 nDataSize;
	STREAM_READ(nDataSize);

	m_aData.resize(nDataSize);
	fread(static_cast<void*>(&(*m_aData.begin())), nDataSize,1,pFile);

	return true;
}

void SRBLightGroup::ClearSectionLMList()
{
	while (!m_aSectionLMs.empty())
	{
		delete m_aSectionLMs.back();
		m_aSectionLMs.pop_back();
	}
}


SRBLightGroup &SRBLightGroup::operator=(const SRBLightGroup &sOther)
{
	if (&sOther == this)
		return *this;

	m_nID = sOther.m_nID;
	m_vColor = sOther.m_vColor;
	m_aVertexIntensities = sOther.m_aVertexIntensities;

	ClearSectionLMList();

	m_aSectionLMs.reserve(sOther.m_aSectionLMs.size());
	TSectionLMList::const_iterator iCurLM = sOther.m_aSectionLMs.begin();
	for (; iCurLM != sOther.m_aSectionLMs.end(); ++iCurLM)
	{
		if (*iCurLM)
		{
			m_aSectionLMs.push_back(new TSubLMList(**iCurLM));
		}
		else
			m_aSectionLMs.push_back(0);
	}
	return *this;
}


SRBLightGroup::~SRBLightGroup()
{
	ClearSectionLMList();
}

SRBLightGroup::SRBLightGroup(const SRBLightGroup &sOther) :
	m_nID(sOther.m_nID),
	m_vColor(sOther.m_vColor),
	m_aVertexIntensities(sOther.m_aVertexIntensities)
{
	m_aSectionLMs.reserve(sOther.m_aSectionLMs.size());
	TSectionLMList::const_iterator iCurLM = sOther.m_aSectionLMs.begin();
	for (; iCurLM != sOther.m_aSectionLMs.end(); ++iCurLM)
	{
		if (*iCurLM)
		{
			m_aSectionLMs.push_back(new TSubLMList(**iCurLM));
		}
		else
			m_aSectionLMs.push_back(0);
	}
}