#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorPluInfo : public CSQLFieldValidatorBase 
{
public:
	static void SetPluNo(__int64 n, __int64& nTarget);
	static void SetDescription(CString str, CString& strTarget);
	static void SetDeptNo(int n, int& nTarget);
	static void SetTaxCode(CString str, CString& strTarget);
	static void SetModType(int n, int& nTarget);
	static void SetPoints(int n, int& nTarget);
	static void SetPCtrlRuleID(int n, int& nTarget);
	static void SetStampWeight(int n, int& nTarget);
};

//**********************************************************************

