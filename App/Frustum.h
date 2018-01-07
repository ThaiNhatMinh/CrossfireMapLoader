#pragma once
#include "stdafx.h"

class Frustum
{
private:
	enum Side{Near, Far, Top, Bottom, Left, Right, NumPlane};
	mat4 m_ProjMatrix;
public:
	//Plane m_Plane[6];			// Plane of frustum in camera space
	vec3 m_NearClip[4];					// 4 point in near clip plane in camera space
	vec3 m_FarClip[4];					// 4 point in far clip plane in camera space
	
	float m_Fov;						// fied of view in radian
	float m_Aspect;						// aspect ratio  - width / height
	float m_Near;						// Near distance
	float m_Far;						// Far distance
	float tanFovOver2;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
protected:
	
public:
	Frustum(float fov, float aspect, float _near, float _far);
	Frustum();
	~Frustum();
	void Init();
	void Init(float fov, float aspect, float _near, float _far);
	bool Inside(const vec3& v)const;
	bool Inside(const vec3& v, float radius)const;
	bool Inside(const vec3& v, const vec3& v2)const;
	//const Plane& Get(Side side){return m_Plane[side];}
	void SetFov(float fov){ m_Fov = fov; Init(m_Fov, m_Aspect, m_Near, m_Far);}
	void SetAspect(float aspect) { m_Aspect = aspect;  Init(m_Fov, m_Aspect, m_Near, m_Far); };
	void SetNear(float _near) { m_Near = _near;  Init(m_Fov, m_Aspect, m_Near, m_Far); };
	void SetFar(float _far) { m_Far = _far;  Init(m_Fov, m_Aspect, m_Near, m_Far); };
	void Update(const vec3& pos,const vec3& front,const vec3& right);
	void Render(Shader* shader, mat4& view);

	const mat4& GetProjMatrix();
};