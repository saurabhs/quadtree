#ifndef _STDAFX
#include "stdafx.h"
#endif
#ifndef _NODE
#include "node.h"
#endif
#ifndef _DX
#include "dx.h"
#endif
#ifndef _GRAPHICAL_QUADTREE
#include "graphicalQuadTree.h"
#endif

#ifndef _QUADTREE
#define _QUADTREE

class QuadTree : public GraphicalQuadTree{
	LPDIRECT3DDEVICE9 d3ddev;
	D3DMATERIAL9 material;
	Model *model[NUM_OF_SQUARES];

	int collide;
public:
	QuadTree(){ /* */ }
	QuadTree(LPDIRECT3DDEVICE9 device);
	~QuadTree(){ /* */ }

	void RunOnce(HWND hwnd);
	void RunInLoop();

	Coords GetModelsData(int index);
	Model* GetModel(int index);

	void DrawSquare();
	void RenderSquare();

	//defines path and move squares
	void SimulateSquare();

	//Min Max for bounding box
	void GetMinMax(Model* model);

	bool CollisionDetection(Model* modelA, Model* modelB);
	void CollisonResponse(Model* modelA, Model* modelB);

	//runs 2D collision detection and response
	void Physics2D();

	void Debug();
};

#endif