//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\PebbleHelpers.h"
//$$******************************************************************
#include "..\SmartPay4Shared\ConsolidationMap.h"
#include "..\SmartPay4Shared\AuditReportTexts.h"
#include "..\SmartPay4Shared\GlobalState.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\ReportConsolidationMap.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLConnectionPool.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\TLogFile.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//$$******************************************************************
#include "AuditReport.h"
//$$******************************************************************
//R101 - Audit
//R102 - Audit Bonus Revaluation
//R103 - Audit Bonus Spend
//R109 - Audit Cash Taken
//R111 - Single Account Audit
//R112 - Points Audit
//R113 - Points Added
//R114 - Audit Points Deducted
//R115 - Audit Sales
//R116 - Audit Purse Additions
//R117 - Audit Refunds
//R123 - Session Audit report
//R127 - Leavers Audit
//R129 - Audit Balance Import
//R137 - Redeem Bonus Audit
//R139 - Purse Credits
//R140 - Transaction Summary
//R141 - Transaction Summary Single
//$$******************************************************************
#define nTOTAL_CARD		0
#define nTOTAL_REPORT	1
//$$******************************************************************

CAuditReportLineInfo::CAuditReportLineInfo(CSQLAuditRecord* pAtc)
{
	m_bHaveColumnCash1 = FALSE;
	m_bHaveColumnPurse1 = FALSE;
	m_bHaveColumnPurse2 = FALSE;
	m_bHaveColumnPurse3 = FALSE;
	m_bHaveColumnPoints = FALSE;
	/*****/
	m_dCash1 = pAtc->GetTransactionCash();
	m_dPurse1Total = pAtc->GetTransactionPurse1Total();
	m_dPurse1Credit = pAtc->GetTransactionPurse1Credit();
	m_dPurse2 = pAtc->GetTransactionPurse2Balance();
	m_dPurse3Total = pAtc->GetTransactionPurse3Total();
	m_dPurse3Credit = pAtc->GetTransactionPurse3Credit();
	m_nPoints = pAtc->GetTransactionPoints();
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

CAuditReportColumnInfo::CAuditReportColumnInfo()
{
	m_nColumnType = AUDIT_COLUMN_CASH1;
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
	m_dTTLColumnCash1Added[nIndex] = 0.0;
	m_dTTLColumnCash1Subed[nIndex] = 0.0;

	m_dTTLColumnPurse1Added[nIndex] = 0.0;
	m_dTTLColumnPurse1Subed[nIndex] = 0.0;

	m_dTTLColumnPurse2Added[nIndex] = 0.0;
	m_dTTLColumnPurse2Subed[nIndex] = 0.0;

	m_dTTLColumnPurse3Added[nIndex] = 0.0;
	m_dTTLColumnPurse3Subed[nIndex] = 0.0;

	m_nTTLColumnPointsAdded[nIndex] = 0;
	m_nTTLColumnPointsSubed[nIndex] = 0;
}

//$$******************************************************************

int CAuditReport::CreateReport()
{
	if (m_Report.Open(m_ReportConfig.GetReportFilename()) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CSmartPayAuditFile auditFile(m_ReportConfig.GetAuditFilename());
	if (auditFile.Open() == FALSE)
	{
		m_Report.Close();
		return nREPORT_OPENERROR;
	}

	int nErrorNo = nREPORT_NOERROR;

	CWorkingDlg progress(szMSG_CREATINGREPORT);
	progress.Create();

	CString strTempCashHistoryFilename = "";					// just in case its needed

	if ((System.GetShowPurchaseTextsFlag() == TRUE) && (m_ReportConfig.GetShowEcrTextFlag() == TRUE) && (m_ReportConfig.GetInt64CardNoFrom() == 0))
	{
		if (::FileExists(Filenames.GetPurchaseHistoryFilename("0")) == TRUE)// see if have cash account history file "000000.dat"
		{
			strTempCashHistoryFilename = Filenames.GetTempCashHistoryFilename();	// temp cash account = "TMPn-xxxxxx.dat"

			CTLogFile tlog("0");													// cash account history file - will merge in any outstanding
			tlog.Extract(strTempCashHistoryFilename, m_ReportConfig.GetDateFrom().GetDate(), m_ReportConfig.GetTimeFrom().GetTime(), m_ReportConfig.GetDateTo().GetDate(), m_ReportConfig.GetTimeTo().GetTime());
		}
	}

	CString strTitle = m_ReportConfig.GetReportTitle();
	strTitle += m_ReportConfig.GetDateString();
	m_Report.SetStyle1(strTitle);

	AddColumnText();

	auditFile.JumpIn(m_ReportConfig.GetDateFrom().GetDate());

	nErrorNo = AuditReport(&auditFile, &progress);

	CFileRecycler::SendToRecycleBin(strTempCashHistoryFilename);		// delete temp cash history file if found

	progress.DestroyWindow();

	m_Report.Close();
	auditFile.Close();

	return nErrorNo;
}

//$$******************************************************************

int CAuditReport::CreateReportFromArray( CStringArray* pAuditArray )
{
	if (m_Report.Open(m_ReportConfig.GetReportFilename()) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CSmartPayAuditFile auditFile(pAuditArray);
	
	int nErrorNo = nREPORT_NOERROR;

	CWorkingDlg progress(szMSG_CREATINGREPORT);
	progress.Create();

	CString strTempCashHistoryFilename = "";					// just in case its needed

	if ((System.GetShowPurchaseTextsFlag() == TRUE) && (m_ReportConfig.GetShowEcrTextFlag() == TRUE) && (m_ReportConfig.GetInt64CardNoFrom() == 0))
	{
		if (::FileExists(Filenames.GetPurchaseHistoryFilename("0")) == TRUE)// see if have cash account history file "000000.dat"
		{
			strTempCashHistoryFilename = Filenames.GetTempCashHistoryFilename();	// temp cash account = "TMPn-xxxxxx.dat"

			CTLogFile tlog("0");													// cash account history file - will merge in any outstanding
			tlog.Extract(strTempCashHistoryFilename, m_ReportConfig.GetDateFrom().GetDate(), m_ReportConfig.GetTimeFrom().GetTime(), m_ReportConfig.GetDateTo().GetDate(), m_ReportConfig.GetTimeTo().GetTime());
		}
	}

	CString strTitle = m_ReportConfig.GetReportTitle();
	strTitle += m_ReportConfig.GetDateString();
	m_Report.SetStyle1(strTitle);

	AddColumnText();

	//auditFile.JumpIn(m_ReportConfig.GetDateFrom().GetDate());

	nErrorNo = AuditReport(&auditFile, &progress);

	CFileRecycler::SendToRecycleBin(strTempCashHistoryFilename);		// delete temp cash history file if found

	progress.DestroyWindow();

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
		m_Report.AddColumn("Location", TA_LEFT, 300);
		m_Report.AddColumn("UserID", TA_LEFT, 210);
		m_Report.AddColumn("Name", TA_LEFT, 300);
		break;

	default:
		m_Report.AddColumn("Audit Date", TA_LEFT, 210);
		m_Report.AddColumn("", TA_LEFT, 120);
		m_Report.AddColumn("Lookup", TA_LEFT, 200);
		m_Report.AddColumn("UserID", TA_LEFT, 210);
		m_Report.AddColumn("Name", TA_LEFT, 300);
		m_Report.AddColumn("Grp", TA_LEFT, 100);
		m_Report.AddColumn("Source", TA_LEFT, 230);
		m_Report.AddColumn("CCNo", TA_LEFT, 140);
		m_Report.AddColumn("OpNo", TA_LEFT, 140);
		m_Report.AddColumn("Pwd", TA_LEFT, 140);
		m_Report.AddColumn("Last Used", TA_LEFT, 190);
		m_Report.AddColumn("", TA_LEFT, 120);
		break;
	}
	
	ResetColumnInfo();

	bool bShowTotalColumns = FALSE;

	switch (nAuditType)
	{
	case nAUDIT_REVALUATIONBONUS:
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "CashTaken");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE2, "BonusCsh");
		AddColumnInfoTran(AUDIT_COLUMN_POINTS, "BonusPts");
		break;

	case nAUDIT_SPENDBONUS:
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "TrnValue");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE2, "BonusCsh");
		AddColumnInfoTran(AUDIT_COLUMN_POINTS, "BonusPts");
		break;

	case nAUDIT_REDEEMBONUS:
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "BonusP1");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE2, "BonusP2");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE3, "BonusP3", System.GetEnablePurse3Flag());
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
	case nAUDIT_PURSEADDITIONS:
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "Purse1");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE2, "Purse2");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE3, "Purse3", System.GetEnablePurse3Flag());
		break;

	case nAUDIT_PURSECREDIT:
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "Purse1");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE3, "Purse3", System.GetEnablePurse3Flag());
		break;

	case nAUDIT_LEAVERS:
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "Purse1");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE2, "Purse2");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE3, "Purse3", System.GetEnablePurse3Flag());
		AddColumnInfoTran(AUDIT_COLUMN_POINTS, "Points", System.GetActiveEnablePointsFlag());
		break;

	case nAUDIT_SUMMARY:
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "Purse1");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE2, "Purse2");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE3, "Purse3", System.GetEnablePurse3Flag());
		AddColumnInfoTran(AUDIT_COLUMN_POINTS, "Points", System.GetActiveEnablePointsFlag());
		AddColumnInfoTran(AUDIT_COLUMN_CASH1, "Cash");
		break;

	case nAUDIT_SALE:
	case nAUDIT_REFUND:
	default:
		AddColumnInfoTran(AUDIT_COLUMN_PURSE1, "Purse1");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE2, "Purse2");
		AddColumnInfoTran(AUDIT_COLUMN_PURSE3, "Purse3", System.GetEnablePurse3Flag());
		AddColumnInfoTran(AUDIT_COLUMN_POINTS, "Points", System.GetActiveEnablePointsFlag());
		AddColumnInfoTran(AUDIT_COLUMN_CASH1, "Cash");
		bShowTotalColumns = TRUE;
		break;
	}

	bool bShowCash = FALSE;
	for (int n = 0; n < m_arrayColumnInfoTran.GetSize(); n++)
	{
		CAuditReportColumnInfo info = m_arrayColumnInfoTran.GetAt(n);
		m_Report.AddColumn(info.m_strColumnName, TA_RIGHT, 230);

		if (AUDIT_COLUMN_CASH1 == info.m_nColumnType)
		{
			bShowCash = TRUE;
		}
	}

	if (nAUDIT_SUMMARY != nAuditType)
	{
		AddColumnInfoBalance(AUDIT_COLUMN_PURSE1, "P1.Ba1");
		AddColumnInfoBalance(AUDIT_COLUMN_PURSE2, "P2.Ba1");
		AddColumnInfoBalance(AUDIT_COLUMN_PURSE3, "P3.Ba1", System.GetEnablePurse3Flag());
		AddColumnInfoBalance(AUDIT_COLUMN_POINTS, "PtsBa1", System.GetActiveEnablePointsFlag());

		for (int n = 0; n < m_arrayColumnInfoBalance.GetSize(); n++)
		{
			CAuditReportColumnInfo info = m_arrayColumnInfoBalance.GetAt(n);
			m_Report.AddColumn(info.m_strColumnName, TA_RIGHT, 230);
		}

		if (TRUE == bShowTotalColumns)
		{
			AddColumnInfoSTD(AUDIT_COLUMN_PURSE1, "P1.STD");
			AddColumnInfoSTD(AUDIT_COLUMN_PURSE2, "P2.STD");
			AddColumnInfoSTD(AUDIT_COLUMN_PURSE3, "P3.STD", System.GetEnablePurse3Flag());
			AddColumnInfoSTD(AUDIT_COLUMN_POINTS, "Pts.TD", System.GetActiveEnablePointsFlag());
			AddColumnInfoSTD(AUDIT_COLUMN_CASH1, "Cash.STD", bShowCash);

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

int CAuditReport::AuditReport(CSmartPayAuditFile* pAudit, CWorkingDlg* pDlgProgress)
{
	int nResult = 0;

	g_GlobalState.BuildUserIDCache();
	m_TLogCache.Reset();
	
	if ((m_ReportConfig.GetSortByCardNoFlag() == TRUE) || (m_ReportConfig.GetSortByGroupFlag() == TRUE))
	{
		nResult = AuditReportByCardNo(pAudit, pDlgProgress);
	}
	else
	{
		nResult = AuditReportByDate(pAudit, pDlgProgress);
	}

	g_GlobalState.ClearUserIDCache();
	m_TLogCache.Reset();

	return nResult;
}

//$$******************************************************************

int CAuditReport::AuditReportByDate(CSmartPayAuditFile* pAudit, CWorkingDlg* pDlgProgress)
{
	int nCount = 0;
	int nProgressCount = 0;
	bool bCancelButton = FALSE;
	CString strLastDate = "";

	CSQLAuditRecord atc;
	while (pAudit->ReadLine(&atc) == TRUE)
	{
		if (( ++nProgressCount % 100) == 0)
		{
			CString strThisDate = atc.GetDateFiled();
			if (strThisDate != strLastDate)
			{
				CString strCaption = "Creating Report ";
				strCaption += strThisDate;
				pDlgProgress->SetCaption1(strCaption);
				strLastDate = strThisDate;
			}

			if (pDlgProgress->SetCaption2RecordsChecked(nProgressCount, TRUE) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}
		}

		if (IsValid(&atc) == TRUE)
		{
			bool bExists = g_GlobalState.CheckUserIDCache( _atoi64(atc.GetCardNo()));
	
			if (SaveLine(&atc, bExists) == TRUE)
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

int CAuditReport::AuditReportByCardNo(CSmartPayAuditFile* pAudit, CWorkingDlg* pDlgProgress)
{
	CReportMultiMap<CMapKeyAuditSort, CMapDataSortedInt> sorter;	
	CSQLAuditRecord atc;

	bool bCancelButton = FALSE;

	int nCount = 0;
	pDlgProgress->SetCaption1("Sorting lines for Audit");

	while (TRUE)
	{
		if ((nCount % 10) == 0)
		{
			if (pDlgProgress->SetCaption2RecordsChecked(nCount, TRUE) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}
		}

		nCount++;

		int nStartFilePos = pAudit->GetFilePos();			// position in file to start from
		if (pAudit->ReadLine(&atc) == FALSE)
		{
			break;
		}

		if (IsValid(&atc) == TRUE)
		{
			CMapKeyAuditSort Key;

			if (m_ReportConfig.GetSortByGroupFlag() == TRUE)
			{
				Key.m_nGroupNo = atc.GetGroupNo();
			}

			if (m_ReportConfig.GetSortByCardNoFlag() == TRUE)
			{
				Key.m_strCardNo = System.FormatCardNo(atc.GetCardNo());
			}

			CMapDataSortedInt Data;
			Data.m_nValue = nStartFilePos;

			sorter.Insert(Key, Data);	
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
	CMapKeyAuditSort LastKey(-1, "");

	pDlgProgress->SetCaption1("Creating Audit Report");

	LONGLONG dwStartTime = GetTickCount64();

	int nProgress = 0;
	bool bFirstLine = TRUE;
	
	for ( const auto pair : sorter )
	{
		// Get key and value from iterator
		const CMapKeyAuditSort& Key = pair.first;
		const CMapDataSortedInt& Data = pair.second;

		if ((nProgress % 100) == 0)
		{
			if (pDlgProgress->SetCaption2RecordsChecked(nProgress, TRUE) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}
		}
		nProgress++;

		pAudit->SetFilePos(Data.m_nValue);

		if (pAudit->ReadLine(&atc) == TRUE)
		{
			__int64 nUserID = _atoi64(atc.GetCardNo());
			bool bExists = g_GlobalState.CheckUserIDCache(nUserID);

			CString strUserID = FormatInt64Value(nUserID);

			// on first time round loop
			// save balance line if line is not a balance line
			if (TRUE == bFirstLine)
			{
				LastKey = Key;
				bFirstLine = FALSE;

				WriteHeaderLine(Key);

				if (m_ReportConfig.GetSortByGroupFlag() == FALSE)
				{
					SaveStartingBalanceLine(&atc, bExists);
				}
			}
			else
			{
				bool bNewBlock = FALSE;
				if (m_ReportConfig.GetSortByGroupFlag() == TRUE)
				{
					bNewBlock = (Key.m_nGroupNo != LastKey.m_nGroupNo);
				}
				else if (m_ReportConfig.GetSortByCardNoFlag() == TRUE)
				{
					bNewBlock = (Key.m_strCardNo != LastKey.m_strCardNo);
				}

				// change of main label
				if ( TRUE == bNewBlock )
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

						WriteHeaderLine(Key);
					}

					nLineCount = 0;
					LastKey = Key;
					ResetTotals(nTOTAL_CARD);

					// save balance line if line is not a balance line
					SaveStartingBalanceLine(&atc, bExists);
				}
			}

			if (SaveLine(&atc, bExists) == TRUE)
			{
				++nLineCount;
			}
		}
	}

	// single report
	if ( (m_ReportConfig.GetReportLabel() == "R111") || (m_ReportConfig.GetReportLabel() == "R123") || (m_ReportConfig.GetReportLabel() == "R141") )
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

void CAuditReport::AppendEODLineFields(CCSV& csv, CSQLAuditRecord* pAtc)
{
	csv.Add("");								// lookup type
	csv.Add("");								// card no
	csv.Add("");								// user name
	csv.Add("");								// Group
	csv.Add(CAuditReportTexts::GetReportSourceText(pAtc->GetSourceType(), pAtc->GetApplicationNo(), pAtc->GetTerminalNo(), pAtc->GetAuditLineExtraText()));
	csv.Add(pAtc->GetCCNoShort());
	csv.Add(pAtc->GetOperatorID());				// OperatorNo
	csv.Add(pAtc->GetReportPasswordNo());		// PasswordNo
	csv.Add("");								// DateLastUsed
	csv.Add("");								// TimeLastUsed
}

//$$******************************************************************

void CAuditReport::WriteHeaderLine(const CMapKeyAuditSort& Key)
{
	CString strHeader = "<..>";
	CString strCardNo = "";

	if (m_ReportConfig.GetSortByGroupFlag() == TRUE)
	{
		CString strGroupNo = "";
		strGroupNo.Format("G%2.2d, ", Key.m_nGroupNo);
		strHeader += strGroupNo;

		CString strName = "";

		CSQLRowGroupFull RowGroup;
		RowGroup.SetGroupNo(Key.m_nGroupNo);
		CSQLRepositoryGroup RepoGroup;
		if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			strName = RowGroup.GetGroupName();
		}

		TrimSpacesFromString(strName);

		if (strName == "")
		{
			strName.Format("Group %d", Key.m_nGroupNo);
		}

		strHeader += strName;
	}
	else
	{
		strHeader += "UserID ";
		strHeader += Key.m_strCardNo;
	}

	m_Report.WriteLine(strHeader);
	m_Report.WriteLine("<LI>");
}

//$$******************************************************************

bool CAuditReport::AppendAuditLineFields(CCSV& csv, CSQLAuditRecord* pAtc, bool bCardExists)
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

	CString strCCNoShort;
	strCCNoShort.Format("%4.4d", pAtc->GetCCNoShort());

	CString strOpNo = pAtc->GetOperatorID();
	if (strOpNo == "0")
	{
		strOpNo = "";
	}

	switch (m_ReportConfig.GetReportType())
	{
	case nAUDIT_SUMMARY:
		{
			CString strLocation = "";

			switch (pAtc->GetSourceType())
			{
			case AUDIT_PC:
				strLocation = "Back Office";
				break;

			case AUDIT_EXT:
				strLocation = "External";
				break;

			default:
				strLocation = Server.GetEposPathLabel(pAtc->GetFolderSet());
				break;
			}

			csv.Add(strLocation);
			csv.Add(strAccountNo);
			csv.Add(pAtc->GetUserName());
		}
		break;

	default:
		csv.Add(pAtc->GetREQFileTypeText());
		csv.Add(strAccountNo);
		csv.Add(pAtc->GetUserName());
		csv.Add(pAtc->GetGroupNo());
		csv.Add(CAuditReportTexts::GetReportSourceText(pAtc->GetSourceType(), pAtc->GetApplicationNo(), pAtc->GetTerminalNo(), pAtc->GetAuditLineExtraText()));
		csv.Add(strCCNoShort);
		csv.Add(strOpNo);
		csv.Add(pAtc->GetReportPasswordNo());
		csv.Add(pAtc->GetDateLastUsed()); // last used at till
		csv.Add(pAtc->GetTimeLastUsed());
		break;
	}
	
	return TRUE;
}

//$$******************************************************************

bool CAuditReport::SaveLine(CSQLAuditRecord* pAtc, bool bCardExists)
{
	int nAuditType = m_ReportConfig.GetReportType();

	bool bIsEODLine = (pAtc->GetSourceType() == AUDIT_EODBALANCE) ? TRUE : FALSE;

	CCSV csv('\t');
	csv.Add(pAtc->GetDateFiled());
	csv.Add(pAtc->GetTimeFiled());

	if (pAtc->GetSourceType() == AUDIT_EODBALANCE)
	{
		AppendEODLineFields(csv, pAtc);
	}
	else
	{
		if (AppendAuditLineFields(csv, pAtc, bCardExists) == FALSE)
		{
			return FALSE;
		}
	}

	CAuditReportLineInfo LineInfo(pAtc);

	switch (nAuditType)
	{
	case nAUDIT_CASHTAKEN:
	case nAUDIT_PURSEADDITIONS:
		LineInfo.EnablePurse1(TRUE);
		LineInfo.EnablePurse2(TRUE);
		LineInfo.EnablePurse3(System.GetEnablePurse3Flag());
		break;

	case nAUDIT_PURSECREDIT:
		LineInfo.EnablePurse1(TRUE);
		LineInfo.EnablePurse2(FALSE);
		LineInfo.EnablePurse3(System.GetEnablePurse3Flag());
		break;

	default:
		LineInfo.EnablePurse1(TRUE);
		LineInfo.EnablePurse2(TRUE);
		LineInfo.EnablePurse3(System.GetEnablePurse3Flag());
		LineInfo.EnablePoints(System.GetActiveEnablePointsFlag());

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
		case APPNO_DELETELEAVER:
		case APPNO_IMPORTHOTLIST:
		case APPNO_IMPORTDELETE:
		case APPNO_HOTLISTREPLACE:
		case APPNO_IMPORTREPLACE:
		case APPNO_BATCHLEAVER:
		case APPNO_IMPORTLEAVER:
		case APPNO_IMPORTCUSTOM:
		case APPNO_INSERTRECORDSIMS:
		case APPNO_INSERTRECORDPPAY:
		case APPNO_INSERTRECORDWISEPAY:
		case APPNO_WPOPENINGBALANCE:
		case APPNO_BIOREG:
		case APPNO_BIOUNREG:
			LineInfo.EnablePurse1(FALSE);
			LineInfo.EnablePurse2(FALSE);
			LineInfo.EnablePurse3(FALSE);
			LineInfo.EnablePoints(FALSE);
			break;

		case APPNO_ADDPOINTS:
			LineInfo.EnablePurse1(FALSE);
			LineInfo.EnablePurse2(FALSE);
			LineInfo.EnablePurse3(FALSE);
			break;

		case APPNO_ADDCASH:							// add cash to Purse1
		case APPNO_PARENTPAY:						// ParentPay import
		case APPNO_WISEPAY:							// WisePay import
		case APPNO_TUCASI:							// Tucasi(Soap) import
		case APPNO_TUCASIv2:						// Tucasi(Json) import
		case APPNO_SCHOOLCOMMS:						// Schoolcomms import
		case APPNO_SQUID:
		case APPNO_PARENTMAIL:
		case APPNO_BROMCOM:
		case APPNO_PEBBLE_TOPUP:
		case APPNO_PEBBLE_TOPUP_REFUND:
		case APPNO_PEBBLE_ORDER:
		case APPNO_PEBBLE_ORDER_REFUND:
			LineInfo.EnablePoints(FALSE);
			break;

		case APPNO_REVALUEBONUSCASH:
		case APPNO_REWARDBONUSCASH:
		case APPNO_REFUNDREWARDBONUSCASH:
			if (nAuditType == nAUDIT_REPORT)
			{
				LineInfo.EnablePurse2(FALSE);
				LineInfo.EnablePurse3(FALSE);
				LineInfo.EnablePoints(FALSE);
				LineInfo.m_dPurse1Total = LineInfo.m_dPurse2;	// bonus value saved in Purse2Transaction field
				LineInfo.m_dPurse2 = 0.0;							// clear dvalue2
			}
			break;

		case APPNO_REVALUEBONUSPOINTS:
		case APPNO_REWARDBONUSPOINTS:
		case APPNO_REFUNDREWARDBONUSPOINTS:
			if (nAuditType == nAUDIT_REPORT)
			{
				LineInfo.EnablePurse1(FALSE);
				LineInfo.EnablePurse2(FALSE);
				LineInfo.EnablePurse3(FALSE);
			}
			break;

		case APPNO_REFRESH:
			LineInfo.EnablePurse1(FALSE);
			LineInfo.EnablePurse3(FALSE);
			LineInfo.EnablePoints(FALSE);
			break;

		case APPNO_SALE:
		case APPNO_MEAL:
		case APPNO_REFUNDSALE:
			LineInfo.EnableCash1(TRUE);
			break;

		case APPNO_EDIT:
		case APPNO_REDEEM:
		case APPNO_BATCHREDEEM:
		case APPNO_XFERIN:
		case APPNO_XFEROUT:
		case APPNO_HOTXFEROUT:
		case APPNO_HOTXFERIN:
		case APPNO_IMPORTTOPUP:
			break;
		}
	}

	AppendTranTotals(csv, LineInfo, pAtc);
	AppendBalanceTotals(csv, pAtc);

	if (pAtc->IsDeleteAppNo())
	{
		AppendDeletionTotals(csv, pAtc);
	}
	
	if (FALSE == bIsEODLine)
	{
		AppendSTDTotals(csv, pAtc);
	}
	else
	{
		for (int n = 0; n < m_arrayColumnInfoSTD.GetSize(); n++)
		{
			csv.Add("");
		}
	}

	if (m_ReportConfig.GetReportType() == nAUDIT_SUMMARY)
	{
		csv.Add("");
	}
	
	csv.Add(GetCommentText(pAtc));

	return m_Report.WriteLine(csv);
}

//$$******************************************************************

void CAuditReport::AppendTranTotals(CCSV& csv, CAuditReportLineInfo& info, CSQLAuditRecord* pAtc)
{
	for (int n = 0; n < m_arrayColumnInfoTran.GetSize(); n++)
	{
		switch (m_arrayColumnInfoTran[n].m_nColumnType)
		{
		case AUDIT_COLUMN_CASH1:
			AppendLineTotals(csv, info.GotColumnCash1(), info.m_dCash1, m_dTTLColumnCash1Added, m_dTTLColumnCash1Subed);
			break;

		case AUDIT_COLUMN_PURSE1:
			if (m_ReportConfig.GetReportType() == nAUDIT_PURSECREDIT)
			{
				AppendLineTotals(csv, info.GotColumnPurse1(), info.m_dPurse1Credit, m_dTTLColumnPurse1Added, m_dTTLColumnPurse1Subed);
			}
			else
			{
				AppendLineTotals(csv, info.GotColumnPurse1(), info.m_dPurse1Total, m_dTTLColumnPurse1Added, m_dTTLColumnPurse1Subed);
			}
			break;

		case AUDIT_COLUMN_PURSE2:
			AppendLineTotals(csv, info.GotColumnPurse2(), info.m_dPurse2, m_dTTLColumnPurse2Added, m_dTTLColumnPurse2Subed);
			break;

		case AUDIT_COLUMN_PURSE3:
			if (m_ReportConfig.GetReportType() == nAUDIT_PURSECREDIT)
			{
				AppendLineTotals(csv, info.GotColumnPurse3(), info.m_dPurse3Credit, m_dTTLColumnPurse3Added, m_dTTLColumnPurse3Subed);
			}
			else
			{
				AppendLineTotals(csv, info.GotColumnPurse3(), info.m_dPurse3Total, m_dTTLColumnPurse3Added, m_dTTLColumnPurse3Subed);
			}
			break; 
			
		case AUDIT_COLUMN_POINTS:
			AppendLineTotals(csv, info.GotColumnPoints(), info.m_nPoints, m_nTTLColumnPointsAdded, m_nTTLColumnPointsSubed);
			break;
		}
	}
}

//$$******************************************************************

void CAuditReport::AppendDeletionTotals(CCSV& csv, CSQLAuditRecord* pAtc)
{
	for (int n = 0; n < m_arrayColumnInfoTran.GetSize(); n++)
	{
		switch (m_arrayColumnInfoTran[n].m_nColumnType)
		{
		case AUDIT_COLUMN_PURSE1:
			if (m_ReportConfig.GetReportType() == nAUDIT_PURSECREDIT)
			{
				AppendLineTotals(csv, TRUE, pAtc -> GetCurrentPurse1Credit() * (-1.0), m_dTTLColumnPurse1Added, m_dTTLColumnPurse1Subed);
			}
			else
			{
				AppendLineTotals(csv, TRUE, pAtc -> GetCurrentPurse1Total() * (-1.0), m_dTTLColumnPurse1Added, m_dTTLColumnPurse1Subed);
			}
			break;

		case AUDIT_COLUMN_PURSE2:
			AppendLineTotals(csv, TRUE, pAtc -> GetCurrentPurse2Balance() * (-1.0), m_dTTLColumnPurse2Added, m_dTTLColumnPurse2Subed);
			break;

		case AUDIT_COLUMN_PURSE3:
			if (m_ReportConfig.GetReportType() == nAUDIT_PURSECREDIT)
			{
				AppendLineTotals(csv, TRUE, pAtc -> GetCurrentPurse3Credit() * (-1.0), m_dTTLColumnPurse3Added, m_dTTLColumnPurse3Subed);
			}
			else
			{
				AppendLineTotals(csv, TRUE, pAtc -> GetCurrentPurse3Total() * (-1.0), m_dTTLColumnPurse3Added, m_dTTLColumnPurse3Subed);
			}
			break; 
			
		case AUDIT_COLUMN_POINTS:
			AppendLineTotals(csv, TRUE, pAtc -> GetCurrentPoints() * (-1), m_nTTLColumnPointsAdded, m_nTTLColumnPointsSubed);
			break;
		}
	}
}

//$$******************************************************************

void CAuditReport::AppendLineTotals(CCSV& csv, bool bGotTotal, double dValue, double(&TotalsAdd)[2], double(&TotalsSub)[2])
{
	if (TRUE == bGotTotal)
	{
		csv.Add(FormatDoubleValue(dValue));

		if (dValue < 0.0)
		{
			TotalsSub[nTOTAL_CARD] += dValue;
			TotalsSub[nTOTAL_REPORT] += dValue;
		}
		else
		{
			TotalsAdd[nTOTAL_CARD] += dValue;
			TotalsAdd[nTOTAL_REPORT] += dValue;
		}
	}
	else
	{
		csv.Add("");
	}
}

//$$******************************************************************

void CAuditReport::AppendLineTotals(CCSV& csv, bool bGotTotal, int nValue, int(&TotalsAdd)[2], int(&TotalsSub)[2])
{
	if (TRUE == bGotTotal)
	{
		csv.Add(FormatIntValue(nValue));

		if (nValue < 0)
		{
			TotalsSub[nTOTAL_CARD] += nValue;
			TotalsSub[nTOTAL_REPORT] += nValue;
		}
		else
		{
			TotalsAdd[nTOTAL_CARD] += nValue;
			TotalsAdd[nTOTAL_REPORT] += nValue;
		}
	}
	else
	{
		csv.Add("");
	}
}

//$$******************************************************************

void CAuditReport::AppendBalanceTotals(CCSV& csv, CSQLAuditRecord* pAtc)
{
	for (int n = 0; n < m_arrayColumnInfoBalance.GetSize(); n++)
	{
		switch (m_arrayColumnInfoBalance[n].m_nColumnType)
		{
		case AUDIT_COLUMN_PURSE1:
			csv.Add(pAtc->GetCurrentPurse1Total(), System.GetDPValue());
			break;

		case AUDIT_COLUMN_PURSE2:
			csv.Add(pAtc->GetCurrentPurse2Balance(), System.GetDPValue());
			break;

		case AUDIT_COLUMN_PURSE3:
			csv.Add(pAtc->GetCurrentPurse3Total(), System.GetDPValue());
			break;

		case AUDIT_COLUMN_POINTS:
			csv.Add(pAtc->GetCurrentPoints());
			break;
		}
	}
}

//$$******************************************************************

void CAuditReport::AppendSTDTotals(CCSV& csv, CSQLAuditRecord* pAtc)
{
	for (int n = 0; n < m_arrayColumnInfoSTD.GetSize(); n++)
	{
		switch (m_arrayColumnInfoSTD[n].m_nColumnType)
		{
		case AUDIT_COLUMN_CASH1:
			csv.Add(pAtc->GetToDateCashSpend(), System.GetDPValue());
			break;

		case AUDIT_COLUMN_PURSE1:
			csv.Add(pAtc->GetToDatePurse1Spend(), System.GetDPValue());
			break;

		case AUDIT_COLUMN_PURSE2:
			csv.Add(pAtc->GetToDatePurse2Spend(), System.GetDPValue());
			break;

		case AUDIT_COLUMN_PURSE3:
			csv.Add(pAtc->GetToDatePurse3Spend(), System.GetDPValue());
			break;

		case AUDIT_COLUMN_POINTS:
			csv.Add(pAtc->GetToDatePoints());
			break;
		}
	}
}

//$$******************************************************************

CString CAuditReport::GetCommentText(CSQLAuditRecord* pAtc)
{
	CString strComment = pAtc->GetLineOrDefaultComment();

	// see if ecr audit line
	switch (pAtc->GetSourceType())
	{
	case AUDIT_POS:
	case AUDIT_EXT:
		switch (pAtc->GetApplicationNo())
		{
		case APPNO_SALE:
		case APPNO_REFUNDSALE:
			if ((TRUE == System.GetShowPurchaseTextsFlag()) && (m_ReportConfig.GetShowEcrTextFlag() == TRUE))
			{
				bool bNewTLogDatabase = FALSE;

				CTLogDatabase* pDatabase = NULL;
				__int64 nUserID = _atoi64(pAtc->GetUserID());

				CSortedIntByInt64 item;
				item.m_nKey = nUserID;

				int nPos = 0;
				if (m_TLogCache.FindMapEntry(item, nPos) == TRUE)
				{
					m_TLogCache.GetMapEntry(nPos, item);
					pDatabase = m_TLogCache.GetTLogDatabase(item.m_nVal);
				}
				else
				{
					item.m_nVal = m_TLogCache.GetTLogDatabaseCount();
					m_TLogCache.InsertMapEntry(nPos, item);

					pDatabase = new (CTLogDatabase);
					m_TLogCache.AddTLogDatabase(pDatabase);
					bNewTLogDatabase = TRUE;
				}

				strComment = pAtc->ExtractPurchaseText(pDatabase, bNewTLogDatabase);
			}
			break;
		}
		break;
	}
	return strComment;
}

//$$***************************************************************

bool CAuditReport::SaveStartingBalanceLine(CSQLAuditRecord* pAtc, bool bCardExists)
{
	int nAuditType = m_ReportConfig.GetReportType();

	CString strCardNo = System.FormatCardNo(pAtc->GetCardNo());

	if (bCardExists == FALSE)						// see if account exists
	{
		if (m_ReportConfig.GetShowDeletedLinesFlag() == FALSE)	// see if show deleted lines set
			return FALSE;								// no - ignore line
		else
			strCardNo += " *";
	}

	//No starting balance on these report types
	switch (nAuditType)
	{
	case nAUDIT_CASHTAKEN:
	case nAUDIT_PURSEADDITIONS:
	case nAUDIT_PURSECREDIT:
	case nAUDIT_SUMMARY:
		return TRUE;
	}

	bool bShowStartingBalance = TRUE;

	switch (pAtc->GetApplicationNo())
	{
	case APPNO_DELETE:
	case APPNO_BATCHDELETE:
	case APPNO_HOTLIST:
	case APPNO_UNHOTLIST:
	case APPNO_INSERT:
	case APPNO_IMPORTINSERT:
	case APPNO_IMPORTUPDATE:
	case APPNO_DELETELEAVER:
	case APPNO_BATCHLEAVER:
	case APPNO_IMPORTLEAVER:
	case APPNO_HOTLISTREPLACE:
	case APPNO_IMPORTHOTLIST:
	case APPNO_IMPORTREPLACE:
	case APPNO_IMPORTDELETE:
	case APPNO_IMPORTCUSTOM:
	case APPNO_INSERTRECORDSIMS:
	case APPNO_INSERTRECORDPPAY:
	case APPNO_INSERTRECORDWISEPAY:
	case APPNO_WPOPENINGBALANCE:
	case APPNO_BIOREG:
	case APPNO_BIOUNREG:
		bShowStartingBalance = FALSE;
		break;

	case APPNO_ADDPOINTS:
	case APPNO_ADDCASH:
	case APPNO_EDIT:
	case APPNO_SALE:
	case APPNO_REVALUEBONUSCASH:
	case APPNO_REWARDBONUSCASH:
	case APPNO_REFUNDREWARDBONUSCASH:
	case APPNO_REVALUEBONUSPOINTS:
	case APPNO_REWARDBONUSPOINTS:
	case APPNO_REFUNDREWARDBONUSPOINTS:
	case APPNO_REFUNDSALE:
	case APPNO_XFEROUT:
	case APPNO_XFERIN:
	case APPNO_MEAL:
	case APPNO_HOTXFEROUT:
	case APPNO_HOTXFERIN:
	case APPNO_PARENTPAY:
	case APPNO_WISEPAY:
	case APPNO_TUCASI:
	case APPNO_TUCASIv2:
	case APPNO_SCHOOLCOMMS:
	case APPNO_SQUID:
	case APPNO_PARENTMAIL:
	case APPNO_BROMCOM:
	case APPNO_PEBBLE_TOPUP:
	case APPNO_PEBBLE_TOPUP_REFUND:
	case APPNO_PEBBLE_ORDER:
	case APPNO_PEBBLE_ORDER_REFUND:
	case APPNO_IMPORTTOPUP:
		break;
	}

	if (bShowStartingBalance == FALSE)
		return TRUE;

	CCSV csv('\t');
	csv.Add(pAtc->GetDateFiled());
	csv.Add(pAtc->GetTimeFiled());
	csv.Add("");										// lookup type
	csv.Add(strCardNo);
	csv.Add(pAtc->GetUserName());
	csv.Add(pAtc->GetGroupNo());
	csv.Add("<..>Starting Balance");					// source
	csv.Add("");										// operator
	csv.Add("");										// password
	csv.Add("");										// ccno
	csv.Add("");										// date last used
	csv.Add("");										// time last used
	csv.Add("");										// lookup type

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
			double dStartingPurseBalance = pAtc->GetCurrentPurse1Total() - pAtc->GetTransactionPurse1Total();
			csv.Add(dStartingPurseBalance, System.GetDPValue());
		}
		break;

		case AUDIT_COLUMN_PURSE2:
		{
			double dStartingPurseBalance = pAtc->GetCurrentPurse2Balance() - pAtc->GetTransactionPurse2Balance();
			csv.Add(dStartingPurseBalance, System.GetDPValue());
		}
		break;

		case AUDIT_COLUMN_PURSE3:
		{
			double dStartingPurseBalance = pAtc->GetCurrentPurse3Total() - pAtc->GetTransactionPurse3Total();
			csv.Add(dStartingPurseBalance, System.GetDPValue());
		}
		break;

		case AUDIT_COLUMN_POINTS:
		{
			int nPointsStartingBalance = pAtc->GetCurrentPoints() - pAtc->GetTransactionPoints();
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

	CWorkingDlg progress(szMSG_CREATINGREPORT);
	progress.Create();

	CString strTitle;
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
		int nCount = ScanArchive(nIndex, &progress);
		if (nCount == -1)
		{
			nErrorNo = nREPORT_CANCEL;
			break;
		}

		strBuf.Format("%s\t<..>( %d )", 
			(const char*) m_ReportConfig.SearchFilenamesGetItem(nIndex), 
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

int CAuditReport::ScanArchive(int nIndex, CWorkingDlg* pProgress)
{
	CSmartPayAuditFile auditFile(m_ReportConfig.SearchFilenamesGetPathname(nIndex));			// filename to scan yyyymmdd.atcstrPathname );
	if (auditFile.Open() == FALSE)
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

	CString strMsg;
	strMsg.Format("Scanning File %d of %d  :  %s",
		nFileNo,
		nFileCounter,
		(const char*)strFileLabel);

	pProgress->SetCaption1(strMsg);

	int nLineCount = 0;									// matching lines found
	bool bBalanceLineSaved = FALSE;

	bool bExists = FALSE;
	if (strCardNo != "")
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserIDString(strCardNo);
		CSQLRepositoryAccount RepoAccount;
		bExists = (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE);
	}

	int nRecordsChecked = 0;

	CSQLAuditRecord atc;
	while (auditFile.ReadLine(&atc) == TRUE)
	{
		nRecordsChecked++;
		if ((nRecordsChecked % 100) == 0)
		{
			if (pProgress->SetCaption2RecordsChecked(nRecordsChecked, TRUE) == TRUE)
			{
				nLineCount = -1;
				break;
			}
		}

		if (m_ReportConfig.GetSearchType() == 0)							// 0 = scan on account
		{
			if (atc.GetCardNo() == strCardNo)
			{
				if (bBalanceLineSaved == FALSE)
				{
					bBalanceLineSaved = SaveStartingBalanceLine(&atc);
				}

				if (SaveLine(&atc, bExists) == TRUE)			// save audit line
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
				strBuf = GetCommentText(&atc);
			}

			strBuf.MakeUpper();

			if (strBuf.Find(strSearchText, 0) != -1)		// see partial text found in name \ comment field
			{
				CString strCardNo = atc.GetCardNo();;
				
				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserIDString(strCardNo);

				CSQLRepositoryAccount RepoAccount;
				bExists = (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE);

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

bool CAuditReport::IsValid(CSQLAuditRecord* pAtc)
{
	//FIDDLE : DISCARD SERVER EDIT LINES CAUSED BY PREVIOUS BUG
	if (pAtc->GetSourceType() == AUDIT_SERVER)
	{
		if (pAtc->GetApplicationNo() == APPNO_EDIT)
		{
			return FALSE;
		}
	}

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
		if (FALSE == m_ReportConfig.GetContinousFlag())
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
		else
		{
			if (pAtc->m_dateFiled == m_ReportConfig.GetDateFrom())
			{
				if (pAtc->m_timeFiled < m_ReportConfig.GetTimeFrom())
				{
					return FALSE;
				}
			}

			if (pAtc->m_dateFiled == m_ReportConfig.GetDateTo())
			{
				if (pAtc->m_timeFiled > m_ReportConfig.GetTimeTo())
				{
					return FALSE;
				}
			}
		}
	}

	int nSourceType = pAtc->GetSourceType();
	int nAppNo = pAtc->GetApplicationNo();

	switch (nSourceType)
	{
	case AUDIT_SERVER:
		if (m_ReportConfig.GetShowServerLinesFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case AUDIT_POS:
		if (m_ReportConfig.GetShowPosLinesFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case AUDIT_EXT:
		if (m_ReportConfig.GetShowExternalLinesFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case AUDIT_PC:
		if (pAtc->IsWebPaymentAppNo() == TRUE)				//10/03/2014 - v1.05x 
		{
			if (m_ReportConfig.GetShowWebPaymentLinesFlag() == FALSE)
			{
				return FALSE;
			}
		}
		else
		{
			if (m_ReportConfig.GetShowPcLinesFlag() == FALSE)
			{
				return FALSE;
			}

			if (m_ReportConfig.GetPasswordFilter() != -1)
			{
				if ( pAtc -> GetPasswordNo() != m_ReportConfig.GetPasswordFilter() )
				{
					return FALSE;
				}
			}
		}
		break;

	case AUDIT_KIOSKLINK:
		if (m_ReportConfig.GetShowCardLinkLinesFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case AUDIT_VENDING:
		if (m_ReportConfig.GetShowVendingLinesFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case AUDIT_EODBALANCE:
		return (m_ReportConfig.GetShowEODLinesFlag()); 
		break;
	}

	switch (pAtc->GetREQFileType())
	{
	case REQFILETYPE_CARDSCAN:
		if (m_ReportConfig.GetShowLookupScanFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case REQFILETYPE_BIO:
		if (m_ReportConfig.GetShowLookupBioFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case REQFILETYPE_MANUAL:
		if (m_ReportConfig.GetShowLookupManualFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case REQFILETYPE_NONE:
	case REQFILETYPE_UNKNOWN:
		if (m_ReportConfig.GetShowLookupUnknownFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case REQFILETYPE_DEFAULT: 
		if (m_ReportConfig.GetShowLookupDefaultFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case REQFILETYPE_INTERNAL:
		if (m_ReportConfig.GetShowLookupInternalFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case REQFILETYPE_PIN:
		if (m_ReportConfig.GetShowLookupPINFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	default:
		if (m_ReportConfig.GetShowLookupOtherFlag() == FALSE)
		{
			return FALSE;
		}
		break;
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
		if (m_ReportConfig.CheckTerminalFilter(pAtc->GetFolderSet(), pAtc->GetTerminalNo()) == FALSE)
		{
			return FALSE;
		}
		break;
	}

	// check on type of report required
	bool bReply = FALSE;

	switch (m_ReportConfig.GetReportType())
	{
	case nAUDIT_REPORT:
	case nAUDIT_LEAVERS:
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
		case APPNO_EDIT:
		case APPNO_ADDCASH:
		case APPNO_IMPORTTOPUP:
		case APPNO_PARENTPAY:
		case APPNO_WISEPAY:
		case APPNO_TUCASI:
		case APPNO_TUCASIv2:
		case APPNO_SCHOOLCOMMS:
		case APPNO_SQUID:
		case APPNO_PARENTMAIL:
		case APPNO_BROMCOM:
		case APPNO_PEBBLE_TOPUP:
		case APPNO_PEBBLE_TOPUP_REFUND:
			if (pAtc->GetTransactionPurse1Total() != 0.0 || pAtc->GetTransactionPurse2Balance() != 0.0 || pAtc->GetTransactionPurse3Total() != 0.0)// see if any transcation value
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
		case APPNO_ADDCASH:
		case APPNO_IMPORTTOPUP:
		case APPNO_PARENTPAY:
		case APPNO_WISEPAY:
		case APPNO_TUCASI:
		case APPNO_TUCASIv2:
		case APPNO_SCHOOLCOMMS:
		case APPNO_SQUID:
		case APPNO_PARENTMAIL:
		case APPNO_BROMCOM:
		case APPNO_PEBBLE_TOPUP:
		case APPNO_PEBBLE_TOPUP_REFUND:
			if ((pAtc->GetTransactionPurse1Total() != 0.0) || (pAtc->GetTransactionPurse2Balance() != 0.0) || (pAtc->GetTransactionPurse3Total() != 0.0))
			{
				bReply = TRUE;
			}
			break;

		case APPNO_REFRESH:
			if (pAtc->GetTransactionPurse2Balance() != 0.0)			// see if Purse2 refreshed
			{
				bReply = TRUE;
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

	case nAUDIT_PURSECREDIT:
		if ( ( pAtc -> GetTransactionPurse1Credit() != 0.0 ) || (pAtc->GetTransactionPurse3Credit() != 0.0) )
		{ 
			bReply = TRUE;
		}
		break;

	case nAUDIT_POINTSAUDIT:
		if (pAtc->GetTransactionPoints() != 0)
		{
			bReply = TRUE;
		}
		break;

	case nAUDIT_POINTSADDED:
		if (pAtc->GetTransactionPoints() > 0)
		{
			bReply = TRUE;
		}
		break;

	case nAUDIT_POINTSDEDUCTED:
		if (pAtc->GetTransactionPoints() < 0)
		{
			bReply = TRUE;
		}
		break;

	case nAUDIT_SALE:
		if ((APPNO_SALE == nAppNo) || (APPNO_MEAL == nAppNo))
		{
			if (pAtc->GetTransactionTotal() != 0.0)
			{
				bReply = TRUE;
			}
		}
		break;

	case nAUDIT_REFUND:
		if (APPNO_REFUNDSALE == nAppNo)
		{
			bReply = TRUE;
		}
		break;

	case nAUDIT_SUMMARY:
		if ((pAtc->GetTransactionPurse1Total() != 0.0) 
			|| (pAtc->GetTransactionPoints() != 0) 
			|| (pAtc->GetTransactionPurse2Balance() != 0)
			|| (pAtc->GetTransactionPurse3Total() != 0)
			|| (pAtc->GetTransactionCash() != 0))
		{
			bReply = TRUE;
		}
		break;
	}

	return bReply;
}

//$$******************************************************************

bool CAuditReport::IsCardInRange(CSQLAuditRecord* pAtc)
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
		nSkip = 5;
	}
	else
	{
		nSkip = 12;
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
		case AUDIT_COLUMN_CASH1:
			csvAdded.Add(::FormatDoubleValue(m_dTTLColumnCash1Added[nIndex]));
			csvSubed.Add(::FormatDoubleValue(m_dTTLColumnCash1Subed[nIndex]));
			strTabLine += "<LI>\t";
			break;

		case AUDIT_COLUMN_PURSE1:
			csvAdded.Add(::FormatDoubleValue(m_dTTLColumnPurse1Added[nIndex]));
			csvSubed.Add(::FormatDoubleValue(m_dTTLColumnPurse1Subed[nIndex]));
			strTabLine += "<LI>\t";
			break;

		case AUDIT_COLUMN_PURSE2:
			csvAdded.Add(::FormatDoubleValue(m_dTTLColumnPurse2Added[nIndex]));
			csvSubed.Add(::FormatDoubleValue(m_dTTLColumnPurse2Subed[nIndex]));
			strTabLine += "<LI>\t";
			break;

		case AUDIT_COLUMN_PURSE3:
			csvAdded.Add(::FormatDoubleValue(m_dTTLColumnPurse3Added[nIndex]));
			csvSubed.Add(::FormatDoubleValue(m_dTTLColumnPurse3Subed[nIndex]));
			strTabLine += "<LI>\t";
			break;

		case AUDIT_COLUMN_POINTS:
			csvAdded.Add(::FormatIntValue(m_nTTLColumnPointsAdded[nIndex]));
			csvSubed.Add(::FormatIntValue(m_nTTLColumnPointsSubed[nIndex]));
			strTabLine += "<LI>\t";
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

	if (nIndex == nTOTAL_CARD)
	{
		m_Report.WriteLine("");
	}
}

//$$******************************************************************

