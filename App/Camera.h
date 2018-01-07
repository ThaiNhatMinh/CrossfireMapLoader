#pragma once
#include "stdafx.h"

// Default camera

class Camera
{
	friend class Frustum;
protected:
	vec3 m_Position; // position of camera
	vec3 m_Front;
	vec3 m_Right;
	vec3 WorldUp;
	vec3 m_Up;
	float m_Speed;
	float m_Pitch;	// x axis
	float m_Yaw;	// y axis
	float MouseSensitivity;
	Frustum m_Frustum;
	Context* m_Context;
protected:
	void UpdateVector();
public:
	Camera();
	
	// target and up vector must be normalize
	Camera(Context*,const vec3& pos, const vec3& target, const vec3& up,float fov,float as,float n,float f);
	~Camera();

	//virtual void OnKeyboard(int key,float deltaTime);
	virtual void Update(float dt);
	//virtual void OnMouse(float dx, float dy);
	mat4 GetViewMatrix();
	mat4 GetProjMatrix();
	mat4 GetVPMatrix();
	inline void SetSpeed(float speed) { m_Speed = speed; };
	inline vec3 GetFront() { return m_Front; };
	inline const vec3 GetPosition()const { return m_Position; };
	inline vec3& GetPosition() { return m_Position; };
	inline void SetPosition(vec3 v) { m_Position = v; }
	inline vec2 GetRorate() { return vec2(m_Pitch, m_Yaw); };
	inline vec3 GetUP() { return m_Up; };
	inline void InvertPitch() { m_Pitch = -m_Pitch; };
	inline void InvertYaw() { m_Yaw = -m_Yaw; };

	inline Frustum& GetFrustum() { return m_Frustum; }

};

