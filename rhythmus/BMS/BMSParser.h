#pragma once
#include <algorithm>
#include <string>
#include <hash_map>
using namespace std;

class BMSParser {
public:
	BOOL ParseBMSFile(TCHAR *filename);
	BOOL ParseBMSData();
	BOOL ProcessBMSLine(TCHAR *text);
	BOOL ParseBMSLine(TCHAR *input, TCHAR *key, TCHAR *value);
	BOOL ParseBMSLine2(TCHAR *input, TCHAR *key, TCHAR *value);
	double getProgress();

	BOOL isBMSLoad;
	TCHAR keydata[MAXBEAT][BMS_MAXCHANNEL][BMS_MAXKEY];	// [Beat][Channel][Keydata] // Depreciated

	int keyCnt[BMS_MAXCHANNEL];					// [Channel]
	double keyBeat[BMS_MAXCHANNEL][BMS_MAXKEY];	// [Channel][MAXKEY]
	int keyValue[BMS_MAXCHANNEL][BMS_MAXKEY];		// [Channel][MAXKEY]

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