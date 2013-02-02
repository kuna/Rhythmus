#include "..\common.h"
#include "SkinManager.h"

void SkinManager::Initalize(DXCore *dx, SkinParser *sp) {
	m_DXCore = dx;

	SkinElementCnt = sp->skinElementCnt;
	m_SkinElement = sp->m_se;
	
	for (int i=0; i<sp->data_img_cnt; i++) {
		TCHAR path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, path);
		wcscat(path, L"\\");
		wcscat(path, sp->data_img_path[i]);
		if (!LoadTexture( path )) {
			// ERROR
			MessageBox(NULL, path, L"ERROR: Cant load texture", 0);
			img_cnt--;
		}
	}
}

BOOL SkinManager::LoadTexture(TCHAR *fpath) {
	return img_texture[img_cnt++].LoadTexture(fpath, m_DXCore->GetD3D9Device());
}

BOOL SkinManager::Draw(LPD3DXSPRITE m_Sprite) {
	/** SPRITE SHOULE HAVE BEGUN DRAWING */
	DXTexture *txt;
	RECT src_rect;
	RECT dst_rect;
	double rotation;
	D3DXCOLOR rgba = D3DXCOLOR(1,1,1,1);

	//SkinElementCnt
	for (int i=0; i<85; i++) {
		SkinElement se = m_SkinElement[i];
		if (!m_Option->CheckOption(se.get_op(), se.get_nop())) continue;
		if (se.src_gr() >= img_cnt) continue;
		txt = &img_texture[ se.src_gr() ];

		// if wid<0 then fullsize
		if (se.src_wid() < 0) se.set_wid( txt->GetRect()->right );
		if (se.src_hei() < 0) se.set_hei( txt->GetRect()->bottom );
		SetRect(&src_rect, se.src_x(), se.src_y(), se.src_x()+se.src_wid(), se.src_y()+se.src_hei());
		se.getDrawingInfo(op, elem);
		
		m_Sprite->Draw(txt->GetTexture(), &src_rect, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(elem[2], elem[3], 0), rgba);
	}

	return TRUE;
}

void SkinManager::InitalizeTick(long tick) {
	for (int i=0; i<SkinElementCnt; i++)
		m_SkinElement[i];
}

void SkinManager::SetTick(long tick) {
	for (int i=0; i<SkinElementCnt; i++)
		m_SkinElement[i].setTime(tick);
}