//$$******************************************************************
#include "WorkingDlg.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\AuditRecord_base.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//$$******************************************************************
#include "AuditFix.h"
//$$******************************************************************

void CAuditFix::DoAuditFix()
{
	/*
	CSSFile fileAuditFix;
	if (fileAuditFix.Open(".\\auditfix.dat", "rb") == FALSE)
	{
		Prompter.Error("Unable to open audit fix file");
		return;
	}

	CString strFixBuffer = "";
	while (fileAuditFix.ReadString(strFixBuffer) == TRUE)
	{
		CCSV csv(strFixBuffer);
		__int64 nAccountNo = csv.GetInt64(0);
		CString strAccountName = csv.GetString(1);
		CString strDate = csv.GetString(2);
		int nThreshold = csv.GetInt(3);
		double dReward = csv.GetDouble(4);

		CSSDate dateFixStart(strDate);

		double dCurrentPurse1 = 0.0;
		double dCurrentPurse2 = 0.0;
		int nCurrentPoints = 0;
		double dPurse1ToDate = 0.0;
		double dPurse2ToDate = 0.0;
		double dCashToDate = 0.0;
		int nPointsToDate = 0;
		bool bFixStarted = FALSE;

		CString strAuditFilename = Filenames.GetAuditFilename();
		CString strAuditFilenameTmp = strAuditFilename + ".tmp";

		if (CopyFile(strAuditFilename, strAuditFilenameTmp, FALSE) == FALSE)
		{
			continue;
		}

		CSSFile fileAuditIn;
		if (fileAuditIn.Open(strAuditFilenameTmp, "rb") == FALSE)
		{
			continue;
		}

		CSSFile fileAuditOut;
		if (fileAuditOut.Open(strAuditFilename, "wb") == FALSE)
		{
			continue;
		}

		CString strCaption = "";
		strCaption.Format("Fixing audit for %I64d, %s", 
			nAccountNo, 
			(const char*)strAccountName);	

		CWorkingDlg WorkingDlg(strCaption, FALSE);

		int nLinesProcessed = 0;

		CAuditRecord_base auditRecord;
		while (auditRecord.ReadLine(&fileAuditIn) == TRUE)
		{
			nLinesProcessed++;
			if ((nLinesProcessed % 1000) == 0)
			{
				WorkingDlg.SetCaption2RecordsChecked(nLinesProcessed, FALSE);
			}

			__int64 nThisAccountNo = _atoi64(auditRecord.GetCardNo());
			if (nThisAccountNo != nAccountNo)
			{
				auditRecord.WriteLineToFile(&fileAuditOut);
				continue;
			}

			CSSDate dateAudit(auditRecord.GetDateFiled());

			if ((dateAudit < dateFixStart) && (bFixStarted == FALSE))
			{
				auditRecord.WriteLineToFile(&fileAuditOut);
				dCurrentPurse1 = auditRecord.GetPurse1();
				dCurrentPurse2 = auditRecord.GetPurse2();
				nCurrentPoints = auditRecord.GetPoints();
				dPurse1ToDate = auditRecord.GetPurse1SpendTD();
				dPurse2ToDate = auditRecord.GetPurse2SpendTD();
				dCashToDate = auditRecord.GetCashSpendTD();
				nPointsToDate = auditRecord.GetPointsTD();
				continue;
			}

			if (auditRecord.GetApplicationNo() == APPNO_REDEEM)
			{
				continue;
			}

			bFixStarted = TRUE;
			dCurrentPurse1 += auditRecord.GetPurse1Transaction();
			dCurrentPurse2 += auditRecord.GetPurse2Transaction();
			nCurrentPoints += auditRecord.GetPointsTransaction();

			switch (auditRecord.GetApplicationNo())
			{
			case APPNO_SALE:
				dPurse1ToDate -= auditRecord.GetPurse1Transaction();
				dPurse2ToDate -= auditRecord.GetPurse2Transaction();
				dCashToDate -= auditRecord.GetCashTransaction();
				break;

			case APPNO_ADDPOINTS:
			case APPNO_SUBPOINTS:
			case APPNO_DEPTBONUSPOINTS:
			case APPNO_REFUNDDEPTBONUSPOINTS:
			case APPNO_REWARDBONUSPOINTS:
			case APPNO_REFUNDREWARDBONUSPOINTS:
				nPointsToDate += auditRecord.GetPointsTransaction();
				break;

			default:
				break;
			}

			auditRecord.SetPurse1(dCurrentPurse1);
			auditRecord.SetPurse2(dCurrentPurse2);
			auditRecord.SetPoints(nCurrentPoints);
			auditRecord.SetPurse1SpendTD(dPurse1ToDate);
			auditRecord.SetPurse2SpendTD(dPurse2ToDate);
			auditRecord.SetCashSpendTD(dCashToDate);
			auditRecord.SetPointsTD(nPointsToDate);
			auditRecord.WriteLineToFile(&fileAuditOut);

			if ((nCurrentPoints >= nThreshold) && (dReward > 0.0))
			{
				nCurrentPoints -= nThreshold;
				dCurrentPurse1 += dReward;
				auditRecord.SetSourceType(AUDIT_PC);
				auditRecord.SetApplicationNo(APPNO_REDEEM);
				auditRecord.SetPoints(nCurrentPoints);
				auditRecord.SetPurse1(dCurrentPurse1);
				auditRecord.SetPointsTransaction(-nThreshold);
				auditRecord.SetPurse1Transaction(dReward);
				auditRecord.SetPurse2Transaction(0.0);
				auditRecord.SetCashTransaction(0.0);
				auditRecord.WriteLineToFile(&fileAuditOut);
			}
		}

		WorkingDlg.DestroyWindow();
		fileAuditIn.Close();
		fileAuditOut.Close();

		CSQLRepositoryAccount SQLRepositoryAccount;
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(nAccountNo);

		if (SQLRepositoryAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			RowAccount.SetPurse1Balance(dCurrentPurse1);
			RowAccount.SetPurse2Balance(dCurrentPurse2);
			RowAccount.SetPoints(nCurrentPoints);
			RowAccount.SetPurse1SpendToDate(dPurse1ToDate);
			RowAccount.SetPurse2SpendToDate(dPurse2ToDate);
			RowAccount.SetCashSpendToDate(dCashToDate);
			RowAccount.SetPointsToDate(nPointsToDate);
			SQLRepositoryAccount.UpdateRow(RowAccount, NULL);
		}
	}
	*/
}

//$$******************************************************************