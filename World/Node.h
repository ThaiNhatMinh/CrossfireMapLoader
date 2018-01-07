#pragma once

class WorldPoly;
class LTPlane;
class Node
{
public:
	Node() { Init(); }
	~Node();
	Node(uint8 nFlags) { Init(); m_Flags = nFlags; }

	void Init()
	{
		m_Flags = 0;
		m_PlaneType = 0;
		m_pPoly = NULL;
		m_Sides[0] = NULL;
		m_Sides[1] = NULL;
	}

	const LTPlane*		GetPlane() const { return m_pPoly->GetPlane(); }
	const WorldPoly*	GetPoly() const { return m_pPoly; }
	const Node*			GetNode(uint32 nSide) const { assert(nSide < 2); return m_Sides[nSide]; }
	uint8				GetFlags() const { return m_Flags; }
	uint8				GetPlaneType() const { return m_PlaneType; }
	void				SetPlaneType(uint8 nType) { m_PlaneType = nType; }

public:

	WorldPoly			*m_pPoly;
	Node				*m_Sides[2];

	uint8				m_Flags;
	uint8				m_PlaneType;
};

