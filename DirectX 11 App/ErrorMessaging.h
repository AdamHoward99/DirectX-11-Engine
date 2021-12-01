#pragma once
#include <comdef.h>

class ErrorMes
{
public:
	static void DisplayErrMessage(HRESULT hr)
	{
		_com_error err = hr;
		MessageBox(NULL, err.ErrorMessage(), (LPCSTR)err.WCode(), MB_OK | MB_ICONERROR);
		exit(-1);
	}
};
