#ifndef _STDAFX
#include "stdafx.h"
#endif

#ifndef _CAMERA
#define _CAMERA

class Camera{
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;
	D3DXVECTOR3 look;

	D3DXVECTOR3 rotation;
	D3DXMATRIX m_Transf;
public:
	D3DXVECTOR3 position;

	Camera();
	Camera(float, float, float);
	~Camera(){ /* */ }

	void initCam(D3DXMATRIX*);

	void Move(float, float, float);

	void walk(float);
	void fly(float);
	void strafe(float);

	void pitch(float);	
	void yaw(float);
	void roll(float);
};

#endif