#pragma once
//**********************************************************************
#include "..\..\SmartPay4Manager\SQLRowSetAccountFind.h"
//**********************************************************************
#include "..\..\SmartPay4ManagerBgnd\DbExportHandler.h"
//**********************************************************************
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLPrepStatAccount.h"
#include "SQLPrepStatLeaver.h"
//**********************************************************************

struct CPurchaseHistorySearchInfo
{
public:
	CPurchaseHistorySearchInfo()
	{
		m_nUserIDFrom = 1;
		m_nUserIDTo = 999999;
		m_nUserIDExact = -1;
		m_bMatchGroup = FALSE;
		m_nGroupNo = 0;
		m_strFilterText = "";
		m_nInfoTextIndex = 0;
	};

public:
	__int64 m_nUserIDFrom;
	__int64 m_nUserIDTo;
	__int64 m_nUserIDExact;
	bool m_bMatchGroup;
	int m_nGroupNo;
	CString m_strFilterText;
	int m_nInfoTextIndex;
};

//**********************************************************************

class CSQLAccountManager
{
public:
	CSQLAccountManager();
	~CSQLAccountManager();

public:
	CSQLResultInfo SelectRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase );
	CSQLResultInfo SelectRowFind(CSQLRowAccountFind& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo InsertLeaversDirect(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo RowExists(__int64 nUserID, CDatabase* pDatabase);
	CSQLResultInfo CountRows(CDatabase* pDatabase);

public:
	CSQLResultInfo CountRowsInRange( __int64 nUserIDFrom, __int64 nUserIDTo, CDatabase* pDatabase);
	CSQLResultInfo GetUserIDListInRange(CArray<__int64, __int64>& arrayUserIDs, __int64 nUserIDFrom, __int64 nUserIDTo, CDatabase* pDatabase);

public:
	CSQLResultInfo GetUserIDList(CArray<__int64, __int64>& arrayUserIDs, CDatabase* pDatabase);
	CSQLResultInfo GetHighestCurrentUserID(CDatabase* pDatabase);
	CSQLResultInfo GetNextFreeUserID(__int64 nStartID, __int64 nMaxID, CDatabase* pDatabase);
	CSQLResultInfo LookupIndex(CString strLocateOn, CString strMemberID, CDatabase* pDatabase);
	CSQLResultInfo LookupMemberID(CString strMemberID, CDatabase* pDatabase);
	CSQLResultInfo LookupExtRef1(CString strMemberID, CDatabase* pDatabase);
	CSQLResultInfo RemoveExtRef1(CString str, CDatabase* pDatabase);
	CSQLResultInfo RemoveExtRef2(CString str, CDatabase* pDatabase);
	CSQLResultInfo RemoveMemberID(CString str, CDatabase* pDatabase);
	
public:
	void SetLeaverYear(int n) { m_nLeaverYear = n; }
	int GetLeaverYear() { return m_nLeaverYear; }
	bool IsLeaverMode();

public:
	void ClearUserIDCache();
	void BuildUserIDCache();
	bool CheckUserIDCache(__int64 nUserID);

public:

#ifndef SYSTEMTYPE_SERVER
	void PrepareExportWhere(CDbExportHandler& dbExportHandler, __int64 nUserID, CString& strWhere, CSQLBuilderBind& BuilderBind);
	void AppendExportWhereCondition(CDbExportHandler& dbExportHandler, CSQLWhereBuilder& WhereBuilder, CSQLBuilderBind& BuilderBind, int nCondition);
#endif

	void PreparePurchaseWhere(CPurchaseHistorySearchInfo& infoSearch, CString& strWhere, CSQLBuilderBind& BuilderBind);

#ifndef SYSTEMTYPE_SERVER
#ifndef SYSTEMTYPE_BACKGROUND
	void PrepareFindWhere(CSQLRowAccountFindInfo& infoSearch, __int64 nUserID, CString& strWhere, CSQLBuilderBind& BuilderBind);
#endif
#endif

public:
	CString GetCurrentTableName();

private:
	int GetFilterYesNoValue(int nFilter);
	CSQLResultInfo NotImplemented();

private:
	CSQLPrepStatAccount m_PrepStatMain;
	CSQLPrepStatLeaver m_PrepStatLeaver;

private:
	int m_nLeaverYear;
	CString m_strLastValidLocateOn;
};

//**********************************************************************
extern CSQLAccountManager SQLAccountManager;
//**********************************************************************

