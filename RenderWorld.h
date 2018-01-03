#pragma once
class RenderWorld
{
private:
	uint32 m_nRenderBlockCount;
	std::vector<std::unique_ptr<RenderBlock>> m_pRenderBlocks;

	bool m_bBlocksDirty;

	// The world model containment structure
	// Note : This is not a hash_map because I'm assuming that the worldmodel
	// key (which is its name) is going to be hashed to a unique value.
	typedef std::map<uint32, RenderWorld *> TWorldModelMap;
	TWorldModelMap m_aWorldModels;

public:
	RenderWorld();
	~RenderWorld();
	bool Load(FILE* pFile);

	uint32 GetWMNameHash(const char *pName);
};

