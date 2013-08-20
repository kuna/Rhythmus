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
	BMSKeyData GetLastBeat(double beat, int channel);	// find last note from beat (beat=0 is default)
	int GetBPM(double beat);
	BMSParser* getBMSParser();

	double TimeOfBMS;
	void GetTimeOfBMS();
};