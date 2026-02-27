#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLFieldValidatorAccount.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFind.h"
//**********************************************************************

struct CSQLRowAccountFindInfo
{
public:
	int m_nGroupSel;
	int m_nGroupNo;
	CString m_strMatchString;
	bool m_bMatchUsername;
	bool m_bMatchForename;
	bool m_bMatchSurname;
	bool m_bMatchMemberID;
	bool m_bMatchInfo1;
	bool m_bMatchInfo2;
	bool m_bMatchInfo3;
	bool m_bMatchInfo4;
	bool m_bMatchInfo5;
	bool m_bMatchInfo6;
	bool m_bMatchInfo7;
	bool m_bMatchInfo8;
	bool m_bWantValidAccounts;
	bool m_bWantInactiveAccounts;
	bool m_bWantExpiredAccounts;
	bool m_bWantAccountsWithAlert;
};

//**********************************************************************

class CSQLRowSetAccountFind
{
public:
	CSQLRowSetAccountFind();
	void Reset() { m_arrayRows.RemoveAll(); }

public:
	void InsertRowByUserID(__int64 nUserID );
	int BuildFromSearch(CSQLRowAccountFindInfo& infoSearch);
	
public:
	int GetSize() { return m_arrayRows.GetSize(); }
	void GetRow(int n, CSQLRowAccountFind& Row);	
	void SetRow(int n, CSQLRowAccountFind& Row);
	void AddRow(CSQLRowAccountFind& RowAccount) { m_arrayRows.Add(RowAccount); }

private:
	CString GetColumnListForSelect();
	bool CheckNonSQLFilter(CSQLRowAccountFindInfo& infoSearch, CSQLRowAccountFind& RowAccount);

private:
	CArray<CSQLRowAccountFind,CSQLRowAccountFind> m_arrayRows;
};

//**********************************************************************
