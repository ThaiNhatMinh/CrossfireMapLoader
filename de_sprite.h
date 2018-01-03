#pragma once


class SharedTexture;

struct SpriteEntry
{
	SharedTexture *m_pTex;
};


struct SpriteAnim
{
	char            m_sName[32];
	SpriteEntry     *m_Frames;
	uint32          m_nFrames;
	uint32          m_MsAnimLength;
	uint32          m_MsFrameRate;
	bool          m_bKeyed;
	uint32          m_ColourKey;
	bool          m_bTranslucent;
};


struct Sprite
{
	Sprite()
	{
		m_bTagged = false;
		m_Link.TieOff();
	}

	LTLink			m_Link;
	SpriteAnim		*m_Anims;
	uint32			m_nAnims;
	bool			m_bTagged;
};


// Tracks sprite frames.
struct SpriteTracker
{
	Sprite          *m_pSprite;
	SpriteAnim      *m_pCurAnim;
	SpriteEntry     *m_pCurFrame;
	uint32          m_MsCurTime;
	uint32          m_Flags;
};