/*
	for common files & settings
	similar to stdafx.h
*/

// common include file
#include <Windows.h>

// BMS definition
#define BMS_NOFILE 10
#define BMS_INVALIDFILE 11

#define BMS_LOCALE_UNKNOWN 0
#define BMS_LOCALE_JP 1
#define BMS_LOCALE_KR 2

#define BMS_PARSER_UNKNOWN 0
#define BMS_PARSER_HEADER 1
#define BMS_PARSER_MAINDATA 2

#define BMS_MAXSTR 1024
#define BMS_MAXLINEBUF 1024
#define BMS_MAXKEY 10240
#define BMS_MAXCHANNEL 70
#define MAXBEAT 256

// encode
#define SHIFT_JIS 932
#define EUC_KR 949