/******************************************************************************/
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
/******************************************************************************/
#include "ReportGroupPeriodSales.h"
/******************************************************************************/

CGroupPeriodSalesReport::CGroupPeriodSalesReport ( const char* szAuditFile, CWnd* pParent )
	: m_auditFile(szAuditFile) 
	, m_dlgFilter("R110",pParent)
{
	m_pParent = pParent;
	m_strTitle = "Group Period Sales";
	m_bPeriodTextReqd = TRUE;
	m_GroupSetText.Open ( Filenames.GetGroupSetFilename() );		// readonly
	SQLRowSetAuditPeriod.LoadPeriods();
	
	Reset();
}

/******************************************************************************/

void CGroupPeriodSalesReport::Reset()
{
	m_bCancelButton = FALSE;
	m_nRestartPos = 0;
	m_nFilePos = 0;
	m_strLastDate = "";
	m_nDayCounter = 0;

	for (int i = 0; i <= 99; i++)
	{
		m_nGroupCount[1][i] = 0;				// grand group totals
		m_nGroupTrans[1][i] = 0;
		m_dGroupPurse1[1][i] = 0;
		m_dGroupPurse2[1][i] = 0;
		m_dGroupPurse3[1][i] = 0;
		m_dGroupCash[1][i] = 0;
	}

	ClearGroupTotals();							// date group total
}

/******************************************************************************/

void CGroupPeriodSalesReport::ClearGroupTotals()
{
	for (int i = 1; i <= 99; i++)
	{
		m_nGroupCount[0][i] = 0;
		m_nGroupTrans[0][i] = 0;
		m_dGroupPurse1[0][i] = 0;
		m_dGroupPurse2[0][i] = 0;
		m_dGroupPurse3[0][i] = 0;
		m_dGroupCash[0][i] = 0;
	}
}

/******************************************************************************/
// [0] = Group or Group Set
// [1] = PeriodTotal
// [2] = Day Total
// [3] = Report Total

void CGroupPeriodSalesReport::ClearTotals ( int n )
{
	m_nCardCount  [n] = 0;
	m_nTransCount [n] = 0;
	m_dTransPurse1[n] = 0;
	m_dTransPurse2[n] = 0;
	m_dTransPurse3[n] = 0;
	m_dTransCash  [n] = 0;
}

//******************************************************************************

bool CGroupPeriodSalesReport::Show()
{
	int nErrorNo = 0;

	CString strReportLabel = m_dlgFilter.GetReportLabel();
	CString strReportFile = Filenames.GetReportFilename(strReportLabel);
	CString strParamsFile = Filenames.GetReportParamsFilename(strReportLabel);

	CRepmanHandler repman(m_strTitle, m_pParent);

	if ((nErrorNo = Create(strReportFile)) == nREPORT_NOERROR)
	{
		nErrorNo = repman.DisplayReport(strReportFile, strParamsFile, m_dlgFilter.GetReportKey());
	}

	return repman.DisplayError(nErrorNo);
}

//******************************************************************************

int CGroupPeriodSalesReport::Create ( const char* szReportFile )
{
	if (m_auditFile.Open() == FALSE)					// try and open audit file
	{
		return nREPORT_NOSOURCE;						// no source data found
	}

	bool bOk = m_auditFile.GetFirstDate ( m_dlgFilter.m_dateAllFrom );	// try & get first date in audit file
	m_auditFile.Close();								// lose audit file

	if (bOk == FALSE)									// see if have first date in audit file
	{
		return nREPORT_INVALIDSOURCE;					// invalid source
	}

	if (m_dlgFilter.DoModal() == IDCANCEL)
	{
		return nREPORT_EXIT;
	}

	CString strReportTitle = m_strTitle;
	strReportTitle += m_dlgFilter.GetDateString();

	if (m_Report.Open(szReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}
	else
	{
		if ( m_auditFile.Open() == FALSE )						// reopen source file
		{
			m_Report.Close();									// close report file
			return nREPORT_OPENERROR;
		}
	}

	CWorkingDlg dlgWorking ( szMSG_CREATINGREPORT, TRUE );
	dlgWorking.Create();

	m_Report.SetStyle1 ( strReportTitle );
	AddColumnText();
		
	Reset();

	bool bReply = ReportByDate( &dlgWorking, ( m_dlgFilter.m_bSummaryReqd ) );

	dlgWorking.DestroyWindow();

	m_Report.Close();
	m_auditFile.Close();										// close audit file open

	if (m_bCancelButton == TRUE)
	{
		return nREPORT_CANCEL;
	}
	if (bReply == FALSE)
	{
		return nREPORT_NOMATCH;
	}

	return nREPORT_NOERROR;
}

/*****************************************************************************/

bool CGroupPeriodSalesReport::ReportByDate ( CWorkingDlg* pWorkingDlg, bool bSummaryReport )
{
	bool bReply = FALSE;
	
	ClearTotals ( 3 );										// clear  total
	m_auditFile.JumpIn ( m_dlgFilter.m_dateFrom.GetDate() );

	while ( TRUE )
	{
		if ( pWorkingDlg -> SetCaption2PercentDone( m_auditFile.GetPercentPosition(), TRUE ) == TRUE )
		{
			m_bCancelButton = TRUE;
			break;
		}

		m_nFilePos = m_auditFile.GetFilePos();				// filepos for start of line
		if (m_auditFile.ReadLine(&m_atc) == FALSE)
		{
			break;
		}

		if (m_dlgFilter.IsValidDate(&m_atc) == TRUE)
		{
			bReply = TRUE;
			GroupByPeriod(pWorkingDlg, bSummaryReport);	// show by single date
			m_auditFile.SetFilePos(m_nRestartPos); 		// back to start of next date section

			if (m_bCancelButton == TRUE)
			{
				break;
			}
		}
	}

	ShowReportTotal ( "Grand" );
	return bReply;
}

/*****************************************************************************/

bool CGroupPeriodSalesReport::Validate ( bool bSummaryReport )
{
	if (bSummaryReport == TRUE)
	{
		return m_dlgFilter.IsValidDate(&m_atc);
	}

	return  ( m_strLastDate == m_atc.GetDateFiled() ) ? TRUE : FALSE;
}

/*****************************************************************************/

void CGroupPeriodSalesReport::GroupByPeriod(CWorkingDlg* pWorkingDlg, bool bSummaryReport)
{
	CRecordSorter sorter;
	m_strLastDate = m_atc.GetDateFiled();					// date in this range (not needed by summary)

	while (Validate(bSummaryReport) == TRUE)
	{
		if ( pWorkingDlg -> SetCaption2PercentDone( m_auditFile.GetPercentPosition(), TRUE ) == TRUE )
		{
			m_bCancelButton = TRUE;
			break;
		}

		m_nRestartPos = m_auditFile.GetFilePos();			// position to go back to if date changes

		bool bUseLine = FALSE;
		switch (m_atc.GetSourceType())
		{
		case AUDIT_POS:
			bUseLine = m_dlgFilter.IsValidTerminalNo(m_atc.GetFolderSet(), m_atc.GetTerminalNo());
			break;

		case AUDIT_EXT:
			bUseLine = TRUE;
			break;
		}

		if ((TRUE == bUseLine) && (m_atc.GetApplicationNo() == APPNO_SALE))
		{
			/*
			bool bValid = FALSE;

			if (m_atc.GetTransactionPurse1Total() < 0 || m_atc.GetTransactionPurse2Balance() < 0 || m_atc.GetTransactionCash() < 0)
			{
				bValid = TRUE;
			}
			else if (System.GetEnablePurse3Flag() == TRUE && m_atc.GetTransactionPurse3Total() < 0)
			{
				bValid = TRUE;
			}

			if (bValid == TRUE)
			*/
			{
				CString strLabel = "";

				if (m_dlgFilter.m_nReportBy == nREPORT_BYGROUP)
				{
					strLabel.Format("%d%2.2d%s", 
						SQLRowSetAuditPeriod.GetPeriodIndex(m_atc.GetTimeFiled()), 
						m_atc.GetGroupNo(),
						(const char*) System.FormatCardNo(m_atc.GetUserID()));
				}
				else
				{
					CSQLRowGroupFull RowGroup;
					RowGroup.SetGroupNo(m_atc.GetGroupNo());

					int nGroupSetNo = 1;

					CSQLRepositoryGroup RepoGroup;
					if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
					{
						nGroupSetNo = RowGroup.GetSetNo();
					}

					strLabel.Format("%d%2.2d%s", 
						SQLRowSetAuditPeriod.GetPeriodIndex(m_atc.GetTimeFiled()), 
						nGroupSetNo, 
						(const char*) System.FormatCardNo(m_atc.GetUserID()));
				}
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
	CString strLastCardNo = "x";						// can't have "" as cash sales are userID 000000 which comes out as ""

	ClearTotals(0);
	ClearTotals(1);
	ClearTotals(2);
	ClearGroupTotals();

	for (int nIndex = 0; nIndex < sorter.GetSize(); nIndex++)
	{
		if ( pWorkingDlg -> CheckCancelButton() == TRUE )
		{
			m_bCancelButton = TRUE;
			break;
		}

		m_auditFile.SetFilePos(sorter.GetRecordNo(nIndex));
		if (m_auditFile.ReadLine(&m_atc) == TRUE)
		{
			CString strCardNo = m_atc.GetUserID();

			int nGroupNo = 1;
			if (m_dlgFilter.m_nReportBy == nREPORT_BYGROUP)
			{
				nGroupNo = m_atc.GetGroupNo();
			}
			else
			{
				CSQLRowGroupFull RowGroup;
				RowGroup.SetGroupNo(m_atc.GetGroupNo());

				CSQLRepositoryGroup RepoGroup;
				if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					nGroupNo = RowGroup.GetSetNo();
				}
			}

			nPeriodIndex = SQLRowSetAuditPeriod.GetPeriodIndex(m_atc.GetTimeFiled());

			if (nIndex == 0)								// only needed on first entry
			{
				nLastPeriodIndex = nPeriodIndex;
				nLastGroupNo = nGroupNo;
			}

			if (nLastPeriodIndex != nPeriodIndex)			// see if period has changed
			{
				ShowGroupTotal(nLastPeriodIndex, nLastGroupNo);
				nLastGroupNo = 0;

				ShowPeriodTotal(nLastPeriodIndex);		// display current values
				nLastPeriodIndex = nPeriodIndex;
				strLastCardNo = "x";
			}

			if (nLastGroupNo != nGroupNo)					// see if group has changed
			{
				ShowGroupTotal(nLastPeriodIndex, nLastGroupNo);
				nLastGroupNo = nGroupNo;
				strLastCardNo = "x";
			}

			if (strLastCardNo != strCardNo)
			{
				++m_nCardCount[0];
				++m_nCardCount[1];
				++m_nCardCount[2];
				++m_nCardCount[3];
				strLastCardNo = strCardNo;
			}

			++m_nTransCount[0];
			++m_nTransCount[1];
			++m_nTransCount[2];
			++m_nTransCount[3];

			double dValue = (m_atc.GetTransactionPurse1Total() < 0) ? -(m_atc.GetTransactionPurse1Total()) : 0;
			m_dTransPurse1[0] += dValue;
			m_dTransPurse1[1] += dValue;
			m_dTransPurse1[2] += dValue;
			m_dTransPurse1[3] += dValue;

			dValue = (m_atc.GetTransactionPurse2Balance() < 0) ? -(m_atc.GetTransactionPurse2Balance()) : 0;	/* - ( -ve ) == positive ) */
			m_dTransPurse2[0] += dValue;
			m_dTransPurse2[1] += dValue;
			m_dTransPurse2[2] += dValue;
			m_dTransPurse2[3] += dValue;

			dValue = (m_atc.GetTransactionPurse3Total() < 0) ? -(m_atc.GetTransactionPurse3Total()) : 0;
			m_dTransPurse3[0] += dValue;
			m_dTransPurse3[1] += dValue;
			m_dTransPurse3[2] += dValue;
			m_dTransPurse3[3] += dValue;


			dValue = (m_atc.GetTransactionCash() < 0) ? -(m_atc.GetTransactionCash()) : 0;	/* - ( -ve ) == positive ) */
			m_dTransCash[0] += dValue;
			m_dTransCash[1] += dValue;
			m_dTransCash[2] += dValue;
			m_dTransCash[3] += dValue;
		}
	}

	ShowGroupTotal(nLastPeriodIndex, nLastGroupNo);
	ShowPeriodTotal(nLastPeriodIndex);

	++m_nDayCounter;
	if (bSummaryReport == TRUE)
	{
		ShowDayTotal("Summary", TRUE);
	}
	else
	{
		ShowDayTotal(m_strLastDate, FALSE);
	}
}

//*********************************************************************

void CGroupPeriodSalesReport::ShowGroupTotal(int nPeriodIndex, int nGroupNo)
{
	if (nGroupNo == 0)
	{
		return;
	}

	CString strPrefixText = "";
	if (m_bPeriodTextReqd == TRUE)
	{
		strPrefixText = SQLRowSetAuditPeriod.GetPeriodName(nPeriodIndex);
		m_bPeriodTextReqd = FALSE;
	}

	CString strName = "Unknown";

	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(nGroupNo);

	CSQLRepositoryGroup RepoGroup;
	if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (m_dlgFilter.m_nReportBy == nREPORT_BYGROUP)
		{
			strName = RowGroup.GetGroupName();
		}
		else
		{
			strName = m_GroupSetText.GetText(nGroupNo);
		}
	}


	CCSV csv('\t');
	csv.Add(strPrefixText);
	csv.Add(strName);

	AddInt(&csv, m_nCardCount[0]);
	AddDouble(&csv, m_dTransPurse1[0]);
	AddDouble(&csv, m_dTransPurse2[0]);

	if (System.GetEnablePurse3Flag() == TRUE)
	{
		AddDouble(&csv, m_dTransPurse3[0]);
	}

	AddDouble(&csv, m_dTransCash[0]);
	AddInt(&csv, m_nTransCount[0]);

	m_Report.WriteLine(csv);

	m_nGroupCount[0][nGroupNo] += m_nCardCount[0];			// group day totals
	m_nGroupTrans[0][nGroupNo] += m_nTransCount[0];
	m_dGroupPurse1[0][nGroupNo] += m_dTransPurse1[0];
	m_dGroupPurse2[0][nGroupNo] += m_dTransPurse2[0];
	m_dGroupPurse3[0][nGroupNo] += m_dTransPurse3[0];
	m_dGroupCash[0][nGroupNo] += m_dTransCash[0];

	m_nGroupCount[1][nGroupNo] += m_nCardCount[0];			// group grand totals
	m_nGroupTrans[1][nGroupNo] += m_nTransCount[0];
	m_dGroupPurse1[1][nGroupNo] += m_dTransPurse1[0];
	m_dGroupPurse2[1][nGroupNo] += m_dTransPurse2[0];
	m_dGroupPurse3[1][nGroupNo] += m_dTransPurse3[0];
	m_dGroupCash[1][nGroupNo] += m_dTransCash[0];

	ClearTotals(0);
}

//*********************************************************************

void CGroupPeriodSalesReport::ShowPeriodTotal ( int nPeriodIndex )
{
	if (SQLRowSetAuditPeriod.GetPeriodCount() > 1)			// have more than 1 period setup
	{
		ShowTotals(1, SQLRowSetAuditPeriod.GetPeriodName(nPeriodIndex));
	}

	m_bPeriodTextReqd = TRUE;
}

//*********************************************************************

void CGroupPeriodSalesReport::ShowDayTotal ( const char* szText, bool bSummary )
{
	// extra blank for summary total
	// as report total will not get used
	if (bSummary == TRUE)
	{
		m_Report.WriteLine("");					
	}
	
	if (SQLRowSetAuditPeriod.GetPeriodCount() > 1)		// have more than 1 period setup
	{
		ShowGroupGrandTotals(0, szText);			// dd/mm/yyyy or 'Summary'
	}

	ShowTotals( 2, szText );
}

//*********************************************************************

void CGroupPeriodSalesReport::ShowReportTotal ( const char* szText )
{
	if ( m_nDayCounter != 1 )						// if count=0 means no data in date range - just show report totals to conform no sales
	{
		if (SQLRowSetAuditPeriod.GetPeriodCount() > 1)	// have more than 1 period setup
		{
			ShowGroupGrandTotals(1, szText);
		}

		ShowTotals ( 3, szText );
	}
}

//*********************************************************************

void CGroupPeriodSalesReport::ShowTotals ( int n, const char* szText )
{
	CString strText = "";
	strText.Format ( "<..>%s Total", szText );

	CCSV csv ( '\t' );

	csv.Add ( strText );
	csv.Add ( "" );
	AddInt ( &csv, m_nCardCount[n] );
	AddDouble ( &csv, m_dTransPurse1[n] );
	AddDouble ( &csv, m_dTransPurse2[n] );
	
	if (System.GetEnablePurse3Flag() == TRUE)
	{
		AddDouble(&csv, m_dTransPurse3[n]);
	}
	
	AddDouble ( &csv, m_dTransCash[n] );
	AddInt ( &csv, m_nTransCount[n] );

	CString strLines = "\t\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>";
	if (System.GetEnablePurse3Flag() == TRUE)
	{
		strLines += "\t<LI>";
	}

	m_Report.WriteLine ( strLines );
	m_Report.WriteLine ( csv );
	m_Report.WriteLine ( "" );

	ClearTotals ( n );
}

//*********************************************************************

void CGroupPeriodSalesReport::ShowGroupGrandTotals ( int n, const char* szText )
{
	CString strPrefixText = "";
	strPrefixText.Format ( "<..>%s Totals", szText );

	for ( int nGroupNo = 1 ; nGroupNo <= 99 ; nGroupNo++ )
	{
		CString strName = "";

		CSQLRowGroupFull RowGroup;
		RowGroup.SetGroupNo(nGroupNo);

		CSQLRepositoryGroup RepoGroup;
		if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if (m_dlgFilter.m_nReportBy == nREPORT_BYGROUP)
			{
				strName = RowGroup.GetGroupName();
			}
			else
			{
				strName = m_GroupSetText.GetText(nGroupNo);
			}
		}

		if (strName == "")									// name not set so skip Group \ Set total
		{
			continue;
		}

		CCSV csv ( '\t' );

		csv.Add ( strPrefixText );								// add text
		if (strPrefixText != "")
		{
			strPrefixText = "";									// clear it for following lines
		}

		csv.Add ( strName );
		AddInt ( &csv, m_nGroupCount[n][nGroupNo] );
		AddDouble ( &csv, m_dGroupPurse1[n][nGroupNo] );
		AddDouble ( &csv, m_dGroupPurse2[n][nGroupNo] );
		
		if (System.GetEnablePurse3Flag() == TRUE)
		{
			AddDouble(&csv, m_dGroupPurse3[n][nGroupNo]);
		}

		AddDouble ( &csv, m_dGroupCash[n][nGroupNo] );
		AddInt ( &csv, m_nGroupTrans[n][nGroupNo] );

		m_Report.WriteLine ( csv );
	}
}

//**********************************************************************

void CGroupPeriodSalesReport::AddInt(CCSV* pCsv, int nValue)
{
	if (nValue == 0)
	{
		pCsv->Add(szNODATA);
	}
	else
	{
		pCsv->Add(nValue);
	}
}

//**********************************************************************

void CGroupPeriodSalesReport::AddDouble(CCSV* pCsv, double dValue)
{
	if (dValue == 0)
	{
		pCsv->Add(szNODATA);
	}
	else
	{
		pCsv->Add(dValue, System.GetDPValue());
	}
}

//**********************************************************************

void CGroupPeriodSalesReport::AddColumnText()
{
	m_Report.AddColumn("Period", TA_LEFT, 300);

	if (m_dlgFilter.m_nReportBy == nREPORT_BYGROUP)
	{
		m_Report.AddColumn("Group", TA_LEFT, 300);
	}
	else
	{
		m_Report.AddColumn("Set", TA_LEFT, 300);
	}

	m_Report.AddColumn("UserCount", TA_RIGHT, 249);
	m_Report.AddColumn("P1.Trans", TA_RIGHT, 249);
	m_Report.AddColumn("P2.Trans", TA_RIGHT, 249);

	if (System.GetEnablePurse3Flag() == TRUE)
	{
		m_Report.AddColumn("P3.Trans", TA_RIGHT, 249);
	}

	m_Report.AddColumn("CashTrans", TA_RIGHT, 249);
	m_Report.AddColumn("TransCount", TA_RIGHT, 249);
}

//*****************************************************************************
