#include "..\common.h"
#include "Option.h"

Option::Option() {
	// just for initalize

	// ALWAYS FALSE
	o[OPTION_TRUE] = TRUE;
	o[OPTION_FALSE] = FALSE;
}

BOOL Option::LoadOption() {
	return TRUE;
}

BOOL Option::SaveOption() {
	return TRUE;
}

BOOL Option::CheckOption(vector <int> op, vector <int> nop) {
	for (int i=0; i<op.size(); i++) {
		if (o[ op[i] ] == FALSE) return FALSE;
	}
	for (int i=0; i<nop.size(); i++) {
		if (o[ nop[i] ] == TRUE) return FALSE;
	}
	return TRUE;
}