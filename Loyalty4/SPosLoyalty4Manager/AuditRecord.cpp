//**********************************************************************
#include "LoyaltyManager.h"
#include "TLogFile.h"
//**********************************************************************
#include "AuditRecord.h"
//**********************************************************************
// extract Tlog purchase descriptions
//
//**********************************************************************

CString CAuditRecord::ExtractPurchaseText(CTLogDatabase* pDatabase, bool bNewDatabase)
{
	CString strText = "Ecr sale";

	if (pDatabase != NULL)
	{
		if (TRUE == bNewDatabase)
		{
			// will import any new waiting updates
			CTLogFile tlogFile(GetCardNo());	
			pDatabase->Open(Filenames.GetPurchaseHistoryFilename(GetCardNo()));
		}

		if (pDatabase->Find(GetDateLastUsed(), GetTimeLastUsed(), GetCCNo()) == TRUE)
		{
			strText = pDatabase->GetSalesText();			// extract purchase text
		}
		//ccno in audit will have been incremented if account was created at time of purchase
		else if (pDatabase->Find(GetDateLastUsed(), GetTimeLastUsed(), GetCCNo() - 1) == TRUE)
		{
			strText = pDatabase->GetSalesText();			// extract purchase text
		}
	}

	return strText;
}

//**********************************************************************

bool CAuditRecord::AuditChanges(CSQLRowAccountFull& RowAccount, bool bForceAudit)
{
	bool bAuditReqd = bForceAudit;
	int nPoints = RowAccount.GetPoints() - m_nPoints;			// change in points
	double dPurse1 = RowAccount.GetPurse1Balance() - m_dPurse1;		// change in purse1
	double dPurse2 = RowAccount.GetPurse2Balance() - m_dPurse2;		// change in purse2

	if (bForceAudit == FALSE)
	{
		if (dPurse1 != 0.0 || dPurse2 != 0.0)
		{
			bAuditReqd = TRUE;
		}

		else if (nPoints != 0 || m_nPointsTD != RowAccount.GetPointsToDate())
		{
			bAuditReqd = TRUE;
		}

		else if (m_dPurse1SpendTD != RowAccount.GetPurse1SpendToDate() || m_dPurse2SpendTD != RowAccount.GetPurse2SpendToDate() || m_dCashSpendTD != RowAccount.GetCashSpendToDate())
		{
			bAuditReqd = TRUE;
		}

		else if (m_strCardName != RowAccount.GetFullname())
		{
			bAuditReqd = TRUE;
		}

		else if (m_nGroupNo != RowAccount.GetGroupNo())
		{
			bAuditReqd = TRUE;
		}
	}

	if (bAuditReqd == TRUE)
	{
		CSSDate date;
		CSSTime time;
		RowAccount.GetLastUsed(date, time);

		m_dateLastUsed.SetDate(date.GetDate());
		m_timeLastUsed.SetTime(time.GetTime());

		m_strCardName = RowAccount.GetFullname();
		m_nGroupNo = RowAccount.GetGroupNo();
		m_nPointsTransaction = nPoints;							// points added
		m_dPurse1Transaction = dPurse1;							// p1 deducted
		m_dPurse2Transaction = dPurse2;							// p2 deducted
		m_dCashTransaction = 0.0;								// no cash transacton
		m_nPoints = RowAccount.GetPoints();						// current points
		m_dPurse1 = RowAccount.GetPurse1Balance();				// current purse1
		m_dPurse2 = RowAccount.GetPurse2Balance();				// current purse2
		m_nPointsTD = RowAccount.GetPointsToDate();				// points todate
		m_dPurse1SpendTD = RowAccount.GetPurse1SpendToDate();	// p1 spend todate
		m_dPurse2SpendTD = RowAccount.GetPurse2SpendToDate();	// p2 spend todate
		m_dCashSpendTD = RowAccount.GetCashSpendToDate();		// cash spend todate

		return TRUE;
	}

	return FALSE;
}

//**********************************************************************
