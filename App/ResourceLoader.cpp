#include "stdafx.h"
#include "..\include\IL\il.h"
#include "..\include\IL\ilu.h"
//#include "LTBFileLoader.h"



namespace LightEngine
{
	const char* RESOURCES_FILE = "Resources.xml";
}

//vector<Texture*> Resources::m_Textures;
//vector<ModelCache*> Resources::m_ModelCaches;

#pragma region CheckLoadedResource



Texture * Resources::HasTexture(const char * filename)
{
	for (size_t i = 0; i < m_Textures.size(); i++)
		if (!strcmp(m_Textures[i]->szName, filename))
			return m_Textures[i].get();
	
	return nullptr;
}

ModelCache * Resources::HasModel(const char * filename)
{
	for (size_t i = 0; i < m_ModelCaches.size(); i++)
		if (!strcmp(m_ModelCaches[i]->szName, filename))
			return m_ModelCaches[i].get();

	return NULL;
}

#pragma endregion

Resources::Resources()
{
	m_ShaderFactory.insert(std::make_pair("MapShader", [](const char*vs, const char* fs) {return new MapShader(vs, fs); }));
	m_ShaderFactory.insert(std::make_pair("ImGuiShader", [](const char*vs, const char* fs) {return new ImGuiShader(vs, fs); }));
	m_ShaderFactory.insert(std::make_pair("Debug", [](const char*vs, const char* fs) {return new DebugShader(vs, fs); }));
	
}


Resources::~Resources()
{
	
}

void Resources::Init(Context* c)
{
	ilInit();
	ILenum Error;
	Error = ilGetError();

	if (Error != IL_NO_ERROR)
		Log::Message(Log::LOG_ERROR, "Can't init Devil Lib.");

	// Load default tex
	m_pDefaultTex =  LoadTexture("GameAssets/TEXTURES/Default.png");

	LoadResources("Assets/" + string(LightEngine::RESOURCES_FILE));

	c->m_pResources = std::unique_ptr<Resources>(this);
		
}

Texture * Resources::LoadTexture(const char * filename)
{
	Texture* tex=nullptr;
	if ((tex = HasTexture(filename)) != nullptr) return tex;

	if (strstr(filename, ".DTX") != 0) return LoadDTX(filename);
	GLint width, height, iType, iBpp;

	string fullpath = m_Path + filename;
	ilLoadImage(fullpath.c_str());
	ILenum Error;
	Error = ilGetError();

	if (Error != IL_NO_ERROR)
	{
		//string error = iluErrorString(Error);
		Log::Message(Log::LOG_ERROR, "Can't load texture " + string(filename));
		//Log::Message(Log::LOG_ERROR, "Devil: " + error);
		return HasTexture("GameAssets/TEXTURE/Default.png");
	}
	
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	iType = ilGetInteger(IL_IMAGE_FORMAT);
	ILubyte *Data = ilGetData();
	iBpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

	tex = new Texture;
	tex->Init();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, iBpp, width, height, 0, iType, GL_UNSIGNED_BYTE, Data);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenerateMipmap(GL_TEXTURE_2D);

	ilResetMemory();
	
	
	sprintf(tex->szName, "%s", filename);
	tex->iWidth = width;
	tex->iHeight = height;

	m_Textures.push_back(std::unique_ptr<Texture>(tex));


	return tex;
}

Texture * Resources::LoadCubeTex(const vector<string>& filelist)
{
	Texture* tex = NULL;
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	GLint width, height, iType, iBpp;
	for (size_t i = 0; i < filelist.size(); i++)
	{
		string fullpath = m_Path + filelist[i];
		ilLoadImage(fullpath.c_str());
		ILenum Error;
		Error = ilGetError();

		if (Error != IL_NO_ERROR)
		{
			//string error = iluErrorString(Error);
			Log::Message(Log::LOG_ERROR, "Can't load texture " + filelist[i]);
			//Log::Message(Log::LOG_ERROR, "Devil: " + error);
			return HasTexture("GameAssets/TEXTURE/Default.png");
		}

		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);
		iType = ilGetInteger(IL_IMAGE_FORMAT);
		ILubyte *Data = ilGetData();
		iBpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 0, iBpp, width, height, 0, iType, GL_UNSIGNED_BYTE, Data);
		
	}
	ilResetMemory();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	//glGenerateMipmap(GL_TEXTURE_2D);

	

	tex = new Texture;
	sprintf(tex->szName, "%s", filelist[0].c_str());
	tex->iIndex = id;
	tex->iWidth = width;
	tex->iHeight = height;

	m_Textures.push_back(std::unique_ptr<Texture>(tex));


	return tex;

}

Texture * Resources::LoadTexMemory(const char* filename,unsigned char * data, int w, int h)
{
	Texture* tex = NULL;
	if ((tex = HasTexture(filename)) != NULL) return tex;

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	tex = new Texture;
	sprintf(tex->szName, "%s", filename);
	tex->iIndex = id;
	tex->iWidth = w;
	tex->iHeight = h;

	m_Textures.push_back(std::unique_ptr<Texture>(tex));


	return tex;

}

Texture * Resources::LoadDTX(const char * filename)
{

	Texture* tex = NULL;
	if ((tex = HasTexture(filename)) != NULL) return tex;

	string fullpath = m_Path + filename;

	FILE* pFile = fopen(fullpath.c_str(), "rb");
	if (!pFile)
	{
		Log::Message(Log::LOG_ERROR, "Can't open file: " + string(filename));
		
		return tex;
	}
	DtxHeader Header;
	memset(&Header, 0, sizeof(DtxHeader));
	fread(&Header, sizeof(DtxHeader), 1, pFile);
	if (Header.iResType != 0 || Header.iVersion != -5 || Header.usMipmaps == 0)
	{
		fclose(pFile);
		return tex;
	}

	int W, H;

	W = Header.usWidth;
	H = Header.usHeight;
	int iBpp = Header.ubExtra[2];
	int iSize;
	int InternalFormat;
	if (iBpp == 3)
	{
		iSize = Header.usWidth * Header.usHeight * 4;
		InternalFormat = GL_RGBA;
	}
	else if (iBpp == 4)
	{
		iSize = (Header.usWidth * Header.usHeight) >> 1;
		InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	}
	else if (iBpp == 5)
	{
		iSize = Header.usWidth * Header.usHeight;
		InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
	}
	else if (iBpp == 6)
	{
		iSize = Header.usWidth * Header.usHeight;
		InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
	}
	else
	{
		iSize = 0;
	}

	int ImageSize = iSize;

	int iBufferSize = 1024 * 1024 * 4;
	if (iSize == 0 || iSize > iBufferSize)
	{
		fclose(pFile);
		return tex;
	}

	unsigned char* ubBuffer = new unsigned char[1024 * 1024 * 4];

	fread(ubBuffer, iSize, 1, pFile);

	if (iBpp == 3)
	{
		for (int i = 0; i < iSize; i += 4)
		{
			ubBuffer[i + 0] ^= ubBuffer[i + 2];
			ubBuffer[i + 2] ^= ubBuffer[i + 0];
			ubBuffer[i + 0] ^= ubBuffer[i + 2];
		}
	}

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	

	if (InternalFormat == GL_RGBA)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, ubBuffer);
	}
	else if (InternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || InternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || InternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
	{
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, W, H, 0, iSize, ubBuffer);
	}


	glBindTexture(GL_TEXTURE_2D, 0);



	tex = new Texture;
	sprintf(tex->szName, "%s", filename);
	tex->iIndex = id;
	tex->iWidth = W;
	tex->iHeight = H;
	
	m_Textures.push_back(std::unique_ptr<Texture>(tex));

	fclose(pFile);
	delete[] ubBuffer;
	return tex;

}

byte * Resources::LoadHeightMap(const char * filename, int& w, int& h)
{
	
	ilLoadImage(filename);
	ILenum Error;
	Error = ilGetError();

	if (Error != IL_NO_ERROR)
	{
		//string error = iluErrorString(Error);
		Log::Message(Log::LOG_ERROR, "Can't load texture " + string(filename));
		//Log::Message(Log::LOG_ERROR, "Devil: " + error);
		return NULL;
	}

	w = ilGetInteger(IL_IMAGE_WIDTH);
	h = ilGetInteger(IL_IMAGE_HEIGHT);
	
	ILubyte *Data = ilGetData();
	
	return Data;

}

ModelCache * Resources::LoadModel(const char * filename)
{
	ModelCache* pModel = nullptr;
	if ((pModel = HasModel(filename)) != nullptr) return pModel;

	string fullpath = m_Path + filename;

	pModel = new ModelCache;
	/*if (LTBFile::BeginLoad(fullpath.c_str()))
	{
		pModel->Prop = LTBFile::LoadProp();
		pModel->pMeshs = LTBFile::LoadMesh();
		pModel->pSkeNodes = LTBFile::LoadSkeleton();
		pModel->wb = LTBFile::LoadWS();
		pModel->ChildName = LTBFile::LoadChildName();
		pModel->pAnims = LTBFile::LoadAnimation(pModel->pSkeNodes);
		pModel->Sockets = LTBFile::LoadSocket();
		LTBFile::EndLoad();

	}
	else
	{
		E_ERROR("Can't open file: " + string(filename));
		return NULL;
	}

	
	
	for (size_t i = 0; i < pModel->pMeshs.size(); i++)
	{
		SkeMesh* pMesh = pModel->pMeshs[i].get();
		// Generate Buffer Object
		pMesh->Init();
	}*/

	strcpy(pModel->szName, filename);
	m_ModelCaches.push_back(std::unique_ptr<ModelCache>(pModel));
	return pModel;
}

ModelCache * Resources::LoadModelXML(const char * XMLFile)
{
	string fullpath = m_Path + XMLFile;
	tinyxml2::XMLDocument doc;
	int errorID = doc.LoadFile(fullpath.c_str());
	if (errorID)
	{
		E_ERROR("Failed to load file: " + string(XMLFile));
		return nullptr;
	}
	tinyxml2::XMLElement* pData = doc.FirstChildElement("Data");

	// load model
	tinyxml2::XMLElement* pModelNode = pData->FirstChildElement("Model");
	const char* pFileName = pModelNode->Attribute("File");

	ModelCache* pModel = LoadModel(pFileName);
	/*if (pModel)
	{
		// load texture
		tinyxml2::XMLElement* pTextureNode = pData->FirstChildElement("Texture");
		vector<std::unique_ptr<SkeMesh>>& ve = pModel->pMeshs;
		for (size_t i = 0; i < ve.size(); i++)
		{
			tinyxml2::XMLElement* pTexture = pTextureNode->FirstChildElement(ve[i]->Name.c_str());
			const char* pTextureFile = pTexture->Attribute("File");
			ve[i]->Tex = LoadDTX(pTextureFile);
		}
	}	*/	

	

	// load material
	tinyxml2::XMLElement* pMaterialData = pData->FirstChildElement("Material");
	Material mat;
	tinyxml2::XMLElement* pKa = pMaterialData->FirstChildElement("Ka");
	mat.Ka.x = pKa->FloatAttribute("r", 1.0f);
	mat.Ka.y = pKa->FloatAttribute("g", 1.0f);
	mat.Ka.z = pKa->FloatAttribute("b", 1.0f);
	tinyxml2::XMLElement* pKd = pMaterialData->FirstChildElement("Kd");
	mat.Kd.x = pKd->FloatAttribute("r", 1.0f);
	mat.Kd.y = pKd->FloatAttribute("g", 1.0f);
	mat.Kd.z = pKd->FloatAttribute("b", 1.0f);
	tinyxml2::XMLElement* pKs = pMaterialData->FirstChildElement("Ks");
	mat.Ks.x = pKs->FloatAttribute("r", 1.0f);
	mat.Ks.y = pKs->FloatAttribute("g", 1.0f);
	mat.Ks.z = pKs->FloatAttribute("b", 1.0f);
	mat.exp = vec3(pKs->FloatAttribute("exp", 32.0f));


	// Done return ModelCache


	return pModel;
}


Shader * Resources::CreateShader(const char * type, const char * vs, const char * fs)
{
	auto func = m_ShaderFactory.find(type);
	if (func == m_ShaderFactory.end()) return nullptr;

	Shader* p = func->second(vs, fs);
	return p;
}

Shader * Resources::LoadShader(string key, const char* type, const char * vs, const char* fs, bool linkshader)
{
	auto pos = m_ShaderList.find(key);
	if (pos != m_ShaderList.end()) return pos->second.get();

	
	string fullPathvs = m_Path + vs;
	string fullPathfs = m_Path + fs;
	std::unique_ptr<Shader> p(CreateShader(type,fullPathvs.c_str(),fullPathfs.c_str()));
	Shader* result = p.get();

	if (linkshader) p->LinkShader();

	m_ShaderList.insert({ key, std::move(p) });

	return result;
}

Shader * Resources::GetShader(string key)
{
	return m_ShaderList[key].get();
}

Texture * Resources::GetTexture(const char * filename)
{
	Texture* tex = nullptr;
	tex = HasTexture(filename);
	if (tex == nullptr)
	{
		E_ERROR("Cound not find texture: " + string(filename));
		return m_pDefaultTex;
	}
	return tex;
	
}

ModelCache * Resources::GetModel(const char * filename)
{
	ModelCache* pModel = nullptr;
	if (strstr(filename, ".xml") != nullptr) pModel = LoadModelXML(filename);
	else pModel = HasModel(filename);
	return pModel;
}


void Resources::LoadResources(string path)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());
	tinyxml2::XMLElement* p = doc.FirstChildElement("Resources");
	m_Path = p->Attribute("Path");
	// Loop through each child element and load the component
	for (tinyxml2::XMLElement* pNode = p->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		const char* name = pNode->Value();
		if (name == nullptr) continue;
		
		if (!strcmp(name, "Texture"))
		{
			const char* pFile = pNode->Attribute("File");
			if (pFile == nullptr) LoadTexture("TEXTURE//Default.png");
			else LoadTexture(pFile);
		}
		else if (!strcmp(name, "Shader"))
		{
			const char* pName = pNode->Attribute("Name");
			const char* pTag = pNode->Attribute("Tag");
			const char* pFileVS = pNode->Attribute("FileVS");
			const char* pFileFS = pNode->Attribute("FileFS");
			if (!pName || !pFileVS || !pFileFS) continue;
			LoadShader(pTag, pName, pFileVS, pFileFS);
		}
		else if (!strcmp(name, "ModelXML"))
		{
			const char* pFile = pNode->Attribute("File");
			if (pFile) LoadModelXML(pFile);
		}
		else if (!strcmp(name, "Model"))
		{
			const char* pFile = pNode->Attribute("File");
			if (pFile) LoadModel(pFile);
		}
	
	}
}

void Resources::ShutDown()
{
	for (size_t i = 0; i < m_Textures.size(); i++)
	{
		m_Textures[i]->Shutdown();
	}

	/*for (size_t i = 0; i < m_ModelCaches.size(); i++)
	{
		
		for (size_t j = 0; j < m_ModelCaches[i]->pMeshs.size(); j++)
		{
			m_ModelCaches[i]->pMeshs[j]->Shutdown();
		}
	}*/

	for (map<string, std::unique_ptr<Shader>>::iterator it = m_ShaderList.begin(); it!= m_ShaderList.end(); it++)
	{
		(it)->second->Shutdown();
	}

}

