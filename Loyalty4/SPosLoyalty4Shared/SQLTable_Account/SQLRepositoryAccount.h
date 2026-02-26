#pragma once
//**********************************************************************
#include "SQLRecordSetAccountUserID.h"
//**********************************************************************
#include "..\..\SPosLoyalty4Manager\DBExportHandler.h"
#include "..\..\SPosLoyalty4Manager\ReportInfoPurchaseHistory.h"
#include "..\..\SPosLoyalty4Manager\SQLRowSetAccountFind.h"
#include "..\..\SPosLoyalty4Manager\SQLRecordSetAccountList.h"
//**********************************************************************
#include "..\..\SPosLoyalty4Server\ServerWrapper.h"
//**********************************************************************
#include "..\SQLResultInfo.h"
#include "..\SQLWhereBuilder.h"
//**********************************************************************
#include "SQLRowAccountFind.h"
#include "SQLRowAccountFull.h"
#include "SQLRowAccountList.h"
#include "SQLRecordSetAccountFull.h"
//**********************************************************************

class CSQLRepositoryAccount
{
public:
	CSQLRepositoryAccount();

public:
	CSQLResultInfo SelectRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo SelectRowFind(CSQLRowAccountFind& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase);
	
	CSQLResultInfo RowExists(__int64 nUserID, CDatabase* pDatabase);
	CSQLResultInfo CountRows(CDatabase* pDatabase);
	CSQLResultInfo CountRowsInRange(__int64 nFrom, __int64 nTo, CDatabase* pDatabase);

	CSQLResultInfo GetUserIDList(CArray<__int64, __int64>& arrayUserIDs, CDatabase* pDatabase);
	CSQLResultInfo GetUserIDListInRange(CArray<__int64, __int64>& arrayUserIDs, __int64 nFrom, __int64 nTo, CDatabase* pDatabase);
	CSQLResultInfo GetNextFreeUserID(__int64 nStartID, __int64 nMaxID, CDatabase* pDatabase);

	CSQLResultInfo GetHighestCurrentUserID(CDatabase* pDatabase);

	CSQLResultInfo LookupMember(CString strLocateOn, CString strMemberID, CDatabase* pDatabase);
	CSQLResultInfo RemoveQRInfo(CString strQRInfo, CDatabase* pDatabase);
	CSQLResultInfo RemoveExtRef(CString strExtRef, CDatabase* pDatabase);

	CSQLResultInfo SetAllAccountsInactive(CDatabase* pDatabase);

#ifndef LOYSERVER
	void PrepareFindWhere(CSQLRowAccountFindInfo& infoSearch, __int64 nUserID, CString& strWhere, CSQLBuilderBind& BuilderBind);
	void PrepareExportWhere(CDbExportHandler& dbExportHandler, __int64 nUserID, CString& strWhere, CSQLBuilderBind& BuilderBind);
	void AppendExportWhereCondition(CDbExportHandler& dbExportHandler, CSQLWhereBuilder& WhereBuilder, CSQLBuilderBind& BuilderBind, int nCondition);
	void PrepareSearchWhere(CReportInfoPurchaseHistory& ReportInfo, CString& strWhere, CSQLBuilderBind& BuilderBind);
#else
	void PrepareServerSearchWhereAccountLookup(CSPOSSearchInfo& infoSearch, CString& strWhere, CSQLBuilderBind& BuilderBind);
	void PrepareServerSearchWhereSnakesAndLadders(CSPOSSearchInfo& infoSearch, CString& strWhere, CSQLBuilderBind& BuilderBind);
#endif

private:
	CSQLResultInfo RemoveMemberInfo(CSQLRecordSetAccountUserID& RecordSet);
};

//**********************************************************************

