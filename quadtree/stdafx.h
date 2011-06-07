#ifndef _STDAFX
#define _STDAFX

#define USE_CMD
//#define GUI_TREE

#include <iostream>
#include <conio.h>

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

using namespace std;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define MAX_DEPTH 5

#define NUM_OF_SQUARES 8

#define MAX_X 70
#define MAX_Y 30

#define NODE_X 0
#define NODE_Y 0
#define NODE_WIDTH 512
#define NODE_HEIGHT 512

#define ERR_MSG(errMsg)			{ MessageBoxW(NULL, errMsg, L"Error", 0); return FALSE; }
#define ERR_MSG_INT(errMsg)		{ MessageBoxW(NULL, errMsg, L"Error", 0); return 0; }

#define SAFE_RELEASE(x)			if(x){ (x)->Release(); (x) = NULL;	}
#define SAFE_DELETE(x)			if(x){ delete x; }

#define KEY(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)

////////////////////////////////////////////

struct COORDS{
	D3DXVECTOR2 position;
	D3DXVECTOR2 dimention;

	COORDS(){
		position.x = 0;
		position.y = 0;
		dimention.x = 0;
		dimention.y = 0;
	}

	COORDS(D3DXVECTOR2 pos, D3DXVECTOR2 dim){
		position.x = pos.x;
		position.y = pos.y;
		dimention.x = dim.x;
		dimention.y = dim.y;
	} 

	COORDS(float x, float y, float w, float h){
		position.x = x;
		position.y = y;
		dimention.x = w;
		dimention.y = h;
	}

	
};

typedef D3DXVECTOR2 Vector2;
typedef COORDS Coords;

////////////////////////////////////////////

static int GetRandMax(int max){
	return (rand() % max);
}

static int GetRandMinMax(int min, int max){
	int temp = -1;
	while(temp < min)
		temp = rand() % max;

	return temp;
}

static int GetRandMinMax2(int min, int max){
	int temp = -9999;
	while(temp < min)
		temp = rand() % max;

	if(temp % 2)
		temp *= -1;

	return temp;
}

////////////////////////////////////////////

typedef struct _BOUNDINGBOX{
	D3DXVECTOR2 position;
	D3DXVECTOR2 dimension;

	_BOUNDINGBOX(){
		position = D3DXVECTOR2(0, 0);
		dimension = D3DXVECTOR2(1, 1);
	}

	_BOUNDINGBOX(D3DXVECTOR2 pos, D3DXVECTOR2 dim){
		position = pos;
		dimension = dim;
	}
}BOUNDINGBOX;

typedef struct MODEL{
	LPD3DXMESH mesh;
	BOUNDINGBOX box;
	D3DMATERIAL9 matModel;
	D3DXVECTOR2 rotation;
	D3DXVECTOR2 scale;

	MODEL(){
		mesh = NULL;
		box = BOUNDINGBOX();
		rotation = D3DXVECTOR2(0, 0);
		scale = D3DXVECTOR2(1, 1);
	}

	MODEL(LPD3DXMESH _mesh, D3DXVECTOR2 rot, D3DXVECTOR2 sca, D3DMATERIAL9 material, D3DXVECTOR2 boxPos, D3DXVECTOR2 boxDim){
		mesh = _mesh;
		box = BOUNDINGBOX(boxPos, boxDim);
		matModel = material;
		rotation = rot;
		scale = sca;
	}
}Model;

#endif