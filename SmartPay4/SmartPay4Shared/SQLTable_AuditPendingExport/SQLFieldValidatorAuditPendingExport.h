#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorAuditPendingExport : public CSQLFieldValidatorBase 
{
public:
	static void SetLineID(__int64 n, __int64& nTarget);
	static void SetUserID(CString str, CString& strTarget);
	static void SetXFerUserID(CString str, CString& strTarget);
	static void SetUserName(CString str, CString& strTarget);
	static void SetGroupNo(int n, int& nTarget);
	static void SetDate(CString str, CString& strTarget);
	static void SetTime(CString str, CString& strTarget);
	static void SetActionType(int n, int& nTarget);
	static void SetSourceType(int n, int& nTarget);
	static void SetAppNo(int n, int& nAppNo);
	static void SetTerminalNo(int n, int& nTarget);
	static void SetCCNo(int n, int& nTarget);
	static void SetText(CString str, CString& strTarget);
	static void SetPointsAdded(int n, int& nTarget);
	static void SetPointsBalance(int n, int& nTarget);
	static void SetPurse1Value(double d, double& dTarget);
	static void SetPurse2Value(double d, double& dTarget);
	static void SetPurse3Value(double d, double& dTarget);
	static void SetCashValue(double d, double& dTarget);
	static void SetPurse1Balance(double d, double& dTarget);
	static void SetPurse2Balance(double d, double& dTarget);
	static void SetPurse3Balance(double d, double& dTarget);
	static void SetNotes(CString str, CString& strTarget);
};

//**********************************************************************

