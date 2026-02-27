//**********************************************************************
#include "..\SmartPay4Shared\GlobalState.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLBuilderBind.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFind.h"
#include "..\SmartPay4Shared\SQLDb.h"
//**********************************************************************
#include <memory>
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
	csvColumns.Add(Account::Username.Label);
	csvColumns.Add(Account::Forename.Label);
	csvColumns.Add(Account::Surname.Label);
	csvColumns.Add(Account::ExpiryDate.Label);
	csvColumns.Add(Account::GroupNo.Label);
	csvColumns.Add(Account::Inactive.Label);
	csvColumns.Add(Account::Info1.Label);
	csvColumns.Add(Account::Info2.Label);
	csvColumns.Add(Account::Info3.Label);
	csvColumns.Add(Account::Info4.Label);
	csvColumns.Add(Account::Info5.Label);
	csvColumns.Add(Account::Info6.Label);
	csvColumns.Add(Account::Info7.Label);
	csvColumns.Add(Account::Info8.Label);
	csvColumns.Add(Account::MemberID.Label);
	csvColumns.Add(Account::AlertCode.Label);
	return csvColumns.GetLine();
}

//**********************************************************************

int CSQLRowSetAccountFind::BuildFromSearch(CSQLRowAccountFindInfo& infoSearch)
{
	CString strWhere = "";
    CSQLBuilderBind BuilderBind{};
	CSQLRepositoryAccount RepoAccount;
    RepoAccount.PrepareFindWhere(infoSearch, -1, strWhere, BuilderBind);

	CSQLRecordSetAccountFull RecordSet(nullptr, 
		RSParams_AccountFull_ByParams{ &BuilderBind, strWhere });

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

	if ( infoSearch.m_bWantValidAccounts && (bIsInactive == FALSE && bIsAlert == FALSE && bIsExpired == FALSE))
	{
		return TRUE;
	}

	if (infoSearch.m_bWantInactiveAccounts && bIsInactive)
	{
		return TRUE;
	}

	if (infoSearch.m_bWantAccountsWithAlert && bIsAlert)
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
	RowAccount.SetLeaverYear(g_GlobalState.GetLeaverYear());

	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRowFind(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		m_arrayRows.Add(RowAccount);
	}
}

//**********************************************************************

void CSQLRowSetAccountFind::GetRow(int n, CSQLRowAccountFind& Row)
{
	if ((n >= 0) && (n < GetSize()))
	{
		Row = m_arrayRows.GetAt(n);
	}
}

//**********************************************************************

void CSQLRowSetAccountFind::SetRow(int n, CSQLRowAccountFind& Row)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.SetAt(n,Row);
	}
}

//**********************************************************************