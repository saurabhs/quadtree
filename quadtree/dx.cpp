#include "dx.h"

dxInit::dxInit(){
	m_d3d		=	NULL;
	m_d3ddev	=	NULL;
	mSprite		=	NULL;
}

bool dxInit::SetParameters(HWND hwnd){
	HRESULT hr = 0;
	m_hWnd = hwnd;

	m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if(FAILED(m_d3d)) ERR_MSG(L"Unable to initialize Direct3D");

	D3DPRESENT_PARAMETERS d3dpp;	
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	D3DDISPLAYMODE d3ddm;
	m_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	d3dpp.Windowed					=	TRUE;
	d3dpp.SwapEffect				=	D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow				=	m_hWnd;
	d3dpp.BackBufferFormat			=	D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth			=	SCREEN_WIDTH;
	d3dpp.BackBufferHeight			=	SCREEN_HEIGHT;

	hr = m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
							 D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_d3ddev);
	
	if(FAILED(hr)) ERR_MSG(L"No Hardware Support");

	//create sprite
	D3DXCreateSprite(m_d3ddev, &mSprite);

	return TRUE;
}

//clean the backbuffer with black color
bool dxInit::ClearBackBuffer(){
	if(FAILED(m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, 0xff002864, 1.0f, 0)))
		return FALSE;
	return TRUE;
}

void dxInit::SetCamera(float z){
	D3DXMATRIX matView;
    D3DXMatrixLookAtLH(&matView, 
					   &D3DXVECTOR3 (0.0f, 0.0f, -174.0f + z),
					   &D3DXVECTOR3 (0.0f, 0.0f, 0.0f),
					   &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));
    m_d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView

    D3DXMATRIX matProjection;
    D3DXMatrixPerspectiveFovLH(&matProjection, D3DXToRadian(45), (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, 1.0f, 1000.0f);
    m_d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);

	m_d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));
}

// this is the function that sets up the lights and materials
void dxInit::Light(){
    D3DLIGHT9 light;    // create the light struct

    ZeroMemory(&light, sizeof(light));
    //light.Type = D3DLIGHT_DIRECTIONAL;
    light.Type = D3DLIGHT_POINT;
    light.Diffuse.r = light.Ambient.r = 0.8f;
    light.Diffuse.g = light.Ambient.g = 0.8f;
    light.Diffuse.b = light.Ambient.b = 0.8f;
    light.Diffuse.a = light.Ambient.a = 1.0f;	

    //D3DVECTOR vecDirection = {1.0f, 1.5f, 0.0f};
    //light.Direction = vecDirection;

	light.Position = D3DXVECTOR3(0, 0, 0);
	light.Attenuation0 = 0.5f;
	light.Range = 500.0f;

    m_d3ddev->SetLight(0, &light);
    m_d3ddev->LightEnable(0, TRUE);
}

//begin scene
bool dxInit::Begin(){
	if(FAILED(m_d3ddev->BeginScene()))
		return FALSE;
	return TRUE;
}

//end scene and set Present Parameters
bool dxInit::End(){
	if(FAILED(m_d3ddev->EndScene()))
		return FALSE;
	if(FAILED(m_d3ddev->Present(NULL, NULL, NULL, NULL)))
		return FALSE;
	return TRUE;
}

//load image files
void dxInit::LoadSprite(std::string filename, LPDIRECT3DTEXTURE9 *texture){
	D3DXCreateTextureFromFileExA(m_d3ddev, filename.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 
									  D3DX_FROM_FILE, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT,
									  D3DX_DEFAULT, 0, NULL, NULL, texture);
}

//draw sprites
void dxInit::DrawSprite(LPDIRECT3DTEXTURE9 texture, int xLeft, int yTop, int xRight, int yBottam, D3DXVECTOR2 coords){
	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 position(coords.x, coords.y, 0.0f);
	RECT rect;
	SetRect(&rect, xLeft, yTop, xRight, yBottam);
	mSprite->Draw(texture, &rect, &center, &position, D3DCOLOR_XRGB(255, 255, 255));

	m_d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	m_d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
}

//draw sprite which can be rotate on z-axix
void dxInit::DrawSprite(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR4 rect4, D3DXVECTOR2 coords, float angle){
	D3DXMATRIX mat;
	D3DXMatrixRotationZ(&mat, angle);
	mSprite->SetTransform(&mat);

	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 position(coords.x, coords.y, 0.0f);
	RECT rect;
	SetRect(&rect, rect4.x, rect4.y, rect4.z, rect4.w);
	mSprite->Draw(texture, &rect, &center, &position, D3DCOLOR_XRGB(255, 255, 255));

	m_d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	m_d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
}

//initialize font object
void dxInit::CreateFontObject(LPDIRECT3DDEVICE9 dev, int fontHeight, UINT fontWidth, LPCWSTR fontName){
	D3DXCreateFont(m_d3ddev, fontHeight, 0, fontWidth, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
				DEFAULT_PITCH | FF_DONTCARE, fontName, &mFont);
}

//draw text
void dxInit::DrawTextOverlay(std::string text, DWORD format, int xLeft, int yTop, int xRight, int yBottom, int red, int green, int blue){
	RECT rect;
	SetRect(&rect, xLeft, yTop, xRight, yBottom);
	mFont->DrawTextA(NULL, text.c_str(), -1, &rect, format, D3DCOLOR_XRGB(red, green, blue));
}