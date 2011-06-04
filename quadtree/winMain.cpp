#ifndef _STDAFX
#include "stdafx.h"
#endif
#ifndef _DX
#include "dx.h"
#endif
#ifndef _QUADTREE
#include "quadTree.h"
#endif

dxInit* dx = new dxInit();
QuadTree* qt = new QuadTree();

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

	dx->SetParameters(hWnd);
	qt->RunOnce(hWnd);

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
			qt->RunInLoop();
		}
	}

	return msg.wParam;
}