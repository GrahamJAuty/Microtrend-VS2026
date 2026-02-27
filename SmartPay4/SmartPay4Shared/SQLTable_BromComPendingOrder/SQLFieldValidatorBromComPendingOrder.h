#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorBromComPendingOrder : public CSQLFieldValidatorBase 
{
public:
	static void SetOrderID(int n, int& nTarget);
	static void SetStatus(CString str, CString& strTarget);
	static void SetLastModified(CString str, CString& strTarget);
};

//**********************************************************************

