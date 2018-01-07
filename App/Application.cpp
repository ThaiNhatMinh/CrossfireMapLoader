#include "stdafx.h"
#include <type_traits>
void Application::SetupSubmodule()
{
	//E_DEBUG("Application StartUp...");
	Context			*C = new Context();
	Windows			*W = new Windows();
	GameTimer		*G = new GameTimer();
	SystemUI		*S = new SystemUI();
	Resources		*R = new Resources();
	OpenGLRenderer	*O = new OpenGLRenderer();
	DirectInput		*D = new DirectInput();
	ISubSystem::m_Context = C;

	
	W->Init(C);
	O->Init(C);
	R->Init(C);
	S->Init(C);
	
	D->Init(C);
	G->Init(C);

	
	m_Context = std::unique_ptr<Context>(C);

	m_MainCamera = std::make_unique<Camera>(C,vec3(0,0,0),vec3(100),vec3(0,1,0),45.0f,4.0/3.0f,1.0f,100000.0f);
	m_MainCamera->SetSpeed(3000);
}

Application::~Application()
{
	
}



void Application::MainLoop()
{
	Setup();
	SetupSubmodule();
	Start();

	m_bRunMainLoop = true;
	// PROBLEM: How every thing update ?
	// 1. Timer
	// 2. Event Manager
	// 3. Input 
	// 4. Physic
	// 5. 
	

	GameTimer		*G = m_Context->m_pTimer.get();
	DirectInput		*D = m_Context->m_pInput.get();
	SystemUI		*S = m_Context->m_pSystemUI.get();
	m_Context->m_pWindows->ShowWindows();

	G->Reset();
	
	FILE* pFile = fopen("Assets//WORLD//AI_ZumaTop", "rb");
	if (pFile == nullptr)
	{
		printf("Can't open file");
		return ;
	}
	m_Context->m_pRenderer->SetDebugShader(m_Context->m_pResources->GetShader("Debug"));

	CMainWorld World(m_Context.get());
	World.Load(pFile);

	while (m_bRunMainLoop)
	{
		glfwPollEvents();
		D->Update();
		if (m_Context->m_pWindows->ShouldClose())	m_bRunMainLoop = false;
		m_Context->m_pRenderer->Clear();
		
		G->Tick();

		m_MainCamera->Update(G->GetDeltaTime());
		S->NewFrame();
		static bool oldi,oldk;
		if (D->KeyDown(DIK_I) &&!oldi) World.m_WorldRenderer.numdraw++;
		if (D->KeyDown(DIK_K) && !oldk) World.m_WorldRenderer.numdraw--;

		oldi = D->KeyDown(DIK_I);
		oldk = D->KeyDown(DIK_K);

		World.m_WorldRenderer.Render(m_MainCamera.get());
	
		m_Context->m_pRenderer->Render(m_MainCamera.get());
		
		S->Render();
		glfwSwapBuffers(m_Context->m_pWindows->Window());


	}
}