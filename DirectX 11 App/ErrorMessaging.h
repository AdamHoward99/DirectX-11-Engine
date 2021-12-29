#pragma once
#include <comdef.h>
#include <iomanip>
#include <sstream>

class ErrorMes
{
public:
	static void DisplayHRErrorMessage(HRESULT& hr, const int codeLine, const std::string& fileLocation, const std::string& functionName)
	{
		//TODO: Automatically find code line and function name
		_com_error err = hr;
		///Obtain Hexidecimal value of the HRESULT
		std::stringstream ss;
		ss << std::showbase << std::hex << err.Error();
		std::string outputMessage = GetHRESULTValueByHexNumber(ss.str()) + ". " + err.ErrorMessage() + "\nContained at line " + std::to_string(codeLine) + " in function: " + functionName;

		MessageBox(NULL, outputMessage.c_str(), fileLocation.c_str(), MB_OK | MB_ICONERROR);
		exit(-1);
	}

private:
	static std::string GetHRESULTValueByHexNumber(const std::string& ss)
	{
		if (ss == "0x80004001")
			return "E_NOTIMPL: Not implemented";

		else if (ss == "0x80004002")
			return "E_NOINTERFACE: No such interface supported";

		else if (ss == "0x80004003")
			return "E_POINTER: Pointer that is not valid";

		else if (ss == "0x80004004")
			return "E_ABORT: Operation aborted";

		else if (ss == "0x80004005")
			return "E_FAIL: Unspecified failure";

		else if (ss == "0x8000FFFF")
			return "E_UNEXPECTED: Unexpected failure";

		else if (ss == "0x80070005")
			return "E_ACCESSDENIED: General access denied error";

		else if (ss == "0x80070006")
			return "E_HANDLE: Handle is not valid";

		else if (ss == "0x8007000E")
			return "E_OUTOFMEMORY: Failed to allocate necessary memory";

		else if (ss == "0x80070057")
			return "E_INVALIDARG: One or more arguments are not valid";

		return "HRESULT_UNKNOWN_ERROR: Incorrect hexidecimal result obtained on line 18 (ErrorMessaging.h)";
	}
};
