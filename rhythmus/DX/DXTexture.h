#include <d3dx9.h>

class DXTexture {
protected:
	RECT txtRect;
	LPDIRECT3DTEXTURE9 pTexture;
public:
	BOOL LoadTexture(TCHAR *path, IDirect3DDevice9* pd3dDevice);
	RECT* GetRect();
	LPDIRECT3DTEXTURE9 GetTexture();
};