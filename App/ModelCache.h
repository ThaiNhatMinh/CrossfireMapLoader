#pragma once
#include "stdafx.h"

class SkeMesh;
struct SkeNode;
struct WeightBlend;
struct Animation;
struct LTBSocket;
struct LTBProp;

class ModelCache
{
public:
	char								szName[128];
	//LTBProp								Prop;
	//vector<std::unique_ptr<SkeMesh>>	pMeshs;
	//vector<std::unique_ptr<SkeNode>>	pSkeNodes;
	//vector<WeightBlend>					wb;
	vector<string>						ChildName;
	//vector<std::unique_ptr<Animation>>	pAnims;
	//vector<LTBSocket>					Sockets;
	ModelCache() {}
	~ModelCache() {}
};