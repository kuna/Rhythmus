#include "..\common.h"
#include "..\bms\bmsutil.h"
#include <stdio.h>
#include <vector>
using namespace std;

#include "SkinParser.h"

#define CMP(A,B) (wcscmp(A,B)==0)

void SkinParser::Initialize(SkinElement *se)
{
	m_se = se;
}

BOOL SkinParser::ReadCSVFile(TCHAR *path) {
	
	// init
	fSize = fPos = 0;
	isUnicode = FALSE;
	locale = BMS_LOCALE_UNKNOWN;

	FILE *fp;
	fp = _wfopen(path, L"rb");
	if (!fp) return BMS_NOFILE;

	fseek(fp, 0, SEEK_END);
	fSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// for unicode reading
	char sig[10];
	fread(sig, 1, 2, fp);
	if (memcmp(sig, new char[0xFF, 0xFE], 2) == 0) {
		isUnicode = TRUE;
	} else {
		fseek(fp, 0, SEEK_SET);
		isUnicode = FALSE;
	}

	if (isUnicode) {
		fileData = (TCHAR*)malloc(fSize + 10);
		fread(fileData, 1, fSize, fp);
	} else {
		// ASCII - check encoding ...
		char *_noteData = (char*)malloc(fSize + 10);
		fread(_noteData, 1, fSize, fp);

		/*************************
		Automatic Encoding
		1. First, JP Encoding - check あえいおう。。。アエイオウ。。。 from tag
		2. if No JP found? then KR Encoding
		*************************/
		//BMSUtil::convert("test", "SHIFT_JIS", &noteData);
		//free(noteData);

		locale = BMS_LOCALE_JP;
		if (!BMSUtil::convert(_noteData, fSize, "SHIFT_JIS", &fileData)) {
			if (!BMSUtil::convert(_noteData, fSize, "CP949", &fileData)) {
				return BMS_INVALIDFILE;
			} else {
				locale = BMS_LOCALE_KR;
			}
		}

		// header len ... or first parse necessary
		for (int i=0;i<1000 && locale == BMS_LOCALE_UNKNOWN;i++) {
			if (fileData[i] >= 44032 && fileData[i] <= 55203) {
				BMSUtil::convert(_noteData, fSize, "CP949", &fileData);
				locale = BMS_LOCALE_KR;
				break;
			}
		}

		free(_noteData);
	}
	fclose(fp);

	if (!ParseCSVData()) {
		return BMS_INVALIDFILE;
	}
	
	if (fileData) free(fileData);
	return TRUE;
}

BOOL SkinParser::ParseCSVData() {
	// init
	fPos = 0;
	int fLen = wcslen(fileData);

	// line parse
	int fPS=0, fPE=0;
	TCHAR lineBuf[BMS_MAXLINEBUF];
	while (true)
	{
		for (int i=fPS; i<fLen-1; i++)
		{
			fPE = i;
			if (memcmp(fileData+i, L"\r\n", sizeof(TCHAR)*2)==0) {
				fPE -= 2;	// 마지막 라인과의 호환성을 위해서
				break;
			}
		}
		
		wcsncpy(lineBuf, fileData+fPS, fPE - fPS+2);
		lineBuf[fPE - fPS+2] = 0;
		ProcessCSVLine(lineBuf);

		fPS = fPE+4;

		// break condition
		if (fPS > fLen - 1) break;
	}

	return TRUE;
}

void SkinParser::ProcessCSVLine(TCHAR *text) {
	if (memcmp(text, L"//", 2*sizeof(TCHAR)) == 0) return;	// do nothing
	if (text[0] != L'#') return; // do nothing

	TCHAR args[50][256];
	int i, ai=0;

	while (wcslen(text) != (i=csv_argindex(text))) {
		if (ai >= 20) break;		// EXCEPTION
		text[i] = 0;
		wcscpy(args[ai++], text);
		text += i+1;
	}
	if (wcslen(text) > 0) wcscpy(args[ai++], text);	// 마지막 인자

	if (CMP(args[0], L"#IF")) {
		if (isConditional) {
			// ERROR
		}

		for (int i=1; i<ai; i++) {
			op.push_back( wtoi(args[i]) );
		}
		isConditional = TRUE;
	} else if (CMP(args[0], L"#ELSEIF")) {
		if (!isConditional) {
			// ERROR
		}
		
		// convert option
		for (int i=0; i<op.size(); i++)
			op[i] = op[i] * -1;
		for (int i=1; i<ai; i++) {
			op.push_back( wtoi(args[i]) );
		}
	} else if (CMP(args[0], L"#ENDIF")) {
		if (!isConditional) {
			// ERROR
		}

		op.clear();
		isConditional = FALSE;
	} else if (CMP(args[0], L"#IMAGE")) {
		// LR2files\theme\remi-s\play\(filepath)...
		// LR2files\theme\ 부분까지 remove
		TCHAR fpath[256];
		wcscpy(fpath, args[1]+15 );
		for (int i=0; i<wcslen(fpath); i++)
			if (fpath[i] == L'¥')
				fpath[i] = L'\\';

		wcscpy(data_img_path[ data_img_cnt++ ], fpath);
	} else if (CMP(args[0], L"#SRC_IMAGE")) {
		// (NULL)	gr	x	y	w	h	div_x	div_y	cycle	timer	op1	op2	op3
		vector <int> argi;
		argi.push_back(wtoi( args[1] ));	// (NULL)
		argi.push_back(wtoi( args[2] ));	// gr
		argi.push_back(wtoi( args[3] ));	// x
		argi.push_back(wtoi( args[4] ));	// y
		argi.push_back(wtoi( args[5] ));	// w
		argi.push_back(wtoi( args[6] ));	// h
		argi.push_back(wtoi( args[7] ));	// divx
		argi.push_back(wtoi( args[8] ));	// divy
		argi.push_back(wtoi( args[9] ));	// cycle
		argi.push_back(wtoi( args[10] ));	// timer
		argi.push_back(wtoi( args[11] ));	// op1	(unused or for other use)
		argi.push_back(wtoi( args[12] ));	// op2
		argi.push_back(wtoi( args[13] ));	// op3
		if (SRCMODE) {
			// EXCEPTION
		} else {
			skinElementCnt++;
			m_se[skinElementCnt-1].set_src(argi);

			// IF구문 Condition은 미리 정해놓는 게 더 편하다
			m_se[skinElementCnt-1].set_op(op);

			SRCMODE = TRUE;
		}
	} else if (CMP(args[0], L"#DST_IMAGE")) {
		// (NULL)	time	x	y	w	h	acc	a	r	g	b	blend	filter	angle	center	loop	timer	op1	op2	op3
		
		vector <int> argi;
		argi.push_back(wtoi( args[1] ));	// (NULL)
		argi.push_back(wtoi( args[2] ));	// time
		argi.push_back(wtoi( args[3] ));	// x
		argi.push_back(wtoi( args[4] ));	// y
		argi.push_back(wtoi( args[5] ));	// w
		argi.push_back(wtoi( args[6] ));	// h
		argi.push_back(wtoi( args[7] ));	// acc
		argi.push_back(wtoi( args[8] ));	// a
		argi.push_back(wtoi( args[9] ));	// r
		argi.push_back(wtoi( args[10] ));	// g
		argi.push_back(wtoi( args[11] ));	// b
		argi.push_back(wtoi( args[12] ));	// blend
		argi.push_back(wtoi( args[13] ));	// filter
		argi.push_back(wtoi( args[14] ));	// angle
		argi.push_back(wtoi( args[15] ));	// center
		argi.push_back(wtoi( args[16] ));	// loop
		argi.push_back(wtoi( args[17] ));	// timer
		//argi.push_back(wtoi( args[17] ));	// op1
		//argi.push_back(wtoi( args[18] ));	// op2
		//argi.push_back(wtoi( args[19] ));	// op3
		
		m_se[skinElementCnt-1].push_dst( argi );
		
		// set op (TODO: dst 개수가 1개일때로 condition 만들 것)
		for (int i=18; i<=20; i++) {
			m_se[skinElementCnt-1].add_op( wtoi(args[i]) );
		}

		SRCMODE = FALSE;
	}
}

int SkinParser::csv_argcnt(TCHAR *t) {
	int r = 0;
	for (int i=0; i<wcslen(t); i++) {
		if (t[i] == L',') r++;
	}
	return r;
}

int SkinParser::csv_argindex(TCHAR *t) {
	int i;
	for (i=0; i<wcslen(t); i++) {
		if (t[i] == L',') return i;
	}
	return i;
}

int SkinParser::wtoi(TCHAR *t) {
	if (t[0] == L'-') {
		return _wtoi(t+1)*-1;
	} else {
		return _wtoi(t);
	}
}