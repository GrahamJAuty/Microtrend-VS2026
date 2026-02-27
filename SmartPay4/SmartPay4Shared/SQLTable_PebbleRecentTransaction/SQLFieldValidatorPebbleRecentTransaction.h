#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorPebbleRecentTransaction : public CSQLFieldValidatorBase 
{
public:
	static void SetTransactionID(CString str, CString& strTarget);
	static void SetDateTime(CString str, CString& strTarget);
};

//**********************************************************************

