#include "..\common.h"
#include "..\AppWindow.h"

#include "DXCore.h"

// http://3dapi.com/bs11_2d_basic/

BOOL DXCore::Initalize(HWND hWnd) {
	m_hWnd = hWnd;
	HRESULT hr;
	
	// IDirect9 Initalize
	pd3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pd3d9) return FALSE;

	// IDirectx9 Device Initalize
	D3DFORMAT format=D3DFMT_R5G6B5; //For simplicity we'll hard-code this for now.
	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp,sizeof(D3DPRESENT_PARAMETERS));
	pp.BackBufferCount= 1;  //We only need a single back buffer
	pp.MultiSampleType=D3DMULTISAMPLE_NONE; //No multi-sampling
	pp.MultiSampleQuality=0;                //No multi-sampling
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;  // Throw away previous frames, we don't need them
	pp.hDeviceWindow=hWnd;  //This is our main (and only) window
	pp.Windowed = TRUE;
	pp.Flags=0;            //No flags to set
	pp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT; //Default Refresh Rate
	pp.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;   //Default Presentation rate
	pp.BackBufferFormat=format;      //Display format
	pp.EnableAutoDepthStencil=FALSE; //No depth/stencil buffer
	
	hr = pd3d9->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&pp,
		&pd3dDevice);
	if (FAILED(hr)) return FALSE;

	return TRUE;
}

BOOL DXCore::CreateSprite(LPD3DXSPRITE *sprite) {
	D3DXCreateSprite(pd3dDevice, sprite);
	return TRUE;
}

BOOL DXCore::LoadTexture(const TCHAR *path, LPDIRECT3DTEXTURE9 *pTexture) {
	//D3DXCreateTextureFromFile(pd3dDevice, path, pTexture);
	D3DXIMAGE_INFO pImgInf;
	if (FAILED(D3DXCreateTextureFromFileEx(pd3dDevice, path,
		D3DX_DEFAULT, D3DX_DEFAULT, 
		1,	// Mip level (2D에서는 반드시 1, 3D는 D3DX_DEFAULT)
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 0x00000001, 0x00000001,
		0x00000000,	// 컬러 키
		&pImgInf,
		NULL,
		pTexture))) return FALSE;

	return TRUE;
}

BOOL DXCore::Release() {


	return TRUE;
}

IDirect3D9* DXCore::GetD3D9() {
	return pd3d9;
}
IDirect3DDevice9* DXCore::GetD3D9Device() {
	return pd3dDevice;
}