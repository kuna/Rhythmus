#pragma once

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "winmm.lib")
#include <d3d9.h>
#include <d3dx9.h>

class DXCore {
protected:
	HWND m_hWnd;

	IDirect3D9*		pd3d9;
	IDirect3DDevice9* pd3dDevice;
	IDirect3DSurface9* BackBuff; 
public:
	BOOL Initalize(HWND hWnd);
	BOOL Release();

	BOOL LoadTexture(const TCHAR *path, LPDIRECT3DTEXTURE9 *pTexture);
	BOOL CreateSprite(LPD3DXSPRITE *sprite);

	IDirect3D9* GetD3D9();
	IDirect3DDevice9* GetD3D9Device();
};