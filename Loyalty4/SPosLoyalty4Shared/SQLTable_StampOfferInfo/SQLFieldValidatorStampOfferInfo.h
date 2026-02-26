#pragma once
//**********************************************************************
#include "..\SQLFieldValidatorBase.h"
//**********************************************************************

class CSQLFieldValidatorStampOfferInfo : public CSQLFieldValidatorBase
{
public:
	static void SetStampOfferID(int n, int& nTarget);
	static void SetEdition(int n, int& nTarget);
	static void SetThreshold(int n, int& nTarget);
	static void SetExpiry(int n, int& nTarget);
	static void SetDescription(CString str, CString& strTarget);
};

//**********************************************************************

