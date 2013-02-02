#include "..\common.h"
#include "Timer.h"

void Timer::setGameTime() {
	isGameStartTimer = TRUE;
	gameStartTime = GetTickCount();
}

DWORD Timer::getGameTime() {
	return (GetTickCount()-gameStartTime);
}

void Timer::resetMainTimer() {
	savedRelativeTime[TIMER_MAIN] = GetTickCount();
}

DWORD Timer::getMainTime() {
	return GetTickCount()-savedRelativeTime[TIMER_MAIN];
}

void Timer::setTime(int index, int relativetime, BOOL loopcount, int looptime) {
	savedRelativeTime[index] = getMainTime() + relativetime;
	this->loopCount[index] = loopcount;
	loopTime[index] = looptime;
}

DWORD Timer::getTime(int index) {
	DWORD gt = gameStartTime;
	if (!isGameStartTimer) gt = savedGameStartTime;
	if (savedRelativeTime[index])
		return (relativeTime[index]-gt);
	else
		return (savedRelativeTime[index]-gt);
}