#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorAuditPeriod : public CSQLFieldValidatorBase 
{
public:
	static void SetPeriodID(int n, int& nTarget);
	static void SetPeriodName(CString str, CString& strTarget);
	static void SetStartTime(int n, int& nTarget);
};

//**********************************************************************

