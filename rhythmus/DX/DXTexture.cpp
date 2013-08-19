#include "DXTexture.h"

BOOL DXTexture::LoadTexture(TCHAR *path, IDirect3DDevice9* pd3dDevice)
{
	D3DXIMAGE_INFO pImgInf;
	if (FAILED(D3DXCreateTextureFromFileEx(pd3dDevice, path,
		D3DX_DEFAULT, D3DX_DEFAULT, 
		1,	// Mip level (2D에서는 반드시 1, 3D는 D3DX_DEFAULT)
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 0x00000001, 0x00000001,
		0x00000000,	// 컬러 키
		&pImgInf,
		NULL,
		&pTexture))) return FALSE;

	txtRect.right = pImgInf.Width;
	txtRect.bottom = pImgInf.Height;

	return TRUE;
}

RECT* DXTexture::GetRect() {
	return &txtRect;
}

LPDIRECT3DTEXTURE9 DXTexture::GetTexture() {
	return pTexture;
}