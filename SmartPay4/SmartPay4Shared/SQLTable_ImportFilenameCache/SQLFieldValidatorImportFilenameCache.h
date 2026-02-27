#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorImportFilenameCache : public CSQLFieldValidatorBase 
{
public:
	static void SetLineID(__int64 n, __int64& nTarget);
	static void SetType(int n, int& nTarget);
	static void SetFilename(CString str, CString& strTarget);
	static void SetDateTime(CString str, CString& strTarget);
};

//**********************************************************************
