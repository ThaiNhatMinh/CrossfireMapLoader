#pragma once

#define MAX_NAME 128

enum blendset
{
	null = -1,
	upper,
	lower,
};


class Context;
class imguiMesh;
class Texture;

class Windows;
class GameTimer;
class SystemUI;
class Resources;
class OpenGLRenderer;
class DirectInput;
class Camera;
class Shader;
class AnimationComponent;

// OpenGL Buffer Object
// All other buffer must deliver form this class
// Call to Init to init buffer
// Call to shutdown to delete buffer
// Not to delete buffer in destructor because it lead to auto delete buffer when still run
class GLBO
{
public:
	virtual ~GLBO() {};
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
};
// mesh interface
class IMesh: public GLBO
{
public:
	// Use to generate Vertex Buffer Object, Vertex Array Object.
	IMesh() :VAO(0), VBO(0), EBO(0){}
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual ~IMesh() {};
	string					Name;
	GLuint					VAO;
	GLuint					VBO;
	GLuint					EBO;
	GLuint					NumIndices;
	Texture*				Tex;
	GLuint					Topology;	
};


class IApplication
{

public:
	virtual void Setup() {};
	virtual void Start() {};
	virtual void MainLoop() = 0;
	virtual ~IApplication() {};
};

class Context;
class ISubSystem
{
protected:
	
public:
	virtual void Init(Context* c) = 0;
	virtual void ShutDown() = 0;
	virtual ~ISubSystem() {};
	static Context* m_Context;
};
