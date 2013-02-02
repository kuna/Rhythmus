/* TIMER
 - Scene Starttime�� ������� �۵���.
 - scene relative timer�� starttime, loopcount, gettime�� �޼��带 ��´�.


*/

#define MAX_TIMER			100

class Timer {
protected:
	DWORD gameStartTime;
	DWORD relativeTime[MAX_TIMER];	// MAX 100
	DWORD savedGameStartTime;
	DWORD savedRelativeTime[MAX_TIMER];
	BOOL isGameStartTimer;                                                                                                                
	int loopCount[MAX_TIMER];
	int loopTime[MAX_TIMER];
public:
	DWORD getGameTime();
	DWORD getTime(int index);
	void setGameTime();	// Initalize
	void stopGameTimer();
	void setTime(int index, int relativetime, BOOL loopcount, int looptime);
	void stopTimer(int index);
	void startTimer(int index);	// relativetime �����ÿ� �ڵ����� Ÿ�̸� �����
	void resetMainTimer();
	DWORD getMainTime();
};


// consts

#define TIMER_MAIN			0
#define TIMER_INPUTSTART	1  // STARTINPUT
#define TIMER_FADEOUT		2
#define TIMER_CLOSE			3
#define TIMER_ENDCHARINPUT	4

#define TIMER_LISTMOVE			10	// �� ��� �̵� �����ҋ� ������ �̵� �����ϸ� ����
#define TIMER_SONGSELECTCHANGE	11	// STARTINPUT���� ���� �� �ð��� ������ �۵���, ���� ����� ������ Ÿ�̸Ӱ� �缳����
#define TIMER_LISTMOVETOP		12	// �� ��Ͽ��� �̵� ���۽ÿ� �۵��ϰ� �̵� ������ ����
#define TIMER_LISTMOVEBOTTOM	13
#define TIMER_LISTSTOP			14	// STARTINPUT �ð������� �۵�
#define TIMER_READMESTART		15
#define TIMER_READMEEND			16
#define TIMER_COURSESELECTSTART	17
#define TIMER_COURSESELECTEND	18

#define TIMER_PANEL1START		21
#define TIMER_PANEL2START		22
#define TIMER_PANEL3START		23
#define TIMER_PANEL4START		24
#define TIMER_PANEL5START		25
#define TIMER_PANEL6START		26
#define TIMER_PANEL7START		27
#define TIMER_PANEL8START		28
#define TIMER_PANEL9START		29
#define TIMER_PANEL1CLOSE		31
#define TIMER_PANEL2CLOSE		32
#define TIMER_PANEL3CLOSE		33
#define TIMER_PANEL4CLOSE		34
#define TIMER_PANEL5CLOSE		35
#define TIMER_PANEL6CLOSE		36
#define TIMER_PANEL7CLOSE		37
#define TIMER_PANEL8CLOSE		38
#define TIMER_PANEL9CLOSE		39

#define TIMER_READY				40
#define TIMER_PLAYSTART			41	// BMS �⺻ Ÿ�̸�, READY�κ��� ������ �ð� ���� �۵�

#define TIMER_GUAGEUP1P			42
#define TIMER_GUAGEUP2P			43
#define TIMER_GUAGEMAX1P		44
#define TIMER_GUAGEMAX2P		45
#define TIMER_JUDGE1P			46
#define TIMER_JUDGE2P			47
#define TIMER_FULLCOMBO1P		48
#define TIMER_FULLCOMBO2P		49
#define TIMER_BOMB				50	// GREAT ���� �̻��϶�
#define TIMER_HOLD				70	// LONGNOTE Ȧ������Ҷ� ����
#define TIMER_KEYSETTING		100	// Ű���������۵��ϰ� Ű���������ߴ�
#define TIMER_KEYOFF			120	// Ű���������۵�
#define TIMER_RHYTHM			140	// �� ���۰� ���ÿ� �۵�, ���� �ٲ𶧸��� �缳��
#define TIMER_LASTNOTE1P		143
#define TIMER_LASTNOTE2P		144

#define TIMER_GRAPHSTART		150
#define TIMER_GRAPHEND			151

#define TIMER_HIGHSCORE			152	// ����Ʈȭ�餷���� Ű�� ���� �� �۵�

#define TIMER_DATABASELOAD		170
#define TIMER_DATABASELOADEND	171

#define TIMER_IRCONNECTSTART	172
#define TIMER_IRCONNECTSUCCESS	173
#define TIMER_IRCONNECTFAIL		174

#define TIMER_COURSEEDITSTART	180
#define TIMER_COURSEEDITEND		181