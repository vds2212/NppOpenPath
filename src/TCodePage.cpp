#include "TCodePage.h"

#include <sstream>
#include <cassert>
#include <cstdlib>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <codecvt>

std::wstring astring2wstring(const char* s)
{
	return astring2wstring(std::string(s));
}

// a string is the encoding of the local system (ACP)

// w string is utf-16

// u string is utf-8

// t is the encoding of the program
// - utf-16 if unicode
// - acp otherwise

std::wstring astring2wstring(const std::string& s)
{
#ifdef _WIN32
	int encoding = CP_ACP;

	// Make utf8 to unicode conversion:
	wchar_t ret[1024];
	int success = MultiByteToWideChar(encoding, 0, s.c_str(), -1, ret, sizeof(ret) / sizeof(wchar_t) - 1);
	if (success == 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		int len = MultiByteToWideChar(encoding, 0, s.c_str(), -1, nullptr, 0);

		wchar_t* buffer = new wchar_t[len];
		MultiByteToWideChar(encoding, 0, s.c_str(), -1, buffer, len);

		std::wstring wret(buffer);
		delete[] buffer;
		return wret;
	}
	return std::wstring(ret);
#else
	// LINUX: Use standard C conversion (UTF-8 to UTF-32)
	if (s.empty()) return std::wstring();
	size_t len = std::mbstowcs(nullptr, s.c_str(), 0);
	if (len == (size_t)-1) return L"";

	std::wstring ret(len, L'\0');
	std::mbstowcs(&ret[0], s.c_str(), len);
	return ret;
#endif
}

std::string wstring2astring(const wchar_t* s)
{
	return wstring2astring(std::wstring(s));
}

std::string u32string2astring(const std::u32string&)
{
	//std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
	//return conv.to_bytes(s);
	return "";
}

std::string wstring2astring(const std::wstring& s)
{
#ifdef _WIN32
	int encoding = CP_ACP;
	// Make Unicode to utf8 conversion:
	char ret[1024];
	int success = WideCharToMultiByte(encoding, 0, s.c_str(), -1, ret, sizeof(ret) / sizeof(char) - 1, nullptr, nullptr);
	ret[1024 - 1] = '\0';
	if (success == 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		int len = WideCharToMultiByte(encoding, 0, s.c_str(), -1, nullptr, 0, nullptr, nullptr);
		char* buffer = new char[len + 1];
		WideCharToMultiByte(encoding, 0, s.c_str(), -1, buffer, len, nullptr, nullptr);
		buffer[len] = '\0';

		std::string wret(buffer);
		delete[] buffer;
		return wret;
	}
	return std::string(ret);
#else
	// LINUX: Use standard C conversion (UTF-32 to UTF-8)
	if (s.empty()) return std::string();
	size_t len = std::wcstombs(nullptr, s.c_str(), 0);
	if (len == (size_t)-1) return "";

	std::string ret(len, '\0');
	std::wcstombs(&ret[0], s.c_str(), len);
	return ret;
#endif
}

std::string wstring2ustring(const std::wstring& s)
{
#ifdef _WIN32
	int encoding = CP_UTF8;
	char ret[1024];
	int success = WideCharToMultiByte(encoding, 0, s.c_str(), -1, ret, sizeof(ret) / sizeof(char) - 1, nullptr, nullptr);
	if (success == 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		int len = WideCharToMultiByte(encoding, 0, s.c_str(), -1, nullptr, 0, nullptr, nullptr);

		char* buffer = new char[len];
		WideCharToMultiByte(encoding, 0, s.c_str(), -1, buffer, len, nullptr, nullptr);

		std::string wret(buffer);
		delete[] buffer;
		return wret;
	}
	return std::string(ret);
#else
	// LINUX: ustring and astring are both standard UTF-8.
	// Just reuse the astring conversion!
	return wstring2astring(s);
#endif
}

std::wstring ustring2wstring(const std::string& s)
{
#ifdef _WIN32
	int encoding = CP_UTF8;
	wchar_t ret[1024];
	int success = MultiByteToWideChar(encoding, 0, s.c_str(), -1, ret, sizeof(ret) / sizeof(wchar_t) - 1);
	if (success == 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		int len = MultiByteToWideChar(encoding, 0, s.c_str(), -1, nullptr, 0);

		wchar_t* buffer = new wchar_t[len];
		MultiByteToWideChar(encoding, 0, s.c_str(), -1, buffer, len);

		std::wstring wret(buffer);
		delete[] buffer;
		return wret;
	}
	return std::wstring(ret);
#else
	// LINUX: ustring and astring are both standard UTF-8.
	return astring2wstring(s);
#endif
}

std::string ustring2astring(const std::string& s)
{
#ifdef _WIN32
	return wstring2astring(ustring2wstring(s));
#else
	// LINUX: They are the exact same format. No conversion needed!
	return s;
#endif
}

std::tstring astring2tstring(const char* s)
{
#if _UNICODE
	return astring2wstring(s);
#else
	return std::tstring(s);
#endif
}

std::tstring astring2tstring(const std::string& s)
{
#if _UNICODE
	return astring2wstring(s);
#else
	return std::tstring(s);
#endif
}

std::string tstring2astring(const TCHAR* s)
{
#if _UNICODE
	return wstring2astring(s);
#else
	return std::string(s);
#endif
}

std::string tstring2astring(const std::tstring& s)
{
#if _UNICODE
	return wstring2astring(s);
#else
	return std::string(s);
#endif
}

std::tstring wstring2tstring(const wchar_t* s)
{
#if _UNICODE
	return std::wstring(s);
#else
	return w2a(s);
#endif
}

std::tstring wstring2tstring(const std::wstring& s)
{
#if _UNICODE
	return s;
#else
	return w2a(s);
#endif
}

std::wstring tstring2wstring(const TCHAR* s)
{
#if _UNICODE
	return std::wstring(s);
#else
	return a2w(s);
#endif
}

std::wstring tstring2wstring(const std::tstring& s)
{
#if _UNICODE
	return s;
#else
	return a2w(s);
#endif
}
