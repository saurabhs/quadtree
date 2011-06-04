#ifndef _STDAFX
#include "stdafx.h"
#endif
#ifndef _DX
#include "dx.h"
#endif

#ifndef _GRAPHICAL_QUADTREE
#define _GRAPHICAL_QUADTREE

#define GRID 4

class GraphicalQuadTree{
	LPDIRECT3DDEVICE9 d3ddev;
	LPD3DXMESH mesh;
	D3DMATERIAL9 material, material2;

	float x, y;
public:
	GraphicalQuadTree(){ /* */ }
	GraphicalQuadTree(LPDIRECT3DDEVICE9 device);
	~GraphicalQuadTree(){ /* */ }

	LPDIRECT3DDEVICE9 GetDevice(){ return d3ddev; }

	void RunOnce();
	void RunInLoop();

	void DrawGrid();
	void RenderGrid();

	void DrawGrid(Coords value);
	void RenderGrid(Coords value);

	void DrawLine();
	void RenderLine(D3DXVECTOR3 pos, float scale);

	//Grid with Nodes!
	//void DrawGrid2(Coords value);
	//void RenderGrid2(Coords value);
};

#endif