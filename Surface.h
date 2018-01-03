#pragma once
class SharedTexture;
class Surface
{
public:

	Surface() { Init(); }
	~Surface();
	void Init()
	{
		m_pTexture = NULL;
		m_Flags = 0;
		m_TextureFlags = 0;
		m_iTexture = 0;
	}

	uint32			GetFlags() const { return m_Flags; }
	SharedTexture*	GetSharedTexture() const { return m_pTexture; }

public:

	SharedTexture   *m_pTexture;		// The texture.    
	uint32			m_Flags;			// Flags on this surface.  
	uint16			m_TextureFlags;		// Texture flags from the DTX file.
	uint16			m_iTexture;			// Index into the world's texture list.
};

