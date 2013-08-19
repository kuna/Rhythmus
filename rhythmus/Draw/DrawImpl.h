#pragma once
#include "..\common.h"

class DrawImpl {
public:
	virtual BOOL DrawNote(int x, int y, int key) { return FALSE; };
	virtual BOOL DrawLine(int x, int y, int wid) { return FALSE; };
};