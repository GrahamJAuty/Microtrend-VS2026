#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorEcrmanDatabase : public CSQLFieldValidatorBase
{
public:
	static void SetDbNo(int n, int& nTarget);
	static void SetDbName(CString str, CString& strTarget);
};

//**********************************************************************

