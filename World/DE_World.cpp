#include "stdafx.h"

uint32 st_GetHashCode(const char *pString)
{
	uint32 nResult = 0;
	for (; *pString; ++pString)
	{
		nResult *= 31;
		nResult += *pString;
	}

	return nResult;
}