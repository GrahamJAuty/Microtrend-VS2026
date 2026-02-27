#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorEposTerminal : public CSQLFieldValidatorBase 
{
public:
	static void SetTerminalNo(int n, int& nTarget);
	static void SetTerminalType(int n, int& nTarget);
	static void SetTerminalName(CString str, CString& strTarget);
	static void SetLastUsedDate(CString str, CString& strTarget);
	static void SetLastUsedTime(CString str, CString& strTarget);
	static void SetCCNo(int n, int& nTarget);
	static void SetUserID(__int64 n, __int64& nTarget);
	static void SetCardValue(double d, double& dTarget);
	static void SetTotalCounter(int n, int& nTarget);
	static void SetTotalInDrawer(double d, double& dTarget);
};

//**********************************************************************

