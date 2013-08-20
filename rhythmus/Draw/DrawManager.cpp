#include "..\common.h"
#include "..\BMS\BMSPlay.h"
#include "DrawManager.h"

int DrawManager::DrawScreen(int milisec) {
	// ���� ��Ʈ ����
	double nbeat = bPlay->GetBeatFromTime(milisec);
	int bpm = bPlay->GetBPM(nbeat);
	
	// invalid note���� 0 ������ �׸�

	int latestPos = 0;
	speed = 1;	// ���ǰ�
	screenHeight = 400;	// ���ǰ�

	BMSParser *bp = bPlay->getBMSParser();
	
	for (int i=0; i<bp->keyDatanum; i++) {
		BMSKeyData data = bp->keyData[i];
		if (data.key == 9) {					// STOP
			continue;
		}

		// line drawing (not necessary)
		if ((int)data.beat == (int)nbeat + 1) {
			latestPos += ((int)data.beat - nbeat) * speed * bpm * bp->keyBeatLength[(int)nbeat] ;
			nbeat = (int)data.beat;
			di->DrawLine(200, screenHeight-latestPos, 200+42*7);
		}

		if (data.key > 10 && data.beat >= nbeat) {
			// calc position
			latestPos += (data.beat - nbeat) * speed * bpm * bp->keyBeatLength[(int)nbeat] ;
			nbeat = data.beat;

			// draw Note
			di->DrawNote(i*42+200, screenHeight-latestPos, i);
		}
		if (data.key == 3 || data.key == 8) {	// BPM
			// calc position
			latestPos += (data.beat - nbeat) * speed * bpm * bp->keyBeatLength[(int)nbeat] ;
			nbeat = data.beat;

			bpm = data.value;
		}
		
		if (latestPos > screenHeight) break;
	}

	return TRUE;
}

void DrawManager::setDrawObject(DrawImpl *d) {
	di = d;
}

void DrawManager::setPlayObject(BMSPlay *p) {
	bPlay = p;
}