//$$******************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SQLAuditRecord_Base.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//$$******************************************************************
#include "AuditFileFilter.h"
//$$******************************************************************

void CAuditFileFilter::FilterAuditFile()
{
	CReportConsolidationArray<CSortedInt64Item> arrayUserIDs;
	CArray<CRunningAuditTotals, CRunningAuditTotals&> arrayAccountTotals;

	CString strAuditFilename = Filenames.GetAuditFilename();
	CString strAuditFilenameKeep = strAuditFilename + ".keep";
	CString strAuditFilenameLose = strAuditFilename + ".lose";

	CSSFile fileAuditIn;
	if (fileAuditIn.Open(strAuditFilename, "rb") == FALSE)
	{
		Prompter.Error("Unable to open Audit file for filtering.");
		return;
	}

	CSSFile fileAuditKeep;
	if (fileAuditKeep.Open(strAuditFilenameKeep, "wb") == FALSE)
	{
		fileAuditIn.Close();
		Prompter.Error("Unable to create file of Audits to keep.");
		return;
	}

	CSSFile fileAuditLose;
	if (fileAuditLose.Open(strAuditFilenameLose, "wb") == FALSE)
	{
		fileAuditIn.Close();
		fileAuditKeep.Close();
		Prompter.Error("Unable to create file of Audits to discard.");
		return;
	}
		
	__int64 nTotalKept = 0;
	__int64 nTotalLost = 0;

	CWorkingDlg dlgWorking("Filtering Audit Lines", FALSE);
	dlgWorking.Create();

	CSQLAuditRecord_base auditRecord;
	while (auditRecord.ReadLine(&fileAuditIn) == TRUE)
	{
		CRunningAuditTotals totals;

		int nPos = 0;
		CSortedInt64Item item;
		item.m_nItem = _atoi64(auditRecord.GetCardNo());
		if (arrayUserIDs.Find(item, nPos) == FALSE)
		{
			arrayUserIDs.InsertAt(nPos, item);
			arrayAccountTotals.InsertAt(nPos, totals);
		}
		else
		{
			totals = arrayAccountTotals.GetAt(nPos);
		}

		bool bChangedTotals = FALSE;

		double dPurse1Credit = auditRecord.GetCurrentPurse1Credit();
		if (::CompareDoubles(totals.m_dPurse1Credit, dPurse1Credit, 3) != 0)
		{
			totals.m_dPurse1Credit = dPurse1Credit;
			bChangedTotals = TRUE;
		}

		double dPurse1Liability = auditRecord.GetCurrentPurse1Liability();
		if (::CompareDoubles(totals.m_dPurse1Liability, dPurse1Liability, 3) != 0)
		{
			totals.m_dPurse1Liability = dPurse1Liability;
			bChangedTotals = TRUE;
		}

		double dPurse2Balance = auditRecord.GetCurrentPurse2Balance();
		if (::CompareDoubles(totals.m_dPurse2Balance, dPurse2Balance, 3) != 0)
		{
			totals.m_dPurse2Balance = dPurse2Balance;
			bChangedTotals = TRUE;
		}

		double dPurse3Credit = auditRecord.GetCurrentPurse3Credit();
		if (::CompareDoubles(totals.m_dPurse3Credit, dPurse3Credit, 3) != 0)
		{
			totals.m_dPurse3Credit = dPurse3Credit;
			bChangedTotals = TRUE;
		}

		double dPurse3Liability = auditRecord.GetCurrentPurse3Liability();
		if (::CompareDoubles(totals.m_dPurse3Liability, dPurse3Liability, 3) != 0)
		{
			totals.m_dPurse3Liability = dPurse3Liability;
			bChangedTotals = TRUE;
		}

		int nPoints = auditRecord.GetCurrentPoints();
		if (totals.m_nPoints != nPoints)
		{
			totals.m_nPoints = nPoints;
			bChangedTotals = TRUE;
		}

		int nGroupNo = auditRecord.GetGroupNo();
		if (totals.m_nGroupNo != nGroupNo)
		{
			totals.m_nGroupNo = nGroupNo;
			bChangedTotals = TRUE;
		}

		double dPurse1ToDate = auditRecord.GetToDatePurse1Spend();
		if (::CompareDoubles(totals.m_dPurse1ToDate, dPurse1ToDate, 3) != 0)
		{
			totals.m_dPurse1ToDate = dPurse1ToDate;
			bChangedTotals = TRUE;
		}

		double dPurse2ToDate = auditRecord.GetToDatePurse2Spend();
		if (::CompareDoubles(totals.m_dPurse2ToDate, dPurse2ToDate, 3) != 0)
		{
			totals.m_dPurse2ToDate = dPurse2ToDate;
			bChangedTotals = TRUE;
		}

		double dPurse3ToDate = auditRecord.GetToDatePurse3Spend();
		if (::CompareDoubles(totals.m_dPurse3ToDate, dPurse3ToDate, 3) != 0)
		{
			totals.m_dPurse3ToDate = dPurse3ToDate;
			bChangedTotals = TRUE;
		}

		double dCashSpendToDate = auditRecord.GetToDateCashSpend();
		if (::CompareDoubles(totals.m_dCashSpendToDate, dCashSpendToDate, 3) != 0)
		{
			totals.m_dCashSpendToDate = dCashSpendToDate;
			bChangedTotals = TRUE;
		}

		CString strUsername = auditRecord.GetUserName();
		if (totals.m_strUsername != strUsername)
		{
			totals.m_strUsername = strUsername;
			bChangedTotals = TRUE;
		}

		if (TRUE == bChangedTotals)
		{
			arrayAccountTotals.SetAt(nPos, totals);
			auditRecord.WriteLineToFile(&fileAuditKeep);
			nTotalKept++;
		}
		else
		{
			switch (auditRecord.GetApplicationNo())
			{
			case APPNO_IMPORTUPDATE:
			case APPNO_IMPORTCUSTOM:
				auditRecord.WriteLineToFile(&fileAuditLose);
				nTotalLost++;
				break;

			default:
				auditRecord.WriteLineToFile(&fileAuditKeep);
				nTotalKept++;
				break;
			}
		}

		if (((nTotalKept + nTotalLost) % 1000) == 0)
		{
			CString strMsg = "";
			strMsg.Format("Checked %I64d, Keep %I64d, Lose %I64d",
				nTotalKept + nTotalLost, nTotalKept, nTotalLost);

			dlgWorking.SetCaption2(strMsg);
		}
	}

	dlgWorking.DestroyWindow();
	fileAuditIn.Close();
	fileAuditKeep.Close();
	fileAuditLose.Close();

	CString strMsg = "";
	strMsg.Format("Audit File Filtering complete:\n\nChecked %I64d lines\n\nKept %I64d lines\n\nDiscarded %I64d lines.",
		nTotalKept + nTotalLost, nTotalKept, nTotalLost);

	AfxMessageBox(strMsg);
}

//$$******************************************************************
