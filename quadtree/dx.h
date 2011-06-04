#ifndef _STDAFX
#include "stdafx.h"
#endif

#ifndef _DX
#define _DX

class dxInit{
	LPDIRECT3D9			m_d3d;					//direct3d obj
	LPDIRECT3DDEVICE9	m_d3ddev;				//direct3d device
	HWND				m_hWnd;					//handle to window obj	

	LPD3DXSPRITE		mSprite;				//D3DXSprite obj for rendering sprites
	LPD3DXFONT			mFont;					//D3DXFont obj for displaying texts

public:
						dxInit();
						~dxInit(){ /* */ }

	bool				SetParameters(HWND);
	bool				ClearBackBuffer();

	void				SetCamera(float z = 0);
	void				Light();

	bool				Begin();
	bool				End();

	void				LoadSprite(string filename, LPDIRECT3DTEXTURE9* texture);

	void				DrawSprite(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR4 rect4, D3DXVECTOR2 coords, float angle = 0.0f);
	void				DrawSprite(LPDIRECT3DTEXTURE9 texture, int xLeft, int yTop, int xRight, int yBottam, D3DXVECTOR2 coords);

	void				ReleaseD3D9();

	LPDIRECT3DDEVICE9	getDevice(){ return m_d3ddev; }
	HWND				getHandle(){ return m_hWnd; }

	LPD3DXSPRITE		getSprite(){ return mSprite; }

	//fonts
	void				CreateFontObject(LPDIRECT3DDEVICE9 dev, int fontHeight = 25, UINT fontWidth = FW_MEDIUM, LPCWSTR fontName = L"Calibri");
	void				DrawTextOverlay(string Text, DWORD format, int xLeft, int yTop, int xRight, int yBottom,
											   int red = 255, int green = 255, int blue = 255);
	LPD3DXFONT			GetFont(){ return mFont; }
};

/*
template<class convert>
std::string toString(const convert &conv){
	std::ostringstream oss;
	oss.clear();
	oss << conv;
	return oss.str();
}
*/

#endif