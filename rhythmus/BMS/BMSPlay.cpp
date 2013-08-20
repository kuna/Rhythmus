#include "..\common.h"
#include "BMSPlay.h"
#include "BMSUtil.h"
#include "BMSParser.h"

/**
현재의 난관: STOP 명령어를 어떻게 처리하지 ...?
Draw는 Beat 기준으로, 판정은 Time 기준으로?
**/

BOOL BMSPlay::Initialize(BMSParser *p) {
	setBMSParser (p);
	GetTimeOfBMS();
	return TRUE;
}

void BMSPlay::GetTimeOfBMS() {
	BMSKeyData *bData = bParser->bpmData;
	int bpm = bParser->getBPM(-1);
	double beat = 0;

	for (int i=0; i<bParser->bpmNum; i++) {
		if (bData[i].key == 3) {	// BPM Change
			TimeOfBMS += (bData[i].beat - beat) * (1.0f / bpm * 60 * 4);
			beat = bData[i].beat;
			bpm = bData[i].value;
		}
		if (bData[i].key == 9) {	// STOP
			TimeOfBMS += bParser->getSTOP(bData[i].value);
		}
	}

	// get to the last beat
	TimeOfBMS += (bParser->keyBeatCount - beat) * (1.0f / bpm * 60 * 4);
}

// x축: Beat, y축: Time 그래프를 상정하여 구해낸다
double BMSPlay::GetBeatFromTime(int milisec) {
	if (milisec/1000.0f > TimeOfBMS)
		return bParser->keyBeatCount;	// Last Beat

	BMSKeyData *bData = bParser->bpmData;
	double beat = 0;
	int time = 0;
	int bpm = bParser->getBPM(-1);
	for (int i=0; i<bParser->bpmNum; i++) {
		// calc to the next point
		// if calced beat is bigger then ret calced val
		// else add to beat and go on

		if (bData[i].key == 9) {	// STOP
			time += bParser->getSTOP(bData[i].value)*1000;
			if (time >= milisec)
				return beat;
			continue;
		}

		time += (bData[i].beat - beat) * (1.0f / bpm * 60 * 4);
		if (time >= milisec) {
			return beat + (milisec-time)*(bpm/60000/4.0f);
		}
		beat = bData[i].beat;
		bpm = bData[i].value;
	}

	return beat;
}

// millisec
// DEPRECIATED ?
int BMSPlay::GetTimeFromBeat(double beat) {
	if (beat > bParser->keyBeatCount)
		return (int)(TimeOfBMS*1000);

	BMSKeyData *bData = bParser->bpmData;
	int bpm = bParser->getBPM(-1);
	double beat = 0;
	double time = 0;

	for (int i=0; i<bParser->bpmNum; i++) {
		if (bData[i].beat > beat)	// ret
		{
			return time + (bData[i].beat - beat) * (1.0f / bpm * 60 * 4);
		}

		if (bData[i].key == 3) {	// BPM Change
			time += (bData[i].beat - beat) * (1.0f / bpm * 60 * 4);
			beat = bData[i].beat;
			bpm = bData[i].value;
		}
		if (bData[i].key == 9) {	// STOP
			time += bParser->getSTOP(bData[i].value);
		}
	}

	return (int)(time*1000);
}

BMSKeyData BMSPlay::GetLastBeat(double beat, int channel) {
	BMSKeyData *b;
	for (int i=0; i<bParser->keyDatanum; i++) {
		if (bParser->keyData[i].beat >= beat) break;
		b = &bParser->keyData[i];
	}
	return *b;
}

int BMSPlay::GetBPM(double beat) {
	int r = bParser->getBPM(-1);
	for (int i=0; i<bParser->bpmNum; i++) {
		r = bParser->bpmData[i].value;
		if (bParser->bpmData[i].beat >= beat)
			break;
	}
	return r;
}


BMSParser* BMSPlay::getBMSParser() {
	return bParser;
}

void BMSPlay::setBMSParser(BMSParser *p) {
	bParser = p;
}