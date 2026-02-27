#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountList.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLFieldValidatorAccount.h"
//**********************************************************************

class CSQLRowSetAccountList
{
public:
	CSQLRowSetAccountList();

public:
	void SelectRows(__int64 nUserIDToFind );
	void SelectRowsForSurnameList(__int64 nUserIDToFind, bool bSortByGroup);
	
public:
	int GetSize() { return m_arrayRows.GetSize(); }
	void GetRow(int n, CSQLRowAccountList& Row);
	int GetUserIDPos() { return m_nUserIDPos; }

private:
	CArray<CSQLRowAccountList,CSQLRowAccountList> m_arrayRows;

private:
	int m_nUserIDPos;
};

//**********************************************************************
