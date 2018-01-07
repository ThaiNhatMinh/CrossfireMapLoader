#include "stdafx.h"



void Camera::UpdateVector()
{

	vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	

	m_Front = glm::normalize(front);
	m_Right = glm::normalize(glm::cross(m_Front, WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));

	m_Frustum.Update(m_Position, m_Front, m_Right);

	
}

Camera::Camera()
{
	m_Position = vec3(0, 0, 200);
	m_Front = glm::normalize(vec3(0, 0, 0) - m_Position);
	WorldUp = vec3(0, 1, 0);
	m_Speed = 200.0f;
	m_Pitch = 0;
	m_Yaw = -90;
	m_Frustum = Frustum(45.0f, 4.0f / 3.0f, 1.0f, 10000.0f);
	MouseSensitivity = 0.25;
	UpdateVector();
}

Camera::Camera(Context* c,const vec3 & pos, const vec3 & target, const vec3 & up, float fov, float as, float n, float f)
{
	m_Context = c;
	m_Position = pos;
	m_Front = glm::normalize(target - m_Position);
	WorldUp = up;
	m_Speed = 200.0f;
	m_Pitch = 0;
	m_Yaw = -90;
	MouseSensitivity = 0.25;
	m_Frustum = Frustum(fov,as,n,f);
	UpdateVector();
}


Camera::~Camera()
{
}

void Camera::Update(float deltaTIme)
{
	// Camera controls
	if (m_Context->m_pInput->KeyDown(DIK_W)) m_Position += m_Front*m_Speed*deltaTIme;
	if (m_Context->m_pInput->KeyDown(DIK_S)) m_Position -= m_Front*m_Speed*deltaTIme;
	if (m_Context->m_pInput->KeyDown(DIK_D)) m_Position += m_Right*m_Speed*deltaTIme;
	if (m_Context->m_pInput->KeyDown(DIK_A)) m_Position -= m_Right*m_Speed*deltaTIme;

	m_Pitch += m_Context->m_pInput->mouseDY()*MouseSensitivity;
	m_Yaw += m_Context->m_pInput->mouseDX()*MouseSensitivity;
	if (m_Pitch > 89.0f) m_Pitch = 89.0f;
	if (m_Pitch < -89.0f) m_Pitch = -89.0f;

	UpdateVector();

	
}



mat4 Camera::GetViewMatrix()
{
	mat4 view;
	//mat4 view = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	//if(m_pCameraC!=nullptr) view = m_pCameraC->GetViewMatrix();
	//else 
		view = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	return view;
}
mat4 Camera::GetProjMatrix()
{
	return m_Frustum.GetProjMatrix();
}
mat4 Camera::GetVPMatrix()
{
	return m_Frustum.GetProjMatrix()*GetViewMatrix();
}
/*
void Camera::SetCameraActor(Actor * p)
{
	m_pCameraC = p->GetComponent<CameraComponent>(CameraComponent::Name);
}*/
