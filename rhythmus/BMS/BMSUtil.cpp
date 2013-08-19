#include "..\common.h"
#include "..\iconv.h"
#include "BMSUtil.h"
#pragma comment(lib, "iconv.lib")

int BMSUtil::KeyValueToInt(const TCHAR *key, int keyLen) {
	int r=0;
	char val[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	for (int i=0; i<keyLen; i++) {
		r *= 16;
		if (key[i] >= L'0' || key[i] <= L'9') r+=key[i]-L'0';
		else if (key[i] >= L'A' || key[i] <= L'F') r+=key[i]-L'A';
		else return -1;
	}
	return r;
}

int BMSUtil::KeyValueToInt(const TCHAR *key) {
	return KeyValueToInt(key, wcslen(key));
}

BOOL BMSUtil::convert(const char *input, int len, const char *from, TCHAR **output)
{
    iconv_t cd;
    if ((cd = iconv_open("UTF-16LE", from)) == (iconv_t) -1) {
        iconv_close(cd);
        return FALSE;
    } else {
        size_t in_bytes = len;
		size_t out_bytes = (in_bytes+1) *4;
        char *out = (char *) calloc(out_bytes, sizeof(char));
        char *outp = out;

		if(iconv(cd, &input, &in_bytes, &outp, &out_bytes)==-1) {
			char *sterr = strerror(errno);
			iconv_close(cd);
			if (out) free(out);
			return FALSE;
		}

		*output = (TCHAR*)malloc(out_bytes);
		wcscpy(*output, (TCHAR*)out);

        iconv_close(cd);
		if (out) free(out);
        return TRUE;
	}
}