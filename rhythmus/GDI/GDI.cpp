#include "GDI.h"

void GDI::setHDC(HDC *hdc) {
	dc = hdc;
}

BOOL GDI::DrawNote(int x, int y, int key) {
	HBRUSH hb_back;
	RECT r;
	hb_back = CreateSolidBrush(RGB(112, 146, 190));
	SetRect(&r, x, y, x+40, y+8); 
	FillRect(*dc, &r, hb_back);
	DeleteObject(hb_back);

	return TRUE;
}

BOOL GDI::DrawLine(int x, int y, int wid) {
	MoveToEx(*dc, x, y, 0);
	LineTo(*dc, x+wid, y);
	return TRUE;
}