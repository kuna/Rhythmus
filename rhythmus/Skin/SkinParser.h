#pragma once

#include <d3dx9.h>
#include "SkinElement.h"

class SkinParser {
protected:
	int locale;
	int fSize, fPos;
	BOOL isUnicode;
	TCHAR *fileData;

	int csv_argcnt(TCHAR *t);
	int csv_argindex(TCHAR *t);

	BOOL ParseCSVData();
	void ProcessCSVLine(TCHAR *text);

	vector <int> op;
	vector <int> nop;
	BOOL isConditional;
public:
	TCHAR	data_img_path[256][256];
	int		data_img_cnt;
	SkinElement *m_se;
	int			skinElementCnt;
public:
	void Initialize(SkinElement *se);
	BOOL ReadCSVFile(TCHAR *path);
};