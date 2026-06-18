#include "TCodePage.h"

#include <sstream>

#include <assert.h>
#include <string>

#include <Windows.h>


std::wstring astring2wstring(const char *s)
{
	return astring2wstring(std::string(s));
}

std::wstring astring2wstring(const std::string &s)
{
	// Make utf8 to unicode conversion:
	wchar_t ret[1024];
	int success = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, ret, sizeof(ret) / sizeof(wchar_t) - 1);
	if (success == 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);

		wchar_t *buffer = new wchar_t[len];
		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, len);

		std::wstring ret2(buffer);
		delete [] buffer;
		return ret2;
	}
	return std::wstring(ret);
}

std::string wstring2astring(const wchar_t *s)
{
	return wstring2astring(std::wstring(s));
}

std::string wstring2astring(const std::wstring &s)
{
	// Make unicode to utf8 conversion:
	char ret[1024];
	int success = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, ret, sizeof(ret) / sizeof(char) - 1, NULL, NULL);
	if (success == 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		int len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, NULL, 0, NULL, NULL);

		char *buffer = new char[len];
		WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, buffer, len, NULL, NULL);

		std::string ret2(buffer);
		delete [] buffer;
		return ret2;
	}
	return std::string(ret);
}
	
std::tstring astring2tstring(const char *s)
{
#if _UNICODE
	return astring2wstring(s);
#else
	return std::tstring(s);
#endif
}
	
std::tstring astring2tstring(const std::string &s)
{
#if _UNICODE
	return astring2wstring(s);
#else
	return std::tstring(s);
#endif
}

std::string tstring2astring(const TCHAR *s)
{
#if _UNICODE
	return wstring2astring(s);
#else
	return std::string(s);
#endif
}

std::string tstring2astring(const std::tstring &s)
{
#if _UNICODE
	return wstring2astring(s);
#else
	return std::string(s);
#endif
}

std::tstring wstring2tstring(const wchar_t *s)
{
#if _UNICODE
	return std::wstring(s);
#else
	return w2a(s);
#endif
}

std::tstring wstring2tstring(const std::wstring &s)
{
#if _UNICODE
	return s;
#else
	return w2a(s);
#endif
}

std::wstring tstring2wstring(const TCHAR *s)
{
#if _UNICODE
	return std::wstring(s);
#else
	return a2w(s);
#endif
}

std::wstring tstring2wstring(const std::tstring &s)
{
#if _UNICODE
	return s;
#else
	return a2w(s);
#endif
}
