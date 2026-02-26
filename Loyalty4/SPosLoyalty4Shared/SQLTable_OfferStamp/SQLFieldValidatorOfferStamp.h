#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorOfferStamp : public CSQLFieldValidatorBase 
{
public:
	static void SetUserID(__int64 n, __int64& nTarget);
	static void SetStampOfferID(int n, int& nTarget);
	static void SetEdition(int n, int& nTarget);
	static void SetAwardDate(CString str, CString& strTarget);
	static void SetExpireDate(CString str, CString& strTarget);
	static void SetStampCount(int n, int& nTarget);
};

//**********************************************************************

