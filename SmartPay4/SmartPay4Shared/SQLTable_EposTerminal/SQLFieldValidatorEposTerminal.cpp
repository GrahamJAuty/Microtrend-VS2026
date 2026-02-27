//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorEposTerminal.h"
//**********************************************************************

void CSQLFieldValidatorEposTerminal::SetTerminalNo(int n, int& nTarget)
{
	SetInt(n, nTarget, EposTerminal::TerminalNo);
}

//**********************************************************************

void CSQLFieldValidatorEposTerminal::SetTerminalType(int n, int& nTarget)
{
	SetInt(n, nTarget, EposTerminal::TerminalType);
}

//**********************************************************************

void CSQLFieldValidatorEposTerminal::SetTerminalName(CString str, CString& strTarget)
{
	SetString(str, strTarget, EposTerminal::TerminalName);
}

//**********************************************************************

void CSQLFieldValidatorEposTerminal::SetLastUsedDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, EposTerminal::LastUsedDate);
}

//**********************************************************************

void CSQLFieldValidatorEposTerminal::SetLastUsedTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, EposTerminal::LastUsedTime);
}

//**********************************************************************

void CSQLFieldValidatorEposTerminal::SetCCNo(int n, int& nTarget)
{
	SetInt(n, nTarget, EposTerminal::CCNo);
}

//**********************************************************************

void CSQLFieldValidatorEposTerminal::SetUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, EposTerminal::UserID);
}

//**********************************************************************

void CSQLFieldValidatorEposTerminal::SetCardValue(double d, double& dTarget)
{
	SetDouble(d, dTarget, EposTerminal::CardValue);
}

//**********************************************************************

void CSQLFieldValidatorEposTerminal::SetTotalCounter(int n, int& nTarget)
{
	SetInt(n, nTarget, EposTerminal::TotalCounter);
}

//**********************************************************************

void CSQLFieldValidatorEposTerminal::SetTotalInDrawer(double d, double& dTarget)
{
	SetDouble(d, dTarget, EposTerminal::TotalInDrawer);
}

//**********************************************************************