#pragma once

#include <vector>
using namespace std;

class SkinElement {
protected:
		// (NULL)	gr	x	y	w	h	div_x	div_y	cycle	timer	op1	op2	op3
	vector <int> src_vec;
	//vector <int> src_op;

	// (NULL)	time	x	y	(5)w	h	acc	a	r	(10)g	b	blend	filter	angle	center	loop
	vector <vector <int>> dst_vec;

	vector <int> op;
	vector <int> nop;

	int frame;		// total frame
	int cycle;		// 애니메이션 한 번의 주기 시간
	int time;		// now gametime
	int getNowFrame();

	bool loop;		// dst loop

public:
	void set_src(vector <int> elem);
	void push_dst(vector <int> elem);
	void getDrawingInfo(vector <int> op, vector <int> &elem);	// op case -> rotation, rgb, also in case

	int src_gr();
	int src_x();
	int src_y();
	int src_wid();
	int src_hei();

	int src_timer();
	int dst_timer();
	void set_srcTime(int time);
	void set_dstTime(int time);

	void set_wid(int v);
	void set_hei(int v);

	//void set_srcop(vector <int> v);
	
	void set_op(vector <int> v);
	void add_op(vector <int> v);
	void add_op(int v);
	void clear_op();
	void set_nop(vector <int> v);
	void add_nop(vector <int> v);
	void add_nop(int v);
	void clear_nop();

	vector <int> get_op();
	vector <int> get_nop();
};