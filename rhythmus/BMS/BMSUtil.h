class BMSUtil {
public:
	static int KeyValueToInt(const TCHAR* Key, int keyLen);
	static int KeyValueToInt(const TCHAR* Key);
	static BOOL convert(const char *input, int len, const char *from, TCHAR **output);
};