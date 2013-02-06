#include "..\common.h"
#include "SkinManager.h"

void SkinManager::Initalize(DXCore *dx, SkinParser *sp, Timer *m, Option *option) {
	m_DXCore = dx;

	SkinElementCnt = sp->skinElementCnt;
	m_SkinElement = sp->m_se;
	m_Timer = m;
	m_Option = option;
	
	for (int i=0; i<sp->data_img_cnt; i++) {
		TCHAR path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, path);
		wcscat(path, L"\\");
		wcscat(path, sp->data_img_path[i]);
		if (!LoadTexture( path )) {
			// ERROR
			printf("ERROR: Cant load texture: %S\n", path);
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
	for (int i=0; i<SkinElementCnt; i++) {
		SkinElement se = m_SkinElement[i];
		if (!m_Option->CheckOption(se.get_op())) continue;
		if (se.src_gr() >= img_cnt) continue;
		
		se.set_srcTime( m_Timer->getTime( se.src_timer() ) );
		se.set_dstTime( m_Timer->getTime( se.dst_timer() ) );
		int t = se.src_gr();
		txt = &img_texture[ se.src_gr() ];

		vector <int> src, dst;
		se.getDrawingInfo(src, dst);
		if ( dst[DST_RES_ISVALID] < 0) continue;
		
		// if wid<0 then fullsize
		if (src[SRC_RES_WID] < 0) src[SRC_RES_WID] = txt->GetRect()->right;
		if (src[SRC_RES_HEI] < 0) src[SRC_RES_HEI] = txt->GetRect()->bottom;
		SetRect(&src_rect, src[SRC_RES_X], src[SRC_RES_Y], 
			src[SRC_RES_X]+src[SRC_RES_WID], src[SRC_RES_Y]+src[SRC_RES_HEI]);
		
		if (dst[DST_RES_WID] < 0) {
			dst[DST_RES_X] += dst[DST_RES_WID];
			dst[DST_RES_WID] *= -1;
		}
		if (dst[DST_RES_HEI] < 0) {
			dst[DST_RES_Y] += dst[DST_RES_HEI];
			dst[DST_RES_HEI] *= -1;
		}

		D3DXVECTOR2 scaleCentre=D3DXVECTOR2(0.0f,0.0f);
		D3DXVECTOR2 scaling( dst[DST_RES_WID]/(float)src[SRC_RES_WID], dst[DST_RES_HEI]/(float)src[SRC_RES_HEI] );
		D3DXVECTOR2 rotateCentre=D3DXVECTOR2(dst[DST_RES_CENTER_X],dst[DST_RES_CENTER_Y]);
		D3DXVECTOR2 position=D3DXVECTOR2(dst[DST_RES_X], dst[DST_RES_Y]);
		float rotation=dst[DST_RES_ANGLE]/(float)360*2*3.14f;

		// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
		D3DXMATRIX mat;
		D3DXMatrixTransformation2D(&mat,&scaleCentre,0.0,&scaling,&rotateCentre,rotation,&position);
		m_Sprite->SetTransform(&mat);

		m_Sprite->Draw(txt->GetTexture(), &src_rect, NULL, NULL, rgba);
	}

	return TRUE;
}
