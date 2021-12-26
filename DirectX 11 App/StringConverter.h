#pragma once
#include <string>

typedef const char* CString;
typedef const wchar_t* CWString;

class StringCon
{
public:
	///String -> WString
	static const std::wstring StringToWString(const std::string& string)
	{
		return std::wstring(string.begin(), string.end());
	}

	///WString -> String
	static const std::string WStringToString(const std::wstring& wstring)
	{
		return std::string(wstring.begin(), wstring.end());
	}

	///String -> C String
	static CString StringToCString(const std::string& string)
	{
		return string.c_str();
	}

	///WString -> C WString
	static CWString WStringToCWString(const std::wstring& wstring)
	{
		return wstring.c_str();
	}

};
