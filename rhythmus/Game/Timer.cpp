#include "..\common.h"
#include "Timer.h"

Timer::Timer() {
}

void Timer::resetGameTime() {
	gameStartTime = GetTickCount();
}

void Timer::stopGameTimer() {
	gameStartTime = 0;
}

DWORD Timer::getGameTime() {
	return (GetTickCount()-gameStartTime);
}

void Timer::resetMainTimer() {
	isTimerEnabled[TIMER_MAIN] = TRUE;
	relativeTime[TIMER_MAIN] = GetTickCount();
}

DWORD Timer::getMainTime() {
	return getTime( TIMER_MAIN );
}

void Timer::setTime(int index, int relativetime) {
	isTimerEnabled[index] = TRUE;
	relativeTime[index] = relativeTime[TIMER_MAIN] + relativetime;
}

int Timer::getTime(int index) {
	DWORD r = GetTickCount();
	if (!isTimerEnabled[index] || r<relativeTime[index])
		return -1;
	else
		return (int)(r-relativeTime[index]);
}

void Timer::resetTimer(int index) {
	isTimerEnabled[index] = TRUE;
	relativeTime[index] = GetTickCount();
}

void Timer::stopTimer(int index) {
	isTimerEnabled[index] = FALSE;
	relativeTime[index] = 0;
}

BOOL Timer::isTimer(int index) {
	return isTimerEnabled[index];
}