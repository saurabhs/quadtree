#ifndef _STDAFX
	#include "stdafx.h"
#endif
#include "node.h"

static int index = 1;

Node::Node(){
	x = 0;
	y = 0;

	width = 512;
	height = 512;

	depthLevel = 0;
	nodeID = 1;

	isLeafNode = true;
}

Node::Node(D3DXVECTOR3 position, D3DXVECTOR3 dimension){
	x = (int)position.x;
	y = (int)position.y;
	width = (int)dimension.x;
	height = (int)dimension.y;

	this->parent = NULL;
	for(int i = 0; i < 4; i++)
		this->child[i] = NULL;

	depthLevel = 0;
	nodeID = 1;

	isLeafNode = true;
}

Node::Node(int xx, int yy, int w, int h){
	x = xx;
	y = yy;
	width = w;
	height = h;

	this->parent = NULL;
	for(int i = 0; i < 4; i++)
		child[i] = NULL;

	depthLevel = 0;
	nodeID = 1;

	isLeafNode = true;

	ObjectCollector = vector<Coords>();
	ObjectContainer = vector<Model>();
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
void Node::AddCoordsToRoot(Model* model){ this->ObjectContainer.push_back(*model); }

//Update Container 
void Node::UpdateRoot(int index, Coords value){
	this->ObjectCollector[index] = value;
	this->GetQuad();
}

void Node::AddNode(){
	//init this node
	this->isLeafNode = false;

	int xx = this->x;
	int yy = this->y;
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
		this->child[i]->ObjectCollector = vector<Coords>();
	}
}

void Node::DeleteNode(){
	if(!this->isLeafNode)
		for(int i = 0; i < 4; i++){
			this->child[i]->DeleteNode();
			delete this->child[i];
		}

	this->isLeafNode = true;
}

void Node::MoveNode(){
	vector<Coords> oldPos = vector<Coords>();
	for(vector<Model>::iterator iter = this->ObjectContainer.begin(); iter != this->ObjectContainer.end(); iter++)
		oldPos.push_back(Coords(iter->box.position.x, iter->box.position.y, iter->box.dimension.x, iter->box.dimension.y));

	int index = 0;
	for(vector<Model>::iterator iter = this->ObjectContainer.begin(); iter != this->ObjectContainer.end(); iter++){
		if(iter->box.position.x != (float)oldPos.at(index).position.x && iter->box.position.y != (float)oldPos.at(index).position.y){

		}

		index++;
	}	
}

/*void Node::GetQuad(){
	if(this->ObjectCollector.size() > 1 && this->depthLevel < MAX_DEPTH){
		this->AddNode();
		for(int i = 0; i < 4; i++){
			for(vector<Coords>::iterator iter = this->ObjectCollector.begin(); iter != this->ObjectCollector.end(); iter++){
				if(iter->dimention.x < this->width && iter->dimention.y < this->height && 
					iter->position.x > this->child[i]->x && iter->position.x < (this->child[i]->x + this->child[i]->width) && 
					iter->position.y > this->child[i]->y && iter->position.y < (this->child[i]->y + this->child[i]->height)){
						this->child[i]->ObjectCollector.push_back(*iter);
						//this->DrawGrid();
						//TODO : delete object which have been already added to its child
				}
			}
			if(this->child[i]->ObjectCollector.size() > 1)
				this->child[i]->GetQuad();
		}
	}
#ifdef USE_CMD
	this->DrawTree();
#endif
}*/

void Node::GetQuad(){
	if(this->ObjectCollector.size() > 1 && this->depthLevel < MAX_DEPTH){
		this->AddNode();
		for(int i = 0; i < 4; i++){
			for(vector<Coords>::iterator iter = this->ObjectCollector.begin(); iter != this->ObjectCollector.end(); iter++){
				if(iter->dimention.x < this->width && iter->dimention.y < this->height && 
					iter->position.x > this->child[i]->x && iter->position.x < (this->child[i]->x + this->child[i]->width) && 
					iter->position.y > this->child[i]->y && iter->position.y < (this->child[i]->y + this->child[i]->height)){
						this->child[i]->ObjectCollector.push_back(*iter);
						//this->DrawGrid();
						//TODO : delete object which have been already added to its child
				}
			}
			if(this->child[i]->ObjectCollector.size() > 1)
				this->child[i]->GetQuad();
		}
	}
#ifdef USE_CMD
	this->DrawTree();
#endif
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

void Node::DrawTree(){
	cout<<"\n\nNode ID : "<<this->nodeID;
	cout<<"\nNode X: "<<this->x<<", Node Y: "<<this->y;
	cout<<"\nNode W: "<<this->width<<", Node H: "<<this->height;
	cout<<"\nDepth: "<<this->depthLevel<<", Object Contains: "<<this->ObjectCollector.size();

	if(!this->isLeafNode)
		for(int i = 0; i < 4; i++){
			cout<<"\n\nNode ID : "<<this->child[i]->nodeID;
			cout<<"\nNode X: "<<this->child[i]->x<<", Node Y: "<<this->child[i]->y;
			cout<<"\nNode W: "<<this->child[i]->width<<", Node H: "<<this->child[i]->height;
			cout<<"\nDepth: "<<this->child[i]->depthLevel<<", Object Contains: "<<this->child[i]->ObjectCollector.size();
			//cout<<"\n\nChild["<<i<<"] :";
			//this->child[i]->DrawTree();
		}

	cout<<"\n";
}