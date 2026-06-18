#pragma once

#ifdef _WIN32
#include <tchar.h> // Standard lowercase MSVC header
#else
typedef char TCHAR;
#define _T(x) x
#define _TEXT(x) x
#include <strings.h>
#define _tcsicmp strcasecmp
#define _stricmp strcasecmp
#define _tcscmp strcmp
#define _tcslen strlen
#define _tcscpy strcpy

// POSIX path mappings
#ifndef _MAX_PATH
#define _MAX_PATH 4096
#endif
#define _tgetcwd getcwd
#define _getcwd getcwd
#define _tchdir chdir

#define _tcscat strcat
#define _tcstok strtok
#define _snprintf snprintf
#define _sntprintf snprintf
#define _tfopen fopen
#define _tsystem system
#define _tremove remove

#endif

#include <string>
#include <sstream> 

// 3. THE TSTRING MACROS (Forced ASCII/UTF-8 for Linux)
#if defined(_WIN32) && defined(_UNICODE)
// --- Windows Unicode (UTF-16) Path ---
#define tstring wstring
#define tstringstream wstringstream
#define tostreamh wostreamh
#define tformat wformat
#else
// --- Linux & Windows MBCS (UTF-8 / ASCII) Path ---
// On Linux, we ALWAYS want to fall into this block so tstring becomes standard string
#define tstring string
#define tstringstream stringstream
#define tostreamh ostreamh
#define tformat format
#endif

std::wstring astring2wstring(const char* s);
std::wstring astring2wstring(const std::string& s);

std::string u32string2astring(const std::u32string& s);

std::string wstring2astring(const wchar_t* s);
std::string wstring2astring(const std::wstring& s);
//std::wstring astring2wstring_ACP(const std::string & s);

std::tstring astring2tstring(const char* s);
std::tstring astring2tstring(const std::string& s);

std::string tstring2astring(const TCHAR* s);
std::string tstring2astring(const std::tstring& s);

std::tstring wstring2tstring(const wchar_t* s);
std::tstring wstring2tstring(const std::wstring& s);

std::wstring tstring2wstring(const TCHAR* s);
std::wstring tstring2wstring(const std::tstring& s);

std::string wstring2ustring(const std::wstring& s);
std::wstring ustring2wstring(const std::string& s);

std::string ustring2astring(const std::string& s);

inline std::wstring a2w(const char* s) { return astring2wstring(s); }
inline std::wstring a2w(const std::string& s) { return astring2wstring(s); }

inline std::string w2a(const wchar_t* s) { return wstring2astring(s); }
inline std::string w2a(const std::wstring& s) { return wstring2astring(s); }

inline std::tstring a2t(const char* s) { return astring2tstring(s); }
inline std::tstring a2t(const std::string& s) { return astring2tstring(s); }

inline std::string t2a(const TCHAR* s) { return tstring2astring(s); }
inline std::string t2a(const std::tstring& s) { return tstring2astring(s); }

inline std::tstring w2t(const wchar_t* s) { return wstring2tstring(s); }
inline std::tstring w2t(const std::wstring& s) { return wstring2tstring(s); }

inline std::wstring t2w(const TCHAR* s) { return tstring2wstring(s); }
inline std::wstring t2w(const std::tstring& s) { return tstring2wstring(s); }

