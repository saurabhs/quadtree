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
	int parentNodeID;

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

	//
	void UpdateQuad();

	//add new node and its children and their children to eternity!
	void AddNode();

	//delete the children and their children to eternity!, make the node a leaf
	void DeleteChildNodes();

	//
	void DeleteObject(D3DXVECTOR2 value);

	//Search for a object in the tree
	Node* SearchNode(Coords value);
	//void SearchNode(Coords value);

	//perform actions for moving a object
	void MoveNode(D3DXVECTOR2 position, int objectToMove);

	//perform actions at the previous position of the object
	void PreMoveNode(Node* node, bool iterateTopNode);

	//perform actions at the new position of the object
	void PostMoveNode(Coords objectTMove);

	//
	void BuildQuad(bool drawTree = true);

	//
	void GetQuad2Grid(Coords value);

	//returns the top most quad for any child / grandchild
	Node* GetAbsoluteParent();

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