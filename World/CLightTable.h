#pragma once
class CLightTable
{
public:
	CLightTable();
	~CLightTable();

	bool Load(FILE* pFile);
	bool Load_RLE_DeCompress(FILE* pStream, uint8* pOutData, uint32 iUncompSize);
	// UnCompressed data...
	uint8*		m_pLightData;
	// Dimensions of the light grid...
	glm::vec3 m_DataDims;

	// Base position (in world co-ords of the table)...
	glm::vec3	m_vWorldBasePos;
	// Use to convert from world co-ords to light data position...
	glm::vec3	m_vWorldToLightDataScale;

};

