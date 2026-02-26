#pragma once
//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLFieldValidatorAccount.h"
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRowAccountFind.h"
//**********************************************************************

struct CSQLRowAccountFindInfo
{
public:
	int m_nGroupSel;
	int m_nGroupNo;
	CString m_strMatchString;
	bool m_bMatchFullname;
	bool m_bMatchForename;
	bool m_bMatchSurname;
	bool m_bMatchInfo;
	bool m_bMatchAddress;
	bool m_bMatchPostcode;
	bool m_bMatchTelephone;
	bool m_bWantValidAccounts;
	bool m_bWantInactiveAccounts;
	bool m_bWantExpiredAccounts;
	bool m_bWantAlertSetAccounts;
	bool m_bWantHotlistedAccounts;
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
	void AddRow(CSQLRowAccountFind& RowAccount) { m_arrayRows.Add(RowAccount); }

private:
	CString GetColumnListForSelect();
	bool CheckNonSQLFilter(CSQLRowAccountFindInfo& infoSearch, CSQLRowAccountFind& RowAccount);

private:
	CArray<CSQLRowAccountFind,CSQLRowAccountFind> m_arrayRows;
};

//**********************************************************************
