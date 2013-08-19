#pragma once

#include <vector>
using namespace std;

class SkinElement {
protected:
		// (NULL)	gr	x	y	w	h	div_x	div_y	cycle	timer	op1	op2	op3
	vector <int> src_vec;

	// (NULL)	time	x	y	(5)w	h	acc	a	r	(10)g	b	blend	filter	angle	center	loop
	vector <vector <int>> dst_vec;

	vector <int> op;

	int frame;		// total frame (src)
	int cycle;		// 애니메이션 한 번의 주기 시간 (src)
	int getNowFrame();

	vector <int> src_res; // SRC - x,y,wid,hei
	vector <int> dst_res; // DST - x,y,wid,hei,a,r,g,b,angle,center_x,center_y

	int srcTime, dstTime;
	bool dstLoop;

	bool loop;		// dst loop
	vector <int> _skinele;

public:
	SkinElement();
	
	void set_src(vector <int> elem);
	void push_dst(vector <int> elem);

	int src_gr();

	int src_timer();
	int dst_timer();
	void set_srcTime(int time);
	void set_dstTime(int time);
	int procSrc();
	int procDst();
	void getDrawingInfo(vector <int> &src, vector <int> &dst);
	void set_wid(int v);
	void set_hei(int v);

	//void set_srcop(vector <int> v);
	
	void set_op(vector <int> v);
	void add_op(vector <int> v);
	void add_op(int v);
	void clear_op();
	vector <int> get_op();
};

// consts

#define SRC_RES_X		0
#define SRC_RES_Y		1
#define SRC_RES_WID		2
#define SRC_RES_HEI		3

#define DST_TIME	1
#define DST_X		2
#define DST_Y		3
#define DST_WID		4
#define DST_HEI		5
#define DST_A		7
#define DST_R		8
#define DST_G		9
#define DST_B		10
#define DST_ANGLE	13
#define DST_CENTER	14

#define DST_RES_ISVALID		0
#define DST_RES_X			1
#define DST_RES_Y			2
#define DST_RES_WID			3
#define DST_RES_HEI			4
#define DST_RES_A			5

#define DST_RES_R			6
#define DST_RES_G			7
#define DST_RES_B			8
#define DST_RES_ANGLE		9
#define DST_RES_CENTER		10
#define DST_RES_CENTER_X	11
#define DST_RES_CENTER_Y	12

#define DST_CENTER_CENTER		0
#define DST_CENTER_TOPLEFT		1
#define DST_CENTER_TOPRIGHT		2
#define DST_CENTER_BOTTOMRIGHT	3
#define DST_CENTER_BOTTOMLEFT	4
