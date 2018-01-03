// MapLoader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"




int main()
{
	FILE* pFile = fopen("WORLD//RUNMAP.DAT", "rb");
	if (pFile == nullptr)
	{
		printf("Can't open file");
		return 1;
	}

	
	CMainWorld World;
	World.Load(pFile);

    return 0;
}

