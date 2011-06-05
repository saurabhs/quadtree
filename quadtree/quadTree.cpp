#include "quadTree.h"

QuadTree::QuadTree(LPDIRECT3DDEVICE9 device){
	d3ddev = device;

	collide = 0;

	for(int i = 0; i < NUM_OF_SQUARES; i++)
		model[i] = new Model();
}

void QuadTree::RunOnce(HWND hwnd){
	//init model
	for(int i = 0; i < NUM_OF_SQUARES; i++){
		//model material
		ZeroMemory(&material, sizeof(D3DMATERIAL9));
		material.Diffuse.r = material.Ambient.r = 1.0f; //(GetRandMax(10) / 10.0f);
		material.Diffuse.g = material.Ambient.g = 1.0f; //(GetRandMax(10) / 10.0f);
		material.Diffuse.b = material.Ambient.b = 0.0f; //(GetRandMax(10) / 10.0f);
		material.Diffuse.a = material.Ambient.a = 1.0f;

		//model and its bounding box's position
		float xx = (float)GetRandMinMax2(0, MAX_X);
		float yy = (float)GetRandMinMax2(0, MAX_Y);
		model[i]->box.position = D3DXVECTOR2(xx, yy);
		
		model[i] = new Model(NULL, D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 1), material, model[i]->box.position, model[i]->box.dimension);
	}

	DrawSquare();

	//init quad tree
	//GetQuadTreetNode();
}

void QuadTree::RunInLoop(){
	RenderSquare();
	SimulateSquare();
	Physics2D();

	//keep updating quad tree with square's pos (???)
	//GetQuadTreetNode();

	Debug();
}

Coords QuadTree::GetModelsData(int index){
	int x = (int)model[index]->box.position.x;
	int y = (int)model[index]->box.position.y;
	int w = (int)model[index]->box.dimension.x;
	int h = (int)model[index]->box.dimension.y;

	return Coords(x, y, w, h);
}

Model* QuadTree::GetModel(int index){
	return model[index];
}

void QuadTree::GetMinMax(Model* model){
	D3DXVECTOR3 Min, Max;
	void* v;

	model->mesh->LockVertexBuffer(0, &v);
	D3DXComputeBoundingBox((D3DXVECTOR3*)v, model->mesh->GetNumVertices(), D3DXGetFVFVertexSize(model->mesh->GetFVF()), &Min, &Max);
	model->mesh->UnlockVertexBuffer();

	D3DXVECTOR2 _min = D3DXVECTOR2(Min.x, Min.y);
	D3DXVECTOR2 _max = D3DXVECTOR2(Max.x, Max.y);

	model->box.dimension = (_max - _min);
}

void QuadTree::DrawSquare(){
	for(int i = 0; i < NUM_OF_SQUARES; i++){
		D3DXCreateBox(d3ddev, 1.5f, 1.5f, 0.01f, &model[i]->mesh, NULL);		//TODO: add random x and y
		GetMinMax(model[i]);
	}
}

void QuadTree::RenderSquare(){
	for(int i = 0; i < NUM_OF_SQUARES; i++){
		D3DXMATRIX Bbox;
		D3DXMATRIX matTranslate, matRotation, matWorld;

		d3ddev->SetMaterial(&material);
		D3DXMatrixTranslation(&matTranslate, model[i]->box.position.x, model[i]->box.position.y, 0);
		
		d3ddev->SetTransform(D3DTS_WORLD, &matTranslate);
		d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		d3ddev->SetMaterial(&model[i]->matModel);
		model[i]->mesh->DrawSubset(0);
	}
}

void QuadTree::SimulateSquare(){
	for(int i = 0; i < NUM_OF_SQUARES; i++){
		if(model[i]->box.position.x < MAX_X){
			float temp = GetRandMax(5) / 10.0f;
			model[i]->box.position.x += temp;
		}
		else
			model[i]->box.position.x = -MAX_X;
	}
}

bool QuadTree::CollisionDetection(Model* modelA, Model* modelB){
	D3DXVECTOR3 gap = modelA->box.position - modelB->box.position;
	gap.x = fabs(gap.x);
	gap.y = fabs(gap.y);
	//gap.z = fabs(gap.z);

	D3DXVECTOR3 maxGap = (modelA->box.dimension + modelB->box.dimension) * 0.5f;
	maxGap.x = fabs(maxGap.x);
	maxGap.y = fabs(maxGap.y);
	//maxGap.z = fabs(maxGap.z);

	if((gap.x < maxGap.x) && (gap.y < maxGap.y)) //&& (gap.z < maxGap.z))
		return true;

	return false;
}

void QuadTree::CollisonResponse(Model* modelA, Model* modelB){
	D3DMATERIAL9 matCollide;
	ZeroMemory(&matCollide, sizeof(D3DMATERIAL9));
    matCollide.Diffuse.r = matCollide.Ambient.r = 1.0f;
    matCollide.Diffuse.g = matCollide.Ambient.g = 0.0f;
    matCollide.Diffuse.b = matCollide.Ambient.b = 0.0f;
    matCollide.Diffuse.a = matCollide.Ambient.a = 1.0f;

	if(CollisionDetection(modelA, modelB)){
		modelA->matModel = matCollide;
		modelB->matModel = matCollide;
	}
	else{
		modelA->matModel = material;
		modelB->matModel = material;
	}
}

void QuadTree::Physics2D(){
	//for(int i = 0; i < NUM_OF_SQUARES; i++)
	//	for(int j = 0; j < NUM_OF_SQUARES; j++)
	//		if(i != j)
	//			CollisonResponse(model[i], model[j]);
}

void QuadTree::Debug(){

}