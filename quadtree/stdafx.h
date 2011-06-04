#ifndef _STDAFX
#define _STDAFX

//#define USE_CMD
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

struct VECTOR2{
	int x;
	int y;
};

struct COORDS{
	VECTOR2 position;
	VECTOR2 dimention;

	COORDS(){
		position.x = 0;
		position.y = 0;
		dimention.x = 0;
		dimention.y = 0;
	}

	COORDS(D3DXVECTOR3 pos, D3DXVECTOR3 dim){
		position.x = (int)pos.x;
		position.y = (int)pos.y;
		dimention.x = (int)dim.x;
		dimention.y = (int)dim.y;
	} 

	COORDS(int x, int y, int w, int h){
		position.x = x;
		position.y = y;
		dimention.x = w;
		dimention.y = h;
	}
};

typedef VECTOR2 Vector2;
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
	D3DXVECTOR3 position;
	D3DXVECTOR3 dimension;

	_BOUNDINGBOX(){
		position = D3DXVECTOR3(0, 0, 0);
		dimension = D3DXVECTOR3(1, 1, 1);
	}

	_BOUNDINGBOX(D3DXVECTOR3 pos, D3DXVECTOR3 dim){
		position = pos;
		dimension = dim;
	}
}BOUNDINGBOX;

typedef struct MODEL{
	LPD3DXMESH mesh;
	BOUNDINGBOX box;
	D3DMATERIAL9 matModel;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;

	MODEL(){
		mesh = NULL;
		box = BOUNDINGBOX();
		rotation = D3DXVECTOR3(0, 0, 0);
		scale = D3DXVECTOR3(1, 1, 1);
	}

	MODEL(LPD3DXMESH _mesh, D3DXVECTOR3 rot, D3DXVECTOR3 sca, D3DMATERIAL9 material, D3DXVECTOR3 boxPos, D3DXVECTOR3 boxDim){
		mesh = _mesh;
		box = BOUNDINGBOX(boxPos, boxDim);
		matModel = material;
		rotation = rot;
		scale = sca;
	}
}Model;

#endif