#include "BMSParser.h"
#include <string>
using namespace std;

class BMSPlay {
private:
	int keyJudge[256];	// [Channel] ... store count of processed note
	BMSParser *bParser;
	void setBMSParser(BMSParser *p);
public:
	BOOL Initialize(BMSParser *p);
	wstring GetKey(double beat, int channel, int keynum);
	wstring GetLastValidKeyData(double beat, int channel);
	int GetKeyCount(int channel);
	void GetTimeOfKey(double beat, int channel, int keynum);

	void PressKey(int channel);
	
	double GetBeatFromTime(int milisec);
	int GetTimeFromBeat(double beat);
	double GetValidBeat(int channel);				// find beat of Unpressed note
	int GetValidKey(int channel);					// find key of Unpressed note
	double GetLastBeat(double beat, int channel);	// find last note from beat (beat=0 is default)
	int GetLastBeatIndex(double beat, int channel);
	int GetBPMOfBeat(double beat);
	BMSParser* getBMSParser();

	int TimeTable[MAXBEAT];
	void initalizeTimeTable();
};