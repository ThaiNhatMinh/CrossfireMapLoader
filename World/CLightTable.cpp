#include "stdafx.h"
#include "CLightTable.h"


CLightTable::CLightTable()
{
}


CLightTable::~CLightTable()
{
}

bool CLightTable::Load(FILE * pFile)
{
	fread(&m_vWorldBasePos,sizeof(glm::vec3),1,pFile);
	glm::vec3 vGridSize;
	STREAM_READ(vGridSize);
	STREAM_READ(m_DataDims);
	m_vWorldToLightDataScale.x = 1.0f / vGridSize.x;
	m_vWorldToLightDataScale.y = 1.0f / vGridSize.y;
	m_vWorldToLightDataScale.z = 1.0f / vGridSize.z;

	// Read in the size of the compressed data...
	uint32 iSizeCompressed;
	STREAM_READ(iSizeCompressed);
	uint32 iSize = m_DataDims.x * m_DataDims.y * m_DataDims.z * 3;

	// Read in the data and store decompressed...
	m_pLightData = new uint8[iSize];
	if (!m_pLightData)
		return false;
	if (!Load_RLE_DeCompress(pFile, m_pLightData, iSize))
		return false;

	return true;
}


// Decompresses the data in pCompressed, storing it in pOut...
bool CLightTable::Load_RLE_DeCompress(FILE* pFile, uint8* pOutData, uint32 iUncompSize)
{
	// Index in the output buffer...
	uint8* pCurrOut = pOutData;
	// Run through the input buffer...
	while (pCurrOut < pOutData + iUncompSize)
	{
		// Read in the tag...
		uint8 nTag; fread(&nTag, sizeof(uint8),1,pFile);
		// Is a run or a span....
		bool bIsRun = (nTag & 0x80) ? true : false;
		uint32 nRunLen = (uint32)(nTag & 0x7F) + 1;
		if (bIsRun)
		{
			uint8 pRunData[3];
			// Read the color...
			fread(pRunData, sizeof(uint8) * 3,1,pFile);
			for (uint32 nCurrPel = 0; nCurrPel < nRunLen; ++nCurrPel)
			{
				pCurrOut[0] = pRunData[0];
				pCurrOut[1] = pRunData[1];
				pCurrOut[2] = pRunData[2];
				// Update the output position...
				pCurrOut += 3;
			}
		}
		else
		{
			fread(pCurrOut, sizeof(uint8) * 3 * nRunLen,1,pFile);
			pCurrOut += 3 * nRunLen;
		}
	}

	// Double check..
	assert(pCurrOut == pOutData + iUncompSize);
	return true;
}
