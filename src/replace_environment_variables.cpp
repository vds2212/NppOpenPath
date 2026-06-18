#include "replace_environment_variables.h"

#include <cstdlib>
#include <string>
#include <algorithm>
#include <cctype>

#ifdef _WIN32
#include <Windows.h>

#define INFO_BUFFER_SIZE 4096
#endif

namespace {

#ifndef _WIN32

//------------------------------------------------
bool isVariableNameChar(char ch)
{
	return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_';
}

//------------------------------------------------
std::string get_environment_value(const std::string& name)
{
	// 1. Try a direct lookup first (Works for native Linux vars like $USER or $HOME)
	if (const char* env_val = std::getenv(name.c_str())) {
		return env_val;
	}

	// 2. Windows-to-Linux Translation Dictionary Fallback
	// Convert the requested name to UPPERCASE to ensure case-insensitive matching
	std::string upper_name = name;
	std::transform(upper_name.begin(), upper_name.end(), upper_name.begin(),
		[](unsigned char c) { return std::toupper(c); });

	if (upper_name == "APPDATA" || upper_name == "LOCALAPPDATA") {
		if (const char* home_val = std::getenv("HOME")) {
			// Linux equivalent of AppData is usually ~/.config
			return std::string(home_val) + "/.config";
		}
	}
	else if (upper_name == "USERNAME") {
		if (const char* user_val = std::getenv("USER")) {
			return user_val;
		}
	}
	else if (upper_name == "USERPROFILE" || upper_name == "HOMEPATH") {
		if (const char* home_val = std::getenv("HOME")) {
			return home_val;
		}
	}

	return {};
}

//------------------------------------------------
std::string replacePercentageVariables(const std::string& value)
{
	std::string result;
	result.reserve(value.size());

	for (std::size_t i = 0; i < value.size();) {
		if (value[i] != '%') {
			result += value[i++];
			continue;
		}

		const std::size_t end_pos = value.find('%', i + 1);
		if (end_pos == std::string::npos) {
			result += value[i++];
			continue;
		}

		const std::string variable_name = value.substr(i + 1, end_pos - i - 1);
		const std::string variable_value = get_environment_value(variable_name);

		if (!variable_value.empty()) {
			result += variable_value;
		}
		else {
			result += value.substr(i, end_pos - i + 1);
		}

		i = end_pos + 1;
	}

	return result;
}

//------------------------------------------------
std::string replaceDollarVariables(const std::string& value)
{
	std::string result;
	result.reserve(value.size());

	for (std::size_t i = 0; i < value.size();) {
		if (value[i] != '$') {
			result += value[i++];
			continue;
		}

		if (i + 1 >= value.size()) {
			result += value[i++];
			continue;
		}

		if (value[i + 1] == '{') {
			const std::size_t end_pos = value.find('}', i + 2);
			if (end_pos == std::string::npos) {
				result += value[i++];
				continue;
			}

			const std::string variable_name = value.substr(i + 2, end_pos - i - 2);
			const std::string variable_value = get_environment_value(variable_name);

			if (!variable_value.empty()) {
				result += variable_value;
			}
			else {
				result += value.substr(i, end_pos - i + 1);
			}

			i = end_pos + 1;
			continue;
		}

		std::size_t end_pos = i + 1;
		while (end_pos < value.size() && isVariableNameChar(value[end_pos])) {
			++end_pos;
		}

		if (end_pos == i + 1) {
			result += value[i++];
			continue;
		}

		const std::string variable_name = value.substr(i + 1, end_pos - i - 1);
		const std::string variable_value = get_environment_value(variable_name);

		if (!variable_value.empty()) {
			result += variable_value;
		}
		else {
			result += value.substr(i, end_pos - i);
		}

		i = end_pos;
	}

	return result;
}

#endif
} 

std::tstring replaceEnvironmentVariables(const std::tstring& value)
{
#ifdef _WIN32
	TCHAR buffer[INFO_BUFFER_SIZE];
	if (DWORD r = ExpandEnvironmentStrings(value.c_str(), buffer, INFO_BUFFER_SIZE); r) {
		return std::tstring(buffer);
	}
	return value;
#else
	std::string result = value;

	result = replacePercentageVariables(result);
	result = replaceDollarVariables(result);

	return result;
#endif
}