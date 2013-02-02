
#include "..\common.h"
#include "SkinElement.h"

void SkinElement::set_src(vector <int> elem) {
	src_vec = elem;
	
	frame = elem[5]*elem[6];
	cycle = elem[7];
}

int SkinElement::getNowFrame() {
	if (cycle <= 0) return 0;

	int t = time%cycle;
	double t2 = cycle/(double)frame;

	return (double)t/t2;
}

void SkinElement::setTime(int time) {
	this->time = time;
}

void SkinElement::push_dst(vector <int> elem) {
	if (elem[5] <= 0) elem[5] = 1;	// divx
	if (elem[6] <= 0) elem[6] = 1;	// divy
	dst_vec.push_back(elem);

	// 첫 dst일 경우
	if (dst_vec.size() == 1) {
		// set loop
		loop = (dst_vec[0][14]>0);
	}
}

int SkinElement::src_gr() {
	return src_vec[1];
}

int SkinElement::src_x() {
	double nw = src_vec[4]/(double)src_vec[6];	// wid/divx
	int nf = getNowFrame();
	return src_vec[2]+(int)(nw*(nf%src_vec[5]));
}

int SkinElement::src_y() {
	double nh = src_vec[5]/(double)src_vec[7];	// hei/divy
	int nf = getNowFrame();
	return src_vec[3]+(int)(nh*(nf/src_vec[5]));
}

int SkinElement::src_wid() {
	return (int)(src_vec[4]/(double)src_vec[6]);
}

int SkinElement::src_hei() {
	return (int)(src_vec[5]/(double)src_vec[7]);
}

void SkinElement::set_wid(int v) {
	src_vec[4] = v;
}

void SkinElement::set_hei(int v) {
	src_vec[5] = v;
}

void SkinElement::getDrawingInfo(vector <int> op, vector <int> &elem) {
	int stime = dst_vec[0][1];
	int etime = dst_vec[ dst_vec.size()-1 ][1];
	int looptime = etime - stime;
	int dst_time;
	if (loop) {
		dst_time = (time-stime) % looptime + stime;
	} else {
		dst_time = time;
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
		elem[0] = -1;
	}
	// 1~2 .. end: 내적값
	for (int i=1; i<dst_vec.size(); i++) {
		if (dst_time >= dst_vec[i-1][1] && dst_time < dst_vec[i][1]) {
			int a=dst_vec[i-1][1]-dst_time;
			int b=dst_time-dst_vec[i][1];
			elem[0]=0;
			for (int j=1; j<15; j++) {
				elem[j] = (a*dst_vec[i][j]+b*dst_vec[i-1][j])/(a+b);
			}
			break;
		}
	}
	// end 이후: end 상태 return
	if (dst_time >= dst_vec[dst_vec.size()-1][1])
		elem = dst_vec[dst_vec.size()-1];
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

void SkinElement::set_nop(vector <int> v) {
	nop = v;
}

void SkinElement::add_nop(vector <int> v) {
	nop.insert(nop.end(), v.begin(), v.end());
}
void SkinElement::add_nop(int v) {
	if (v>0) nop.push_back(v);
}

void SkinElement::clear_nop() {
	nop.clear();
}

vector <int>  SkinElement::get_op() {
	return op;
}

vector <int> SkinElement::get_nop() {
	return nop;
}

int SkinElement::src_timer() {
	return src_vec[9];
}

int SkinElement::dst_timer() {
	return dst_vec[0][16];
}