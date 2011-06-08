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
	float x; 
	float y; 

	int width;
	int height;

	int depthLevel;
	int nodeID;

	bool isLeafNode;

	Node* parent;
	Node* child[4];

	Model model[NUM_OF_SQUARES];

	vector<Coords> ObjectCollector;
	//vector<Model> ObjectContainer;

#ifndef USE_CMD
	GraphicalQuadTree* grid;
	LPDIRECT3DDEVICE9 d3ddev;
#endif

	//constructors
	Node();
	Node(D3DXVECTOR3 position, D3DXVECTOR3 dimension);
	Node(float xx, float yy, int w, int h);

#ifdef GUI_TREE
	Node(int xx, int yy, int w, int h, LPDIRECT3DDEVICE9 device);
#endif

	//add Coords value to root node
	void AddCoordsToRoot(Coords value);

	//add Model* to root node
	void AddCoordsToRoot(Model* model);

	//Update node on the event of deletion or object movement
	void UpdateRoot();
	void UpdateRoot(Node* node);

	//add new node and its children and their children to eternity!
	void AddNode();

	//delete the children and their children to eternity!, make the node a leaf
	void DeleteNode();

	//
	void DeleteObject(Coords value);

	//
	Node* SearchNode(Coords value);
	//void SearchNode(Coords value);

	//
	void MoveNode(D3DXVECTOR2 position, int objectToDelete);

	//
	void GetQuad(bool drawTree = true);

	//
	void GetQuad2Grid(Coords value);

	//
	void Foobar();

#ifdef GUI_TREE
	//grid
	void DrawGrid();
#endif

	//for debuging
	void DrawTree();
	void Debug();
};

#endif