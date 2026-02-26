/******************************************************************************/
#include "DateRangeDlg.h"
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
#include "ReportInfoDateRange.h"
#include "WorkingDlg.h"
/******************************************************************************/
#include "ReportDailyDifference.h"
/******************************************************************************/

CReportDailyDifference::CReportDailyDifference(CWnd* pParent)
{
	m_pParent = pParent;
	m_strReportLabel = "R116";
	m_strCaption = "Daily Differ";
	m_bShowDetail = TRUE;
	ResetAllTotals();
}

/**********************************************************************/

CReportDailyDifference::~CReportDailyDifference()
{
}

/**********************************************************************/

void CReportDailyDifference::ResetAllTotals()
{
	m_dPurse1TotalAddTopup = 0.0;
	m_dPurse1TotalAddOther = 0.0;
	m_dPurse1TotalSubSpend = 0.0;
	m_dPurse1TotalSubOther = 0.0;
	m_dPurse2TotalAddTopup = 0.0;
	m_dPurse2TotalAddOther = 0.0;
	m_dPurse2TotalSubSpend = 0.0;
	m_dPurse2TotalSubOther = 0.0;
	m_nPurse1TotalAddTopupCount = 0;
	m_nPurse1TotalAddOtherCount = 0;
	m_nPurse1TotalSubSpendCount = 0;
	m_nPurse1TotalSubOtherCount = 0;
	m_nPurse2TotalAddTopupCount = 0;
	m_nPurse2TotalAddOtherCount = 0;
	m_nPurse2TotalSubSpendCount = 0;
	m_nPurse2TotalSubOtherCount = 0;
	ResetLineTotals();
}

/**********************************************************************/

void CReportDailyDifference::ResetLineTotals()
{
	for (int a = 0; a < CASHLESS_ACTION_CNT; a++)
	{
		m_arrayLineTotals[a].Reset();
	}
}

/******************************************************************************/

void CReportDailyDifference::Show()
{
	CRepmanHandler repman(m_strCaption, m_pParent);

	int nErrorNo = m_auditFile.ValidateFile();				// check main audit file ok to report on
	if (nErrorNo == nREPORT_NOERROR)						// audit file ok to report on
	{
		CString strReportFile = Filenames.GetReportFilename(m_strReportLabel);
		CString strParams = Filenames.GetReportParamsFilename(m_strReportLabel);

		do
		{
			ResetAllTotals();
			if ((nErrorNo = Create(strReportFile)) == nREPORT_NOERROR)
			{
				nErrorNo = repman.DisplayReport(strReportFile, strParams);
				repman.DisplayError(nErrorNo);
			}
		} 
		while (nErrorNo != nREPORT_CANCEL);
	}

	repman.DisplayError(nErrorNo);
}

//*******************************************************************

int CReportDailyDifference::Create(CString strReportFile)
{
	CReportInfoDateRange ReportInfo(m_strReportLabel, m_strCaption);
	CDateRangeDlg dateRange(m_strReportLabel, m_pParent);
	dateRange.SetReportInfo(&ReportInfo);
	
	if (dateRange.DoModal() == IDCANCEL)
	{
		return nREPORT_CANCEL;
	}

	m_bShowDetail = (ReportInfo.GetSummaryFlag() == FALSE);

	if (m_Report.Open(strReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	if (m_auditFile.OpenReadOnly() == FALSE)						// open main audit file
	{
		return nREPORT_OPENERROR;
	}

	m_auditFile.JumpIn(ReportInfo.GetDateFromString());

	if (m_auditFile.ReadLine(&m_atc) == FALSE)
	{
		m_auditFile.Close();
		return nREPORT_NOMATCH;
	}

	CWorkingDlg progress("Creating report");
	progress.Create();
	progress.SetCaption1("Scanning Audit");

	ResetLineTotals();
	
	CString strTitle = "";
	strTitle.Format("%s%s",
		(const char*)m_strCaption,
		ReportInfo.GetDateRangeString());

	m_Report.SetStyle1(strTitle);
	AddColumnText();

	int nCount = 0;
	CString strLastDate = "";
	bool bCancelButton = FALSE;

	int nChecked = 0;
	while (m_auditFile.ReadLine(&m_atc) == TRUE)
	{
		if ( progress.IncrementRecordsChecked(10, TRUE) == TRUE )
		{
			bCancelButton = TRUE;
			break;
		}

		if (ReportInfo.IsValid(m_atc.GetDateFiled(), m_atc.GetTimeFiled()) == TRUE) // see if date in range
		{
			if (strLastDate != m_atc.GetDateFiled())
			{	
				if (strLastDate != "")
				{
					CreateReportLine(strLastDate);				
				}
				strLastDate = m_atc.GetDateFiled();
				ResetLineTotals();
			}

			ProcessLine();
		}
	}

	if (strLastDate != "")
	{
		CreateReportLine(strLastDate);
	}

	CreateTotalsLine();

	m_Report.Close();
	m_auditFile.Close();

	progress.DestroyWindow();

	if (bCancelButton == TRUE)
	{
		return nREPORT_CANCEL;
	}

	return nREPORT_NOERROR;
}

//*****************************************************************************

void CReportDailyDifference::ProcessLine() 
{
	int nAppNo = m_atc.GetApplicationNo();

	switch ( m_atc.GetSourceType() )
	{
	case AUDIT_SERVER:		
	case AUDIT_PC:			
	case AUDIT_POS:
	case AUDIT_EXT:
		break;

	default:
		return;
	}

	double dPurse1Amount = m_atc.GetPurse1Transaction();
	double dPurse2Amount = m_atc.GetPurse2Transaction();

	switch (m_atc.GetApplicationNo())
	{
	case APPNO_SALE:
		ConsolidateMinus(CASHLESS_ACTION_SALE, 1, dPurse1Amount);
		ConsolidateMinus(CASHLESS_ACTION_SALE, 2, dPurse2Amount);
		break;

	case APPNO_EDIT:
	case APPNO_BATCHUPDATE:
		Consolidate(CASHLESS_ACTION_ADJUST, 1, dPurse1Amount);
		Consolidate(CASHLESS_ACTION_ADJUST, 2, dPurse2Amount);
		break;

	case APPNO_SUBCASH:
	case APPNO_ADDGIFT:
	case APPNO_ADDGIFTX:
		Consolidate(CASHLESS_ACTION_TOPUP, 1, dPurse1Amount);
		Consolidate(CASHLESS_ACTION_TOPUP, 2, dPurse2Amount);
		break;

	case APPNO_REVALUEBONUSCASH:
		Consolidate(CASHLESS_ACTION_TOPUPBONUS, 1, dPurse1Amount);
		Consolidate(CASHLESS_ACTION_TOPUPBONUS, 2, dPurse2Amount);
		break;

	case APPNO_REDEEM:
	case APPNO_BATCHREDEEM:
		ConsolidatePlus(CASHLESS_ACTION_REDEEM, 1, dPurse1Amount);
		ConsolidatePlus(CASHLESS_ACTION_REDEEM, 2, dPurse2Amount);
		break;

	case APPNO_REWARDBONUSCASH:
		ConsolidatePlus(CASHLESS_ACTION_SPENDBONUS, 1, dPurse1Amount);
		ConsolidatePlus(CASHLESS_ACTION_SPENDBONUS, 2, dPurse2Amount);
		break;

	case APPNO_REFUNDREWARDBONUSCASH:
		ConsolidateMinus(CASHLESS_ACTION_SPENDBONUS, 1, dPurse1Amount);
		ConsolidateMinus(CASHLESS_ACTION_SPENDBONUS, 2, dPurse2Amount);
		break;

	case APPNO_MANUAL_SPEND:
		ConsolidateMinus(CASHLESS_ACTION_MANUALSPEND, 1, dPurse1Amount);
		break;

	case APPNO_MANUAL_REFUND:
		ConsolidatePlus(CASHLESS_ACTION_MANUALSPEND, 1, dPurse1Amount);
		break;

	case APPNO_REFRESH:
		ConsolidatePlus(CASHLESS_ACTION_REFRESH, 2, dPurse2Amount);
		break;

	case APPNO_DEPTBONUSCASH:
		ConsolidatePlus(CASHLESS_ACTION_DEPTBONUS, 1, dPurse1Amount);
		ConsolidatePlus(CASHLESS_ACTION_DEPTBONUS, 2, dPurse2Amount);
		break;

	case APPNO_REFUNDDEPTBONUSCASH:
		ConsolidateMinus(CASHLESS_ACTION_DEPTBONUS, 1, dPurse1Amount);
		ConsolidateMinus(CASHLESS_ACTION_DEPTBONUS, 2, dPurse2Amount);
		break;

	case APPNO_PURSE1_MANTOPUP:
	case APPNO_PURSE2_MANTOPUP:
	case APPNO_PURSE1_IMPTOPUP:
	case APPNO_PURSE2_IMPTOPUP:
		Consolidate(CASHLESS_ACTION_TOPUP, 1, dPurse1Amount);
		Consolidate(CASHLESS_ACTION_TOPUP, 2, dPurse2Amount);
		break;

	default:
		{
			int nPurseNo = 0;
			int nTopupNo = 0;

			CAuditReportTexts::GetAddCashIndexByAppNo(nAppNo, nPurseNo, nTopupNo);
			if (( 1 == nPurseNo ) || ( 2 == nPurseNo ))
			{
				Consolidate(CASHLESS_ACTION_TOPUP, 1, dPurse1Amount);
				Consolidate(CASHLESS_ACTION_TOPUP, 2, dPurse2Amount);
			}
		}
		break;
	}
}

//*******************************************************************

void CReportDailyDifference::Consolidate(int nAction, int nPurseNo, double dAmount)
{
	if (nAction < 0 || nAction >= CASHLESS_ACTION_CNT)
	{
		return;
	}

	m_arrayLineTotals[nAction].Consolidate(nPurseNo, dAmount);
}

//*******************************************************************

void CReportDailyDifference::ConsolidatePlus(int nAction, int nPurseNo, double dAmount)
{
	if (nAction < 0 || nAction >= CASHLESS_ACTION_CNT)
	{
		return;
	}

	m_arrayLineTotals[nAction].ConsolidatePlus(nPurseNo, dAmount);
}

//*******************************************************************

void CReportDailyDifference::ConsolidateMinus(int nAction, int nPurseNo, double dAmount)
{
	if (nAction < 0 || nAction >= CASHLESS_ACTION_CNT)
	{
		return;
	}

	m_arrayLineTotals[nAction].ConsolidateMinus(nPurseNo, dAmount);
}

//*******************************************************************

void CReportDailyDifference::AddColumnText()
{
	m_Report.AddColumn("Date", TA_LEFT, 190);
	m_Report.AddColumn("", TA_RIGHT, 120);
	
	if (TRUE == m_bShowDetail)
	{
		m_Report.AddColumn("P1Topup", TA_RIGHT, 210);
		m_Report.AddColumn("", TA_RIGHT, 120);
		m_Report.AddColumn("P1Other+", TA_RIGHT, 210);
		m_Report.AddColumn("", TA_RIGHT, 120);
		m_Report.AddColumn("P1Spend", TA_RIGHT, 200);
		m_Report.AddColumn("", TA_RIGHT, 120);
		m_Report.AddColumn("P1Other-", TA_RIGHT, 210);
		m_Report.AddColumn("", TA_RIGHT, 120);
	}
	
	m_Report.AddColumn("P1Total+", TA_RIGHT, 210);
	m_Report.AddColumn("", TA_RIGHT, 120);
	m_Report.AddColumn("P1Total-", TA_RIGHT, 210);
	m_Report.AddColumn("P1Differ", TA_RIGHT, 250);

	if (System.GetEnablePurse2Flag() == TRUE)
	{
		m_Report.AddColumn("", TA_RIGHT, 30);				// separator
		m_Report.AddColumn("", TA_RIGHT, 120);
		
		if (TRUE == m_bShowDetail)
		{
			m_Report.AddColumn("P2Topup", TA_RIGHT, 210);
			m_Report.AddColumn("", TA_RIGHT, 120);
			m_Report.AddColumn("P2Other+", TA_RIGHT, 210);
			m_Report.AddColumn("", TA_RIGHT, 120);
			m_Report.AddColumn("P2Spend", TA_RIGHT, 200);
			m_Report.AddColumn("", TA_RIGHT, 120);
			m_Report.AddColumn("P2Other-", TA_RIGHT, 210);
			m_Report.AddColumn("", TA_RIGHT, 120);
		}

		m_Report.AddColumn("P2Total+", TA_RIGHT, 210);
		m_Report.AddColumn("", TA_RIGHT, 120);
		m_Report.AddColumn("P2Total-", TA_RIGHT, 210);
		m_Report.AddColumn("P2Differ", TA_RIGHT, 250);
	}
}

//*******************************************************************

void CReportDailyDifference::CreateReportLine(CString strDate)
{
	double dPurse1AddTopup = 0.0;
	double dPurse1AddOther = 0.0;
	double dPurse1SubSpend = 0.0;
	double dPurse1SubOther = 0.0;
	double dPurse2AddTopup = 0.0;
	double dPurse2AddOther = 0.0;
	double dPurse2SubSpend = 0.0;
	double dPurse2SubOther = 0.0;
	int nPurse1AddTopupCount = 0;
	int nPurse1AddOtherCount = 0;
	int nPurse1SubSpendCount = 0;
	int nPurse1SubOtherCount = 0;
	int nPurse2AddTopupCount = 0;
	int nPurse2AddOtherCount = 0;
	int nPurse2SubSpendCount = 0;
	int nPurse2SubOtherCount = 0;

	for (int a = 0; a < CASHLESS_ACTION_CNT; a++)
	{
		switch (a)
		{
		case CASHLESS_ACTION_SALE:
		case CASHLESS_ACTION_MANUALSPEND:
			dPurse1SubSpend += m_arrayLineTotals[a].m_dPurse1Minus;
			dPurse2SubSpend += m_arrayLineTotals[a].m_dPurse2Minus;
			dPurse1SubSpend -= m_arrayLineTotals[a].m_dPurse1Plus;
			dPurse2SubSpend -= m_arrayLineTotals[a].m_dPurse2Plus;
			nPurse1SubSpendCount += m_arrayLineTotals[a].m_nPurse1MinusCount;
			nPurse2SubSpendCount += m_arrayLineTotals[a].m_nPurse2MinusCount;
			break;

		case CASHLESS_ACTION_TOPUP:
			dPurse1AddTopup += m_arrayLineTotals[a].m_dPurse1Plus;
			dPurse2AddTopup += m_arrayLineTotals[a].m_dPurse2Plus;
			dPurse1AddTopup -= m_arrayLineTotals[a].m_dPurse1Minus;
			dPurse2AddTopup -= m_arrayLineTotals[a].m_dPurse2Minus;
			nPurse1AddTopupCount += m_arrayLineTotals[a].m_nPurse1PlusCount;
			nPurse2AddTopupCount += m_arrayLineTotals[a].m_nPurse2PlusCount;
			break;

		case CASHLESS_ACTION_TOPUPBONUS:
		case CASHLESS_ACTION_DEPTBONUS:
		case CASHLESS_ACTION_SPENDBONUS:
		case CASHLESS_ACTION_REDEEM:
		case CASHLESS_ACTION_REFRESH:
		case CASHLESS_ACTION_ADJUST:
			dPurse1AddOther += m_arrayLineTotals[a].m_dPurse1Plus;
			dPurse2AddOther += m_arrayLineTotals[a].m_dPurse2Plus;
			dPurse1SubOther += m_arrayLineTotals[a].m_dPurse1Minus;
			dPurse2SubOther += m_arrayLineTotals[a].m_dPurse2Minus;
			nPurse1AddOtherCount += m_arrayLineTotals[a].m_nPurse1PlusCount;
			nPurse2AddOtherCount += m_arrayLineTotals[a].m_nPurse2PlusCount;
			nPurse1SubOtherCount += m_arrayLineTotals[a].m_nPurse1MinusCount;
			nPurse2SubOtherCount += m_arrayLineTotals[a].m_nPurse2MinusCount;
			break;

		default:
			break;
		}
	}

	double dPurse1TotalPlus = dPurse1AddTopup + dPurse1AddOther;
	double dPurse1TotalMinus = dPurse1SubSpend + dPurse1SubOther;
	double dPurse2TotalPlus = dPurse2AddTopup + dPurse2AddOther;
	double dPurse2TotalMinus = dPurse2SubSpend + dPurse2SubOther;


	CCSV csv('\t');
	csv.Add(strDate);
	
	if (TRUE == m_bShowDetail)
	{
		csv.Add(nPurse1AddTopupCount);
		csv.Add(dPurse1AddTopup, System.GetDPValue());
		csv.Add(nPurse1AddOtherCount);
		csv.Add(dPurse1AddOther, System.GetDPValue());
		csv.Add(nPurse1SubSpendCount);
		csv.Add(dPurse1SubSpend, System.GetDPValue());
		csv.Add(nPurse1SubOtherCount);
		csv.Add(dPurse1SubOther, System.GetDPValue());
	}
	csv.Add(nPurse1AddTopupCount + nPurse1AddOtherCount);
	csv.Add(dPurse1TotalPlus, System.GetDPValue());
	csv.Add(nPurse1SubSpendCount + nPurse1SubOtherCount);
	csv.Add(dPurse1TotalMinus, System.GetDPValue());
	csv.Add(::SignedFormatDoubleValue(dPurse1TotalPlus - dPurse1TotalMinus));
	csv.Add("");
	
	if (TRUE == m_bShowDetail)
	{
		csv.Add(nPurse2AddTopupCount);
		csv.Add(dPurse2AddTopup, System.GetDPValue());
		csv.Add(nPurse2AddOtherCount);
		csv.Add(dPurse2AddOther, System.GetDPValue());
		csv.Add(nPurse2SubSpendCount);
		csv.Add(dPurse2SubSpend, System.GetDPValue());
		csv.Add(nPurse2SubOtherCount);
		csv.Add(dPurse2SubOther, System.GetDPValue());
	}

	csv.Add(nPurse2AddTopupCount + nPurse2AddOtherCount);
	csv.Add(dPurse2TotalPlus, System.GetDPValue());
	csv.Add(nPurse2SubSpendCount + nPurse2SubOtherCount);
	csv.Add(dPurse2TotalMinus, System.GetDPValue());
	csv.Add(::SignedFormatDoubleValue(dPurse2TotalPlus - dPurse2TotalMinus));
	
	m_Report.WriteLine(csv.GetLine());

	m_dPurse1TotalAddTopup += dPurse1AddTopup;
	m_dPurse1TotalAddOther += dPurse1AddOther;
	m_dPurse1TotalSubSpend += dPurse1SubSpend;
	m_dPurse1TotalSubOther += dPurse1SubOther;
	m_dPurse2TotalAddTopup += dPurse2AddTopup;
	m_dPurse2TotalAddOther += dPurse2AddOther;
	m_dPurse2TotalSubSpend += dPurse2SubSpend;
	m_dPurse2TotalSubOther += dPurse2SubOther;
	m_nPurse1TotalAddTopupCount += nPurse1AddTopupCount;
	m_nPurse1TotalAddOtherCount += nPurse1AddOtherCount;
	m_nPurse1TotalSubSpendCount += nPurse1SubSpendCount;
	m_nPurse1TotalSubOtherCount += nPurse1SubOtherCount;
	m_nPurse2TotalAddTopupCount += nPurse2AddTopupCount;
	m_nPurse2TotalAddOtherCount += nPurse2AddOtherCount;
	m_nPurse2TotalSubSpendCount += nPurse2SubSpendCount;
	m_nPurse2TotalSubOtherCount += nPurse2SubOtherCount;
}

//*******************************************************************

void CReportDailyDifference::CreateTotalsLine()
{
	{
		CCSV csv('\t');
		csv.Add("");
		csv.Add("");

		if (TRUE == m_bShowDetail)
		{
			csv.Add("<LI>");
			csv.Add("");
			csv.Add("<LI>");
			csv.Add("");
			csv.Add("<LI>");
			csv.Add("");
			csv.Add("<LI>");
			csv.Add("");
		}

		csv.Add("<LI>");
		csv.Add("");
		csv.Add("<LI>");
		csv.Add("<LI>");
		csv.Add("");
		csv.Add("");
		
		if (TRUE == m_bShowDetail)
		{
			csv.Add("<LI>");
			csv.Add("");
			csv.Add("<LI>");
			csv.Add("");
			csv.Add("<LI>");
			csv.Add("");
			csv.Add("<LI>");
			csv.Add("");
		}

		csv.Add("<LI>");
		csv.Add("");
		csv.Add("<LI>");
		csv.Add("<LI>");
		m_Report.WriteLine(csv.GetLine());
	}

	{
		double dPurse1TotalPlus = m_dPurse1TotalAddTopup + m_dPurse1TotalAddOther;
		double dPurse1TotalMinus = m_dPurse1TotalSubSpend + m_dPurse1TotalSubOther;
		double dPurse2TotalPlus = m_dPurse2TotalAddTopup + m_dPurse2TotalAddOther;
		double dPurse2TotalMinus = m_dPurse2TotalSubSpend + m_dPurse2TotalSubOther;

		CCSV csv('\t');
		csv.Add("<..>Totals");
		
		if (TRUE == m_bShowDetail)
		{
			csv.Add(m_nPurse1TotalAddTopupCount);
			csv.Add(m_dPurse1TotalAddTopup, System.GetDPValue());
			csv.Add(m_nPurse1TotalAddOtherCount);
			csv.Add(m_dPurse1TotalAddOther, System.GetDPValue());
			csv.Add(m_nPurse1TotalSubSpendCount);
			csv.Add(m_dPurse1TotalSubSpend, System.GetDPValue());
			csv.Add(m_nPurse1TotalSubOtherCount);
			csv.Add(m_dPurse1TotalSubOther, System.GetDPValue());
		}

		csv.Add(m_nPurse1TotalAddTopupCount + m_nPurse1TotalAddOtherCount);
		csv.Add(dPurse1TotalPlus, System.GetDPValue());
		csv.Add(m_nPurse1TotalSubSpendCount + m_nPurse1TotalSubOtherCount);
		csv.Add(dPurse1TotalMinus, System.GetDPValue());
		csv.Add(::SignedFormatDoubleValue(dPurse1TotalPlus - dPurse1TotalMinus));
		csv.Add("");
		
		if (TRUE == m_bShowDetail)
		{
			csv.Add(m_nPurse2TotalAddTopupCount);
			csv.Add(m_dPurse2TotalAddTopup, System.GetDPValue());
			csv.Add(m_nPurse2TotalAddOtherCount);
			csv.Add(m_dPurse2TotalAddOther, System.GetDPValue());
			csv.Add(m_nPurse2TotalSubSpendCount);
			csv.Add(m_dPurse2TotalSubSpend, System.GetDPValue());
			csv.Add(m_nPurse2TotalSubOtherCount);
			csv.Add(m_dPurse2TotalSubOther, System.GetDPValue());
		}

		csv.Add(m_nPurse2TotalAddTopupCount + m_nPurse2TotalAddOtherCount);
		csv.Add(dPurse2TotalPlus, System.GetDPValue());
		csv.Add(m_nPurse2TotalSubSpendCount + m_nPurse2TotalSubOtherCount);
		csv.Add(dPurse2TotalMinus, System.GetDPValue());
		csv.Add(::SignedFormatDoubleValue(dPurse2TotalPlus - dPurse2TotalMinus));

		m_Report.WriteLine(csv.GetLine());
	}
}

//*******************************************************************
