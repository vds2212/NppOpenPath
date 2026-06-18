#pragma once

#include "TCodePage.h"

#include <string>
#include <vector>
#include <filesystem>

class CPath {
public:
	CPath() = default;

	explicit CPath(const std::tstring& path);

	virtual ~CPath() = default;

	bool isRelatif() const;

	std::tstring getPath() const;
	bool empty() const;

	std::tstring getTail() const;
	void setTail(const std::tstring& tail);

	CPath getDir() const;

	std::tstring getName() const;

	std::tstring getExt() const;
	void setExt(const std::tstring& ext);

	bool operator<(const CPath& path) const;
	bool operator>=(const CPath& path) const { return !(*this < path); }
	bool operator>(const CPath& path) const { return (path < *this); }
	bool operator<=(const CPath& path) const { return !(path < *this); }
	bool operator!=(const CPath& path) const { return *this < path || *this > path; }
	bool operator==(const CPath& path) const { return !(*this != path); }

	CPath& operator+=(const CPath& path);
	CPath& operator-=(const CPath& path);

	static const TCHAR g_separator;

private:
	void reduce();
	std::filesystem::path m_path;
};

CPath operator+(const CPath& pathA, const CPath& pathB);

CPath operator-(const CPath& pathA, const CPath& pathB);

std::string getRelativePath(const std::string& path, const std::string& referencePath = "");
