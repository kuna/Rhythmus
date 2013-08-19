#include "..\common.h"
#include "..\BMS\BMSPlay.h"
#include "DrawManager.h"

int DrawManager::DrawScreen(int milisec) {
	// btnArr
	int btnArr[] = {11,12,13,14,15,18,19,16};

	// ���� ��Ʈ ����
	double nbeat = bPlay->GetBeatFromTime(milisec);

	// invalid note���� 0 ������ �׸�
	double nb[10];
	nb[0] = bPlay->GetLastBeat(nbeat, 8);		// BPM Channel
	nb[1] = bPlay->GetValidBeat(btnArr[0]);		// note
	nb[2] = bPlay->GetValidBeat(btnArr[1]);
	nb[3] = bPlay->GetValidBeat(btnArr[2]);
	nb[4] = bPlay->GetValidBeat(btnArr[3]);
	nb[5] = bPlay->GetValidBeat(btnArr[4]);
	nb[6] = bPlay->GetValidBeat(btnArr[5]);
	nb[7] = bPlay->GetValidBeat(btnArr[6]);
	nb[8] = bPlay->GetValidBeat(btnArr[7]);		// scratch
	int bpm = bPlay->getBMSParser()->getBPM(-1);
	
	for (int i=0; i<8; i++) {
		int k = btnArr[i];
		while (nb[i+1] < nbeat) {
			/* draw �ڵ� ���� �ۼ� */
			nb[i+1] = bPlay->GetLastBeat(nb[i+1], k);
		}
	}
	
	int latestPos = 0;
	speed = 1;	// ���ǰ�
	screenHeight = 400;	// ���ǰ�
	while (1) {
		nb[0] = bPlay->GetLastBeat(nbeat, 8);	// get BPM
		if (nb[0] > 0 && nb[0] < ((int)nbeat) + 1) {
			// BPM ���Ӻκб��� �׸���
			for (int i=0; i<8; i++) {
				int k = btnArr[i];
				while (nb[i+1] < nb[0])	// bpm ĿƮ ������ �׸���
				{
					int pos = latestPos + speed * (nb[i+1] - nbeat) * bpm * bPlay->getBMSParser()->keyBeatLength[(int)nbeat]; // beat * BPM * speed * nodeLength
					if (pos > screenHeight) break; // ���� pos�� ��ũ������ �� ũ�� exit
					di->DrawNote(i*42+200, screenHeight-pos, i);

					nb[i+1] = bPlay->GetLastBeat(nb[i+1], btnArr[i]);
				}
			}

			// ����
			latestPos += speed * (nb[0] - nbeat) * bpm * bPlay->getBMSParser()->keyBeatLength[(int)nbeat];
			nbeat = nb[0];
			nb[0] = bPlay->GetLastBeat(nb[0], 8);
			bpm = bPlay->GetBPMOfBeat(nb[0]);
		} else {
			// ��Ʈ ������ �׸� ��, �� �߱�
			for (int i=0; i<8; i++) {
				int k = btnArr[i];
				while (nb[i+1] < (int)nbeat + 1)
				{
					int pos = latestPos + speed * (nb[i+1] - nbeat) * bpm * bPlay->getBMSParser()->keyBeatLength[(int)nbeat]; // beat * BPM * speed * nodeLength
					if (pos > screenHeight) break; // ���� pos�� ��ũ������ �� ũ�� exit
					di->DrawNote(i*42+200, screenHeight-pos, i);

					nb[i+1] = bPlay->GetLastBeat(nb[i+1], btnArr[i]);
				}
			}

			// ����
			latestPos += speed * ((int)nbeat - nbeat + 1) * bpm * bPlay->getBMSParser()->keyBeatLength[(int)nbeat];
			nbeat = ((int)nbeat) + 1;
			
			di->DrawLine(200, screenHeight-latestPos, 200+42*7);
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