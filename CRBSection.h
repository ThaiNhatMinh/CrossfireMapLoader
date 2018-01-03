#pragma once


// For now, the default shader list is explicitly known
// Note : The light animation code currently only updates vertices of shaders < eShader_Lightmap
enum ERenderShader {
	eShader_Gouraud = 0,
	eShader_Gouraud_Texture,
	eShader_Gouraud_Detail,
	eShader_Gouraud_EnvMap,
	eShader_Gouraud_Alpha_EnvMap,
	eShader_Gouraud_EnvBumpMap,
	eShader_Gouraud_EnvBumpMap_NoFallback,
	eShader_Gouraud_Texture_Fullbright,
	eShader_Gouraud_Detail_Fullbright,
	eShader_Gouraud_EnvMap_Fullbright,
	eShader_Gouraud_DualTexture,
	eShader_Gouraud_DOT3BumpMap,
	eShader_Gouraud_DOT3EnvBumpMap,
	eShader_Gouraud_Effect,
	eShader_Lightmap,
	eShader_Lightmap_Texture,
	eShader_Lightmap_Texture_Detail,
	eShader_Lightmap_Texture_EnvMap,
	eShader_Lightmap_Texture_EnvBumpMap,
	eShader_Lightmap_Texture_EnvBumpMap_NoFallback,
	eShader_Lightmap_DualTexture,
	eShader_Lightmap_Texture_DOT3BumpMap,
	eShader_Lightmap_Texture_DOT3EnvBumpMap,
	k_eShader_Num, // Number of valid shaders
	eShader_Invalid, // Invalid shader ID
	k_eShader_Force_uint32 = 0xFFFFFFFF // Force to 32-bit storage
};

class CTextureScriptInstance;

class CRBSection
{
public:
	enum { kNumTextures = 2 };

	SharedTexture			*m_pTexture[kNumTextures];
	CTextureScriptInstance	*m_pTextureEffect;
	ERenderShader			m_eShader;
	ERenderShader			m_eBoundShader;
	uint32					m_nIndex; // Section index in the render block
	uint32					m_nStartIndex;
	uint32					m_nTriCount;
	uint32					m_nStartVertex;
	uint32					m_nVertexCount;
	uint32					m_nLightmapSize;
	uint32					m_nLightmapWidth, m_nLightmapHeight;
	uint8					*m_pLightmapData;

	//class that holds information about a sprite for the render block
	class CSpriteData
	{
	public:
		//the tracker of the sprite to maintain the current frame
		SpriteTracker	m_SpriteTracker;

		//the actual sprite being used. Note that this should not be deleted since
		//the actual sprite object resides in the global file data
		Sprite			*m_pSprite;
	};

	//the sprite data for this section, or NULL if none is associated with it
	CSpriteData				*m_pSpriteData[kNumTextures];

public:
	

	CRBSection();
	~CRBSection();
	CRBSection(const CRBSection &cOther);
	CRBSection &operator=(const CRBSection &cOther);
};

