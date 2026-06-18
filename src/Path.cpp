#pragma warning(disable : 4786)

#include "Path.h"
#include "replace_environment_variables.h"
#include <filesystem>
namespace fs = std::filesystem;

#ifdef _WIN32
const TCHAR CPath::g_separator = _T('\\');
#else
const TCHAR CPath::g_separator = _T('/');
#endif


CPath::CPath(const std::tstring& path)
{
	// Start by replacing variable names from windows (for example %username%, %appdata% ...)
	std::tstring replaced_path = replaceEnvironmentVariables(path);

	// std::filesystem handles all tokenization automatically!
	m_path = fs::path(replaced_path);
	reduce();
}

bool CPath::isRelatif() const
{
	return m_path.is_relative();
}

std::tstring CPath::getPath() const
{
	if (m_path.empty()) {
		return _T(".");
	}
#if UNICODE
	return m_path.wstring();
#else
	return m_path.string();
#endif
}

bool CPath::empty() const
{
	return m_path.empty();
}

std::tstring CPath::getTail() const
{
#if UNICODE
	return m_path.filename().wstring();
#else
	return m_path.filename().string();
#endif
}

void CPath::setTail(const std::tstring& tail)
{
	m_path.replace_filename(tail);
}

CPath CPath::getDir() const
{
	CPath ret;
	ret.m_path = m_path.parent_path();
	return ret;
}

std::tstring CPath::getName() const
{
#if UNICODE
	return m_path.filename().wstring();
#else
	return m_path.stem().string();
#endif
}

std::tstring CPath::getExt() const
{
	// extension() natively handles the '.' character logic
#if UNICODE
	return m_path.extension().wstring();
#else
	return m_path.extension().string();
#endif
}

void CPath::setExt(const std::tstring& ext)
{
	m_path.replace_extension(ext);
}

bool CPath::operator<(const CPath& path) const
{
	// std::filesystem naturally sorts case-insensitive on Windows, case-sensitive on Linux!
	return m_path < path.m_path;
}

CPath& CPath::operator+=(const CPath& path)
{
	// The /= operator in std::filesystem safely appends paths using OS separators
	m_path /= path.m_path;
	reduce();
	return *this;
}

CPath& CPath::operator-=(const CPath& path)
{
	// std::filesystem natively calculates relative differences between two paths!
	m_path = fs::relative(m_path, path.m_path);
	return *this;
}

void CPath::reduce()
{
	// lexically_normal() resolves all ".." and "." automatically
	m_path = m_path.lexically_normal();
}

CPath operator+(const CPath& pathA, const CPath& pathB)
{
	CPath ret(pathA);
	ret += pathB;
	return ret;
}

CPath operator-(const CPath& pathA, const CPath& pathB)
{
	CPath ret(pathA);
	ret -= pathB;
	return ret;
}

std::string getRelativePath(const std::string& path, const std::string& referencePath /*= ""*/)
{
	fs::path filePath(path);
	fs::path currentPath(referencePath);
	if (referencePath.empty())
		currentPath = fs::current_path();
	auto relativePath = fs::relative(filePath, currentPath).generic_string();
	return relativePath;
}
