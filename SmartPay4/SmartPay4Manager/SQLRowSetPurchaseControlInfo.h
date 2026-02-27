#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRowPurchaseControlRule.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLFieldValidatorPurchaseControlRule.h"
//**********************************************************************

class CSQLRowSetPurchaseControlInfo
{
public:
	CSQLRowSetPurchaseControlInfo();

public:
	void LoadDatabasePurchaseControlInfo();
	
public:
	int GetSize() { return m_arrayRows.GetSize(); }
	void GetRow(int n, CSQLRowPurchaseControlRule& Row);
	void SetRow(int n, CSQLRowPurchaseControlRule& Row);
	
private:
	CArray<CSQLRowPurchaseControlRule, CSQLRowPurchaseControlRule> m_arrayRows;
};

//**********************************************************************
