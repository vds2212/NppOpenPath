// Path.h: interface for the CPath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATH_H__69397CA8_998D_4987_836B_E8992F0FF975__INCLUDED_)
#define AFX_PATH_H__69397CA8_998D_4987_836B_E8992F0FF975__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TCodePage.h"

#include <string>
#include <vector>

class CPath  
{
public:
	// Constructors:
	CPath();
	explicit CPath(std::tstring path);

	virtual ~CPath();

	bool isRelatif() const;

	std::tstring getPath() const;

	std::tstring getTail() const;
	void setTail(std::tstring tail);

	CPath getDir() const;

	std::tstring getName() const;

	std::tstring getExt() const;
	void setExt(std::tstring ext);

	bool operator < (const CPath &path) const;
	bool operator >= (const CPath &path) const { return !(*this < path); }
	bool operator > (const CPath &path) const { return (path < *this); }
	bool operator <= (const CPath &path) const { return !(path < *this); }
	bool operator != (const CPath &path) const { return *this < path || *this > path; }
	bool operator == (const CPath &path) const { return !(*this != path); }

	CPath& operator += (const CPath &path);
	CPath& operator -= (const CPath &path);

	static const TCHAR g_separator;

	//friend CPath operator - (const CPath &pathA, const CPath &pathB);

private:
	void reduce();

	bool m_isRelatif;
	std::vector<std::tstring> m_parts;
};


CPath operator + (const CPath &pathA, const CPath &pathB);

CPath operator - (const CPath &pathA, const CPath &pathB);

#endif // !defined(AFX_PATH_H__69397CA8_998D_4987_836B_E8992F0FF975__INCLUDED_)
