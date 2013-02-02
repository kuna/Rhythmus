#include "..\common.h"
#include "BMSPlay.h"
#include "BMSUtil.h"
#include "BMSParser.h"

/**
������ ����: STOP ��ɾ ��� ó������ ...?
Draw�� Beat ��������, ������ Time ��������?
**/

BOOL BMSPlay::Initialize(BMSParser *p) {
	setBMSParser (p);
	return TRUE;
}

void BMSPlay::initalizeTimeTable() {
	int esclipedTime = 0;

	int s=0, b=0;
	double nb,ob;
	int mode;
	int bpm=bParser->getBPM(-1);
	for (int i=0; i<=bParser->keyBeatCount; i++) {
		ob = nb = (double)i;
		
		while (1) {
			// stop or bpm order... FIFO
			mode = 0;
			if (i==(int)bParser->keyBeat[8][b] && b<bParser->keyCnt[8]) {
				nb = bParser->keyBeat[8][b];
				mode = 8;
			}
			if (i==(int)bParser->keyBeat[9][s] && s<bParser->keyCnt[9] && nb>bParser->keyBeat[9][s]) {
				nb = bParser->keyBeat[9][s];
				mode = 9;
			}

			if (mode == 8) {
				// ������������ bpm ��� ó�� (#02 ���)
				// �׷������� �����Ͽ� �߰����� �����غ���
				esclipedTime += (nb-ob) / ((double)bpm / 60000.0f / 4) * bParser->keyBeatLength[i] * (double)bParser->keyBeatLength[i];
				// bpm ����
				bpm = bParser->getBPM( bParser->keyValue[8][b] );
				ob = nb;
				b++;
			} else if (mode == 9) {
				esclipedTime += bParser->getSTOP( bParser->keyValue[9][s] );
				s++;
			} else {
				// ���� ������ bpm ���
				esclipedTime += ((double)i-nb+1) / ((double)bpm / 60000.0f / 4) * bParser->keyBeatLength[i] * (double)bParser->keyBeatLength[i];
				break;
			}
		}

		TimeTable[i] = esclipedTime;
	}
}

// x��: Beat, y��: Time �׷����� �����Ͽ� ���س���
double BMSPlay::GetBeatFromTime(int milisec) {
	// �̸� ������� timetable�� �̿��Ͽ� �Ҹ�Ǵ� �ð��� ����
	double beat=0;
	int ntime=0;

	for (int i=0; i<=bParser->keyBeatCount; i++) {
		if (milisec < TimeTable[i]) {
			break;
		} else {
			ntime = TimeTable[i];
			beat = i+1;
		}
	}

	milisec -= ntime;
	// ���� beat ���ϱ�
	int s=0,b=0;
	double nb=0, ob=0;
	int bpm = GetBPMOfBeat(beat);
	for (s=0; s<bParser->keyCnt[9] && (int)beat>bParser->keyBeat[9][s]; s++) {}
	for (b=0; b<bParser->keyCnt[8] && (int)beat>bParser->keyBeat[8][b]; b++) {}
	while (1) {
		// stop or bpm order... FIFO
		int i = (int)beat;
		int mode = 0;
		if ((int)beat==(int)bParser->keyBeat[8][b] && b<bParser->keyCnt[8]) {
			nb = bParser->keyBeat[8][b];
			mode = 8;
		}
		if ((int)beat==(int)bParser->keyBeat[9][s] && s<bParser->keyCnt[9] && nb>bParser->keyBeat[9][s]) {
			nb = bParser->keyBeat[9][s];
			mode = 9;
		}

		if (mode == 8) {
			// ������������ bpm ��� ó�� (#02 ���)
			// dx(beat):dy(time) = (nb-ob):(nb-ob) / ((double)bpm / 60000.0f / 4) * bParser->keyBeatLength[i] * (double)bParser->keyBeatLength[i]
			// = x:milisec, x = milisec / (~~). (~~)�� x�� nb-ob���� ũ�� nb-ob, time ó���Ͽ� ����, �ƴϸ� x ����
			double calc = (double)milisec * ((double)bpm / 60000.0f / 4) * bParser->keyBeatLength[i] * (double)bParser->keyBeatLength[i];
			if (calc > nb-ob) {
				beat += nb-ob;
				milisec -= (nb-ob) / ((double)bpm / 60000.0f / 4) * bParser->keyBeatLength[i] * (double)bParser->keyBeatLength[i];
			} else {
				beat += calc;
				break;
			}
			// bpm ����
			bpm = bParser->getBPM( bParser->keyValue[8][b] );
			ob = nb;
			b++;
		} else if (mode == 9) {
			// stop �����ȿ� milisec ��������� break
			// �׷��� ������ milisec ����
			milisec -= bParser->getSTOP( bParser->keyValue[9][s] );
			if (milisec <= 0) break;
			s++;
		} else {
			// ���� ���� ���
			double calc = (double)milisec * ((double)bpm / 60000.0f / 4) * bParser->keyBeatLength[i] * (double)bParser->keyBeatLength[i];
			if (calc > 1.0f+(double)i-nb) {
				beat += 1.0f+(double)i-nb;
				milisec -= (1.0f+(double)i-nb) / ((double)bpm / 60000.0f / 4) * bParser->keyBeatLength[i] * (double)bParser->keyBeatLength[i];
			} else {
				beat += calc;
				break;	// ������������ ��� ���������Ƿ� exit
			}
		}
	}

	// �������: beat or stop count�� 0���϶�?
	return beat;
}

/* DEPRECIATED */
int BMSPlay::GetTimeFromBeat(double beat) {
	// stop �� bpm ���������� Ȯ��
	int bpm=bParser->getBPM(0);
	return 0;
}

/* DEPRECIATED */
wstring BMSPlay::GetKey(double beat, int channel, int keynum) {
	TCHAR key[3];
	wcsncpy(key, bParser->keydata[(int)beat][channel]+keynum*2, 2);
	return wstring(key);
}

int BMSPlay::GetKeyCount(int channel) {
	return bParser->keyCnt[channel];
}

double BMSPlay::GetLastBeat(double beat, int channel) {
	int r=0;
	for (int i=0; i<GetKeyCount(channel); i++) {
		if (bParser->keyBeat[channel][i] > beat) return bParser->keyBeat[channel][r];
		else r++;
	}
	return bParser->keyBeat[channel][r];
}

int BMSPlay::GetLastBeatIndex(double beat, int channel) {
	int r=0;
	for (int i=0; i<GetKeyCount(channel); i++) {
		if (bParser->keyBeat[channel][i] > beat) return bParser->keyBeat[channel][r];
		else r++;
	}
	return bParser->keyBeat[channel][r];
}

double BMSPlay::GetValidBeat(int channel) {
	if (keyJudge[channel] > GetKeyCount(channel)) return 0;
	else return bParser->keyBeat[channel][ keyJudge[channel] ];
}

int BMSPlay::GetBPMOfBeat(double beat) {
	int r = bParser->getBPM(-1);
	for (int b=0; b<GetKeyCount(8); b++) {
		if (beat <= bParser->keyBeat[8][b]) r = bParser->keyBeat[8][b];
	}
	return r;
}

BMSParser* BMSPlay::getBMSParser() {
	return bParser;
}

void BMSPlay::setBMSParser(BMSParser *p) {
	bParser = p;
}