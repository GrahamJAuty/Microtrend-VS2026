//$$******************************************************************
#include "..\SPosLoyalty4Shared\GlobalState.h"
#include "..\SPosLoyalty4Shared\SQLResultInfo.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_EcrmanDatabase\SQLRepositoryEcrmanDatabase.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//$$******************************************************************
#include "AuditRecord.h"
#include "AuditReportDlg.h"
#include "LoyaltyManager.h"
#include "RecSort.h"
//$$******************************************************************
#include "AuditReport.h"
//$$******************************************************************
//R101 - Audit
//R102 - Audit Bonus Revaluation
//R103 - Audit Bonus Spend
//R109 - Audit Cash Taken
//R110 - Redeem Bonus
//R112 - Points Audit
//R113 - Points Added
//R114 - Audit Points Deducted
//R115 - Audit Sales
//R119 - Purse Additions
//R120 - Refund
//R123 - Single Audit
//R132 - Transaction Summary
//R133 - Transaction Summary Single
//$$******************************************************************
#define nTOTAL_CARD		0
#define nTOTAL_REPORT	1
//$$******************************************************************

CAuditReportLineInfo::CAuditReportLineInfo(CAuditRecord* pAtc)
{
	for (int p = 0; p <= 2; p++)
	{
		for (int n = 0; n <= REVALUE_PLUNO_PERPURSE_COUNT; n++)
		{
			m_bHaveColumnEPOSTopup[p][n] = FALSE;
			m_dEPOSTopup[p][n] = 0.0;
		}
	}

	m_bHaveColumnManualTopupPurse1 = FALSE;
	m_bHaveColumnManualTopupPurse2 = FALSE;
	m_bHaveColumnImportTopupPurse1 = FALSE;
	m_bHaveColumnImportTopupPurse2 = FALSE;
	m_bHaveColumnGift = FALSE;
	m_bHaveColumnPurse1 = FALSE;
	m_bHaveColumnPurse2 = FALSE;
	m_bHaveColumnPoints = FALSE;
	m_bHaveColumnNonPurse = FALSE;
	/*****/
	m_dPurse1 = pAtc->GetPurse1Transaction();
	m_dPurse2 = pAtc->GetPurse2Transaction();
	m_nPoints = pAtc->GetPointsTransaction();
	m_dNonPurse = pAtc->GetCashTransaction();
	/*****/
	m_dGift = 0.0;
	m_dManualTopupPurse1 = 0.0;
	m_dManualTopupPurse2 = 0.0;
	m_dImportTopupPurse1 = 0.0;
	m_dImportTopupPurse2 = 0.0;
}

//$$******************************************************************

bool CAuditReportLineInfo::GotColumnEPOSTopup(int p, int t)
{
	bool bResult = FALSE;

	if ( ( 1 == p) || ( 2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			bResult = m_bHaveColumnEPOSTopup[p][t];
		}
	}

	return bResult;
}

//$$******************************************************************

void CAuditReportLineInfo::EnableEPOSTopup(int p, int t, bool b)
{
	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			m_bHaveColumnEPOSTopup[p][t] = b;
		}
	}
}

//$$******************************************************************
//$$******************************************************************

CAuditReportColumnInfo::CAuditReportColumnInfo()
{
	m_nColumnType = AUDIT_COLUMN_PURSE1_TOPUP1;
	m_strColumnName = "Cash";
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

CAuditReport::CAuditReport( CAuditReportConfig& ReportConfig)
	: m_ReportConfig(ReportConfig)
{
	m_bShowTotalAdded = TRUE;
	m_bShowTotalDeducted = TRUE;

	ResetTotals(nTOTAL_CARD);
	ResetTotals(nTOTAL_REPORT);
}

//$$******************************************************************

void CAuditReport::ResetTotals(int nIndex)
{
	for (int p = 0; p <= 2; p++)
	{
		for (int n = 0; n <= REVALUE_PLUNO_PERPURSE_COUNT; n++)
		{
			m_TotalEPOSTopup[p][n].Reset(nIndex);	
		}
	}

	m_TotalGift.Reset(nIndex);
	m_TotalPurse1.Reset(nIndex);
	m_TotalPurse1ManualTopup.Reset(nIndex);
	m_TotalPurse1ImportTopup.Reset(nIndex);
	m_TotalPurse2.Reset(nIndex);
	m_TotalPurse2ManualTopup.Reset(nIndex);
	m_TotalPurse2ImportTopup.Reset(nIndex);
	m_TotalNonPurse.Reset(nIndex);
	m_TotalPoints.Reset(nIndex);
}

//$$******************************************************************

void CAuditReport::ClearTLogArrays()
{
	m_arrayTLogMap.RemoveAll();

	for (int n = 0; n < m_arrayTLogDatabases.GetSize(); n++)
	{
		delete (m_arrayTLogDatabases.GetAt(n));
	}

	m_arrayTLogDatabases.RemoveAll();
}

//$$******************************************************************

void CAuditReport::GetEPOSTopupColumnIndex(int nColumnType, int& nPurseNo, int& nTopupNo)
{
	nPurseNo = 0;
	nTopupNo = 0;

	switch (nColumnType)
	{
	case AUDIT_COLUMN_PURSE1_TOPUP1:
		nPurseNo = 1;
		nTopupNo = 1;
		break;

	case AUDIT_COLUMN_PURSE1_TOPUP2:
		nPurseNo = 1;
		nTopupNo = 2;
		break;

	case AUDIT_COLUMN_PURSE1_TOPUP3:
		nPurseNo = 1;
		nTopupNo = 3;
		break;

	case AUDIT_COLUMN_PURSE1_TOPUP4:
		nPurseNo = 1;
		nTopupNo = 4;
		break;

	case AUDIT_COLUMN_PURSE1_TOPUP5:
		nPurseNo = 1;
		nTopupNo = 5;
		break;

	case AUDIT_COLUMN_PURSE2_TOPUP1:
		nPurseNo = 2;
		nTopupNo = 1;
		break;

	case AUDIT_COLUMN_PURSE2_TOPUP2:
		nPurseNo = 2;
		nTopupNo = 2;
		break;

	case AUDIT_COLUMN_PURSE2_TOPUP3:
		nPurseNo = 2;
		nTopupNo = 3;
		break;

	case AUDIT_COLUMN_PURSE2_TOPUP4:
		nPurseNo = 2;
		nTopupNo = 4;
		break;

	case AUDIT_COLUMN_PURSE2_TOPUP5:
		nPurseNo = 2;
		nTopupNo = 5;
		break;
	}
}

//$$******************************************************************

int CAuditReport::CreateReport()
{
	if (m_Report.Open(m_ReportConfig.GetReportFilename()) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CLoyaltyAuditFile auditFile(m_ReportConfig.GetAuditFilename());
	if (auditFile.OpenReadOnly() == FALSE)
	{
		m_Report.Close();
		return nREPORT_OPENERROR;
	}

	int nErrorNo = nREPORT_NOERROR;

	CWorkingDlg WorkingDlg("Creating Report", TRUE);
	WorkingDlg.Create();

	CString strTitle = m_ReportConfig.GetReportTitle();
	strTitle += m_ReportConfig.GetDateString();
	m_Report.SetStyle1(strTitle);

	Server.Read(Filenames.GetServerDataFilename());

	CSQLRepositoryEcrmanDatabase repoDb;
	repoDb.GetDatabaseNameList(m_arrayDbNames, NULL);
	
	AddColumnText();

	auditFile.JumpIn(m_ReportConfig.GetDateFrom().GetDate());

	nErrorNo = AuditReport(&auditFile, &WorkingDlg);

	WorkingDlg.DestroyWindow();

	m_Report.Close();
	auditFile.Close();

	return nErrorNo;
}

//$$******************************************************************

void CAuditReport::ResetColumnInfo()
{
	m_arrayColumnInfoTran.RemoveAll();
	m_arrayColumnInfoBalance.RemoveAll();
	m_arrayColumnInfoSTD.RemoveAll();
}

//$$******************************************************************

void CAuditReport::AddColumnInfoTran(int nColumnType, CString strColumnName, bool bFlag)
{
	if (TRUE == bFlag)
	{
		CAuditReportColumnInfo info;
		info.m_nColumnType = nColumnType;
		info.m_strColumnName = strColumnName;
		m_arrayColumnInfoTran.Add(info);
	}
}

//$$******************************************************************

void CAuditReport::AddColumnInfoBalance(int nColumnType, CString strColumnName, bool bFlag)
{
	if (TRUE == bFlag)
	{
		CAuditReportColumnInfo info;
		info.m_nColumnType = nColumnType;
		info.m_strColumnName = strColumnName;
		m_arrayColumnInfoBalance.Add(info);
	}
}

//$$******************************************************************

void CAuditReport::AddColumnInfoSTD(int nColumnType, CString strColumnName, bool bFlag)
{
	if (TRUE == bFlag)
	{
		CAuditReportColumnInfo info;
		info.m_nColumnType = nColumnType;
		info.m_strColumnName = strColumnName;
		m_arrayColumnInfoSTD.Add(info);
	}
}

//$$******************************************************************

void CAuditReport::AddColumnText()
{
	int nAuditType = m_ReportConfig.GetReportType();

	switch (nAuditType)
	{
	case nAUDIT_SUMMARY:
		m_Report.AddColumn("Date", TA_LEFT, 210);
		m_Report.AddColumn("Time", TA_LEFT, 120);

		if (System.GetMaxEcrmanDatabases() > 1)
		{
			m_Report.AddColumn("Database", TA_LEFT, 300);
		}

		m_Report.AddColumn("Location", TA_LEFT, 300);
		m_Report.AddColumn(System.GetCardNoText(), TA_LEFT, 210);
		m_Report.AddColumn("Name", TA_LEFT, 300);
		break;

	default:
		m_Report.AddColumn("Audit Date", TA_LEFT, 210);
		m_Report.AddColumn("", TA_LEFT, 120);
		m_Report.AddColumn(System.GetCardNoText(), TA_LEFT, 210);
		m_Report.AddColumn("Name", TA_LEFT, 300);
		m_Report.AddColumn("Grp", TA_LEFT, 100);
		m_Report.AddColumn("Source", TA_LEFT, 230);
		m_Report.AddColumn("CCNo", TA_LEFT, 140);
		m_Report.AddColumn("OpNo", TA_LEFT, 140);
		m_Report.AddColumn("Last Used", TA_LEFT, 190);
		m_Report.AddColumn("", TA_LEFT, 120);
		break;
	}

	ResetColumnInfo();

	bool bShowTotalColumns = FALSE;

	switch (nAuditType)
	{
	case nAUDIT_REVALUATIONBONUS:
		AddColumnInfoTran(AUDIT_COLUMN_NONPURSE, "CashTaken");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "BonusP1");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE2, "BonusP2");
		AddColumnInfoTran(AUDIT_COLUMN_POINTS, "BonusPts");
		break;

	case nAUDIT_SPENDBONUS:
		AddColumnInfoTran(AUDIT_COLUMN_NONPURSE, "TrnValue");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "BonusP1");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE2, "BonusP2");
		AddColumnInfoTran(AUDIT_COLUMN_POINTS, "BonusPts");
		break;

	case nAUDIT_REDEEMBONUS:
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "BonusP1");
		AddColumnInfoTran(AUDIT_COLUMN_POINTS, "RedeemPts");
		break;

	case nAUDIT_POINTSAUDIT:
		AddColumnInfoTran(AUDIT_COLUMN_POINTS, "Points");
		break;

	case nAUDIT_POINTSADDED:
		AddColumnInfoTran(AUDIT_COLUMN_POINTS, "Points");
		m_bShowTotalDeducted = FALSE;
		break;

	case nAUDIT_POINTSDEDUCTED:
		AddColumnInfoTran(AUDIT_COLUMN_POINTS, "Points");
		m_bShowTotalAdded = FALSE;
		break;

	case nAUDIT_CASHTAKEN:
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1_TOPUP1, "P1.Topup1");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1_TOPUP2, "P1.Topup2", Server.HaveRevaluationPluCode(1, 2));
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1_TOPUP3, "P1.Topup3", Server.HaveRevaluationPluCode(1, 3));
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1_TOPUP4, "P1.Topup4", Server.HaveRevaluationPluCode(1, 4));
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1_TOPUP5, "P1.Topup5", Server.HaveRevaluationPluCode(1, 5));
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1_MANTOPUP, "P1.Manual");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1_IMPTOPUP, "P1.Import");

		if (System.GetEnablePurse2Flag() == TRUE)
		{
			AddColumnInfoTran(AUDIT_COLUMN_PURSE2_TOPUP1, "P2.Topup1", Server.HaveRevaluationPluCode(2, 1));
			AddColumnInfoTran(AUDIT_COLUMN_PURSE2_TOPUP2, "P2.Topup2", Server.HaveRevaluationPluCode(2, 2));
			AddColumnInfoTran(AUDIT_COLUMN_PURSE2_TOPUP3, "P2.Topup3", Server.HaveRevaluationPluCode(2, 3));
			AddColumnInfoTran(AUDIT_COLUMN_PURSE2_TOPUP4, "P2.Topup4", Server.HaveRevaluationPluCode(2, 4));
			AddColumnInfoTran(AUDIT_COLUMN_PURSE2_TOPUP5, "P2.Topup5", Server.HaveRevaluationPluCode(2, 5));
			AddColumnInfoTran(AUDIT_COLUMN_PURSE2_MANTOPUP, "P2.Manual");
			AddColumnInfoTran(AUDIT_COLUMN_PURSE2_IMPTOPUP, "P2.Import");
		}

		AddColumnInfoTran(AUDIT_COLUMN_GIFT, "Gift", Server.HaveStaffGiftCode());
		break;

	case nAUDIT_PURSEADDITIONS:
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "TrnP1");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE2, "TrnP2", System.GetEnablePurse2Flag());
		break;

	case nAUDIT_SUMMARY:
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "Purse1");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE2, "Purse2");
		AddColumnInfoTran(AUDIT_COLUMN_POINTS, "Points");

		if (m_ReportConfig.GetShowNonPurseFlag() == TRUE)
		{
			AddColumnInfoTran(AUDIT_COLUMN_NONPURSE, "NonPurse");
		}

		break;

	case nAUDIT_SALE:
	case nAUDIT_REFUND:
	default:
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "TrnP1");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE2, "TrnP2", System.GetEnablePurse2Flag());
		AddColumnInfoTran(AUDIT_COLUMN_POINTS, "TrnPts");
		AddColumnInfoTran(AUDIT_COLUMN_NONPURSE, "TrnCash");
		bShowTotalColumns = TRUE;
		break;
	}

	for (int n = 0; n < m_arrayColumnInfoTran.GetSize(); n++)
	{
		CAuditReportColumnInfo info = m_arrayColumnInfoTran.GetAt(n);
		m_Report.AddColumn(info.m_strColumnName, TA_RIGHT, 230);
	}

	if (nAUDIT_SUMMARY != nAuditType)
	{
		AddColumnInfoBalance(AUDIT_COLUMN_PURSE1, "P1.Bal");
		AddColumnInfoBalance(AUDIT_COLUMN_PURSE2, "P2.Bal", System.GetEnablePurse2Flag());
		AddColumnInfoBalance(AUDIT_COLUMN_POINTS, "PtsBal");

		for (int n = 0; n < m_arrayColumnInfoBalance.GetSize(); n++)
		{
			CAuditReportColumnInfo info = m_arrayColumnInfoBalance.GetAt(n);
			m_Report.AddColumn(info.m_strColumnName, TA_RIGHT, 230);
		}

		if (TRUE == bShowTotalColumns)
		{
			AddColumnInfoSTD(AUDIT_COLUMN_PURSE1, "P1Spend");
			AddColumnInfoSTD(AUDIT_COLUMN_PURSE2, "P2Spend", System.GetEnablePurse2Flag());
			AddColumnInfoSTD(AUDIT_COLUMN_TTLSPEND, "TTLSpend");
			AddColumnInfoSTD(AUDIT_COLUMN_POINTS, "TTLPoints");

			for (int n = 0; n < m_arrayColumnInfoSTD.GetSize(); n++)
			{
				CAuditReportColumnInfo info = m_arrayColumnInfoSTD.GetAt(n);
				m_Report.AddColumn(info.m_strColumnName, TA_RIGHT, 230);
			}
		}
	}
	else
	{
		m_Report.AddColumn("", TA_LEFT, 150);
	}

	m_Report.AddColumn("Comment", TA_LEFT, 300);
}

//$$******************************************************************

int CAuditReport::AuditReport(CLoyaltyAuditFile* pAudit, CWorkingDlg* pDlgWorking)
{
	int nResult = 0;

	g_GlobalState.BuildUserIDCache();
	ClearTLogArrays();
	m_pLastTLogDatabase = NULL;
	
	if ( (m_ReportConfig.GetSortByCardNoFlag() == TRUE) || (m_ReportConfig.GetSortByGroupNoFlag() == TRUE) )
	{
		nResult = AuditReportByCardNo(pAudit, pDlgWorking);
	}
	else
	{
		nResult = AuditReportByDate(pAudit, pDlgWorking);
	}
	g_GlobalState.ClearUserIDCache();
	ClearTLogArrays();

	return nResult;
}

//$$******************************************************************

int CAuditReport::AuditReportByDate(CLoyaltyAuditFile* pAudit, CWorkingDlg* pDlgWorking)
{
	int nCount = 0;
	bool bCancelButton = FALSE;

	CAuditRecord atc;
	while (pAudit->ReadLine(&atc) == TRUE)
	{
		if ( pDlgWorking -> SetCaption2PercentDone( pAudit->GetPercentPosition(), TRUE ) == TRUE )
		{
			bCancelButton = TRUE;
			break;
		}

		if (IsValid(&atc) == TRUE)
		{
			bool bExists = g_GlobalState.CheckUserIDCache(_atoi64(atc.GetCardNo()));
			
			if (SaveLine(&atc, bExists ) == TRUE)
			{
				++nCount;
			}
		}
	}

	int nErrorNo = nREPORT_NOERROR;

	if (nCount > 0)
	{
		SaveTotals(nTOTAL_REPORT);
	}
	else
	{
		nErrorNo = nREPORT_NOMATCH;
	}
	
	return (bCancelButton == TRUE) ? nREPORT_CANCEL : nErrorNo;
}

//$$******************************************************************

int CAuditReport::AuditReportByCardNo(CLoyaltyAuditFile* pAudit, CWorkingDlg* pDlgWorking)
{
	CRecordSorter sorter{};
	CAuditRecord atc{};

	bool bCancelButton = FALSE;

	while (TRUE)
	{
		if (pDlgWorking->SetCaption2PercentDone(pAudit->GetPercentPosition() / 2, TRUE) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		int nStartFilePos = pAudit->GetFilePos();			// position in file to start from
		if (pAudit->ReadLine(&atc) == FALSE)
		{
			break;
		}

		if (IsValid(&atc) == TRUE)
		{
			CString strLabel = "";
			
			if (m_ReportConfig.GetSortByGroupNoFlag() == TRUE)
			{
				strLabel.Format("%3.3d", atc.GetGroupNo());
			}

			if (m_ReportConfig.GetSortByCardNoFlag() == TRUE)
			{
				strLabel += System.FormatCardNo(atc.GetCardNo());
			}

			sorter.Add(strLabel, nStartFilePos);
		}
	}

	if (bCancelButton == TRUE)
	{
		return nREPORT_CANCEL;
	}

	if (sorter.GetSize() == 0)
	{
		return nREPORT_NOMATCH;
	}

	int nLineCount = 0;
	CString strLastLabel = "";

	for (int nIndex = 0; nIndex < sorter.GetSize(); nIndex++)
	{
		if (pDlgWorking->SetCaption2PercentDone(50 + ((nIndex * 50L) / sorter.GetSize()), TRUE) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		pAudit->SetFilePos(sorter.GetRecordNo(nIndex));
		if (pAudit->ReadLine(&atc) == TRUE)
		{
			int nError = 0;
			CString strLabel = sorter.GetLabel(nIndex);
			
			bool bExists = g_GlobalState.CheckUserIDCache(_atoi64(atc.GetCardNo()));

			// on first time round loop
			// save balance line if line is not a balance line
			if (nIndex == 0)
			{
				strLastLabel = strLabel;

				WriteHeaderLine(strLabel);

				if (m_ReportConfig.GetSortByGroupNoFlag() == FALSE)
				{
					SaveStartingBalanceLine(&atc, bExists);
				}
			}
			else
			{
				bool bNewBlock = FALSE;
				if (m_ReportConfig.GetSortByGroupNoFlag() == TRUE)
				{
					bNewBlock = (strLabel.Left(3) != strLastLabel.Left(3));
				}
				else
				{
					bNewBlock = (strLabel != strLastLabel);
				}

				// change of main label
				if (TRUE == bNewBlock)
				{
					if (nLineCount > 0)
					{
						SaveTotals(nTOTAL_CARD);

						if ((m_ReportConfig.GetNewPageFlag() == TRUE) && (m_ReportConfig.GetSortByCardNoFlag() == TRUE))
						{
							m_Report.WriteLine("<NEWPAGE>,1");
						}
						else
						{
							m_Report.WriteLine("");
						}

						WriteHeaderLine(strLabel);
					}

					nLineCount = 0;
					strLastLabel = strLabel;
					ResetTotals(nTOTAL_CARD);

					// save balance line if line is not a balance line
					if (m_ReportConfig.GetSortByCardNoFlag() == TRUE)
					{
						SaveStartingBalanceLine(&atc, bExists);
					}
				}
			}

			if (SaveLine(&atc, bExists) == TRUE)
			{
				++nLineCount;
			}
		}
	}

	// single report
	if ( (m_ReportConfig.GetReportLabel() == "R123") || (m_ReportConfig.GetReportLabel() == "R133") )
	{
		SaveTotals(nTOTAL_CARD);
	}
	else
	{
		if (nLineCount > 0)
		{
			SaveTotals(nTOTAL_CARD);
		}

		if (m_ReportConfig.GetNewPageFlag() == TRUE)
		{
			m_Report.WriteLine("<NEWPAGE>,1");
		}

		SaveTotals(nTOTAL_REPORT);
	}

	return (bCancelButton == TRUE) ? nREPORT_CANCEL : nREPORT_NOERROR;
}

//$$******************************************************************

void CAuditReport::WriteHeaderLine(CString strLabel)
{
	CString strHeader = "<..>";
	CString strCardNo = "";

	if (m_ReportConfig.GetSortByGroupNoFlag() == TRUE)
	{
		int nGroupNo = atoi(strLabel.Left(3));

		CString strGroupNo = "";
		strGroupNo.Format("G%2.2d, ", nGroupNo );
		strHeader += strGroupNo;

		CString strName = "";

		CSQLRowGroup RowGroup;
		RowGroup.SetGroupNo(nGroupNo);

		CSQLRepositoryGroup repoGroup;
		if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			strName = RowGroup.GetGroupName();
		}

		SolutionGlobalFunctions::TrimSpacesFromString(strName, FALSE);

		if (strName == "")
		{
			strName.Format("Group %d", nGroupNo);
		}

		strHeader += strName;
	}
	else
	{
		strHeader += "UserID ";
		strHeader += strLabel;
	}

	m_Report.WriteLine(strHeader);
	m_Report.WriteLine("<LI>");
}

//$$******************************************************************

bool CAuditReport::AppendAuditLineFields(CCSV& csv, CAuditRecord* pAtc, bool bCardExists)
{
	CString strAccountNo = System.FormatCardNo(pAtc->GetCardNo());

	if (bCardExists == FALSE)
	{
		if (m_ReportConfig.GetShowDeletedLinesFlag() == FALSE)
		{
			return FALSE;
		}
		else
		{
			strAccountNo += " *";
		}
	}

	CString strCCNo;
	strCCNo.Format("%4.4d", pAtc->GetCCNo());

	CString strOpNo;
	strOpNo.Format("%d", pAtc->GetOperatorNo());
	if (strOpNo == "0")
	{
		strOpNo = "";
	}

	CServerDataFolderSetIndex FolderSetIndex = pAtc->GetFolderSetIndex();
	
	switch (m_ReportConfig.GetReportType())
	{
	case nAUDIT_SUMMARY:
		{
			CString strDatabase = "";
			CString strLocation = "";

			switch (pAtc->GetSourceType())
			{
			case AUDIT_PC:
				strDatabase = "Back Office";
				break;

			case AUDIT_EXT:
				strDatabase = "External";
				break;

			default:
				{
					int nDbNo = FolderSetIndex.m_nDbNo;

					if ((nDbNo >= 1) && (nDbNo <= m_arrayDbNames.GetSize()))
					{
						strDatabase = m_arrayDbNames.GetAt(nDbNo - 1);
						strLocation = Server.GetEposPathLabel(FolderSetIndex);
					}
					else
					{
						strDatabase.Format("Database %d", nDbNo);
					}
				}
				break;
			}

			if (System.GetMaxEcrmanDatabases() > 1)
			{
				csv.Add(strDatabase);
			}

			csv.Add(strLocation);
			csv.Add(strAccountNo);
			csv.Add(pAtc->GetUserName());
		}
		break;

	default:
		csv.Add(strAccountNo);
		csv.Add(pAtc->GetUserName());
		csv.Add(pAtc->GetGroupNo());
		csv.Add(CAuditReportTexts::GetReportSourceText(pAtc->GetSourceType(), pAtc->GetApplicationNo(), pAtc->GetTerminalNo()));
		csv.Add(strCCNo);
		csv.Add(strOpNo);
		csv.Add(pAtc->GetDateLastUsed()); // last used at till
		csv.Add(pAtc->GetTimeLastUsed());
		break;
	}

	return TRUE;
}

//$$******************************************************************

bool CAuditReport::SaveLine(CAuditRecord* pAtc, bool bCardExists)
{
	int nAuditType = m_ReportConfig.GetReportType();

	CCSV csv('\t');
	csv.Add(pAtc->GetDateFiled());
	csv.Add(pAtc->GetTimeFiled());

	if (AppendAuditLineFields(csv, pAtc, bCardExists) == FALSE)
	{
		return FALSE;
	}

	CAuditReportLineInfo LineInfo(pAtc);

	for (int p = 1; p <= 2; p++)
	{
		for (int n = 1; n <= REVALUE_PLUNO_PERPURSE_COUNT; n++)
		{
			LineInfo.EnableEPOSTopup(p, n, FALSE);
		}
	}
	
	LineInfo.EnableGift(FALSE);
	int nTopupPurseNo = 0;
	int nTopupTopupNo = 0;
	CAuditReportTexts::GetAddCashIndexByAppNo(pAtc->GetApplicationNo(), nTopupPurseNo, nTopupTopupNo);

	switch (nAuditType)
	{
	case nAUDIT_CASHTAKEN:

		switch (pAtc->GetApplicationNo())
		{
		case APPNO_ADDGIFT:
		case APPNO_ADDGIFTX:
			LineInfo.EnableGift(TRUE);
			LineInfo.m_dGift = LineInfo.m_dPurse1;
			break;

		case APPNO_PURSE1_MANTOPUP:
			LineInfo.EnableManualTopupPurse1(TRUE);
			LineInfo.m_dManualTopupPurse1 = LineInfo.m_dPurse1;
			break;

		case APPNO_PURSE2_MANTOPUP:
			LineInfo.EnableManualTopupPurse2(TRUE);
			LineInfo.m_dManualTopupPurse2 = LineInfo.m_dPurse2;
			break;

		case APPNO_PURSE1_IMPTOPUP:
			LineInfo.EnableImportTopupPurse1(TRUE);
			LineInfo.m_dImportTopupPurse1 = LineInfo.m_dPurse1;
			break;

		case APPNO_PURSE2_IMPTOPUP:
			LineInfo.EnableImportTopupPurse2(TRUE);
			LineInfo.m_dImportTopupPurse2 = LineInfo.m_dPurse2;
			break;

		default:
			{
				if ((1 == nTopupPurseNo) || (2 == nTopupPurseNo))
				{
					LineInfo.EnableEPOSTopup(nTopupPurseNo, nTopupTopupNo, TRUE);
					LineInfo.m_dEPOSTopup[nTopupPurseNo][nTopupTopupNo] = LineInfo.m_dPurse1 + LineInfo.m_dPurse2;
				}
			}
			break;
		}
		break;

	default:

		LineInfo.EnablePurse1(TRUE);
		LineInfo.EnablePurse2(System.GetEnablePurse2Flag());
		LineInfo.EnablePoints(TRUE);

		switch (pAtc->GetApplicationNo())
		{
		case APPNO_INSERT:
		case APPNO_DELETE:
		case APPNO_HOTLIST:
		case APPNO_UNHOTLIST:
		case APPNO_BALANCE:
		case APPNO_IMPORTUPDATE:
		case APPNO_IMPORTINSERT:
		case APPNO_BATCHDELETE:
		case APPNO_EXTENDEXPIRE:
		case APPNO_SETPURSE1:
			LineInfo.EnablePurse1(FALSE);
			LineInfo.EnablePurse2(FALSE);
			LineInfo.EnablePoints(FALSE);
			break;

		case APPNO_ADDPOINTS:
			LineInfo.EnablePurse1(FALSE);
			LineInfo.EnablePurse2(FALSE);
			break;

		case APPNO_ADDGIFT:
		case APPNO_ADDGIFTX:
			LineInfo.EnablePurse2(FALSE);
			LineInfo.EnablePoints(FALSE);
			break;

		case APPNO_MANUAL_SPEND:
		case APPNO_MANUAL_REFUND:
			LineInfo.EnablePurse2(FALSE);
			break;

		case APPNO_REVALUEBONUSCASH:
			switch( nAuditType )
			{ 
			case nAUDIT_REPORT:
				LineInfo.EnablePoints(FALSE);
				break;

			case nAUDIT_REVALUATIONBONUS:
				LineInfo.EnableNonPurse(TRUE);
				break;
			}
			break;

		case APPNO_REWARDBONUSCASH:
		case APPNO_REFUNDREWARDBONUSCASH:
			switch (nAuditType)
			{
			case nAUDIT_REPORT:
				LineInfo.EnablePoints(FALSE);
				break;

			case nAUDIT_SPENDBONUS:
				LineInfo.EnableNonPurse(TRUE);
				break;
			}
			break;

		case APPNO_REVALUEBONUSPOINTS:
		case APPNO_REWARDBONUSPOINTS:
		case APPNO_REFUNDREWARDBONUSPOINTS:
			switch (nAuditType)
			{
			case nAUDIT_REPORT:
				LineInfo.EnablePurse1(FALSE);
				LineInfo.EnablePurse2(FALSE);
				break;

			case nAUDIT_REVALUATIONBONUS:
				LineInfo.EnableNonPurse(TRUE);
				break;
			}
			break;

		case APPNO_REFRESH:
			LineInfo.EnablePurse1(FALSE);
			LineInfo.EnablePoints(FALSE);
			break;

		case APPNO_SALE:
		case APPNO_REFUNDSALE:
			LineInfo.EnableNonPurse(TRUE);
			break;

		case APPNO_EDIT:
		case APPNO_REDEEM:
		case APPNO_BATCHREDEEM:
		case APPNO_XFERIN:
		case APPNO_XFEROUT:
		case APPNO_BATCHUPDATE:
		case APPNO_SUBCASH:
		case APPNO_SUBPOINTS:
			break;

		default:
			if ((1 == nTopupPurseNo) || (2 == nTopupPurseNo))
			{
				LineInfo.EnablePoints(FALSE);
			}
			break;
		}
	}

	AppendTranTotals(csv, LineInfo, pAtc);
	AppendBalanceTotals(csv, pAtc);
	AppendSTDTotals(csv, pAtc);

	if (m_ReportConfig.GetReportType() == nAUDIT_SUMMARY)
	{
		csv.Add("");
	}

	csv.Add(GetCommentText(pAtc));

	return m_Report.WriteLine(csv);
}

//$$******************************************************************

void CAuditReport::AppendTranTotals(CCSV& csv, CAuditReportLineInfo& info, CAuditRecord* pAtc)
{
	for (int n = 0; n < m_arrayColumnInfoTran.GetSize(); n++)
	{
		switch (m_arrayColumnInfoTran[n].m_nColumnType)
		{
		case AUDIT_COLUMN_GIFT:
			AppendLineTotals(csv, info.GotColumnGift(), info.m_dGift, m_TotalGift);
			break;

		case AUDIT_COLUMN_PURSE1:
			AppendLineTotals(csv, info.GotColumnPurse1(), info.m_dPurse1, m_TotalPurse1);
			break;

		case AUDIT_COLUMN_PURSE2:
			AppendLineTotals(csv, info.GotColumnPurse2(), info.m_dPurse2, m_TotalPurse2);
			break;

		case AUDIT_COLUMN_POINTS:
			AppendLineTotals(csv, info.GotColumnPoints(), info.m_nPoints, m_TotalPoints);
			break;

		case AUDIT_COLUMN_PURSE1_MANTOPUP:
			AppendLineTotals(csv, info.GotColumnManualTopupPurse1(), info.m_dManualTopupPurse1, m_TotalPurse1ManualTopup);
			break;

		case AUDIT_COLUMN_PURSE2_MANTOPUP:
			AppendLineTotals(csv, info.GotColumnManualTopupPurse2(), info.m_dManualTopupPurse2, m_TotalPurse2ManualTopup);
			break;

		case AUDIT_COLUMN_PURSE1_IMPTOPUP:
			AppendLineTotals(csv, info.GotColumnImportTopupPurse1(), info.m_dImportTopupPurse1, m_TotalPurse1ImportTopup);
			break;

		case AUDIT_COLUMN_PURSE2_IMPTOPUP:
			AppendLineTotals(csv, info.GotColumnImportTopupPurse2(), info.m_dImportTopupPurse2, m_TotalPurse2ImportTopup);
			break;

		case AUDIT_COLUMN_NONPURSE:
			AppendLineTotals(csv, info.GotColumnNonPurse(), info.m_dNonPurse, m_TotalNonPurse);
			break;

		default:
			{
				int nPurseNo = 0;
				int nTopupNo = 0;
				GetEPOSTopupColumnIndex(m_arrayColumnInfoTran[n].m_nColumnType, nPurseNo, nTopupNo);

				if ( (1 == nPurseNo ) || (2 == nPurseNo ))
				{
					AppendLineTotals(csv, info.GotColumnEPOSTopup(nPurseNo,nTopupNo), info.m_dEPOSTopup[nPurseNo][nTopupNo], m_TotalEPOSTopup[nPurseNo][nTopupNo]);
				}
			}
			break;
		}
	}
}

//$$******************************************************************

void CAuditReport::AppendLineTotals(CCSV& csv, bool bGotTotal, double dValue, CAuditTotalDouble& Total)
{
	if (TRUE == bGotTotal)
	{
		csv.Add(SignedFormatDoubleValue(dValue));
		Total.Add(dValue);
	}
	else
	{
		csv.Add("");
	}
}

//$$******************************************************************

void CAuditReport::AppendLineTotals(CCSV& csv, bool bGotTotal, int nValue, CAuditTotalInt& Total)
{
	if (TRUE == bGotTotal)
	{
		csv.Add(SignedFormatIntValue(nValue));
		Total.Add(nValue);
	}
	else
	{
		csv.Add("");
	}
}

//$$******************************************************************

void CAuditReport::AppendBalanceTotals(CCSV& csv, CAuditRecord* pAtc)
{
	for (int n = 0; n < m_arrayColumnInfoBalance.GetSize(); n++)
	{
		switch (m_arrayColumnInfoBalance[n].m_nColumnType)
		{
		case AUDIT_COLUMN_PURSE1:
			csv.Add(pAtc->GetPurse1(), System.GetDPValue());
			break;

		case AUDIT_COLUMN_PURSE2:
			csv.Add(pAtc->GetPurse2(), System.GetDPValue());
			break;

		case AUDIT_COLUMN_POINTS:
			csv.Add(pAtc->GetPoints());
			break;
		}
	}
}

//$$******************************************************************

void CAuditReport::AppendSTDTotals(CCSV& csv, CAuditRecord* pAtc)
{
	for (int n = 0; n < m_arrayColumnInfoSTD.GetSize(); n++)
	{
		switch (m_arrayColumnInfoSTD[n].m_nColumnType)
		{
		case AUDIT_COLUMN_PURSE1:
			csv.Add(pAtc->GetPurse1SpendTD(), System.GetDPValue());
			break;

		case AUDIT_COLUMN_PURSE2:
			csv.Add(pAtc->GetPurse2SpendTD(), System.GetDPValue());
			break;

		case AUDIT_COLUMN_TTLSPEND:
			csv.Add(pAtc->GetTotalSpendTD(), System.GetDPValue());
			break;

		case AUDIT_COLUMN_POINTS:
			csv.Add(pAtc->GetPointsTD());
			break;
		}
	}
}

//$$******************************************************************

CString CAuditReport::GetCommentText(CAuditRecord* pAtc)
{
	CString strComment = pAtc->GetComment();

	// see if ecr audit line
	switch (pAtc->GetSourceType())
	{
	case AUDIT_POS:
	case AUDIT_EXT:
		switch (pAtc->GetApplicationNo())
		{
		case APPNO_SALE:
		case APPNO_REFUNDSALE:
			if ( (System.GetShowAuditPurchasesFlag() == TRUE) && ( m_ReportConfig.GetShowEcrTextFlag() == TRUE) )
			{
				bool bNewTLogDatabase = FALSE;

				CTLogDatabase* pDatabase = NULL;
				__int64 nUserID = _atoi64(pAtc -> GetCardNo());

				CSortedIntByInt64 item;
				item.m_nKey = nUserID;

				int nPos = 0;
				if (m_arrayTLogMap.Find(item, nPos) == TRUE)
				{
					m_arrayTLogMap.GetAt(nPos, item);
					pDatabase = m_arrayTLogDatabases.GetAt(item.m_nVal);
					m_pLastTLogDatabase = pDatabase;
				}
				else
				{
					item.m_nVal = m_arrayTLogDatabases.GetSize();
					m_arrayTLogMap.InsertAt(nPos, item);

					pDatabase = new (CTLogDatabase);
					m_pLastTLogDatabase = pDatabase;
					m_arrayTLogDatabases.Add(pDatabase);

					bNewTLogDatabase = TRUE;
				}

				strComment = pAtc->ExtractPurchaseText(pDatabase, bNewTLogDatabase);
			}
			break;
		}
		break;
	}

	if (strComment == "") 
	{
		strComment = CAuditReportTexts::GetReportSourceText(pAtc->GetSourceType(), pAtc->GetApplicationNo(), pAtc->GetTerminalNo(), TRUE);
	}

	return strComment;
}

//$$***************************************************************

bool CAuditReport::SaveStartingBalanceLine(CAuditRecord* pAtc, bool bCardExists)
{
	int nAuditType = m_ReportConfig.GetReportType();

	CString strCardNo = System.FormatCardNo(pAtc->GetCardNo());

	if (bCardExists == FALSE)						// see if account exists
	{
		if (m_ReportConfig.GetShowDeletedLinesFlag() == FALSE)	// see if show deleted lines set
		{
			return FALSE;								// no - ignore line
		}
		else
		{
			strCardNo += " *";
		}
	}

	// no starting bal on cash added report
	switch (nAuditType)
	{
	case nAUDIT_CASHTAKEN:
	case nAUDIT_SUMMARY:
		return TRUE;
	}

	switch (pAtc->GetApplicationNo())
	{
	case APPNO_DELETE:
	case APPNO_BATCHDELETE:
	case APPNO_HOTLIST:
	case APPNO_UNHOTLIST:
	case APPNO_INSERT:
	case APPNO_IMPORTINSERT:
	case APPNO_IMPORTUPDATE:
	case APPNO_EXTENDEXPIRE:
		return TRUE;
	}

	CCSV csv('\t');
	csv.Add(pAtc->GetDateFiled());
	csv.Add(pAtc->GetTimeFiled());
	csv.Add(strCardNo);
	csv.Add(pAtc->GetUserName());
	csv.Add(pAtc->GetGroupNo());
	csv.Add("<..>Starting Balance");					// source
	csv.Add("");										// operator
	csv.Add("");										// ccno
	csv.Add("");										// date last used
	csv.Add("");										// time last used

	for (int n = 0; n < m_arrayColumnInfoTran.GetSize(); n++)
	{
		csv.Add("");
	}

	for (int n = 0; n < m_arrayColumnInfoBalance.GetSize(); n++)
	{
		switch (m_arrayColumnInfoBalance[n].m_nColumnType)
		{
		case AUDIT_COLUMN_PURSE1:
		{
			double dStartingPurseBalance = pAtc->GetPurse1() - pAtc->GetPurse1Transaction();
			csv.Add(dStartingPurseBalance, System.GetDPValue());
		}
		break;

		case AUDIT_COLUMN_PURSE2:
		{
			double dStartingPurseBalance = pAtc->GetPurse2() - pAtc->GetPurse2Transaction();
			csv.Add(dStartingPurseBalance, System.GetDPValue());
		}
		break;

		case AUDIT_COLUMN_POINTS:
		{
			int nPointsStartingBalance = pAtc->GetPoints() - pAtc->GetPointsTransaction();
			csv.Add(nPointsStartingBalance);
		}
		break;
		}
	}

	return m_Report.WriteLine(csv);
}

//$$***************************************************************
//$$ single card audit that scans archive files *******************
//$$***************************************************************

int CAuditReport::CreateWithArchive(const char* szReportFile)
{
	int nErrorNo = nREPORT_NOERROR;

	if (m_Report.Open(szReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CWorkingDlg WorkingDlg("Creating Report", TRUE);
	WorkingDlg.Create();

	CString strTitle = "";
	switch (m_ReportConfig.GetSearchType())
	{
	case 0:		
		strTitle.Format("Archive Search Account : %s", (const char*) m_ReportConfig.GetSearchCardNo());	
		break;

	case 1:
		strTitle.Format("Archive Search Name : %s", (const char*) m_ReportConfig.GetSearchText());		
		break;

	case 2:		
		strTitle.Format("Archive Search Comment : %s", (const char*) m_ReportConfig.GetSearchText());		
		break;
	}

	m_Report.SetStyle1(strTitle);
	AddColumnText();

	CString strBuf;
	int nTotalCount = 0;
	CStringArray arrayLabels;

	for (int nIndex = 0; nIndex < m_ReportConfig.SearchFilenamesGetCount(); nIndex++)
	{
		int nCount = ScanArchive(nIndex, &WorkingDlg);
		if (nCount == -1)
		{
			nErrorNo = nREPORT_CANCEL;
			break;
		}

		strBuf.Format("%s\t<..>( %d )", 
			(const char*)m_ReportConfig.SearchFilenamesGetItem(nIndex), 
			nCount);	// add "dd/mm/yy {count} of lines found in file
		arrayLabels.Add(strBuf);

		if (nCount > 0)									// no lines found in file
		{
			m_Report.WriteLine("");						// separator line in report file
			nTotalCount += nCount;
		}
	}

	if (nErrorNo == nREPORT_CANCEL)
	{
		return nREPORT_CANCEL;
	}

	SaveTotals(nTOTAL_REPORT);

	m_Report.WriteLine("");
	strBuf.Format("<..>Files scanned = %d, Entries found = %d", m_ReportConfig.SearchFilenamesGetCount(), nTotalCount);
	m_Report.WriteLine(strBuf);

	for (int nIndex = 0; nIndex < arrayLabels.GetSize(); nIndex++)
	{
		m_Report.WriteLine(arrayLabels.GetAt(nIndex));
	}

	m_Report.Close();

	return nREPORT_NOERROR;
}

//$$******************************************************************
// return number of lines ( -1 = cancel button )

int CAuditReport::ScanArchive(int nIndex, CWorkingDlg* pDlgWorking)
{
	CLoyaltyAuditFile auditFile(m_ReportConfig.SearchFilenamesGetPathname(nIndex));			// filename to scan yyyymmdd.atcstrPathname );
	if (auditFile.OpenReadOnly() == FALSE)
	{
		return 0;
	}

	// start date of file dd/mm/yyyy
	// card no. to scan for
	// or text to search for & make uppercase
	CString strFileLabel = m_ReportConfig.SearchFilenamesGetItem(nIndex);
	CString strCardNo = m_ReportConfig.GetSearchCardNo();
	CString strSearchText = m_ReportConfig.GetSearchText();
	strSearchText.MakeUpper();

	// current file number being scanned
	// total files to scan
	int nFileNo = nIndex + 1;
	int nFileCounter = m_ReportConfig.SearchFilenamesGetCount();

	CString strMsg = "";
	strMsg.Format("Scanning File %d of %d : %s",
		nFileNo,
		nFileCounter,
		(const char*)strFileLabel);

	pDlgWorking->SetCaption1(strMsg);

	int nLineCount = 0;									// matching lines found
	bool bBalanceLineSaved = FALSE;

	bool bAccountExists = TRUE;
	if (strCardNo != "")
	{
		CSQLRepositoryAccount repoAccount;
		bAccountExists = (repoAccount.RowExists(_atoi64(strCardNo), NULL).GetSQLResult() != 0);
	}

	CAuditRecord atc;
	while (auditFile.ReadLine(&atc) == TRUE)
	{
		if ( pDlgWorking -> SetCaption2PercentDone( auditFile.GetPercentPosition(), TRUE ) == TRUE )
		{
			nLineCount = -1;
			break;
		}

		if (m_ReportConfig.GetSearchType() == 0)							// 0 = scan on account
		{
			if (atc.GetCardNo() == strCardNo)
			{
				if (FALSE == bBalanceLineSaved)
				{
					bBalanceLineSaved = SaveStartingBalanceLine(&atc);
				}

				if (SaveLine(&atc, bAccountExists) == TRUE)			// save audit line
				{
					++nLineCount;
				}
			}
		}
		else													// 1=Account name, 2= audit comment
		{
			CString strBuf = "";
			if (m_ReportConfig.GetSearchType() == 1)
			{
				strBuf = atc.GetUserName();						
			}
			else
			{
				strBuf = GetCommentText(&atc);					// get comment text
			}
				
			strBuf.MakeUpper();

			if (strBuf.Find(strSearchText, 0) != -1)		// see partial text found in name \ comment field
			{
				CString strCardNo = atc.GetCardNo();;
				CSQLRepositoryAccount repoAccount;
				bool bExists = (repoAccount.RowExists(_atoi64(strCardNo), NULL).GetSQLResult() != 0);
				
				if (SaveLine(&atc, bExists) == TRUE)
				{
					++nLineCount;
				}
			}
		}
	}
	auditFile.Close();

	return nLineCount;
}

//$$******************************************************************

bool CAuditReport::IsValid(CAuditRecord* pAtc)
{
	if (pAtc->m_dateFiled < m_ReportConfig.GetDateFrom())
	{
		return FALSE;
	}

	if (pAtc->m_dateFiled > m_ReportConfig.GetDateTo())
	{
		return FALSE;
	}

	if (m_ReportConfig.GetTimeRangeFlag() == TRUE)
	{
		if (pAtc->m_timeFiled < m_ReportConfig.GetTimeFrom())
		{
			return FALSE;
		}

		if (pAtc->m_timeFiled > m_ReportConfig.GetTimeTo())
		{
			return FALSE;
		}
	}

	int nSourceType = pAtc->GetSourceType();
	int nAppNo = pAtc->GetApplicationNo();

	if ((nSourceType == AUDIT_SERVER) && (m_ReportConfig.GetShowServerLinesFlag() == FALSE))
	{
		return FALSE;
	}

	if ((nSourceType == AUDIT_POS) && (m_ReportConfig.GetShowPosLinesFlag() == FALSE))
	{
		return FALSE;
	}

	if ((nSourceType == AUDIT_PC) && (m_ReportConfig.GetShowPcLinesFlag() == FALSE))
	{
		return FALSE;
	}

	if ((nSourceType == AUDIT_EXT) && (m_ReportConfig.GetShowExtLinesFlag() == FALSE))
	{
		return FALSE;
	}

	if (m_ReportConfig.GetShowAllActionsFlag() == FALSE)
	{
		if (m_ReportConfig.GetShowActionFlag(nAppNo) == FALSE)
		{
			return FALSE;				
		}
	}

	if (IsCardInRange(pAtc) == FALSE)
	{
		return FALSE;
	}

	// check Epos terminal list
	switch (nSourceType)
	{
	case AUDIT_POS:
	case AUDIT_SERVER:
		{
			CServerDataFolderSetIndex FolderSetIndex = pAtc->GetFolderSetIndex();	
			if (m_ReportConfig.CheckTerminalFilter(FolderSetIndex.m_nDbNo,FolderSetIndex.m_nSetNo,pAtc->GetTerminalNo()) == FALSE)
			{
				return FALSE;
			}
		}
		break;
	}

	//** check on type of report required

	bool bReply = FALSE;
	int nTopupPurseNo = 0;
	int nTopupTopupNo = 0;
	CAuditReportTexts::GetAddCashIndexByAppNo(nAppNo,nTopupPurseNo,nTopupTopupNo);

	switch (m_ReportConfig.GetReportType())
	{
	case nAUDIT_REPORT:
		bReply = TRUE;
		break;

	case nAUDIT_REVALUATIONBONUS:
		switch (nAppNo)
		{
		case APPNO_REVALUEBONUSCASH:
		case APPNO_REVALUEBONUSPOINTS:
			bReply = TRUE;
			break;
		}
		break;

	case nAUDIT_SPENDBONUS:
		switch (nAppNo)
		{
		case APPNO_REWARDBONUSCASH:
		case APPNO_REFUNDREWARDBONUSCASH:
		case APPNO_REWARDBONUSPOINTS:
		case APPNO_REFUNDREWARDBONUSPOINTS:
			bReply = TRUE;
			break;
		}
		break;

	case nAUDIT_CASHTAKEN:
		switch (nAppNo)
		{
		case APPNO_ADDGIFT:
		case APPNO_ADDGIFTX:
		case APPNO_SUBCASH:
		case APPNO_PURSE1_MANTOPUP:
		case APPNO_PURSE2_MANTOPUP:
		case APPNO_PURSE1_IMPTOPUP:
		case APPNO_PURSE2_IMPTOPUP:
			bReply = TRUE;
			break;

		default:
			if ((1 == nTopupPurseNo) || (2 == nTopupPurseNo))
			{
				bReply = TRUE;
			}
			break;
		}
		break;

	case nAUDIT_PURSEADDITIONS:
		switch (nAppNo)
		{
		case APPNO_EDIT:
		case APPNO_ADDGIFT:
		case APPNO_ADDGIFTX:
			if (pAtc->GetPurse1Transaction() != 0.0)			// see if any purse1 transaction value
			{
				bReply = TRUE;
			}
			break;

		case APPNO_REFRESH:
			if (pAtc->GetPurse2Transaction() != 0.0)			// see if Purse2 refreshed
			{
				bReply = TRUE;
			}
			break;

		default:
			if ((1 == nTopupPurseNo) || (2 == nTopupPurseNo))
			{
				if ((pAtc->GetPurse1Transaction() != 0.0) || (pAtc->GetPurse2Transaction() != 0.0))
				{
					bReply = TRUE;
				}
			}
			break;
		}
		break;

	case nAUDIT_REDEEMBONUS:
		switch (nAppNo)
		{
		case APPNO_REDEEM:
		case APPNO_BATCHREDEEM:
			bReply = TRUE;
			break;
		}
		break;

	case nAUDIT_POINTSAUDIT:
		if (pAtc->GetPointsTransaction() != 0)
		{
			bReply = TRUE;
		}
		break;

	case nAUDIT_POINTSADDED:
		if (pAtc->GetPointsTransaction() > 0)
		{
			bReply = TRUE;
		}
		break;

	case nAUDIT_POINTSDEDUCTED:
		if (pAtc->GetPointsTransaction() < 0)
		{
			bReply = TRUE;
		}
		break;

	case nAUDIT_SALE:
		switch (nAppNo)
		{
		case APPNO_SALE:
		case APPNO_MANUAL_SPEND:
			if (pAtc->GetTotalTransaction() != 0.0)
			{
				bReply = TRUE;
			}
			break;
		}
		break;

	case nAUDIT_REFUND:
		switch (nAppNo)
		{
			case APPNO_REFUNDSALE:
			case APPNO_MANUAL_REFUND:
			{
				bReply = TRUE;
			}
			break;
		}
		break;

	case nAUDIT_SUMMARY:
		if ((pAtc->GetPurse1Transaction() != 0.0) || (pAtc->GetPointsTransaction() != 0) || ( pAtc ->GetPurse2Transaction() != 0 ) || (pAtc -> GetCashTransaction() != 0) )
		{
			bReply = TRUE;
		}
		break;
	}

	return bReply;
}

//$$******************************************************************

bool CAuditReport::IsCardInRange(CAuditRecord* pAtc)
{
	CString strCardNo = pAtc->GetCardNo();

	if (m_ReportConfig.GetReportRangeType() == nREPORT_LIST)
	{
		return m_ReportConfig.IsValidCard(strCardNo, Filenames.GetReportCardListFilename(m_ReportConfig.GetReportLabel()));
	}

	__int64 nCardNo = _atoi64(strCardNo);
	if (nCardNo < m_ReportConfig.GetInt64CardNoFrom() || nCardNo > m_ReportConfig.GetInt64CardNoTo())
	{
		return FALSE;
	}

	int nGroupNo = pAtc->GetGroupNo();
	if ((m_ReportConfig.GetSingleGroupFlag() == TRUE) && (m_ReportConfig.GetSingleGroupNo() != nGroupNo))
	{
		return FALSE;
	}

	return TRUE;
}

//$$******************************************************************

void CAuditReport::SaveTotals(int nIndex)
{
	CCSV csvAdded('\t');
	CCSV csvSubed('\t');
	CCSV csvNet('\t');

	int nSkip = 0;
	if (m_ReportConfig.GetReportType() == nAUDIT_SUMMARY)
	{
		if (System.GetMaxEcrmanDatabases() > 1)
		{
			nSkip = 6;
		}
		else
		{
			nSkip = 5;
		}
	}
	else
	{
		nSkip = 10;
	}

	CString strTabLine = "";
	for (int n = 1; n <= nSkip; n++)
	{
		strTabLine += "\t";

		if (n != 4)
		{
			csvAdded.Add("");
			csvSubed.Add("");
			csvNet.Add("");
		}
		else
		{
			csvAdded.Add("<..>Total ( + )");
			csvSubed.Add("<..>Total ( - )");
			csvNet.Add("<..>Total (net)");
		}
	}

	for (int n = 0; n < m_arrayColumnInfoTran.GetSize(); n++)
	{
		switch (m_arrayColumnInfoTran[n].m_nColumnType)
		{
		case AUDIT_COLUMN_GIFT:
			csvAdded.Add(::SignedFormatDoubleValue(m_TotalGift.GetTotalPlus(nIndex)));
			csvSubed.Add(::SignedFormatDoubleValue(m_TotalGift.GetTotalMinus(nIndex)));
			csvNet.Add(::SignedFormatDoubleValue(m_TotalGift.GetTotalNet(nIndex)));
			strTabLine += "<LI>\t";
			break;

		case AUDIT_COLUMN_PURSE1:
			csvAdded.Add(::SignedFormatDoubleValue(m_TotalPurse1.GetTotalPlus(nIndex)));
			csvSubed.Add(::SignedFormatDoubleValue(m_TotalPurse1.GetTotalMinus(nIndex)));
			csvNet.Add(::SignedFormatDoubleValue(m_TotalPurse1.GetTotalNet(nIndex)));
			strTabLine += "<LI>\t";
			break;

		case AUDIT_COLUMN_PURSE2:
			csvAdded.Add(::SignedFormatDoubleValue(m_TotalPurse2.GetTotalPlus(nIndex)));
			csvSubed.Add(::SignedFormatDoubleValue(m_TotalPurse2.GetTotalMinus(nIndex)));
			csvNet.Add(::SignedFormatDoubleValue(m_TotalPurse2.GetTotalNet(nIndex)));
			strTabLine += "<LI>\t";
			break;
		
		case AUDIT_COLUMN_POINTS:
			csvAdded.Add(::SignedFormatIntValue(m_TotalPoints.GetTotalPlus(nIndex)));
			csvSubed.Add(::SignedFormatIntValue(m_TotalPoints.GetTotalMinus(nIndex)));
			csvNet.Add(::SignedFormatIntValue(m_TotalPoints.GetTotalNet(nIndex)));
			strTabLine += "<LI>\t";
			break;

		case AUDIT_COLUMN_PURSE1_MANTOPUP:
			csvAdded.Add(::SignedFormatDoubleValue(m_TotalPurse1ManualTopup.GetTotalPlus(nIndex)));
			csvSubed.Add(::SignedFormatDoubleValue(m_TotalPurse1ManualTopup.GetTotalMinus(nIndex)));
			csvNet.Add(::SignedFormatDoubleValue(m_TotalPurse1ManualTopup.GetTotalNet(nIndex)));
			strTabLine += "<LI>\t";
			break;
			
		case AUDIT_COLUMN_PURSE2_MANTOPUP:
			csvAdded.Add(::SignedFormatDoubleValue(m_TotalPurse2ManualTopup.GetTotalPlus(nIndex)));
			csvSubed.Add(::SignedFormatDoubleValue(m_TotalPurse2ManualTopup.GetTotalMinus(nIndex)));
			csvNet.Add(::SignedFormatDoubleValue(m_TotalPurse2ManualTopup.GetTotalNet(nIndex)));
			strTabLine += "<LI>\t";
			break;

		case AUDIT_COLUMN_PURSE1_IMPTOPUP:
			csvAdded.Add(::SignedFormatDoubleValue(m_TotalPurse1ImportTopup.GetTotalPlus(nIndex)));
			csvSubed.Add(::SignedFormatDoubleValue(m_TotalPurse1ImportTopup.GetTotalMinus(nIndex)));
			csvNet.Add(::SignedFormatDoubleValue(m_TotalPurse1ImportTopup.GetTotalNet(nIndex)));
			strTabLine += "<LI>\t";
			break;

		case AUDIT_COLUMN_PURSE2_IMPTOPUP:
			csvAdded.Add(::SignedFormatDoubleValue(m_TotalPurse2ImportTopup.GetTotalPlus(nIndex)));
			csvSubed.Add(::SignedFormatDoubleValue(m_TotalPurse2ImportTopup.GetTotalMinus(nIndex)));
			csvNet.Add(::SignedFormatDoubleValue(m_TotalPurse2ImportTopup.GetTotalNet(nIndex)));
			strTabLine += "<LI>\t";
			break;

		case AUDIT_COLUMN_NONPURSE:
			csvAdded.Add(::SignedFormatDoubleValue(m_TotalNonPurse.GetTotalPlus(nIndex)));
			csvSubed.Add(::SignedFormatDoubleValue(m_TotalNonPurse.GetTotalMinus(nIndex)));
			csvNet.Add(::SignedFormatDoubleValue(m_TotalNonPurse.GetTotalNet(nIndex)));
			strTabLine += "<LI>\t";
			break;

		default:
			{
				int nPurseNo = 0;
				int nTopupNo = 0;
				GetEPOSTopupColumnIndex(m_arrayColumnInfoTran[n].m_nColumnType, nPurseNo, nTopupNo);

				if ((1 == nPurseNo) || (2 == nPurseNo))
				{
					if ((nTopupNo >= 1) && (nTopupNo <= REVALUE_PLUNO_PERPURSE_COUNT))
					{
						csvAdded.Add(::SignedFormatDoubleValue(m_TotalEPOSTopup[nPurseNo][nTopupNo].GetTotalPlus(nIndex)));
						csvSubed.Add(::SignedFormatDoubleValue(m_TotalEPOSTopup[nPurseNo][nTopupNo].GetTotalMinus(nIndex)));
						csvNet.Add(::SignedFormatDoubleValue(m_TotalEPOSTopup[nPurseNo][nTopupNo].GetTotalNet(nIndex)));
						strTabLine += "<LI>\t";
					}
				}
			}
			break;
		}
	}

	m_Report.WriteLine(strTabLine);

	if (m_bShowTotalAdded == TRUE)
	{
		m_Report.WriteLine(csvAdded);
	}

	if (m_bShowTotalDeducted == TRUE)
	{
		m_Report.WriteLine(csvSubed);
	}

	if ((TRUE == m_bShowTotalAdded) && (TRUE == m_bShowTotalDeducted))
	{
		m_Report.WriteLine(strTabLine);
		m_Report.WriteLine(csvNet);
	}

	if (nIndex == nTOTAL_CARD)
	{
		m_Report.WriteLine("");
	}
}

//$$******************************************************************
