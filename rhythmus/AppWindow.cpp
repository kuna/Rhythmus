#include "common.h"
#include "AppWindow.h"

#include "BMS\BMSParser.h"
#include "BMS\BMSPlay.h"
#include "Draw\DrawManager.h"
#include "GDI\GDI.h"
#include "DX\DXCore.h"
#include "Skin\SkinManager.h"
#include "Skin\SkinParser.h"
#include "Skin\SkinElement.h"

LPD3DXSPRITE g_pSprite9 = NULL;
LPDIRECT3DTEXTURE9 texture = NULL;
DXTexture dtexture;

BMSParser bParser;
BMSPlay bPlay;
DrawManager dm;
DXCore dx;
SkinParser sp;
SkinManager sm;
SkinElement se[1024];

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);
HWND hWnd;
HINSTANCE g_hInst;
void render_frame();

// temp
HWND hButton;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	//save value
	g_hInst = hInstance;

	WNDCLASS WndClass;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=(WNDPROC)WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	
	RECT wndRect;
	SetRect(&wndRect, 0, 0, windowWidth, windowHeight);
	AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd=CreateWindow(lpszClass,lpszTitle,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,wndRect.right-wndRect.left,wndRect.bottom-wndRect.top,
		  NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	

	/* INITALIZE */
	if (!dx.Initalize(hWnd)) {
		MessageBox(hWnd, L"FAILED TO INITALIZE DIRECTX", L"ERROR", 0);
		PostQuitMessage(-1);
	}

	// load texture
	dtexture.LoadTexture(L"basic.tga", dx.GetD3D9Device());
	dx.CreateSprite(&g_pSprite9);

	sp.Initialize(se);
	sp.ReadCSVFile(L"REMI-S\\Play\\7Key_Left.csv");
	sm.Initalize(&dx, &sp);
	sm.InitalizeTick(GetTickCount());

	/* GAME LOOP */
	MSG msg;
	while (1) {
		while(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) break;

		// render part
		sm.SetTick(GetTickCount());
		render_frame();
	}

	/* CLEANUP N RELEASE */
	dx.Release();

    return (int)msg.wParam;
}

double t = 0;
double b = 0;
bool init = false;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch(iMessage) {
	case WM_CREATE:
		hButton = CreateWindow(L"button", L"Load Song", WS_CHILD | WS_VISIBLE, 10, 10, 100, 30, hWnd, (HMENU)1001, g_hInst, 0);
		break;
	case WM_COMMAND:
		if (wParam == 1001) {
			// load BMS
			BOOL res = bParser.ParseBMSFile(L"D:\\Game\\LR2\\SP1\\約束\\yaku_7.bme");
			if (res == TRUE) {
				// do progress
				bPlay.Initialize(&bParser);
				bPlay.initalizeTimeTable();

				MessageBox(hWnd, L"Success", 0, 0);
				dm.setPlayObject(&bPlay);
				init = true;
			} else {
				MessageBox(hWnd, L"Error", 0, 0);
			}
		}
		break;
	case WM_PAINT:
		/*
		{
			PAINTSTRUCT ps;
			HDC dc;
			GDI *g = new GDI();
			DrawImpl *di = g;

			BeginPaint (hWnd, &ps);

			dc = CreateCompatibleDC(ps.hdc);
			HBITMAP hbb = CreateCompatibleBitmap(dc, 640, 480);
			SelectObject(dc, hbb);

			HBRUSH hb_back;
			RECT r;
			hb_back = CreateSolidBrush(RGB(255, 255, 255));
			SetRect(&r, 0, 0, 640, 480);
			FillRect(dc, &r, hb_back);

			g->setHDC(&dc);
			//di->DrawNote(100, 10, 4);
			//di->DrawNote(400, 20, 0);

			TCHAR lp[256];
			swprintf(lp, L"Time %f, Beat %f\n", t, b);
			OutputDebugString(lp);

			if (init) {
				dm.setDrawObject(di);
				dm.DrawScreen(t*1000);
			}

			BitBlt(ps.hdc, 0,  0, 640, 480, dc, 0,  0, SRCCOPY);

			DeleteDC(dc);
			DeleteObject(hbb);

			EndPaint(hWnd, &ps);
			delete g;
		}*/
		break;
	case WM_MOUSEMOVE:
		if (!init) break;
		t += 0.01;
		b = bPlay.GetBeatFromTime((int)(t * 1000));
		//InvalidateRect(hWnd, 0, FALSE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

void render_frame() {
	RECT r;
	SetRect(&r, 0, 0, 640, 480);

	dx.GetD3D9Device()->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	dx.GetD3D9Device()->BeginScene();

	D3DXVECTOR3 m_vPos(0, 0, 0);
	D3DXVECTOR3 m_vCenter(0, 0, 0);

	// 매번 그릴 때마다
	// 1. 텍스쳐의 surface 먼저 생성할 것 (D3DXCOLOR)
	// 2. 텍스쳐 surface로부터 src~dst 그리기

	/*m_vPos.x = 0.5f;
	m_vPos.y = 0.5f;
	m_vCenter.x = 1;
	m_vCenter.y = 1;
	*/
	g_pSprite9->Begin( D3DXSPRITE_ALPHABLEND );

	/*
	D3DXVECTOR2 spriteCentre=D3DXVECTOR2(0.0f,0.0f);
	// Screen position of the sprite
	D3DXVECTOR2 trans=D3DXVECTOR2(50.0f,80.0f);
	// Rotate based on the time passed
	float rotation=timeGetTime()/500.0f;
	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;
	D3DXVECTOR2 scaling(2.0f,1.0f);
	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&spriteCentre,rotation,&trans);
	g_pSprite9->SetTransform(&mat);
	g_pSprite9->Draw(texture, &r, 
		0, 
		0, D3DXCOLOR(1,1,1,1));*/
	sm.Draw(g_pSprite9);

	//g_pSprite9->Draw(dtexture.GetTexture(), &r, 
	//	&m_vCenter, 
	//	&m_vPos, D3DXCOLOR(1,1,1,1));

	g_pSprite9->End();

	dx.GetD3D9Device()->EndScene();
	dx.GetD3D9Device()->Present(0, 0, 0, 0);
}