#ifndef TCODEPAGE_H
#define TCODEPAGE_H

#include <tchar.h>

#include <wchar.h>
#include <string>

#ifdef _UNICODE

#define tstring wstring
#define tstringstream wstringstream

#else

#define tstring string
#define tstringstream stringstream

#endif

std::wstring astring2wstring(const char *s);	
std::wstring astring2wstring(const std::string &s);	

std::string wstring2astring(const TCHAR *s);
std::string wstring2astring(const std::wstring &s);
	
std::tstring astring2tstring(const char *s);
std::tstring astring2tstring(const std::string &s);

std::string tstring2astring(const TCHAR *s);
std::string tstring2astring(const std::tstring &s);

std::tstring wstring2tstring(const wchar_t *s);
std::tstring wstring2tstring(const std::wstring &s);

std::wstring tstring2wstring(const TCHAR *s);
std::wstring tstring2wstring(const std::tstring &s);

inline std::wstring a2w(const char *s) { return astring2wstring(s); }
inline std::wstring a2w(const std::string &s) { return astring2wstring(s); }

inline std::string w2a(const TCHAR *s) { return wstring2astring(s); }
inline std::string w2a(const std::wstring &s) { return wstring2astring(s); }
	
inline std::tstring a2t(const char *s) { return astring2tstring(s); }
inline std::tstring a2t(const std::string &s) { return astring2tstring(s); }

inline std::string t2a(const TCHAR *s) { return tstring2astring(s); }
inline std::string t2a(const std::tstring &s) { return tstring2astring(s); }

inline std::tstring w2t(const wchar_t *s) { return wstring2tstring(s); }
inline std::tstring w2t(const std::wstring &s) { return wstring2tstring(s); }

inline std::wstring t2w(const TCHAR *s) { return tstring2wstring(s); }
inline std::wstring t2w(const std::tstring &s) { return tstring2wstring(s); }

#endif
