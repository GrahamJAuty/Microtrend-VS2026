//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLBuilderBind.h"
#include "..\SPOSLoyalty4Shared\SQLDb.h"
#include "..\SPOSLoyalty4Shared\SQLDefines.h"
#include "..\SPOSLoyalty4Shared\SQLHelpers.h"
#include "..\SPOSLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRowAccountFind.h"
//**********************************************************************
#include "SQLRowSetAccountFind.h"
//**********************************************************************

CSQLRowSetAccountFind::CSQLRowSetAccountFind()
{
}

//**********************************************************************

CString CSQLRowSetAccountFind::GetColumnListForSelect()
{
	CCSV csvColumns;
	csvColumns.Add(Account::UserID.Label);
	csvColumns.Add(Account::Fullname.Label);
	csvColumns.Add(Account::Forename.Label);
	csvColumns.Add(Account::Surname.Label);
	csvColumns.Add(Account::ExpiryDate.Label);
	csvColumns.Add(Account::GroupNo.Label);
	csvColumns.Add(Account::Inactive.Label);
	csvColumns.Add(Account::Info1.Label);
	csvColumns.Add(Account::Info2.Label);
	csvColumns.Add(Account::Info3.Label);
	csvColumns.Add(Account::Info4.Label);
	csvColumns.Add(Account::Address1.Label);
	csvColumns.Add(Account::Address2.Label);
	csvColumns.Add(Account::Address3.Label);
	csvColumns.Add(Account::Address4.Label);
	csvColumns.Add(Account::Address5.Label);
	csvColumns.Add(Account::Phone1.Label);
	csvColumns.Add(Account::Phone2.Label);
	csvColumns.Add(Account::AlertCodes.Label);
	return csvColumns.GetLine();
}

//**********************************************************************

int CSQLRowSetAccountFind::BuildFromSearch(CSQLRowAccountFindInfo& infoSearch)
{
	CString strWhere = "";
	CSQLBuilderBind BuilderBind{};
	CSQLRepositoryAccount repoAccount;
	repoAccount.PrepareFindWhere(infoSearch, -1, strWhere, BuilderBind);

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NormalByParams{ &BuilderBind, strWhere });
	
	m_arrayRows.RemoveAll();

	CSQLRowAccountFind RowAccount;
	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (CheckNonSQLFilter(infoSearch, RowAccount) == TRUE)
		{
			m_arrayRows.Add(RowAccount);
		}
	}

	return SQLCRUD_ERR_NONE;
}

//**********************************************************************

bool CSQLRowSetAccountFind::CheckNonSQLFilter(CSQLRowAccountFindInfo& infoSearch, CSQLRowAccountFind& RowAccount)
{
	bool bIsInactive = RowAccount.GetInactive();
	bool bIsAlert = RowAccount.IsAlertSet();
	bool bIsExpired = RowAccount.HasUserExpired();
	bool bIsHotlisted = RowAccount.IsCardHotlisted();

	if ( infoSearch.m_bWantValidAccounts && (bIsInactive == FALSE && bIsAlert == FALSE && bIsExpired == FALSE && bIsHotlisted == FALSE ))
	{
		return TRUE;
	}

	if (infoSearch.m_bWantInactiveAccounts && bIsInactive)
	{
		return TRUE;
	}

	if (infoSearch.m_bWantAlertSetAccounts && bIsAlert)
	{
		return TRUE;
	}

	if (infoSearch.m_bWantHotlistedAccounts && bIsHotlisted)
	{
		return TRUE;
	}

	if (infoSearch.m_bWantExpiredAccounts && bIsExpired)
	{
		return TRUE;
	}

	return FALSE;
}

//**********************************************************************

void CSQLRowSetAccountFind::InsertRowByUserID(__int64 nUserID)
{
	CSQLRowAccountFind RowAccount;
	RowAccount.SetUserID(nUserID);

	CSQLRepositoryAccount repoAccount;
	if (repoAccount.SelectRowFind(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		m_arrayRows.Add(RowAccount);
	}
}

//**********************************************************************

void CSQLRowSetAccountFind::GetRow(int nRow, CSQLRowAccountFind& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		Row = m_arrayRows.GetAt(nRow);
	}
}

//**********************************************************************