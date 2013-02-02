#include "..\Draw\DrawImpl.h"

class GDI:public DrawImpl {
public:
	BOOL Initialize();

	virtual BOOL DrawNote(int x, int y, int key);
	virtual BOOL DrawLine(int x, int y, int wid);

	void setHDC(HDC *hdc);
private:
	HDC *dc;
};