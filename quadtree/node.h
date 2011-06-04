#ifndef _STDAFX
#include "stdafx.h"
#endif
#ifndef _GRAPHICAL_QUADTREE
#include "graphicalQuadTree.h"
#endif

#ifndef _NODE
#define _NODE

class Node{
public:
	int x; 
	int y; 

	int width;
	int height;

	int depthLevel;
	int nodeID;

	bool isLeafNode;

	Node* parent;
	Node* child[4];

	Model* model[NUM_OF_SQUARES];

	vector<Coords> ObjectCollector;

#ifndef USE_CMD
	GraphicalQuadTree* grid;
	LPDIRECT3DDEVICE9 d3ddev;
#endif

	//constructors
	Node();
	Node(D3DXVECTOR3 position, D3DXVECTOR3 dimension);
	Node(int xx, int yy, int w, int h);

#ifdef GUI_TREE
	Node(int xx, int yy, int w, int h, LPDIRECT3DDEVICE9 device);
#endif

	//add Coords value to root nore
	void AddCoordsToRoot(Coords value);

	//
	void UpdateRoot(int index, Coords value);

	//add new node and its children and their children to eternity!
	void AddNode();

	//delete the children and their children to eternity!, make the node a leaf
	void DeleteNode();

	//
	void GetQuad();

	//
	void GetQuad2Grid(Coords value);

	void NodeCorrection();

#ifdef GUI_TREE
	//grid
	void DrawGrid();
#endif

	//for debuging
	void DrawTree();
	void Debug();
};

#endif