#pragma once
class LTPlane
{
public:
	/*!
	unit normal
	*/
	glm::vec3 m_Normal;

	/*!
	negative of the signed distance from
	the origin \f$ {\bf O} = (0,0,0)\f$
	to the plane
	*/
	float m_Dist;
public:
	LTPlane();
	~LTPlane();
};

