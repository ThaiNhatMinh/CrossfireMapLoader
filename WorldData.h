#pragma once
class WorldData
{
public:
	WorldData();
	~WorldData();

	void Clear();
	void Term();

	//call this term on a worldmodel that is an inherited copy from another worldmodel.
	void TermInherited();

	inline void SetValidBsp() {
		m_pValidBsp = m_pWorldBsp ? m_pWorldBsp : m_pOriginalBsp;
	}

	//gives our data to the given other WorldData
	bool InheritTo(WorldData *dest_model);

public:

	// Combination of WD_ flags.
	uint32          m_Flags;

	// This version is a secondary version that has transformed vertex positions
	// (for object->world collisions).
	// NOTE: this is NULL for non-moving world models.
	WorldBSP        *m_pWorldBsp;

	// This points to m_pWorldBsp unless it's null, otherwise it points to m_pOriginalBsp.
	WorldBSP        *m_pValidBsp;

private:
	// Unmodified version.  This is always valid.
	WorldBSP        *m_pOriginalBsp;


public:
	//accessors.
	const WorldBSP *OriginalBSP() const { return m_pOriginalBsp; }
	WorldBSP *OriginalBSP() { return m_pOriginalBsp; }

	// Set the original bsp
	void SetOriginalBSP(WorldBSP *bsp) { m_pOriginalBsp = bsp; }
	// Delete the original alloced bsp
	void DeleteOriginalBsp();
};

