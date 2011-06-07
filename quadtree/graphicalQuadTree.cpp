#include "graphicalQuadTree.h"

#define D3DFVF_TLVERTEX ( D3DFVF_XYZRHW | 

GraphicalQuadTree::GraphicalQuadTree(LPDIRECT3DDEVICE9 device){
	d3ddev = device;

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse.r = material.Ambient.r = 1.0f;
	material.Diffuse.g = material.Ambient.g = 1.0f;
	material.Diffuse.b = material.Ambient.b = 0.0f;
	material.Diffuse.a = material.Ambient.a = 1.0f;

	x = -256, y = 128;
}

void GraphicalQuadTree::RunOnce(){
	DrawGrid();
}

void GraphicalQuadTree::RunInLoop(){
	RenderGrid();
}

/////////////////////////////////////////////////////////

void GraphicalQuadTree::DrawGrid(){
	D3DXCreateBox(d3ddev, 8, 8, 0.1f, &mesh, NULL);
}

void GraphicalQuadTree::RenderGrid(){
	D3DXMATRIX matTranslate[GRID];

	float x = -32, y = 28;
	int k = 0;

	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			D3DXMatrixTranslation(&matTranslate[k], x,  y, 8.0f);
			k++;
			x += 8;
		}
		x = -32;
		y -= 8;
	}
				
	for(int i = 0; i < GRID; i++){
		d3ddev->SetTransform(D3DTS_WORLD, &matTranslate[i]);
		d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		
		d3ddev->SetMaterial(&material);
		mesh->DrawSubset(0);
	}

	if(KEY('A')){
		d3ddev->SetTransform(D3DTS_WORLD, &matTranslate[GetRandMax(GRID)]);
		d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		d3ddev->SetMaterial(&material);
		mesh->DrawSubset(0);
	}
}

/////////////////////////////////////////////////////////

void GraphicalQuadTree::DrawGrid(Coords value){
	float w = (float)value.dimention.x;
	float h = (float)value.dimention.y;

	D3DXCreateBox(d3ddev, w, h, 0.1f, &mesh, NULL);
}

void GraphicalQuadTree::RenderGrid(Coords value){
	D3DXMATRIX matTranslate[GRID*GRID];

	float x = (float)value.position.x;
	float y = (float)value.position.y;
	float w = (float)value.dimention.x;
	float h = (float)value.dimention.y;

	int k = 0;

	for(int i = 0; i < GRID; i++){
		for(int j = 0; j < GRID; j++){
			D3DXMatrixTranslation(&matTranslate[k], -w/2 + x, h/2 + y, 8);
			k++;
			x += value.dimention.x;
		}
		x = (float)value.position.x;
		y -= h;
	}

	for(int i = 0; i < GRID*GRID; i++){
		d3ddev->SetTransform(D3DTS_WORLD, &matTranslate[i]);
		d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		
		d3ddev->SetMaterial(&material);
		mesh->DrawSubset(0);
	}
}

void GraphicalQuadTree::DrawLine(){
	D3DXCreateBox(d3ddev, 0.1f, 150, 0.1f, &mesh, NULL);
}

void GraphicalQuadTree::RenderLine(D3DXVECTOR3 pos, float scale){
	D3DXMATRIX matTranslate[2], matRotate, matScale;

	D3DXMatrixTranslation(&matTranslate[0], pos.x, pos.y, 8);
	D3DXMatrixScaling(&matScale, 1, scale, 1);
	d3ddev->SetTransform(D3DTS_WORLD, &(matScale * matTranslate[0]));
	d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
				
	d3ddev->SetMaterial(&material);
	mesh->DrawSubset(0);

	D3DXMatrixTranslation(&matTranslate[1], pos.x, pos.y, 8);
	D3DXMatrixScaling(&matScale, 1, scale, 1);
	D3DXMatrixRotationZ(&matRotate, D3DXToRadian(90));
	d3ddev->SetTransform(D3DTS_WORLD, &(matScale * matRotate * matTranslate[1]));
	d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
				
	d3ddev->SetMaterial(&material);
	mesh->DrawSubset(0);
}