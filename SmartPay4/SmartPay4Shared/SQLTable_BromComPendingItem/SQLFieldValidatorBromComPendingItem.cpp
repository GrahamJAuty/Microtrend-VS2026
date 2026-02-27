//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorBromComPendingItem.h"
//**********************************************************************

void CSQLFieldValidatorBromComPendingItem::SetOrderID(int n, int& nTarget)
{
	SetInt(n, nTarget, BromComPendingItem::OrderID);
}

//**********************************************************************

void CSQLFieldValidatorBromComPendingItem::SetPersonID(int n, int& nTarget)
{
	SetInt(n, nTarget, BromComPendingItem::PersonID);
}

//**********************************************************************

void CSQLFieldValidatorBromComPendingItem::SetAmount(double d, double& dTarget)
{
	SetDouble(d, dTarget, BromComPendingItem::Amount);
}

//**********************************************************************

