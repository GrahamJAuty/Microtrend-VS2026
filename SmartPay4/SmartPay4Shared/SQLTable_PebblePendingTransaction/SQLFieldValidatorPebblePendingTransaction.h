#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorPebblePendingTransaction : public CSQLFieldValidatorBase 
{
public:
	static void SetTransactionID(CString str, CString& strTarget);
	static void SetMemberID(CString str, CString& strTarget);
	static void SetDateTime(CString str, CString& strTarget);
	static void SetName(CString str, CString& strTarget);
	static void SetAmount(int n, int& nTarget);
};

//**********************************************************************

