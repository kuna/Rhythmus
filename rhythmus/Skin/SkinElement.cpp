
#include "..\common.h"
#include "SkinElement.h"

#define SQUARE(x) ( (((x)*(x))+(x)*4)/5 )

SkinElement::SkinElement() {
	_skinele.push_back(2);
	_skinele.push_back(3);
	_skinele.push_back(4);
	_skinele.push_back(5);
	_skinele.push_back(7);
	_skinele.push_back(8);
	_skinele.push_back(9);
	_skinele.push_back(10);
	_skinele.push_back(13);
	_skinele.push_back(14);
}

void SkinElement::set_src(vector <int> elem) {
	src_vec = elem;
	
	frame = elem[5]*elem[6];
	cycle = elem[7];
}

void SkinElement::push_dst(vector <int> elem) {
	if (elem[6] <= 0) elem[6] = 1;	// divx
	if (elem[7] <= 0) elem[7] = 1;	// divy
	dst_vec.push_back(elem);

	// 첫 dst일 경우
	if (dst_vec.size() == 1) {
		// set loop
		loop = (dst_vec[0][15]>0);
	}
}

// SRC - x,y,wid,hei
int SkinElement::procSrc() {
	// init
	src_res.clear();

	if (cycle <= 0) {
		// src를 별도로 움직이지 않음
		src_res.push_back( src_vec[2] );
		src_res.push_back( src_vec[3] );
		src_res.push_back( src_vec[4] );
		src_res.push_back( src_vec[5] );
		return FALSE;
	}

	int t = srcTime%cycle;
	double t2 = cycle/(double)frame;
	int nf = (int)((double)t/t2);

	int xi = nf % src_vec[6];
	int yi = nf / src_vec[6];
	double nw = src_vec[4]/(double)src_vec[6];	// wid/divx
	double nh = src_vec[5]/(double)src_vec[7];	// hei/divy
	
	int x = src_vec[2]+(int)(nw*xi);
	int y = src_vec[3]+(int)(nh*yi);
	int w = (int)nw;
	int h = (int)nh;
	
	src_res.push_back(x);
	src_res.push_back(y);
	src_res.push_back(w);
	src_res.push_back(h);

	return TRUE;
}

// DST - (isdraw),x,y,wid,hei,a,r,g,b,angle,center_x,center_y
// (acc: 0 등속, 1 가속, 2 감속, 3 불연속)
int SkinElement::procDst() {
	// init
	dst_res.clear();

	int dst_time;
	if (loop) {
		int stime = dst_vec[0][15];					// 루프 시작점
		int etime = dst_vec[ dst_vec.size()-1 ][1];	// 루프 시간 끝 (끝)
		int looptime = stime - etime;		// 루프 시작점{
		if (looptime == 0) {
			dst_time = dstTime;
			if (dst_time > etime) dst_time = etime;
		} else {
			dst_time = (dstTime - stime) % looptime + stime;
		}
	} else {
		dst_time = dstTime;
	}

	int ndst = 0;
	for (int i=0; i<dst_vec.size(); i++) {
		if (dst_time > dst_vec[i][0]) {
			ndst = i;
			break;
		}
	}

	// 1st time 이전: dont draw
	if (dst_time < dst_vec[0][1]) {
		dst_res.push_back(-1);
		return FALSE;
	}  else {
		dst_res.push_back(TRUE);
	}
	// 1~2 .. end: 내적값 - (x,2,3,4,5,7,8,9,10,13,14,14)
	for (int i=1; i<dst_vec.size(); i++) {
		if (dst_time >= dst_vec[i-1][1] && dst_time < dst_vec[i][1]) {
			dst_res[0]=0;
			int accl = dst_vec[i-1][7];
			int dist = dst_vec[i][1] - dst_vec[i-1][1];
			double a = (dst_time - dst_vec[i-1][1]) / (double)dist; // 0 div
			double b = 1-a;
			if (accl == 1) {		// 가속
				a = SQUARE(a);
				b = 1-a;
			} else if (accl == 2) {	// 감속
				b = SQUARE(b);
				a = 1-b;
			} else if (accl == 3) {	// EaseInOut (불규칙)
				double x = SQUARE(0.5-a);
				if (a>0.5) a = 0.5+x;
				else a = 0.5-x;
				b = 1-a;
			}

			for (int k=0; k<_skinele.size(); k++) {
				int j=_skinele[k];
				dst_res.push_back( (a*dst_vec[i][j]+b*dst_vec[i-1][j])/(a+b) );
			}
			
			break;
		}
	}
	// end 이후: end 상태 return
	if (dst_time >= dst_vec[dst_vec.size()-1][1]) {
		for (int i=0; i<_skinele.size(); i++) {
			int j = _skinele[i];
			dst_res.push_back( dst_vec[dst_vec.size()-1][j] );
		}
	}

	// centre
	int centre = dst_res[ DST_RES_CENTER ];
	if (centre == DST_CENTER_CENTER) {
		dst_res.push_back( dst_res[DST_RES_WID]/2 );
		dst_res.push_back( dst_res[DST_RES_HEI]/2 );
	} else if (centre == DST_CENTER_TOPLEFT) {
		dst_res.push_back( 0 );
		dst_res.push_back( 0 );
	} else if (centre == DST_CENTER_TOPRIGHT) {
		dst_res.push_back( dst_res[DST_RES_WID] );
		dst_res.push_back( 0 );
	} else if (centre == DST_CENTER_BOTTOMRIGHT) {
		dst_res.push_back( dst_res[DST_RES_WID] );
		dst_res.push_back( dst_res[DST_RES_HEI] );
	} else if (centre == DST_CENTER_BOTTOMLEFT) {
		dst_res.push_back( 0 );
		dst_res.push_back( dst_res[DST_RES_HEI] );
	}

	return dst_time;
}

void SkinElement::getDrawingInfo(vector <int> &src, vector <int> &dst) {
	src_res.clear();
	dst_res.clear();
	procSrc();
	procDst();
	src = src_res;
	dst = dst_res;
}

void SkinElement::set_srcTime(int time) {
	this->srcTime = time;
}

void SkinElement::set_dstTime(int time) {
	this->dstTime = time;
}

int SkinElement::src_gr() {
	return src_vec[1];
}

void SkinElement::set_wid(int v) {
	src_vec[4] = v;
}

void SkinElement::set_hei(int v) {
	src_vec[5] = v;
}

//void SkinElement::set_srcop(vector <int> v) {
//	src_op = v;
//}

void SkinElement::set_op(vector <int> v) {
	op = v;
}

void SkinElement::add_op(vector <int> v) {
	op.insert(op.end(), v.begin(), v.end());
}

void SkinElement::add_op(int v) {
	if (v>0) op.push_back(v);
}

void SkinElement::clear_op() {
	op.clear();
}

vector <int>  SkinElement::get_op() {
	return op;
}

int SkinElement::src_timer() {
	return src_vec[9];
}

int SkinElement::dst_timer() {
	return dst_vec[0][16];
}