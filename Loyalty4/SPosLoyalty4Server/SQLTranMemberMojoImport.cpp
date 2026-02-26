/**********************************************************************/
#include "..\SPosLoyalty4Shared\AuditRecord_Base.h"
#include "..\SPosLoyalty4Shared\AuditReportTexts.h"
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/**********************************************************************/
#include "SQLTranMemberMojoImport.h"
/**********************************************************************/

CSQLTranMemberMojoImport::CSQLTranMemberMojoImport() : CSQLTranBase()
{
	m_nCurrentRecordAppNo = 0;
}

/**********************************************************************/

bool CSQLTranMemberMojoImport::SetAllAccountsInactive()
{
	CSQLRepositoryAccount repoAccount;
	return (repoAccount.SetAllAccountsInactive(m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE);
}

/**********************************************************************/

__int64 CSQLTranMemberMojoImport::GetUserIDForQRInfo(CString strQRInfo, __int64& nStartFromUserID)
{
	__int64 nUserID = 0;

	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		if (strQRInfo != "")
		{
			CSQLRepositoryAccount repoAccount;
			nUserID = repoAccount.LookupMember(Account::QRInfo.Label, strQRInfo, m_pDatabase).GetSQLResult();

			if (0 == nUserID)
			{
				nUserID = repoAccount.GetNextFreeUserID(nStartFromUserID, _atoi64(System.GetHighCardNo()), NULL).GetSQLResult();

				if (nUserID != 0)
				{
					nStartFromUserID = nUserID + 1;
				}
			}
		}
	}

	return nUserID;
}

/**********************************************************************/

bool CSQLTranMemberMojoImport::GetAccountRecord(__int64 nUserID, CSQLRowAccountFull& RowAccount, int& nAdded, int& nModified )
{
	bool bResult = FALSE;
	m_nCurrentRecordAppNo = 0;
	RowAccount.SetUserID(nUserID);

	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		CSQLRepositoryAccount repoAccount;
		if (repoAccount.RowExists(nUserID, m_pDatabase).GetSQLResult() != 0)
		{
			m_nCurrentRecordAppNo = APPNO_IMPORTUPDATE;
			nModified++;
			bResult = (repoAccount.SelectRow(RowAccount, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE);
		}
		else
		{
			m_nCurrentRecordAppNo = APPNO_IMPORTINSERT;
			nAdded++;
			bResult = (repoAccount.InsertRow(RowAccount, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE);
		}
	}

	return bResult;
}

/**********************************************************************/

bool CSQLTranMemberMojoImport::UpdateAccountRecord(CSQLRowAccountFull& RowAccount)
{
	bool bResult = FALSE;
	
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		CSQLRepositoryAccount repoAccount;
		bResult = ( repoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE );
	}

	if (TRUE == bResult)
	{
		CAuditRecord_base atc;
		atc.SetSQL(RowAccount);
		atc.SetSourceType(AUDIT_SERVER);
		atc.SetApplicationNo(m_nCurrentRecordAppNo);
		atc.SetPoints(RowAccount.GetPoints());
		atc.SetPointsTD(RowAccount.GetPointsToDate());
		atc.SetPurse1(RowAccount.GetPurse1Balance());
		atc.SetPurse2(RowAccount.GetPurse2Balance());
		atc.SetPurse1SpendTD(RowAccount.GetPurse1SpendToDate());
		atc.SetPurse2SpendTD(RowAccount.GetPurse2SpendToDate());
		atc.SetCashSpendTD(RowAccount.GetCashSpendToDate());
		atc.SaveLineToArray(m_arrayAuditBuffer);
	}

	return bResult;
}

/**********************************************************************/

