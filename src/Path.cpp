// Path.cpp: implementation of the CPath class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)

#include "Path.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const TCHAR CPath::g_separator = _T('\\');

CPath::CPath()
: m_isRelatif(true)
{

}

CPath::~CPath()
{

}

CPath::CPath(std::tstring path)
: m_isRelatif(true)
{
	TCHAR *buffer = new TCHAR[path.size() + 1];
	_tcscpy(buffer, path.c_str());
	
	// ndv : we add a "\" if the path start with "\\" like "\\Vivaldi\users\ndv\config\..." 
	// so we can start the application or open an setup file from another machine.
	if (_tcsncmp(buffer, _T("\\\\"), 2) == 0) {
		m_parts.push_back(_T("\\"));
		m_isRelatif = false;
	}
	
	for (TCHAR *part = _tcstok(buffer, _T("\\/")); part; part = _tcstok(NULL, _T("\\/"))) {
		m_parts.push_back(part);
	}

	if (m_parts.size() && (m_parts[0].size() == 2 && m_parts[0][1] == _T(':') || m_parts[0] == _T("")))
		m_isRelatif = false;
	delete [] buffer;
	reduce();
}

bool CPath::isRelatif() const
{
	return m_isRelatif;
}

std::tstring CPath::getPath() const
{
	tstring ret;
	for (unsigned int i = 0; i < m_parts.size(); ++i) {
		if (i > 0)
			ret += g_separator;
		ret += m_parts[i];
	}

	if (ret == _T(""))
		ret = _T(".");

	return ret;
}

std::tstring CPath::getTail() const
{
	if (m_parts.size())
		return m_parts.back();
	return _T("");
}

void CPath::setTail(std::tstring tail)
{
	if (m_parts.size())
		m_parts.pop_back();

	m_parts.push_back(tail);
}

CPath CPath::getDir() const
{
	CPath ret;
	if(m_parts.begin() < m_parts.end())
		ret.m_parts.insert(ret.m_parts.begin(), m_parts.begin(), m_parts.end() - 1);
	ret.m_isRelatif = m_isRelatif;
	return ret;
}

std::tstring CPath::getName() const
{
	std::tstring tail = getTail();
	size_t i = tail.find_last_of('.');
	if (i == -1)
		return tail;

	return tail.substr(0, i);
}

std::tstring CPath::getExt() const
{
	std::tstring tail = getTail();
	size_t i = tail.find_last_of('.');
	if (i == -1)
		return _T("");

	return tail.substr(i);
}

void CPath::setExt(std::tstring ext)
{
	std::tstring name = getName();
	setTail(name + ext);
}

bool CPath::operator < (const CPath &path) const
{
	// Relatif before non relatif path:
	if (m_isRelatif && !path.m_isRelatif)
		return true;

	if (!m_isRelatif && path.m_isRelatif)
		return false;

	// Lexicographic order otherwise:
	for (unsigned int i = 0; i < m_parts.size() && i < path.m_parts.size(); ++i) {
		int c = _tcsicmp(m_parts[i].c_str(), path.m_parts[i].c_str());
		if (c < 0)
			return true;

		if (c > 0)
			return false;
	}
	if (m_parts.size() < path.m_parts.size())
		return true;

	return false;
}

#if 0
bool CPath::operator == (const CPath &path) const
{
	if (m_parts != path.m_parts)
		return false;

	if (m_isRelatif != path.m_isRelatif)
		return false;

	if (m_parts.size() != path.m_parts.size())
		return false;

	return true;
}
#endif

CPath& CPath::operator += (const CPath &path)
{
	if (path.isRelatif()) {
		m_parts.insert(m_parts.end(), path.m_parts.begin(), path.m_parts.end());
		reduce();
	}
	else if (&path != this) {
		*this = path;
	}
	return *this;
}

CPath& CPath::operator -= (const CPath &path)
{
	if (isRelatif() != path.isRelatif()) {
		return *this;
	}
	unsigned int i = 0;
	for (i = 0; i < m_parts.size() && i < path.m_parts.size() && m_parts[i] == path.m_parts[i]; ++i);
	if (!isRelatif() && !path.isRelatif() && i == 0) {
		return *this;
	}

	m_isRelatif = true;
	m_parts.erase(m_parts.begin(), m_parts.begin() + i);
	m_parts.insert(m_parts.begin(), path.m_parts.size() - i, _T(".."));

	return *this;
}

void CPath::reduce()
{
	vector<tstring>::iterator e = m_parts.begin();
	while (e != m_parts.end()) {
		// Get rid of '.' part:
		if (*e == _T(".")) {
			e = m_parts.erase(e);
			continue;
		}
		// Get rid of 'foo\..' constructions:
		if (*e == _T("..") && e != m_parts.begin() && *(e - 1) != _T("..")) {
			e = m_parts.erase(e - 1, e + 1);
			if (e == m_parts.begin()) {
				m_isRelatif = true;
			}
			continue;
		}
		++e;
	}
}

CPath operator + (const CPath &pathA, const CPath &pathB)
{
	CPath ret(pathA);
	ret += pathB;
	return ret;
}

CPath operator - (const CPath &pathA, const CPath &pathB)
{
	CPath ret(pathA);
	ret -= pathB;
	return ret;
}
