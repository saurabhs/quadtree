#include "camera.h"

Camera::Camera(){															//Camera at Origin
	right	 = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	up		 = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	look	 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

Camera::Camera(float camX, float camY, float camZ){
	right	 = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	up		 = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	look	 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	position = D3DXVECTOR3(camX, camY, camZ);
}

void Camera::initCam(D3DXMATRIX* pView){
	D3DXVec3Normalize(&look, &look);
	D3DXVec3Cross(&up, &look, &right);
	D3DXVec3Normalize(&up, &up);
	D3DXVec3Cross(&right, &up, &look);
	D3DXVec3Normalize(&right, &right);

	float x = -D3DXVec3Dot(&right, &position);
	float y = -D3DXVec3Dot(&up, &position);
	float z = -D3DXVec3Dot(&look, &position);

	(*pView)(0, 0) = right.x;
	(*pView)(0, 1) = up.x;
	(*pView)(0, 2) = look.x;
	(*pView)(0, 3) = 0.0f;

	(*pView)(1, 0) = right.y;
	(*pView)(1, 1) = up.y;
	(*pView)(1, 2) = look.y;
	(*pView)(1, 3) = 0.0f;

	(*pView)(2, 0) = right.z;
	(*pView)(2, 1) = up.z;
	(*pView)(2, 2) = look.z;
	(*pView)(2, 3) = 0.0f;

	(*pView)(3, 0) = x;
	(*pView)(3, 1) = y;
	(*pView)(3, 2) = z;
	(*pView)(3, 3) = 1.0f;
}

void Camera::walk(float unit){
	//p += D3DXVECTOR3(0.0f, 0.0f, d.z) * unit;
	position += look * unit;
}

void Camera::fly(float unit){
	//p.y += D3DXVECTOR3(0.0f, d.y, 0.0f) * unit;
	position += up * unit;
}

void Camera::strafe(float unit){
	//p.x += D3DXVECTOR3(d.x, 0.0f, 0.0f) * unit;
	position += right * unit;
}

void Camera::Move(float unitX, float unitY, float unitZ){
	position += right * unitX;
	position += up * unitY;
	position += look * unitZ;
}

void Camera::pitch(float angle){
	D3DXMatrixRotationAxis(&m_Transf, &right, angle);
	D3DXVec3TransformCoord(&up, &up, &m_Transf);
	D3DXVec3TransformCoord(&look, &look, &m_Transf);
}

void Camera::yaw(float angle){
	D3DXMatrixRotationAxis(&m_Transf, &up, angle);
	D3DXVec3TransformCoord(&right, &right, &m_Transf);
	D3DXVec3TransformCoord(&look, &look, &m_Transf);
}

void Camera::roll(float angle){
	D3DXMatrixRotationAxis(&m_Transf, &look, angle);
	D3DXVec3TransformCoord(&right, &right, &m_Transf);
	D3DXVec3TransformCoord(&up, &up, &m_Transf);
}