#include <d3d9.h>

#include "..\DX\DXCore.h"
#include "..\DX\DXTexture.h"
#include "..\Game\Option.h"
#include "..\Game\Timer.h"
#include "SkinElement.h"
#include "SkinParser.h"

class SkinManager {
protected:
	long l_tick;
	
public:
	DXTexture img_texture[100];
	int img_cnt;
	BOOL LoadTexture(TCHAR *fpath);

	DXCore *m_DXCore;
	SkinElement *m_SkinElement;
	Option *m_Option;
	Timer *m_Timer;
	int SkinElementCnt;
public:
	void Initalize(DXCore *dx, SkinParser *sp);
	BOOL ReadLR2Skin(TCHAR *path);	// csv format
	void InitalizeTick(long tick);
	void SetTick(long tick);
	BOOL Draw(LPD3DXSPRITE m_Sprite);
};