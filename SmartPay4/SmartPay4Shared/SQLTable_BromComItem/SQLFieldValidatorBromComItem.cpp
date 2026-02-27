//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorBromComItem.h"
//**********************************************************************

void CSQLFieldValidatorBromComItem::SetOrderID(int n, int& nTarget)
{
	SetInt(n, nTarget, BromComItem::OrderID);
}

//**********************************************************************

void CSQLFieldValidatorBromComItem::SetPersonID(int n, int& nTarget)
{
	SetInt(n, nTarget, BromComItem::PersonID);
}

//**********************************************************************

void CSQLFieldValidatorBromComItem::SetAmount(double d, double& dTarget)
{
	SetDouble(d, dTarget, BromComItem::Amount);
}

//**********************************************************************

void CSQLFieldValidatorBromComItem::SetStatus(CString str, CString& strTarget)
{
	SetString(str, strTarget, BromComItem::Status);
}

//**********************************************************************