#include "DrawImpl.h"

class DrawManager {
public:
	void setDrawObject(DrawImpl *d);
	int DrawScreen(int milisec);
	void setPlayObject(BMSPlay *p);
private:
	DrawImpl *di;
	BMSPlay *bPlay;
	int speed;
	int screenHeight;
};