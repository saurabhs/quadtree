#ifndef _STDAFX
	#include "stdafx.h"
#endif
#include "node.h"

static int index = 1;
Node* nodeToDelete = new Node();

Node::Node(){
	x = 0;
	y = 0;

	width = 512;
	height = 512;

	depthLevel = 0;
	nodeID = 1;
	parentNodeID = 0;

	isLeafNode = true;
}

Node::Node(D3DXVECTOR3 position, D3DXVECTOR3 dimension){
	x = position.x;
	y = position.y;
	width = (int)dimension.x;
	height = (int)dimension.y;

	this->parent = NULL;
	for(int i = 0; i < 4; i++)
		this->child[i] = NULL;

	depthLevel = 0;
	nodeID = 1;
	parentNodeID = 0;

	isLeafNode = true;
}

Node::Node(float xx, float yy, int w, int h){
	x = xx;
	y = yy;
	width = w;
	height = h;

	this->parent = NULL;
	for(int i = 0; i < 4; i++)
		child[i] = NULL;

	depthLevel = 0;
	nodeID = 1;
	parentNodeID = 0;

	isLeafNode = true;

	ObjectCollector = vector<Coords>();
	//ObjectContainer = vector<Model>();
}

#ifdef GUI_TREE
Node::Node(int xx, int yy, int w, int h, LPDIRECT3DDEVICE9 device){
	x = xx;
	y = yy;
	width = w;
	height = h;

	d3ddev = device;

	grid = new GraphicalQuadTree(device);
	grid->DrawLine();

	this->parent = NULL;

	/*for(int i = 0; i < 4; i++)
		child[i] = NULL;*/

	depthLevel = 0;
	nodeID = 1;

	isLeafNode = true;
}
#endif

//Add to container
void Node::AddCoordsToRoot(Coords value){ this->ObjectCollector.push_back(value); }
//void Node::AddCoordsToRoot(Model* model){ this->ObjectContainer.push_back(*model); }

void Node::AddNode(){
	//init this node
	this->isLeafNode = false;

	float xx = this->x;
	float yy = this->y;
	int w = this->width / 2;
	int h = this->height / 2;

	//set children's position
#ifdef GUI_TREE
	this->child[0] = new Node(xx,	  yy,	  w, h, grid->GetDevice());
	this->child[1] = new Node(xx + w, yy,	  w, h, grid->GetDevice());
	this->child[2] = new Node(xx + w, yy + h, w, h, grid->GetDevice());
	this->child[3] = new Node(xx,	  yy + h, w, h, grid->GetDevice());
#else
	this->child[0] = new Node(xx, yy, w, h);
	this->child[1] = new Node(xx + w, yy, w, h);
	this->child[2] = new Node(xx + w, yy + h, w, h);
	this->child[3] = new Node(xx, yy + h, w, h);
#endif

	for(int i = 0; i < 4; i++){
		this->child[i]->parent = this;
		this->child[i]->depthLevel = this->depthLevel + 1;
		this->child[i]->nodeID = ++index;
		this->child[i]->parentNodeID = this->nodeID;
		this->child[i]->ObjectCollector = vector<Coords>();
	}
}

void Node::DeleteChildNodes(){
	if(!this->isLeafNode && this->ObjectCollector.size() < 2){
		for(int i = 0; i < 4; i++){
			this->child[i]->DeleteChildNodes();
			delete this->child[i];
			this->isLeafNode = true;
		}
	}
}

void Node::DeleteObject(D3DXVECTOR2 value){
	for(vector<Coords>::iterator iter = this->ObjectCollector.begin(); iter != this->ObjectCollector.end(); iter++)
		if(value == iter->position){
			this->ObjectCollector.erase(iter);
			break;
		}

	if(!this->isLeafNode)
		for(int i = 0; i < 4; i++)
			this->child[i]->DeleteObject(value);

	this->DeleteChildNodes();
}

//void Node::SearchNode(Coords value){
Node* Node::SearchNode(Coords value){
	if(this->isLeafNode) return this;
	for(int i = 0; i < 4; i++)
		if(value.position.x > this->child[i]->x && value.position.x < (this->child[i]->x + this->child[i]->width) &&
			value.position.y > this->child[i]->y && value.position.y < (this->child[i]->y + this->child[i]->height)){
			this->child[i]->SearchNode(value);
			continue;
		}

	//return NULL; //func should return some value (?)
}

void Node::MoveNode(D3DXVECTOR2 position, int objectToDelete){
	vector<Coords>::iterator iter = this->ObjectCollector.begin() + objectToMove;
	iter->position = position;
	this->UpdateQuad();

	//this->DrawTree();
}

/*void Node::UpdateRoot(){
	if(this->ObjectCollector.size() > 1 && this->isLeafNode){
		this->AddNode();
		this->BuildQuad();
	}

	if(!this->isLeafNode)
		for(int i = 0; i < 4; i++)
			this->child[i]->UpdateRoot();
}*/

void Node::UpdateQuad(){
	int objectCountChild = 0;
	bool matched = false;

	if(!this->isLeafNode) 
		for(int i = 0; i < 4; i++) 
			objectCountChild += this->child[i]->ObjectCollector.size();

	if(!this->isLeafNode)
		if(objectCountChild != this->ObjectCollector.size())
			for(int i = 0; i < 4; i++)
				for(vector<Coords>::iterator iter = this->ObjectCollector.begin(); iter != this->ObjectCollector.end(); iter++)
					if(iter->position.x > this->child[i]->x && iter->position.x < (this->x + this->width) && iter->position.y > this->child[i]->y && iter->position.y < (this->y + this->height)){
						for(vector<Coords>::iterator iter2 = this->child[i]->ObjectCollector.begin(); iter2 != this->child[i]->ObjectCollector.end(); iter2++){
							if(iter->position.x == iter2->position.x && iter->position.y == iter2->position.y){
								matched = true;
							}
						}
						if(!matched){
							this->child[i]->BuildQuad(false);
							this->child[i]->UpdateQuad();
						}
					}
}

void Node::BuildQuad(bool drawTree){
	if(this->ObjectCollector.size() > 1 && this->depthLevel < MAX_DEPTH){
		this->AddNode();
		for(int i = 0; i < 4; i++){
			for(vector<Coords>::iterator iter = this->ObjectCollector.begin(); iter != this->ObjectCollector.end(); iter++){
				if(iter->dimention.x < this->width && iter->dimention.y < this->height && 
					iter->position.x > this->child[i]->x && iter->position.x < (this->child[i]->x + this->child[i]->width) && 
					iter->position.y > this->child[i]->y && iter->position.y < (this->child[i]->y + this->child[i]->height))
						this->child[i]->ObjectCollector.push_back(*iter);
			}
			if(this->child[i]->ObjectCollector.size() > 1)
				this->child[i]->BuildQuad();
		}
	}

	if(drawTree)
		this->DrawTree();
}

#ifdef GUI_TREE
void Node::DrawGrid(){
	float scaleFactor = pow(2.0f, this->depthLevel);
	float posX = (this->x / this->width) * 150.0f;
	float posY = (this->y / this->height) * 150.0f;

	this->grid->RenderLine(D3DXVECTOR3(posX, posY, 0), 1/scaleFactor);
	
	if(!this->isLeafNode)
		for(int i = 0; i < 4; i++)
			this->child[i]->DrawGrid();
}
#endif

void Node::Foobar(){
	int choice;
	int item;
	float x, y, w, h;
	Coords newPos;
	Node* temp;

	vector<Coords>::iterator iter;
	cout<<"\n1. Add Object\n2. Delete Object\n3. Move Object\n4. Search Object\n\nEnter 1/2/3/4 : ";
	cin>>choice;

	system("cls");

	switch(choice){
		case 1:
			cout<<"\nEnter object position and dimension: ";
			cin>>x>>y>>w>>h;
			newPos = Coords(x, y, w, h);
			this->AddCoordsToRoot(newPos);
			this->BuildQuad();
			break;

		case 2:
			cout<<"\nEnter object ID to delete (container size is "<<this->ObjectCollector.size()<<") : ";
			cin>>item;
			
			cout<<"\nTree before deletion: ";
			this->BuildQuad();

			iter = this->ObjectCollector.begin() + item;
			
			cout<<"\nTree after deletion: ";
			this->DeleteObject(iter->position);
			this->DrawTree();
			break;

		case 3:
			cout<<"\nEnter object ID to move (container size is "<<this->ObjectCollector.size()<<") : ";
			cin>>item;
			iter = this->ObjectCollector.begin() + item;
			cout<<"\nEnter new position of object (Current position : "<<iter->position.x<<", "<<iter->position.y<<") : ";
			cin>>x>>y;
			
			cout<<"\nTree before moving the object: ";
			this->BuildQuad();
			
			cout<<"\n\nTree after moving the object: ";
			this->MoveNode(D3DXVECTOR2(x, y), item);
			break;

		case 4:
			cout<<"\nEnter object ID to search (container size is "<<this->ObjectCollector.size()<<") : ";
			cin>>item;
			iter = this->ObjectCollector.begin() + item;

			cout<<"\nCurrent tree structure :";
			this->BuildQuad();

			temp = this->SearchNode(Coords(iter->position, iter->dimention));
			cout<<"\nObject position: ";
			cout<<temp->nodeID;
			break;
	}
}

void Node::DrawTree(){
	//if(!this->isLeafNode){
	cout<<"\n\nNode ID : "<<this->nodeID<<"\nChild of Node : "<<this->parentNodeID;
	cout<<"\nNode X: "<<this->x<<", Node Y: "<<this->y;
	cout<<"\nNode W: "<<this->width<<", Node H: "<<this->height;
	cout<<"\nDepth: "<<this->depthLevel<<", Object Contains: "<<this->ObjectCollector.size();
	cout<<"\nIs Leaf : "<<this->isLeafNode;
	//}

	if(!this->isLeafNode)
		for(int i = 0; i < 4; i++){
			cout<<"\n\nNode ID : "<<this->child[i]->nodeID; //<<"\nChild of Node : "<<this->parentNodeID;
			cout<<"\nNode X: "<<this->child[i]->x<<", Node Y: "<<this->child[i]->y;
			cout<<"\nNode W: "<<this->child[i]->width<<", Node H: "<<this->child[i]->height;
			cout<<"\nDepth: "<<this->child[i]->depthLevel<<", Object Contains: "<<this->child[i]->ObjectCollector.size();
			cout<<"\nIs Leaf : "<<this->child[i]->isLeafNode;

			//cout<<"\n\nChild["<<i<<"] :";
			//this->child[i]->DrawTree();
		}
}