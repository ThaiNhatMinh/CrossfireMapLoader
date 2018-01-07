#pragma once


class Application : public IApplication
{
public:
	Application(){};
	~Application();
	

	
	
	void MainLoop();
private:
	bool m_bRunMainLoop;
	void SetupSubmodule();

	void RenderUI() {};
protected:
	virtual void Setup() {};
	virtual void Start() {};

	std::unique_ptr<Context> m_Context;

	std::unique_ptr<Camera> m_MainCamera;

	
};