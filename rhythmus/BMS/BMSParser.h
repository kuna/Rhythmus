#pragma once
#include <algorithm>
#include <string>
#include <hash_map>
using namespace std;

struct BMSKeyData {
	double beat;
	int key;
	int value;
	int attr;

	bool operator< ( const BMSKeyData &o ) {
		return (this->beat)<(o.beat);
	}
};

class BMSParser {
public:
	BOOL ParseBMSFile(TCHAR *filename);
	BOOL ParseBMSData();
	BOOL ProcessBMSLine(TCHAR *text);
	BOOL ParseBMSLine(TCHAR *input, TCHAR *key, TCHAR *value);
	BOOL ParseBMSLine2(TCHAR *input, TCHAR *key, TCHAR *value);
	double getProgress();

	BOOL isBMSLoad;

	// BPM, Stop Data is included mixed
	// keynote, Longnote, invisible note is included mixed
	
	int keyDatanum;
	BMSKeyData keyData[BMS_MAXNOTE];	// keydata (PS: if should departed by player)
	
	int bpmNum;
	BMSKeyData bpmData[BMS_MAXNOTE];	// BPM Channel: #03 and #08 + STOP Channel: #09
	
	int BGANum;
	BMSKeyData BGAData[BMS_MAXNOTE];	// BGA Channel: #04, #06(Poor), #07(Overlay)

	int StopNum;
	BMSKeyData StopData[BMS_MAXNOTE];	// STOP Channel: #09

	int BGMNum;
	BMSKeyData BGMData[BMS_MAXNOTE];	// BGM Channel: #01, 



	// for #02 channel (beat size)
	// default value: 1
	float keyBeatLength[MAXBEAT];			// [MAXBEAT]
	int keyBeatCount;					// beat count = end of beat

	int getBPM(int KeyValue);
	int getSTOP(int KeyValue);
private:
	int BMSParseMode;
	TCHAR *noteData;
	int locale;
	BOOL isUnicode;
	int fSize;
	int fPos;
	BOOL customwcscmp(TCHAR *t1, TCHAR *t2);

	int b_Player;
	TCHAR b_Title[BMS_MAXSTR];
	TCHAR b_Artist[BMS_MAXSTR];
	TCHAR b_Genre[BMS_MAXSTR];
	int b_BPM;
	int b_Playlevel;
	int b_Rank;
	int b_Total;
	int b_Volwav;
	TCHAR b_Stagefile[BMS_MAXSTR];
	TCHAR b_Videofile[BMS_MAXSTR];
	int b_WAVKey[BMS_MAXKEY];
	int b_BPMKey[BMS_MAXKEY];
	int b_STOPKey[BMS_MAXKEY];

};