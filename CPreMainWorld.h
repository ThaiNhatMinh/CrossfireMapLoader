#pragma once


// This is the container for all the world models in a world.
class CMainWorld
{
public:
	char m_pInfoString[256];
	// Box enclosing all geometry.
	glm::vec3 m_PosMin;
	glm::vec3 m_PosMax;
	glm::vec3 m_vWorldOffset;
	// No objects are stored in here, but this is created and maintained 
	// so at runtime it knows how to create the WorldTree.
	WorldTree m_WorldTree;
	// Terrain and WorldModel BSPs.
	std::vector<std::unique_ptr<WorldData>>			m_WorldModels;

	//Light table..
	CLightTable light_table;
	//list of our StaticLight objects.
	std::vector<StaticLight> static_light_list;
	CMainWorld();
	~CMainWorld();

	bool Load(FILE* pFIle);

	//Calculate bounding spheres for polies and leaves of the given worldmodels.
	void CalcBoundingSpheres();
	void InsertStaticLights(WorldTree &world_tree);
	void LoadLightGrid(FILE *pStream);
	void AddStaticLights(FILE* pStream);

	void Term();
	bool LoadRenderData(FILE* pFile);


};

