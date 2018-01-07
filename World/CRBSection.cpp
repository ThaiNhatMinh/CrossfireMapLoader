#include "stdafx.h"
#include "CRBSection.h"


CRBSection::CRBSection():
m_nStartIndex(0),
m_nTriCount(0),
m_nStartVertex(0),
m_nVertexCount(0),
m_nLightmapWidth(0),
m_nLightmapHeight(0),
m_nLightmapSize(0),
m_pLightmapData(NULL),
m_pTextureEffect(NULL),
m_eShader(eShader_Invalid),
m_eBoundShader(eShader_Invalid)
{

	for (uint32 nCurrTex = 0; nCurrTex < kNumTextures; nCurrTex++)
	{
		m_pTexture[nCurrTex] = NULL;
		m_pSpriteData[nCurrTex] = NULL;
	}
}


CRBSection::~CRBSection()
{
	// Dereference our textures...
	for (uint32 nCurrTex = 0; nCurrTex < kNumTextures; nCurrTex++)
	{
		if (m_pTexture[nCurrTex])
		{
			//m_pTexture[nCurrTex]->SetRefCount(m_pTexture[nCurrTex]->GetRefCount() - 1);
			m_pTexture[nCurrTex] = 0;
		}
	}

	// Release our reference to the texture effect
	if (m_pTextureEffect)
	{
		//CTextureScriptMgr::GetSingleton().ReleaseInstance(m_pTextureEffect);
	}

	// Release our sprite information if applicable
	for (auto nCurrTex = 0; nCurrTex < kNumTextures; nCurrTex++)
		delete m_pSpriteData[nCurrTex];

	// Release the lightmap data
	if (m_pLightmapData)
	{
		delete[] m_pLightmapData;
		m_pLightmapData = 0;
	}
}


CRBSection::CRBSection(const CRBSection &cOther) :
	m_nStartIndex(cOther.m_nStartIndex),
	m_nTriCount(cOther.m_nTriCount),
	m_nStartVertex(cOther.m_nStartVertex),
	m_nVertexCount(cOther.m_nVertexCount),
	m_nLightmapWidth(cOther.m_nLightmapWidth),
	m_nLightmapHeight(cOther.m_nLightmapHeight),
	m_nLightmapSize(cOther.m_nLightmapSize),
	m_pLightmapData(0),
	m_pTextureEffect(NULL),
	m_eShader(cOther.m_eShader),
	m_eBoundShader(cOther.m_eBoundShader)
{
	for (uint32 nCurrTex = 0; nCurrTex < kNumTextures; nCurrTex++)
	{
		m_pTexture[nCurrTex] = cOther.m_pTexture[nCurrTex];

		if (m_pTexture[nCurrTex])
		{
			//m_pTexture[nCurrTex]->SetRefCount(m_pTexture[nCurrTex]->GetRefCount() + 1);
		}
		// Copy over the sprite object if necessary
		if (cOther.m_pSpriteData[nCurrTex])
		{
			// Make us our own sprite object that is a copy of the other's
			//LT_MEM_TRACK_ALLOC(m_pSpriteData[nCurrTex] = new CSpriteData(*cOther.m_pSpriteData[nCurrTex]), LT_MEM_TYPE_SPRITE);
			m_pSpriteData[nCurrTex] = new CSpriteData(*cOther.m_pSpriteData[nCurrTex]);
		}
		else
			m_pSpriteData[nCurrTex] = NULL;
	}

	// Copy over the texture effect from the other object and add our reference
	m_pTextureEffect = cOther.m_pTextureEffect;
	if (m_pTextureEffect)
	{
		//m_pTextureEffect->AddRef();
	}
	if (m_nLightmapSize)
	{
		//LT_MEM_TRACK_ALLOC(m_pLightmapData = new uint8[m_nLightmapSize], LT_MEM_TYPE_RENDER_LIGHTMAP);
		m_pLightmapData = new uint8[m_nLightmapSize];
		memcpy(m_pLightmapData, cOther.m_pLightmapData, m_nLightmapSize);
	}
}

CRBSection &CRBSection::operator=(const CRBSection &cOther)
{
	if (&cOther == this)
		return *this;

	for (uint32 nCurrTex = 0; nCurrTex < kNumTextures; nCurrTex++)
	{
		if (m_pTexture[nCurrTex])
		{
			//m_pTexture[nCurrTex]->SetRefCount(m_pTexture[nCurrTex]->GetRefCount() - 1);
		}
		m_pTexture[nCurrTex] = cOther.m_pTexture[nCurrTex];
		if (m_pTexture[nCurrTex])
		{
			//m_pTexture[nCurrTex]->SetRefCount(m_pTexture[nCurrTex]->GetRefCount() + 1);
		}
		// Free our current sprite object
		delete m_pSpriteData[nCurrTex];
		m_pSpriteData[nCurrTex] = NULL;

		// Copy over the other object's
		if (cOther.m_pSpriteData[nCurrTex])
		{
			// Make us our own sprite object that is a copy of the other's
			//LT_MEM_TRACK_ALLOC(m_pSpriteData[nCurrTex] = new CSpriteData(*cOther.m_pSpriteData[nCurrTex]), LT_MEM_TYPE_SPRITE);
			m_pSpriteData[nCurrTex] = new CSpriteData(*cOther.m_pSpriteData[nCurrTex]);
		}
	}

	m_nStartIndex = cOther.m_nStartIndex;
	m_nTriCount = cOther.m_nTriCount;
	m_nStartVertex = cOther.m_nStartVertex;
	m_nVertexCount = cOther.m_nVertexCount;
	delete[] m_pLightmapData;
	m_nLightmapWidth = cOther.m_nLightmapWidth;
	m_nLightmapHeight = cOther.m_nLightmapHeight;
	m_nLightmapSize = cOther.m_nLightmapSize;
	m_eShader = cOther.m_eShader;
	m_eBoundShader = cOther.m_eBoundShader;

	// Copy over the texture instance, freeing ours if we have one
	if (m_pTextureEffect)
	{
		//CTextureScriptMgr::GetSingleton().ReleaseInstance(m_pTextureEffect);
	}
	m_pTextureEffect = cOther.m_pTextureEffect;
	if (m_pTextureEffect)
	{
		//m_pTextureEffect->AddRef();
	}

	if (m_nLightmapSize)
	{
		//LT_MEM_TRACK_ALLOC(m_pLightmapData = new uint8[m_nLightmapSize], LT_MEM_TYPE_RENDER_LIGHTMAP);
		m_pLightmapData = new uint8[m_nLightmapSize];
		memcpy(m_pLightmapData, cOther.m_pLightmapData, m_nLightmapSize);
	}
	return *this;
}