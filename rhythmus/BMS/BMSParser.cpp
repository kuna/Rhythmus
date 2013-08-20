/* ************************************
BMS Parser
by Kuna, GPL License

reference
http://bit.sparcs.org/~tokigun/article/bmsguide.php
************************************ */

#include "..\common.h"
#include "BMSParser.h"
#include "BMSUtil.h"

BOOL BMSParser::ParseBMSFile(TCHAR *filename) {
	// init
	fSize = fPos = 0;
	isUnicode = FALSE;
	locale = BMS_LOCALE_UNKNOWN;

	FILE *fp;
	fp = _wfopen(filename, L"rb");
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
		noteData = (TCHAR*)malloc(fSize + 10);
		fread(noteData, 1, fSize, fp);
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
		if (!BMSUtil::convert(_noteData, fSize, "SHIFT_JIS", &noteData))
			return BMS_INVALIDFILE;

		// header len ... or first parse necessary
		for (int i=0;i<1000;i++) {
			if (noteData[i] >= 44032 && noteData[i] <= 55203) {
				locale = BMS_LOCALE_KR;
				break;
			}
		}

		if (locale == BMS_LOCALE_KR) {
			free(noteData);
			BMSUtil::convert(_noteData, fSize, "CP949", &noteData);
		}

		free(_noteData);
	}
	fclose(fp);

	if (!ParseBMSData()) {
		return BMS_INVALIDFILE;
	}

	if (noteData) free(noteData);
	return TRUE;
}

BOOL BMSParser::ParseBMSData() {
	// init
	fPos = 0;
	int fLen = wcslen(noteData);
	for (int i=0; i<MAXBEAT; i++) {
		keyBeatLength[i] = 1.0f;
	}

	// line parse
	int fPS=0, fPE=0;
	TCHAR lineBuf[BMS_MAXLINEBUF];
	while (true)
	{
		for (int i=fPS; i<fLen-1; i++)
		{
			fPE = i;
			if (memcmp(noteData+i, L"\r\n", sizeof(TCHAR)*2)==0) {
				fPE -= 2;	// 마지막 라인과의 호환성을 위해서
				break;
			}
		}
		
		wcsncpy(lineBuf, noteData+fPS, fPE - fPS+2);
		lineBuf[fPE - fPS+2] = 0;
		ProcessBMSLine(lineBuf);

		fPS = fPE+4;

		// break condition
		if (fPS > fLen - 1) break;
	}

	// sort data
	std::sort(bpmData, bpmData+bpmNum);
	std::sort(BGMData, BGMData+BGMNum);
	std::sort(BGAData, BGAData+BGANum);
	std::sort(StopData, StopData+StopNum);
	std::sort(OverBGAData, OverBGAData+OverBGANum);
	std::sort(PoorBGAData, PoorBGAData+PoorBGANum);
	std::sort(keyData, keyData+keyDatanum);

	return TRUE;
}

BOOL BMSParser::ProcessBMSLine(TCHAR *text) {
	// Header Parse
	if (BMSParseMode == BMS_PARSER_HEADER) {
		TCHAR key[BMS_MAXLINEBUF], value[BMS_MAXLINEBUF];
		ParseBMSLine(text, key, value);
		if (customwcscmp(L"#PLAYER", key)) {
			b_Player = _wtoi(value);
		} else
		if (customwcscmp(L"#GENRE", key)) {
			wcscpy_s(b_Genre, BMS_MAXSTR, value);
		} else
		if (customwcscmp(L"#TITLE", key)) {
			wcscpy_s(b_Title, BMS_MAXSTR, value);
		} else
		if (customwcscmp(L"#ARTIST", key)) {
			wcscpy_s(b_Artist, BMS_MAXSTR, value);
		} else
		if (customwcscmp(L"#BPM", key)) {
			b_BPM = _wtoi(value);
		} else
		if (customwcscmp(L"#PLAYLEVEL", key)) {
			b_Playlevel = _wtoi(value);
		} else
		if (customwcscmp(L"#RANK", key)) {
			b_Rank = _wtoi(value);
		} else
		if (customwcscmp(L"#TOTAL", key)) {
			b_Total = _wtoi(value);
		} else
		if (customwcscmp(L"#VOLWAV", key)) {
			b_Volwav = _wtoi(value);
		} else
		if (customwcscmp(L"#STAGEFILE", key)) {
			wcscpy_s(b_Stagefile, BMS_MAXSTR, value);
		} else
		if (customwcscmp(L"#WAV00", key)) {

		} else
		if (customwcscmp(L"#BMP00", key)) {

		} else
		if (customwcscmp(L"#BPM00", key)) {
			b_BPMKey[BMSUtil::KeyValueToInt(key+5)] = _wtoi(value);
		} else
		if (customwcscmp(L"#STOP00", key)) {
			b_STOPKey[BMSUtil::KeyValueToInt(key+6)] = _wtoi(value);

		//} else
		//if (customwcscmp(L"#BMP00", key)) {

		} else {
			/* EXCEPTION */
		}
	}

	// Maindata Parse
	if (BMSParseMode == BMS_PARSER_MAINDATA) {
		TCHAR key[BMS_MAXLINEBUF], value[BMS_MAXLINEBUF];
		ParseBMSLine2(text, key, value);
		if (wcslen(key) == 6 && key[0] == L'#') {
			TCHAR str_beat[5], str_channel[5];
			str_beat[3]=str_channel[2]=0;
			int beat, channel;
			wcsncpy(str_beat, key+1, 3);
			wcsncpy(str_channel, key+4, 2);
			beat = _wtoi(str_beat);
			channel = _wtoi(str_channel);
			//wcscpy(keydata[beat][channel], value);

			// #02 channel - beat length
			if (channel == 02) {
				keyBeatLength[beat] = (float)_wtof(value);
			}

			// fill out keyCnt, keyBeat, keyValue
			int ncb = wcslen(value)/2;
			for (int i=0; i<ncb; i++) {
				int nkv = BMSUtil::KeyValueToInt(value+ i*2, 2);
				if (nkv == 0) continue;	// 00은 무시

				// data will separately move by channel
				double nb = beat + (double)i / (double)ncb;

				BMSKeyData *pBMSData;

				switch (channel) {
				case 8:	// extend BPM channel
					nkv = getBPM(nkv);
				case 3:	// BPM
				case 9:	// STOP
					pBMSData = &bpmData[bpmNum];
					pBMSData->beat = nb;
					pBMSData->key = channel;
					pBMSData->value = nkv;
					bpmNum++;
					pBMSData = &keyData[keyDatanum];	// keydata also includes BPM Info
					keyDatanum++;
					break;
				case 7:
				case 6:
				case 4:
					pBMSData = &BGAData[BGANum];
					BGANum++;
					break;
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 18:
				case 19:
					pBMSData = &keyData[keyDatanum];
					keyDatanum++;
					break;
				}

				pBMSData->beat = nb;
				pBMSData->key = channel;
				pBMSData->value = nkv;
			}

			if (keyBeatCount < beat) keyBeatCount = beat;
		} else {
			/* EXCEPTION */
		}
	}
	
	// BMS Parse mode
	if (wcscmp(text, L"*---------------------- HEADER FIELD") ==0) {
		BMSParseMode = BMS_PARSER_HEADER;
	}
	if (wcscmp(text, L"*---------------------- MAIN DATA FIELD") == 0) {
		BMSParseMode = BMS_PARSER_MAINDATA;
	}

	return TRUE;
}

BOOL BMSParser::ParseBMSLine(TCHAR *input, TCHAR *key, TCHAR *value) {
	// addon: auto Trim
	int s = 0;
	int e = wcslen(input);
	for (int i=0; i<wcslen(input); i++) {
		if (memcmp(input+i ,L" ", 2) != 0) {
			s = i;
			break;
		}
	}
	
	for (int i=0; i<wcslen(input); i++) {
		if (memcmp(input+(wcslen(input)-i) ,L" ", 2) != 0) {
			e = wcslen(input)-i;
			break;
		}
	}

	for (int i=s; i<e; i++) {
		if (memcmp(input+i ,L" ", 2) == 0) {
			wcsncpy(key, input+s, i-s);
			key[i-s] = 0;
			wcsncpy(value, input+i+1, e-i-1);
			value[e-i-1] = 0;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL BMSParser::ParseBMSLine2(TCHAR *input, TCHAR *key, TCHAR *value) {
	// addon: auto Trim
	int s = 0;
	int e = wcslen(input);
	for (int i=0; i<wcslen(input); i++) {
		if (memcmp(input+i ,L" ", 2) != 0) {
			s = i;
			break;
		}
	}
	
	for (int i=0; i<wcslen(input); i++) {
		if (memcmp(input+(wcslen(input)-i) ,L" ", 2) != 0) {
			e = wcslen(input)-i;
			break;
		}
	}

	for (int i=s; i<e; i++) {
		if (memcmp(input+i ,L":", 2) == 0) {
			wcsncpy(key, input+s, i-s);
			key[i-s] = 0;
			wcsncpy(value, input+i+1, e-i-1);
			value[e-i-1] = 0;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL BMSParser::customwcscmp(TCHAR *t1, TCHAR *t2)
{
	// return (wcslen(t1)==wcslen(t2))?(wcscmp(t1, t2)==0):FALSE;
	// for custom wcscmp

	if (wcslen(t1)!=wcslen(t2)) return FALSE;
	for (int i=0; i<wcslen(t1); i++) {
		if (t1[i] == L'0') {
			if (((t2[i]-L'0')>9 || (t2[i]-L'0')<0) && (t2[i]<L'A' || t2[i]>'F')) return FALSE;
		} else if (t1[i] != t2[i]) return FALSE;
	}
	return TRUE;
}

double BMSParser::getProgress() {
	if (fSize == 0) return 0;
	return (double)fSize / (double)fSize * 100;
}

int BMSParser::getSTOP(int KeyValue) {
	return b_STOPKey[KeyValue];
}

int BMSParser::getBPM(int KeyValue) {
	if (KeyValue<0) return b_BPM;
	return b_BPMKey[KeyValue];
}