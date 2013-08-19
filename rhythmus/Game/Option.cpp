#include "..\common.h"
#include "Option.h"

Option::Option() {
	// just for initalize
	o[OPTION_AUTOPLAY_OFF] = TRUE;
	o[OPTION_AUTOPLAY_ON] = FALSE;

	o[OPTION_PANEL_1START] = TRUE;
	o[OPTION_SCOREGRAPH_ON] = TRUE;
	o[OPTION_GHOST_TYPEA] = TRUE;

	o[OPTION_1P_GROOVE] = TRUE;
	o[OPTION_1P_SURVIVAL] = FALSE;
	o[OPTION_2P_GROOVE] = FALSE;
	o[OPTION_2P_SURVIVAL] = FALSE;
	
	o[OPTION_SCORE_DISABLE] = FALSE;
	o[OPTION_SCORE_ENABLE] = TRUE;
	o[OPTION_CLEAR_EASY] = FALSE;
	o[OPTION_CLEAR_NORMAL] = TRUE;
	o[OPTION_CLEAR_HARD] = FALSE;
	o[OPTION_FULLCOMBO] = FALSE;

	o[OPTION_PLAY_1P_AA] = TRUE;
	o[OPTION_PLAY_AA] = TRUE;
	o[OPTION_RESULT_BEFORE_AA] = TRUE;

	o[920] = TRUE;
	o[904] = TRUE;
	o[907] = TRUE;
	o[910] = TRUE;
	o[912] = TRUE;
	o[916] = TRUE;
	o[914] = TRUE;

	// ALWAYS
	o[OPTION_TRUE] = TRUE;
	o[OPTION_FALSE] = FALSE;
}

BOOL Option::LoadOption() {
	return TRUE;
}

BOOL Option::SaveOption() {
	return TRUE;
}

BOOL Option::CheckOption(vector <int> op) {
	for (int i=0; i<op.size(); i++) {
		int _op = op[i];
		if (_op < 0) {
			if (o[_op*-1] == TRUE) return FALSE;
		} else {
			if (o[_op] == FALSE) return FALSE;
		}
	}
	return TRUE;
}