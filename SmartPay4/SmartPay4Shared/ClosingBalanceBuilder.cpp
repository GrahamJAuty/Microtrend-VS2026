/**********************************************************************/
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_ClosingBalance\SQLRepositoryClosingBalance.h"
/**********************************************************************/
#include "ClosingBalanceBuilder.h"
/**********************************************************************/

CClosingBalanceBuilder::CClosingBalanceBuilder()
{
}

/**********************************************************************/
/**********************************************************************/
// calculate Closing balance @ Midnight from given date from Audits & database etc

bool CClosingBalanceBuilder::CalculateBalanceForDate(COleDateTime& timeBalance, CSmartPayAuditFile* pAudit)
{
#ifdef SYSTEMTYPE_MANAGER
	CWaitCursor cursor;
#endif

	m_oleTimeBalance = timeBalance;
	{
		CTime ctimeBalance = CTime(m_oleTimeBalance.GetYear(), m_oleTimeBalance.GetMonth(), m_oleTimeBalance.GetDay(), 0, 0, 0, -1);
		m_dateBalance.SetDate(ctimeBalance);
	}
	// scan audit file ***********************************************************

	CRecordSorter sorter;

	CString strDateFrom;
	strDateFrom.Format("%2.2d%2.2d%4.4d", 
		m_oleTimeBalance.GetDay(),
		m_oleTimeBalance.GetMonth(),
		m_oleTimeBalance.GetYear() - 1);	// go back 1 year

	CSQLRowClosingBalance RowBalance;
	
	pAudit->JumpIn(strDateFrom);										// starting position StartDate - 1Year back

	while (TRUE)
	{
		int nStartFilePos = pAudit->GetFilePos();						// position in file to start from
		if (pAudit->ReadLine(&m_atc) == FALSE)
		{
			break;
		}

		CString strUserID = m_atc.GetUserID();
		if (strUserID != "")
		{
			switch (CheckAuditLineDate(&m_atc))
			{
			case -1:
				//AUDIT LINE BEFORE REQUIRED DATE
				//OVERWRITE ANY PREVIOUS ENTRY
				sorter.Update(System.FormatCardNo(strUserID), nStartFilePos, TRUE);	
				break;							
				
			case 0:
				//AUDIT LINE ON REQUIRED DATE
				//OVERWRITE ONLY IF NO PREVIOUS ENTRY
				sorter.Update(System.FormatCardNo(strUserID), nStartFilePos, FALSE);	
				break;												

			case 1:
				//AUDIT LINE AFTER REQUIRED DATE
				//IGNORE
				break;												
			}
		}
	}

	// scan database ****************************************************************

	{
		CArray<__int64, __int64> arrayUserIDs;
		CSQLRepositoryAccount RepoAccount;
		RepoAccount.GetUserIDList(arrayUserIDs, NULL);

		for (int n = 0; n < arrayUserIDs.GetSize(); n++)
		{
			__int64 nUserID = arrayUserIDs.GetAt(n);
			sorter.Update(System.FormatCardNo(nUserID, TRUE), -1, FALSE);
		}
	}

	if (sorter.GetSize() == 0)
	{
		return FALSE;
	}

	// see which data to use ( database record or audit line ) ************************

	for (int nIndex = 0; nIndex < sorter.GetSize(); nIndex++)
	{
		int nFilePos = sorter.GetRecordNo(nIndex);

		if (nFilePos < 0)											// database record entry not found in audit
		{
			MergeAccountBalanceFromDatabase(sorter.GetLabel(nIndex), RowBalance);			// get record number
		}
		else
		{
			pAudit->SetFilePos(nFilePos);
			if (pAudit->ReadLine(&m_atc) == TRUE)				// should always read
			{
				MergeAccountBalanceFromAudit((CheckAuditLineDate(&m_atc) == 0), RowBalance);
			}
		}
	}

	SaveCalculatedBalanceForDate(RowBalance);

	return TRUE;
}

//*****************************************************************************
// to get to here the record is in database but no lines have been found in the audit file

void CClosingBalanceBuilder::MergeAccountBalanceFromDatabase(const char* szUserID, CSQLRowClosingBalance& RowBalance)
{
	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserIDString(szUserID);

	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		// Purse1

		CSSDate datePurse1(RowAccount.GetPurse1LastSpendDate());

		if (datePurse1 < m_dateBalance)
		{
			RowBalance.AddPurse1LiabilityAudit(RowAccount.GetPurse1Liability());
			RowBalance.AddPurse1CreditAudit(RowAccount.GetPurse1Credit());
		}

		// Purse2

		CSSDate datePurse2(RowAccount.GetPurse2LastSpendDate());

		if (datePurse2 < m_dateBalance)							// is lastused date before 'from' date - yes can used it
		{
			RowBalance.AddPurse2BalanceAudit(RowAccount.GetPurse2Balance());
		}

		//Purse3

		CSSDate datePurse3(RowAccount.GetPurse3LastSpendDate());

		// is lastused date before 'from' date - yes can used it
		if (datePurse3 < m_dateBalance)
		{
			RowBalance.AddPurse3LiabilityAudit(RowAccount.GetPurse3Liability());
			RowBalance.AddPurse3CreditAudit(RowAccount.GetPurse3Credit());
		}
	}
}

//*****************************************************************************

void CClosingBalanceBuilder::MergeAccountBalanceFromAudit(bool bCalcBalance, CSQLRowClosingBalance& RowBalance)
{
	if (m_atc.IsDeleteAppNo() == TRUE)					// Apno = delete \ batch delete \ import delete
	{
		CSSDate date(m_atc.GetDateFiled());				// date record deleted

		if (date < m_dateBalance)
		{
			return;											// card deleted before start date
		}

		if (date > m_dateBalance)
		{
			return;
		}
	}

	RowBalance.AddPurse1LiabilityAudit(m_atc.GetCurrentPurse1Liability());		// Purse1 total
	RowBalance.AddPurse1CreditAudit(m_atc.GetCurrentPurse1Credit());				// Purse1 total
	RowBalance.AddPurse2BalanceAudit(m_atc.GetCurrentPurse2Balance());			// Purse2 total
	RowBalance.AddPurse3LiabilityAudit(m_atc.GetCurrentPurse3Liability());		// Purse3 total
	RowBalance.AddPurse3CreditAudit(m_atc.GetCurrentPurse3Credit());				// Purse3 total

	if (TRUE == bCalcBalance)
	{
		RowBalance.AddPurse1LiabilityAudit(-m_atc.GetTransactionPurse1Liability());
		RowBalance.AddPurse1CreditAudit(-m_atc.GetTransactionPurse1Credit());
		RowBalance.AddPurse2BalanceAudit(-m_atc.GetTransactionPurse2Balance());
		RowBalance.AddPurse3LiabilityAudit(-m_atc.GetTransactionPurse3Liability());
		RowBalance.AddPurse3CreditAudit(-m_atc.GetTransactionPurse3Credit());
	}
}

//*****************************************************************************

bool CClosingBalanceBuilder::SaveCalculatedBalanceForDate(CSQLRowClosingBalance& RowBalance)
{
	RowBalance.SetKeyDate(CSQLRowClosingBalance::MakeKeyDate(m_oleTimeBalance));
	RowBalance.SetDate(CSQLRowClosingBalance::MakeDbDate(m_oleTimeBalance));
	RowBalance.SetTime("00:00");
	
	bool bResult = FALSE;

	CSQLRepositoryClosingBalance RepoClosing;
	if (RepoClosing.UpsertRow(RowBalance,NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		m_ClosingBalanceCurrent = RowBalance;
		bResult = TRUE;
	}

	return bResult;
}

//********************************************************************************

int CClosingBalanceBuilder::CheckAuditLineDate(CSQLAuditRecord* atcRecord)
{
	CSSDate dateAudit(atcRecord->GetDateFiled());

	if (dateAudit < m_dateBalance)					// before day before start date
	{
		return -1;
	}

	if (dateAudit > m_dateBalance)					// past required date
	{
		return 1;
	}

	return 0;
}

//************************************************************************


