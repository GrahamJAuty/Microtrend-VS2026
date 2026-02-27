//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorBromComPendingOrder.h"
//**********************************************************************

void CSQLFieldValidatorBromComPendingOrder::SetOrderID(int n, int& nTarget)
{
	SetInt(n, nTarget, BromComPendingOrder::OrderID);
}

//**********************************************************************

void CSQLFieldValidatorBromComPendingOrder::SetStatus(CString str, CString& strTarget)
{
	SetString(str, strTarget, BromComPendingOrder::Status);
}

//**********************************************************************

void CSQLFieldValidatorBromComPendingOrder::SetLastModified(CString str, CString& strTarget)
{
	SetString(str, strTarget, BromComPendingOrder::LastModified);
}

//**********************************************************************