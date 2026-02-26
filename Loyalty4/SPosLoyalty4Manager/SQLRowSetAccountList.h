#pragma once
//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRowAccountList.h"
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLFieldValidatorAccount.h"
//**********************************************************************

class CSQLRowSetAccountList
{
public:
	CSQLRowSetAccountList();

public:
	void SelectRows(__int64 nUserIDToFind );
	//void SelectRowsForSurnameList(__int64 nUserIDToFind, bool bSortByGroup);
	
public:
	int GetSize() { return m_arrayRows.GetSize(); }
	void GetRow(int n, CSQLRowAccountList& Row);
	void SetRow(int n, CSQLRowAccountList& Row);
	int GetUserIDPos() { return m_nUserIDPos; }
	void FindUserID(__int64 nUserID);

private:
	CArray<CSQLRowAccountList,CSQLRowAccountList> m_arrayRows;

private:
	int m_nUserIDPos;
};

//**********************************************************************
