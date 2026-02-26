#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorStaffCard : public CSQLFieldValidatorBase 
{
public:
	static void SetFolderIndexDbNo(int n, int& nTarget);
	static void SetFolderIndexSetNo(int n, int& nTarget);
	static void SetServerNo(int n, int& nTarget);
	static void SetUserID(__int64 n, __int64& nTarget);
};

//**********************************************************************

