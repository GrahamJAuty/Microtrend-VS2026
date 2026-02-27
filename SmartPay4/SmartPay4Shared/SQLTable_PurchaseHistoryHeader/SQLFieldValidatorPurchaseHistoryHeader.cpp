//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorPurchaseHistoryHeader.h"
//**********************************************************************

void CSQLFieldValidatorPurchaseHistoryHeader::SetLineID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, PurchaseHistoryHeader::LineID);
}

//**********************************************************************

void CSQLFieldValidatorPurchaseHistoryHeader::SetDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, PurchaseHistoryHeader::Date);
}

//**********************************************************************

void CSQLFieldValidatorPurchaseHistoryHeader::SetTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, PurchaseHistoryHeader::Time);
}

//**********************************************************************

