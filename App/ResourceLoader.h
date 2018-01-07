#pragma once
#include "stdafx.h"

struct DtxHeader
{
	unsigned int iResType;
	int iVersion;
	unsigned short usWidth;
	unsigned short usHeight;
	unsigned short usMipmaps;
	unsigned short usSections;
	int iFlags;
	int iUserFlags;
	unsigned char ubExtra[12];
	char szCommandString[128];
};
#define MAX_FILE_NAME 128


class Material
{
public:

	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	vec3 exp;
	Material() :Ka(1.0f), Kd(1.0f), Ks(1.0f), exp(128)
	{

	}
};

class Resources : public ISubSystem
{
private:
	std::map<string, std::function<Shader*(const char *, const char*)>> m_ShaderFactory;

	vector<std::unique_ptr<Texture>> m_Textures;
	vector<std::unique_ptr<ModelCache>> m_ModelCaches;
	map<string, std::unique_ptr<Shader>> m_ShaderList;
	
	// this list store primitive shape 
	vector<std::unique_ptr<IMesh>>	m_PrimList;
	// Default texture when can't found tex
	Texture* m_pDefaultTex;
	// Path to resource
	std::string		m_Path;
private:
	Shader* CreateShader(const char* type, const char* vs, const char* fs);
	Texture* HasTexture(const char* filename);
	ModelCache* HasModel(const char* filename);

	Texture* LoadTexture(const char* filename);
	Texture* LoadCubeTex(const vector<string>& filelist);
	Texture* LoadTexMemory(const char* filename, unsigned char* data, int w, int h);
	Texture* LoadDTX(const char* filename);
	unsigned char* LoadHeightMap(const char* filename, int& w, int& h);
	ModelCache* LoadModel(const char* filename);
	ModelCache* LoadModelXML(const char* XMLFile);
	
	Shader* LoadShader(string key,const char* type, const char* vs, const char* fs, bool linkshader = true);

	void LoadResources(string path);

public:
	Resources();
	~Resources();
	virtual void  Init(Context* c);
	virtual void  ShutDown();

	
	Shader*		GetShader(string key);
	Texture*	GetTexture(const char* filename);
	ModelCache*	GetModel(const char* filename);
	
	const char* GetPath(Shader* p) { return nullptr; };
	const char* GetPath(Texture* p) { return nullptr; };
	const char* GetPath(ModelCache* p) { return nullptr; };
};
