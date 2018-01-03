#include "stdafx.h"
#include "CPreMainWorld.h"


CMainWorld::CMainWorld()
{
}


CMainWorld::~CMainWorld()
{
}
ELoadWorldStatus g_iWorldBlockerData(FILE* pFile)
{
	uint32 nNumPolys;
	STREAM_READ(nNumPolys);

	for (size_t i = 0; i<nNumPolys; i++)
	{
		float dump[4];
		fread(dump, sizeof(float)*4, 1, pFile);
		uint32 nVertCount = 0;
		STREAM_READ(nVertCount);
		for (size_t j = 0; j < nVertCount; j++)
		{
			glm::vec3 v;
			fread(&v, sizeof(float) * 3, 1, pFile);
			printf(" ");
		}
	}

	// Dummy field for future expansion
	uint32 nDummy;
	STREAM_READ(nDummy);
	assert(nDummy == 0);
	return LoadWorld_Ok;
};

bool CMainWorld::Load(FILE * pFile)
{

	uint32 version;
	uint32 objectListMarker;
	uint32 blindDataMarker;
	uint32 lightGridMarker;
	uint32 physicsDataMarker;
	uint32 particleDataMarker;
	uint32 renderDataMarker;
	// Write a small header.
	fread(&version, sizeof(uint32), 1, pFile);
	fread(&objectListMarker, sizeof(uint32), 1, pFile);
	fread(&blindDataMarker, sizeof(uint32), 1, pFile);
	fread(&lightGridMarker, sizeof(uint32), 1, pFile);
	fread(&physicsDataMarker, sizeof(uint32), 1, pFile);
	fread(&particleDataMarker, sizeof(uint32), 1, pFile);
	fread(&renderDataMarker, sizeof(uint32), 1, pFile);

	uint32 temp[8];
	fread(temp, sizeof(uint32), 8, pFile);

	uint32 len;
	fread(&len, sizeof(uint32), 1, pFile);
	fread(m_pInfoString, len, 1, pFile);

	// Write the position box.

	fread(&m_PosMin, sizeof(float) * 3, 1, pFile);
	fread(&m_PosMax, sizeof(float) * 3, 1, pFile);

	//save out the offset from this world to the source world
	
	fread(&m_vWorldOffset, sizeof(float) * 3, 1, pFile);




	m_WorldTree.Load(pFile);

	uint32 m_WorldModelsGetSize;
	fread(&m_WorldModelsGetSize, sizeof(uint32), 1, pFile);
	for (size_t i = 0; i < m_WorldModelsGetSize; i++)
	{
		m_WorldModels.push_back(std::make_unique<WorldData>());

		uint32 temp;
		//dummy 4 bytes
		fread(&temp, sizeof(uint32), 1, pFile);

		//get the starting position of this worldmodel.
		uint32 start_position;
		start_position = ftell(pFile);

		WorldBSP *loaded_bsp = new WorldBSP;
		if (loaded_bsp->Load(pFile,true) != LoadWorld_Ok)
		{
			//delete the bsp we allocated.
			delete loaded_bsp;

			//return the error status.
			return false;
			
		}

		//put the loaded bsp into the worldmodel.
		m_WorldModels[i]->SetOriginalBSP(loaded_bsp);

		//Check if we should read in a second time to create the transformed version.
		if (m_WorldModels[i]->OriginalBSP()->m_WorldInfoFlags & WIF_MOVEABLE)
		{
			//reset the stream position.
			fseek(pFile,start_position,SEEK_SET);

			//allocate another bsp.
			loaded_bsp = new WorldBSP;

			//make sure we had memory.
			if (loaded_bsp == NULL)
			{
				//no memory.
				return LoadWorld_Error;
			}

			//load the bsp.
			ELoadWorldStatus loadbsp_status = loaded_bsp->Load(pFile, false);

			//check if we loaded it correctly.
			if (loadbsp_status != LoadWorld_Ok) {
				//delete the bsp
				delete loaded_bsp;

				//return the error status.
				return loadbsp_status;
			}

			// Remember that we've got a movable BSP
			m_WorldModels[i]->m_pWorldBsp = loaded_bsp;
		}

		//set the flags to say the bsps were allocated.
		m_WorldModels[i]->m_Flags |= WD_ORIGINALBSPALLOCED | WD_WORLDBSPALLOCED;

		//set the validbsp pointer.
		m_WorldModels[i]->SetValidBsp();

		//set the worldmodel original bsp index.
		m_WorldModels[i]->OriginalBSP()->m_Index = (uint16)i;

		//set the worldmodel world bsp index.
		if (m_WorldModels[i]->m_pWorldBsp) {
			m_WorldModels[i]->m_pWorldBsp->m_Index = (uint16)i;
		}
	}

	//
	//Precalculate stuff.
	//

	//get our ambient light from the info string.
	glm::vec3 ambient_light;
	//ParseAmbientLight(world_info_string, &ambient_light);

	//Figure out world model leaf spheres..
	CalcBoundingSpheres();

	//Gen our list of static lights...
	AddStaticLights(pFile);

	//insert the static light objects into the given world tree.
	InsertStaticLights(m_WorldTree);

	// Read in the lightgrid...
	fseek(pFile, lightGridMarker,SEEK_SET);
	LoadLightGrid(pFile);

	// Read the blocker data
	fseek(pFile, physicsDataMarker, SEEK_SET);
	//assert();



	

	ELoadWorldStatus eResult = g_iWorldBlockerData(pFile);

	if (eResult != LoadWorld_Ok)
	{
		Term();
		return eResult;
	}

	// Read the particle blocker data
	fseek(pFile, particleDataMarker, SEEK_SET);
	auto g_iWorldParticleBlockerData = [](FILE* pFile)
	{
		// read in the number of blocker polys
		uint32 numPolys;
		STREAM_READ(numPolys);
		for (uint32 i = 0; i < numPolys; i++)
		{

			float dump;
			fread(&dump, sizeof(float), 4, pFile);
			uint32 m_NumVerts;
			STREAM_READ(m_NumVerts);
			// load the blocker verts
			for (uint32 i = 0; i < m_NumVerts; i++)
			{
				fread(&dump, sizeof(float) * 3,1, pFile);

				// build the bounds
				if (i == 0)
				{
					//m_MinBounds = m_MaxBounds = m_Verts[0];
				}
				else
				{
					/*if (m_Verts[i].x < m_MinBounds.x)
						m_MinBounds.x = m_Verts[i].x;
					else if (m_Verts[i].x > m_MaxBounds.x)
						m_MaxBounds.x = m_Verts[i].x;
					if (m_Verts[i].y < m_MinBounds.y)
						m_MinBounds.y = m_Verts[i].y;
					else if (m_Verts[i].y > m_MaxBounds.y)
						m_MaxBounds.y = m_Verts[i].y;
					if (m_Verts[i].z < m_MinBounds.z)
						m_MinBounds.z = m_Verts[i].z;
					else if (m_Verts[i].z > m_MaxBounds.z)
						m_MaxBounds.z = m_Verts[i].z;
						*/
				}
			}
		}

		// read in the dummy field
		uint32 dummy;
		STREAM_READ(dummy);
		assert(dummy == 0);
		return LoadWorld_Ok;
	};
	eResult = g_iWorldParticleBlockerData(pFile);
	if (eResult != LoadWorld_Ok)
	{
		Term();
		return eResult;
	}

	//////////////////////////////////////////////////////////
	//read in the rendering data
	fseek(pFile, renderDataMarker, SEEK_SET);
	if (!LoadRenderData(pFile))
	{
		Term();
		return LoadWorld_Error;
	}


	//loaded everything ok.
	return LoadWorld_Ok;

}

void CMainWorld::CalcBoundingSpheres() {
	//go through all the worldmodels.
	for (uint32 i = 0; i < m_WorldModels.size(); i++) {
		WorldData *pWorldModel = m_WorldModels[i].get();

		if (pWorldModel->OriginalBSP()) {
			pWorldModel->OriginalBSP()->CalcBoundingSpheres();
		}

		if (pWorldModel->m_pWorldBsp) {
			pWorldModel->m_pWorldBsp->CalcBoundingSpheres();
		}
	}
}


// Creates the light table for a WorldBsp.
void CMainWorld::LoadLightGrid(FILE* pFile)
{
	// Just load the sucka up...
	if (!light_table.Load(pFile)) return;

	//increment world memory usage total.
	//g_WorldGeometryMemory += light_table.GetMemAllocSize();
}

// Goes thru the light objects in the world file and add all the static lights...
void CMainWorld::AddStaticLights(FILE *pFile) {
	//flag to know when to use fast light object option.
	//the way this is set up, any light object that doesnt supply the
	//"FastLightObjects" property will use this default or the value
	//of the last light object that did specify this.  It is unknown if
	//this is the proper behaviour.
	bool do_fast_light_objects = true;

	//read the number of objects.
	uint32 num_objects = 0;
	STREAM_READ(num_objects);

	//read each object.
	for (uint32 i = 0; i < num_objects; i++)
	{
		//read the length of the object data.
		uint16 object_data_len = 0;
		STREAM_READ(object_data_len);

		//remember the start of the object's data.
		uint32 object_data_start_pos;
		object_data_start_pos = ftell(pFile);

		//read the object type string.
		char object_type[256]; uint16 len;
		STREAM_READ(len);
		fread(object_type, len, 1, pFile);

		object_type[len] = 0;

		//check if this object is a light object.
		if (strcmp(object_type, "Light") == 0 || strcmp(object_type, "DirLight") == 0 ||strcmp(object_type, "ObjectLight") == 0)
		{
			//
			//This is a light object.
			//

			//the color of the light.
			glm::vec3 light_color;

			//the radius of the light.
			float light_radius = 0.0f;

			//assume we light objects.
			bool do_light_objects = true;
			bool light_castshadow = true;

			//light direction.
			glm::vec3 light_dir;

			//light fov.
			float light_fov = -1.0f;

			// brightscale
			float brightscale = 1.0f;

			//amount to convert to ambient
			float fConvertToAmbient = 0.0f;

			//the amount we need to scale this light for objects
			float fObjectBrightScale = 1.0f;

			//the ID of the lightgroup that this light belongs to
			uint32 nLightGroupID = 0;

			//light attenuation...
			glm::vec3 light_attcoefs(1.0f, 0.0f, 19.0f), light_exp(0.0f, -1.0f, -2.0f);
			ELightAttenuationType light_attenuation;

			//the position of the light.
			glm::vec3 light_position;

			//read the number of properties for this object.
			uint32 num_properties;
			STREAM_READ(num_properties);

			//go through all the properties.
			for (uint32 prop_index = 0; prop_index < num_properties; prop_index++)
			{
				//read the property name.
				char property_name[256]; uint16 len;
				STREAM_READ(len);
				fread(property_name, len, 1, pFile);
				property_name[len] = 0;
				printf("Name: %s\n", property_name);
				//read property code (type)
				uint8 prop_code;
				STREAM_READ(prop_code);

				//property flags, not used, but need to skip past it.
				uint32 prop_flags;
				STREAM_READ(prop_flags)

					//property data length.
					uint16 prop_data_len;
				STREAM_READ(prop_data_len);

				//buffer for the property data 
				char property_data[256];

				//check
				if (prop_data_len > sizeof(property_name))
				{
					fseek(pFile,object_data_start_pos + object_data_len,SEEK_SET);
				}
				else
				{
					fread(property_data, prop_data_len,1,pFile);
				}

				if (prop_code == PT_REAL && strcmp(property_name, "LightRadius") == 0)
				{
					light_radius = *((float*)property_data);
				}
				else if (prop_code == PT_REAL && strcmp(property_name, "ConvertToAmbient") == 0)
				{
					fConvertToAmbient = *((float*)property_data);
				}
				else if (prop_code == PT_COLOR && (strcmp(property_name, "LightColor") == 0 || strcmp(property_name, "InnerColor") == 0))
				{
					light_color.x = ((glm::vec3*)property_data)->x;
					light_color.y = ((glm::vec3*)property_data)->y;
					light_color.z = ((glm::vec3*)property_data)->z;
				}
				else if (prop_code == PT_REAL && strcmp(property_name, "BrightScale") == 0)
				{
					brightscale = *((float*)property_data);
				}
				else if (prop_code == PT_REAL && strcmp(property_name, "ObjectBrightScale") == 0)
				{
					fObjectBrightScale = *((float*)property_data);
				}
				else if (prop_code == PT_VECTOR && strcmp(property_name, "AttCoefs") == 0)
				{
					light_attcoefs.x = ((glm::vec3*)property_data)->x;
					light_attcoefs.y = ((glm::vec3*)property_data)->y;
					light_attcoefs.z = ((glm::vec3*)property_data)->z;
				}
				else if (prop_code == PT_VECTOR && strcmp(property_name, "AttExps") == 0)
				{
					light_exp.x = ((glm::vec3*)property_data)->x;
					light_exp.y = ((glm::vec3*)property_data)->y;
					light_exp.z = ((glm::vec3*)property_data)->z;
				}
				else if (prop_code == PT_VECTOR && strcmp(property_name, "Pos") == 0)
				{
					light_position.x = ((glm::vec3*)property_data)->x;
					light_position.y = ((glm::vec3*)property_data)->y;
					light_position.z = ((glm::vec3*)property_data)->z;
				}
				else if (prop_code == PT_BOOL && strcmp(property_name, "LightObjects") == 0)
				{
					do_light_objects = *((char*)property_data) != 0;
				}
				else if (prop_code == PT_BOOL && strcmp(property_name, "FastLightObjects") == 0)
				{
					do_fast_light_objects = *((char*)property_data) != 0;
				}
				else if (prop_code == PT_BOOL && strcmp(property_name, "CastShadows") == 0)
				{
					light_castshadow = *((char*)property_data) != 0;
				}
				else if (prop_code == PT_ROTATION && strcmp(property_name, "Rotation") == 0)
				{
					glm::vec3 vRight, vUp, vForward;
					//glm::vec3 v = (glm::vec3*)property_data;
					//glm::mat3 as = glm::orientate3();
					
					//light_dir = as[0];
					printf("DEGUB_LIGHT\n");
					light_dir = glm::vec3(0, 1, 0);
				}
				else if (prop_code == PT_REAL && strcmp(property_name, "FOV") == 0)
				{
					light_fov = (float)cos(*((float*)property_data) * MATH_PI / 360.0f);
				}
				else if (prop_code == PT_STRING && strcmp(property_name, "LightGroup") == 0)
				{
					//get the name of the light group...
					uint16 nStrLen = *((uint16*)property_data);
					char* pszData = property_data + sizeof(uint16);
					pszData[nStrLen] = '\0';

					//ok, now figure out the ID of the light group, this is ugly, but there is
					//no access to the client or server here, so we need to just do the same that
					//the other ID stuff is generating
					nLightGroupID = st_GetHashCode(pszData);
					//system("pause");
				}
				else if (prop_code == PT_STRING && _stricmp(property_name, "Attenuation") == 0)
				{
					//grab the string length
					uint16 nStrLen = *((uint16*)property_data);
					char* pszData = property_data + sizeof(uint16);

					//null terminate it
					pszData[nStrLen] = '\0';

					if (_stricmp(pszData, "D3D") == 0)
						light_attenuation = eAttenuation_D3D;
					else if (_stricmp(pszData, "Quartic") == 0)
						light_attenuation = eAttenuation_Quartic;
					else if (_stricmp(pszData, "Linear") == 0)
						light_attenuation = eAttenuation_Linear;
				}

			}

			// Light the sample points up.
			if (do_light_objects)
			{
				// Factor the brightness into the light color, including the object lighting
				// since these static lights are only used for object lighting
				light_color *= brightscale * fObjectBrightScale;

				// Calc lighting attenuation co-effs...
				light_attcoefs.x = light_attcoefs.x * powf(light_radius, light_exp.x);
				light_attcoefs.y = light_attcoefs.y * powf(light_radius, light_exp.y);
				light_attcoefs.z = light_attcoefs.z * powf(light_radius, light_exp.z);

				if (!do_fast_light_objects)
				{
					//create a static light object.
					StaticLight element;

					{
						//get the light itself.
						StaticLight &light = element;

						//fill in all the data we have read in.
						light.m_Color = light_color;
						light.m_AttCoefs = light_attcoefs;
						light.m_Pos = light_position;
						light.m_Radius = light_radius;
						light.m_Dir = light_dir;
						light.m_FOV = light_fov;
						light.m_Flags = (light_castshadow ? FLAG_CASTSHADOWS : 0);
						light.m_eAttenuation = light_attenuation;
						light.m_fConvertToAmbient = fConvertToAmbient;
						light.m_nLightGroupID = nLightGroupID;

						//update the bounding box
						light.UpdateBBox(light.m_Pos, glm::vec3(light.m_Radius, light.m_Radius, light.m_Radius));

						
						//insert the light into our list.
						static_light_list.push_back(element);
					}
				}
			}
		}
		else
		{
			fseek(pFile,object_data_start_pos + object_data_len,SEEK_SET);
		}
	}
}

void CMainWorld::Term()
{
}

bool CMainWorld::LoadRenderData(FILE * pFile)
{
	return m_WorldRenderer.Load(pFile);
}

void CMainWorld::InsertStaticLights(WorldTree &world_tree) {
	//get first element of the list.
	auto itr = static_light_list.begin();

	//loop over all the elements.
	while (itr!= static_light_list.end()) {
		//get the light.
		StaticLight &light = *itr;

		//insert the light into the tree.
		world_tree.InsertObject(&light, NOA_Lights);

		//go to next element
		itr++;
	}
}