#include <conio.h>
#include <time.h>
#ifndef _STDAFX
	#include "stdafx.h"
#endif

#ifndef _NODE
	#include "node.h"
#endif

#ifdef USE_CMD

int main(){
	Node* foo = new Node();
	
	int id = 0;
	Coords Ball = Coords(400, 400, 32, 32);
	Coords Ball2 = Coords(10, 10, 32, 32);
	Coords Ball3 = Coords(300, 100, 32, 32);
	Coords Ball4 = Coords(300, 200, 32, 32);
	Coords Ball5 = Coords(268, 168, 32, 32);
	Coords Ball6 = Coords(68, 368, 32, 32);

	foo->AddCoordsToRoot(Ball);
	foo->AddCoordsToRoot(Ball2);
	foo->AddCoordsToRoot(Ball3);
	foo->AddCoordsToRoot(Ball4);
	foo->AddCoordsToRoot(Ball5);
	//foo->AddCoordsToRoot(Ball6);

	foo->Foobar();

	/*int BAR = 25;
	Coords test[25];

	for(int i = 0; i < BAR; i++){
		test[i] = Coords(GetRandMax(512), GetRandMax(512), 32, 32);
		foo->AddCoordsToRoot(test[i]);
	}*/

	//foo->GetQuad();

	_getch();
}

#else

#ifndef _DX
#include "dx.h"
#endif
#ifndef _QUADTREE
#include "quadTree.h"
#endif
#ifndef _GRAPHICAL_QUADTREE
#include "graphicalQuadTree.h"
#endif

dxInit* dx = new dxInit();
QuadTree* qt;
GraphicalQuadTree* gqt;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN:
			switch(wParam){
				case VK_ESCAPE:
					PostQuitMessage(0);
					return 0;
			}
	}

	return DefWindowProcW(hWnd, message, wParam, lParam);
}

int main(){
	HWND	hWnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize		=	sizeof(WNDCLASSEX);
	wc.style		=	CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	=	(WNDPROC)WndProc;
	wc.hInstance	=	GetModuleHandle(NULL);
	wc.hCursor		=	LoadCursorW(NULL, IDC_ARROW);
	wc.lpszClassName=	L"QuadTree";

	if(!RegisterClassEx(&wc)) 
		MessageBoxW(NULL, L"RegisterClass Failed", L"ERROR...", MB_OK);

	if(!(hWnd = CreateWindowEx(NULL, L"QuadTree", L"QuadTree", WS_OVERLAPPEDWINDOW, 0, 0,
								 SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, wc.hInstance, NULL)))
		MessageBoxW(NULL, L"RegisterClass Failed", L"ERROR...", MB_OK);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	srand((unsigned) time(NULL));

	dx->SetParameters(hWnd);
	dx->Light();
	dx->SetCamera();

	qt = new QuadTree(dx->getDevice());
	qt->RunOnce(hWnd);

#ifdef GUI_TREE
	Node* node = new Node(-512, -512, 1024, 1024, dx->getDevice());
#else
	Node* node = new Node(-512, -512, 1024, 1024);
#endif

	for(int i = 0; i < NUM_OF_SQUARES; i++) 
		node->AddCoordsToRoot(qt->GetModel(i));

	node->BuildQuad();

	srand(GetTickCount());

	MSG msg;
	
	while(TRUE){
		if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)){
			if(msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			dx->ClearBackBuffer();
			dx->Begin();
			{
				node->MoveNode();
				//dx->SetCamera();
				qt->RenderSquare();
				qt->SimulateSquare();
#ifdef GUI_TREE
				node->DrawGrid();
#endif
			}
			dx->End();
		}
	}

	return msg.wParam;
}

#endif