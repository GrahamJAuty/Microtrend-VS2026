/*****************************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
/*****************************************************************************/
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
/*****************************************************************************/
#include "GroupComboHelpers.h"
#include "RecSort.h"
#include "RepmanHandler.h"
#include "WorkingDlg.h"
/*****************************************************************************/
#include "ReportGroupPeriodSales.h"
/*****************************************************************************/

CGroupPeriodSalesReport::CGroupPeriodSalesReport(const char* szLabel, const char* szAuditFile, CWnd* pParent) :
	m_dlgFilter(pParent)
	, m_auditFile(szAuditFile)
{
	m_pParent = pParent;
	m_strReportLabel = szLabel;

	m_strTitle = m_ReportInfo.GetReportTitle();
	m_bPeriodTextReqd = TRUE;

	SQLRowSetAuditPeriod.LoadPeriods(NULL);

	m_bCancelButton = FALSE;

	Reset();
}

/*****************************************************************************/

CGroupPeriodSalesReport::~CGroupPeriodSalesReport()
{
}

/*****************************************************************************/

void CGroupPeriodSalesReport::Reset()
{
	m_bCancelButton = FALSE;
	m_nRestartPos = 0;
	m_nFilePos = 0;
	m_strLastDate = "";
	m_nDayCounter = 0;

	for (int n = 0; n <= 3; n++)
	{
		ClearReportTotals(n);
	}

	ClearGroupDateTotals();
	ClearGroupGrandTotals();
}

/*****************************************************************************/
// [0] = Group or Group Set
// [1] = PeriodTotal
// [2] = Day Total
// [3] = Report Total

void CGroupPeriodSalesReport::ClearReportTotals(int n)
{
	if ( ( n >= 0 ) && ( n <= 3 ) )
	{
		m_SectionTotals[n].Reset();
	}
}

/*****************************************************************************/

void CGroupPeriodSalesReport::ClearGroupDateTotals()
{
	ClearGroupTotals(0);
}

/*****************************************************************************/

void CGroupPeriodSalesReport::ClearGroupGrandTotals()
{
	ClearGroupTotals(1);
}

/*****************************************************************************/

void CGroupPeriodSalesReport::ClearGroupTotals(int n)
{
	switch (n)
	{
	case 0:
	case 1:
		for (int i = 1; i <= 99; i++)
		{
			m_GroupTotals[n][i].Reset();
		}
		break;
	}
}

/*****************************************************************************/

bool CGroupPeriodSalesReport::Show()
{
	CRepmanHandler repman ( m_strTitle, m_pParent );

	int nErrorNo = m_auditFile.ValidateFile();
	if ( nErrorNo == nREPORT_NOERROR )
	{
		CString strReportFile = Filenames.GetReportFilename ( m_strReportLabel );
		CString strParamsFile = Filenames.GetReportParamsFilename ( m_strReportLabel );

		if ((nErrorNo = Create(strReportFile)) == nREPORT_NOERROR)
		{
			nErrorNo = repman.DisplayReport(strReportFile, strParamsFile, m_ReportInfo.GetReportKey());
		}
	}

	return repman.DisplayError ( nErrorNo );
}

/*****************************************************************************/

int CGroupPeriodSalesReport::Create ( const char* szReportFile )
{
	m_dlgFilter.SetReportInfo( &m_ReportInfo );
	if (m_dlgFilter.DoModal() == IDCANCEL)
	{
		return nREPORT_EXIT;
	}

	int nErrorNo = nREPORT_NOERROR;

	if ( m_Report.Open ( szReportFile ) == TRUE )
	{
		CString strReportTitle = m_strTitle;
		strReportTitle += m_ReportInfo.GetDateRangeString();
		m_Report.SetStyle1 ( strReportTitle );

		AddColumnText();
	
		if ( m_auditFile.OpenReadOnly() == TRUE )				// open main audit file
		{
			nErrorNo = ReportByDate( ( m_ReportInfo.GetSummaryFlag() ) );

			m_auditFile.Close();
		}
		else
		{
			nErrorNo = nREPORT_OPENERROR;
		}

		m_Report.Close();
	}
	else
	{
		nErrorNo = nREPORT_CREATEFAIL;
	}

	return ( m_bCancelButton == TRUE ) ? nREPORT_CANCEL : nErrorNo; 
}

/*****************************************************************************/

int CGroupPeriodSalesReport::ReportByDate ( bool bSummaryReport )
{
	int nReply = nREPORT_NOMATCH;
	
	CWorkingDlg DlgWorking ( "Creating Report" );
	DlgWorking.Create();

	Reset();
	m_auditFile.JumpIn ( m_ReportInfo.GetDateFromString() );

	while ( TRUE )
	{
		DlgWorking.IncrementRecordsChecked ( 10 );
		if ((m_bCancelButton = DlgWorking.CheckCancelButton()) == TRUE)
		{
			break;
		}

		m_nFilePos = m_auditFile.GetFilePos();				// filepos for start of line
		if (m_auditFile.ReadLine(&m_atc) == FALSE)
		{
			break;
		}

		if ( m_ReportInfo.IsValidDate ( m_atc.GetDateFiled() ) == TRUE )
		{
			nReply = nREPORT_NOERROR;

			GroupByPeriod ( &DlgWorking, bSummaryReport );	// show by single date
			m_auditFile.SetFilePos ( m_nRestartPos ); 		// back to start of next date section

			if (m_bCancelButton == TRUE)
			{
				break;
			}
		}
	}

	ShowReportTotal ( "Grand" );

	DlgWorking.DestroyWindow();

	return nReply;
}

/*****************************************************************************/

bool CGroupPeriodSalesReport::Validate ( bool bSummaryReport )
{
	if (bSummaryReport == TRUE)
	{
		return m_ReportInfo.IsValidDate(m_atc.GetDateFiled());
	}
	else
	{
		return  (m_strLastDate == m_atc.GetDateFiled()) ? TRUE : FALSE;
	}
}

/*****************************************************************************/

void CGroupPeriodSalesReport::GroupByPeriod(CWorkingDlg* pWorkingDlg, bool bSummaryReport)
{
	CRecordSorter sorter;
	m_strLastDate = m_atc.GetDateFiled();					// date in this range (not needed by summary)

	CString strRenewalTransaction = "";

	while (Validate(bSummaryReport) == TRUE)
	{
		pWorkingDlg->IncrementRecordsChecked(10);
		if ((m_bCancelButton = pWorkingDlg->CheckCancelButton()) == TRUE)
		{
			break;
		}

		m_nRestartPos = m_auditFile.GetFilePos();			// position to go back to if date changes

		bool bProcessAsSale = FALSE;
		bool bCheckTerminal = FALSE;

		switch (m_atc.GetApplicationNo())
		{
		case APPNO_EXTENDEXPIRE:
			strRenewalTransaction = m_atc.GetEposTranNo();
			break;

		case APPNO_SALE:
			switch (m_atc.GetSourceType())
			{
			case AUDIT_POS:
			case AUDIT_EXT:
				switch (m_ReportInfo.GetSalesTypeFilter())
				{
				case 1:
					bProcessAsSale = strRenewalTransaction != m_atc.GetEposTranNo();
					break;

				case 2:
					bProcessAsSale = strRenewalTransaction == m_atc.GetEposTranNo();
					break;

				default:
					bProcessAsSale = TRUE;
					break;
				}
				bCheckTerminal = TRUE;
				break;
			}
			break;

		case APPNO_MANUAL_SPEND:
			bProcessAsSale = m_ReportInfo.GetManualSpendFlag();
			break;
		}

		if (TRUE == bCheckTerminal)
		{
			CServerDataFolderSetIndex FolderSetIndex = m_atc.GetFolderSetIndex();
			if (m_ReportInfo.CheckTerminalFilter(FolderSetIndex.m_nDbNo, FolderSetIndex.m_nSetNo, m_atc.GetTerminalNo()) == FALSE)
			{
				bProcessAsSale = FALSE;
			}
		}

		if (TRUE == bProcessAsSale)
		{
			if ((m_atc.GetPurse1Transaction() < 0.0) || (m_atc.GetPurse2Transaction() < 0.0) || (m_atc.GetCashTransaction() < 0.0) || (m_atc.GetTotalDiscount() != 0.0))
			{
				CString strLabel = "";
				strLabel.Format("%d%2.2d%s",
					SQLRowSetAuditPeriod.GetPeriodIndex(m_atc.GetTimeFiled()),
					m_atc.GetGroupNo(),
					(const char*)System.FormatCardNo(m_atc.GetCardNo()));

				sorter.Add(strLabel, m_nFilePos);
			}
		}

		m_nFilePos = m_auditFile.GetFilePos();				/* start of next line */
		if (m_auditFile.ReadLine(&m_atc) == FALSE)
		{
			break;
		}
	}

	if (m_bCancelButton == TRUE || sorter.GetSize() == 0)
	{
		return;
	}

	int nLastGroupNo = 0;
	int nPeriodIndex = 0;
	int nLastPeriodIndex = 0;
	bool bHaveGroup = FALSE;
	bool bHavePeriod = FALSE;
	CString strLastCardNo = "";

	ClearReportTotals(0);
	ClearReportTotals(1);
	ClearReportTotals(2);
	ClearGroupDateTotals();

	for (int nIndex = 0; nIndex < sorter.GetSize(); nIndex++)
	{
		pWorkingDlg->IncrementRecordsChecked(10);
		if ((m_bCancelButton = pWorkingDlg->CheckCancelButton()) == TRUE)
		{
			break;
		}

		m_auditFile.SetFilePos(sorter.GetRecordNo(nIndex));
		if (m_auditFile.ReadLine(&m_atc) == TRUE)
		{
			CString strCardNo = m_atc.GetCardNo();
			int nGroupNo = m_atc.GetGroupNo();

			nPeriodIndex = SQLRowSetAuditPeriod.GetPeriodIndex(m_atc.GetTimeFiled());

			if (nIndex == 0)
			{
				nLastPeriodIndex = nPeriodIndex;			// only need on first entry
				nLastGroupNo = nGroupNo;
			}

			if (nLastPeriodIndex != nPeriodIndex)			// period has changed
			{
				if (bHaveGroup == TRUE)					// see if any group data to flush
				{
					ShowGroupTotal(nLastPeriodIndex, nLastGroupNo);
					bHaveGroup = FALSE;
					nLastGroupNo = 0;
				}

				if (bHavePeriod == TRUE)					// make sure have some period data to flush
				{
					ShowPeriodTotal(nLastPeriodIndex);	// display current values
					bHavePeriod = FALSE;
				}

				nLastPeriodIndex = nPeriodIndex;
				strLastCardNo = "";
			}

			if (nLastGroupNo != nGroupNo)					// group has changed
			{
				if (bHaveGroup == TRUE)
				{
					ShowGroupTotal(nLastPeriodIndex, nLastGroupNo);
					bHaveGroup = FALSE;
				}
				nLastGroupNo = nGroupNo;
				strLastCardNo = "";
			}

			CGroupPeriodSalesTotals LineTotals;

			if (strLastCardNo != strCardNo)
			{
				LineTotals.m_nCardCount = 1;
				strLastCardNo = strCardNo;
			}

			LineTotals.m_nTransCount = 1;
			LineTotals.m_dPurse1 = (m_atc.GetPurse1Transaction() < 0.0) ? -(m_atc.GetPurse1Transaction()) : 0.0;
			LineTotals.m_dPurse2 = (m_atc.GetPurse2Transaction() < 0.0) ? -(m_atc.GetPurse2Transaction()) : 0.0;	/* - ( -ve ) == positive ) */
			LineTotals.m_dCash = (m_atc.GetCashTransaction() < 0.0) ? -(m_atc.GetCashTransaction()) : 0.0;	/* - ( -ve ) == positive ) */

			if (m_ReportInfo.GetDiscountFlag() == TRUE)
			{
				LineTotals.m_dDiscount = m_atc.GetTotalDiscount();
			}

			m_SectionTotals[0].Add(LineTotals);			// add to group total
			m_SectionTotals[1].Add(LineTotals);			// add to period total
			m_SectionTotals[2].Add(LineTotals);			// add to day total
			m_SectionTotals[3].Add(LineTotals);			// add to report total

			bHavePeriod = TRUE;
			bHaveGroup = TRUE;
		}
	}

	if (bHaveGroup == TRUE)
	{
		ShowGroupTotal(nLastPeriodIndex, nLastGroupNo);
	}
	if (bHavePeriod == TRUE)
	{
		ShowPeriodTotal(nLastPeriodIndex);
	}

	++m_nDayCounter;
	if (bSummaryReport == TRUE)
	{
		if (SQLRowSetAuditPeriod.GetPeriodCount() > 1)
		{
			m_Report.WriteLine("");
		}
		ShowDayTotal("Summary", TRUE);
	}
	else
	{
		ShowDayTotal(m_strLastDate, FALSE);
	}
}

/*****************************************************************************/

void CGroupPeriodSalesReport::ShowGroupTotal(int nPeriodIndex, int nGroupNo)
{
	CCSV csv('\t');

	if (m_bPeriodTextReqd == TRUE)
	{
		csv.Add(SQLRowSetAuditPeriod.GetPeriodName(nPeriodIndex));
		m_bPeriodTextReqd = FALSE;
	}
	else
	{
		csv.Add("");
	}

	CSQLRowGroup RowGroup;
	RowGroup.SetGroupNo(nGroupNo);

	CSQLRepositoryGroup repoGroup;
	if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		csv.Add(RowGroup.GetGroupName());
	}
	else
	{
		csv.Add("Unknown");
	}

	AddTotals(csv, m_SectionTotals[0]);

	m_Report.WriteLine(csv);

	m_GroupTotals[0][nGroupNo].Add(m_SectionTotals[0]);
	m_GroupTotals[1][nGroupNo].Add(m_SectionTotals[0]);

	ClearReportTotals(0);
}

/*****************************************************************************/

void CGroupPeriodSalesReport::ShowPeriodTotal ( int nPeriodIndex )
{
	if (SQLRowSetAuditPeriod.GetPeriodCount() > 1)			// have more than 1 period setup
	{
		ShowTotals(1, SQLRowSetAuditPeriod.GetPeriodName(nPeriodIndex));
		m_Report.WriteLine("");
	}

	m_bPeriodTextReqd = TRUE;
}

/*****************************************************************************/

void CGroupPeriodSalesReport::ShowDayTotal(const char* szText, bool bSummary)
{
	if (SQLRowSetAuditPeriod.GetPeriodCount() > 1)
	{
		m_Report.WriteLine("");
		ShowGroupGrandTotals(0, szText);
	}

	ShowTotals(2, szText);
	m_Report.WriteLine("");
	m_Report.WriteLine("");

	if (SQLRowSetAuditPeriod.GetPeriodCount() > 1)
	{
		m_Report.WriteLine("");
	}
}

/*****************************************************************************/

void CGroupPeriodSalesReport::ShowReportTotal ( const char* szText )
{
	if ( m_nDayCounter > 1 )
	{
		//if ( m_periodData.GetPeriodCount() > 1 )	// have more than 1 period setup
		{
			ShowGroupGrandTotals(1, szText);
		}

		ShowTotals ( 3, szText );
	}
}

/*****************************************************************************/

void CGroupPeriodSalesReport::ShowTotals(int n, const char* szText)
{
	CString strText = szText;
	if ( strText == "Grand")
	{
		strText = "<..>Grand Total";
	}
	else
	{
		CString strTemp = strText;
		strText = "<..>Total: ";
		strText += strTemp;
	}

	CCSV csv('\t');
	CString strTabLine = "\t\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>";

	if ( TRUE == m_ReportInfo.GetDiscountFlag() )
	{
		strTabLine += "\t<LI>\t<LI>";
	}

	csv.Add("");
	csv.Add(strText);
	
	AddTotals(csv, m_SectionTotals[n]);
	
	m_Report.WriteLine(strTabLine);
	m_Report.WriteLine(csv);

	ClearReportTotals(n);
}

/*****************************************************************************/

void CGroupPeriodSalesReport::ShowGroupGrandTotals(int n, const char* szText)
{
	int bTextReqd = TRUE;

	for (int nGroupNo = 1; nGroupNo <= 99; nGroupNo++)
	{
		if ((m_GroupTotals[n][nGroupNo].m_nCardCount == 0) && 
			(m_GroupTotals[n][nGroupNo].m_nTransCount == 0) && 
			(m_GroupTotals[n][nGroupNo].m_dPurse1 == 0.0) &&
			(m_GroupTotals[n][nGroupNo].m_dPurse2 == 0.0) && 
			(m_GroupTotals[n][nGroupNo].m_dCash == 0.0))
		{
			continue;
		}

		CCSV csv('\t');

		if (bTextReqd == TRUE)
		{
			CString strText = szText;
			if (strText == "Grand")
			{
				strText = "<..>Grand Total";
			}
			else
			{
				CString strTemp = strText;
				strText = "<..>";
				strText += strTemp;
			}

			csv.Add(strText);
			bTextReqd = FALSE;
		}
		else
		{
			csv.Add("");
		}

		CSQLRowGroup RowGroup;
		RowGroup.SetGroupNo(nGroupNo);

		CSQLRepositoryGroup repoGroup;
		if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			csv.Add(RowGroup.GetGroupName());
		}
		else
		{
			csv.Add("Unknown");
		}

		AddTotals(csv, m_GroupTotals[n][nGroupNo]);

		m_Report.WriteLine(csv);
	}
}

/*****************************************************************************/

void CGroupPeriodSalesReport::AddTotals(CCSV& csv, CGroupPeriodSalesTotals& totals)
{
	double dSpendTotal = 0.0;
	dSpendTotal += totals.m_dPurse1;
	dSpendTotal += totals.m_dCash;

	if (System.GetEnablePurse2Flag() == TRUE)
	{
		dSpendTotal += totals.m_dPurse2;
	}

	csv.Add(totals.m_nCardCount);

	if (m_ReportInfo.GetDiscountFlag() == TRUE)
	{
		csv.Add(dSpendTotal + totals.m_dDiscount, System.GetDPValue());
		csv.Add(totals.m_dDiscount, System.GetDPValue());
	}

	csv.Add(totals.m_dPurse1, System.GetDPValue());

	if (System.GetEnablePurse2Flag() == TRUE)
	{
		csv.Add(totals.m_dPurse2, System.GetDPValue());
	}

	csv.Add(totals.m_dCash, System.GetDPValue());
	csv.Add(dSpendTotal, System.GetDPValue());
	csv.Add(totals.m_nTransCount);
}

/*****************************************************************************/

void CGroupPeriodSalesReport::AddColumnText()
{
	m_Report.AddColumn("Period", TA_LEFT, 300);
	m_Report.AddColumn("Group", TA_LEFT, 300);
	m_Report.AddColumn("Cards", TA_RIGHT, 250);

	if (m_ReportInfo.GetDiscountFlag() == TRUE)
	{
		m_Report.AddColumn("FullCost", TA_RIGHT, 250);
		m_Report.AddColumn("Discount", TA_RIGHT, 250);
	}

	m_Report.AddColumn("P1.Trans", TA_RIGHT, 250);

	if (System.GetEnablePurse2Flag() == TRUE)
	{
		m_Report.AddColumn("P2.Trans", TA_RIGHT, 250);
	}

	m_Report.AddColumn("CashTrans", TA_RIGHT, 250);
	m_Report.AddColumn("TotalTrans", TA_RIGHT, 250);
	m_Report.AddColumn("TransCount", TA_RIGHT, 250);
}

/*****************************************************************************/
