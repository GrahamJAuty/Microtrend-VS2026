/**********************************************************************/
// UR?? - Database user defined reports
// R001 - Hotlist report
// R010 - Never used
/**********************************************************************/
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
/**********************************************************************/
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
#include "RecSort.h"
#include "WorkingDlg.h"
#include "ReportDatabase.h"
//**********************************************************************

CDatabaseReport::CDatabaseReport ( const char* szLabel, CWnd* pParent )
{
	m_strReportLabel = szLabel;
	m_pParent = pParent;
	m_strReportTitle	= "";								// gets filled out later
}

//**********************************************************************

CDatabaseReport::~CDatabaseReport()
{
}

//**********************************************************************

void CDatabaseReport::ClearTotals ( int nIndex )
{
	m_nCount[nIndex]	= 0;
	m_nWarnings[nIndex]	= 0;
}

//*******************************************************************
// Compile & show report : return of FALSE = fatal error exit calling routine

bool CDatabaseReport::Show ( bool bEditReqd )
{
	if ( bEditReqd == TRUE )							// see if edit of label allowed
	{
		CDbExportEditor editor ( TRUE, NULL );
		if (editor.EditLabel(m_strReportLabel) == FALSE)
		{
			return FALSE;
		}
	}

	if (m_DbReporter.Read(m_strReportLabel) == FALSE)
	{
		return TRUE;
	}

// ok to continue & create report file

	CString strReportFile	= m_DbReporter.GetReportFilename();
	m_strReportTitle		= m_DbReporter.GetReportTitle();

	ClearTotals ( nREPORT_GROUP );
	ClearTotals ( nREPORT_TOTAL );
	m_DbReporter.ClearTotals();

	CRepmanHandler repman ( m_strReportTitle, m_pParent );

	int nErrorNo; 
 	if ( ( nErrorNo = Create ( strReportFile ) ) == nREPORT_NOERROR )
	{
		repman.SetupSaveAs ( m_DbReporter.GetSaveAsType(), m_DbReporter.GetSaveAsFile() );
		nErrorNo = repman.DisplayReport( strReportFile, m_DbReporter.GetParamsFilename(), m_DbReporter.GetReportKey() );
	}

	return repman.DisplayError ( nErrorNo );
}

//*******************************************************************

int CDatabaseReport::Create(const char* szReportFile)
{
	if (m_Report.Open(szReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CWorkingDlg WorkingDlg("Creating Report");
	WorkingDlg.Create();
	bool bCancelButton = FALSE;

	CString strWhere = "";
	CSQLBuilderBind BuilderBind{};
	CSQLRepositoryAccount repoAccount;
	repoAccount.PrepareExportWhere(m_DbReporter, -1, strWhere, BuilderBind);

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ExportByParams{ &BuilderBind, strWhere });
	RecordSet.m_strSort = m_DbReporter.GetSQLOrderBy();
	
	int nChecked = 0;
	CSQLRowAccountFull RowAccount;
	CArray<int, int> arrayHeader;
	int nGroupNo = 1;
	bool bGotLine = FALSE;

	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (0 == nChecked++)
		{
			m_Report.SetStyle1(m_strReportTitle);
			AddColumnText();
			LoyaltyHeaderHelpers.CreateNumericHeader(m_DbReporter.m_strHeader, arrayHeader);
		}

		if (WorkingDlg.SetCaption2RecordsChecked(nChecked) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		if (m_DbReporter.NonSQLFilter(RowAccount) == TRUE)
		{
			bool bReqd = TRUE;
			if (m_strReportLabel == "R010")				// never used report
			{
				CSSDate date{};
				CSSTime time{};
				RowAccount.GetLastUsed(date, time);		// get date purse1 \ purse2 used

				if (date.IsSet() == TRUE)
				{
					bReqd = FALSE;
				}
			}

			if (TRUE == bReqd)
			{
				if (m_DbReporter.GetSortByGroup() == TRUE)
				{
					if (nGroupNo != RowAccount.GetGroupNo())
					{
						SaveTotals(arrayHeader, nREPORT_GROUP, nGroupNo);
						nGroupNo = RowAccount.GetGroupNo();
						ClearTotals(nREPORT_GROUP);
						m_DbReporter.ClearTotals(nREPORT_GROUP);
					}
				}
				SaveLine(arrayHeader, RowAccount);
				bGotLine = TRUE;
			}
		}
	}
	if (m_DbReporter.GetSortByGroup() == TRUE)
	{
		SaveTotals(arrayHeader, nREPORT_GROUP, nGroupNo);
	}

	SaveTotals(arrayHeader, nREPORT_TOTAL);

	m_Report.Close();
	
// exit

	if (TRUE == bCancelButton == TRUE)
	{
		return nREPORT_CANCEL;
	}

	if (FALSE == bGotLine)
	{
		return nREPORT_NOMATCH;
	}

	return nREPORT_NOERROR;
}

/**********************************************************************/

void CDatabaseReport::AddColumnText()
{
	m_DbReporter.AddOrderByColumnText ( &m_Report );		// add OrderBy column headings

	if ( m_DbReporter.GetGroupSummary() == FALSE )
	{
		if (m_strReportLabel == "R001")				// hotlist report
		{
			m_Report.AddColumn("", TA_LEFT, 70);		// Warning column
		}

		m_DbReporter.CreateReportHeader ( &m_Report );	// add additional fields
	}
}

//**********************************************************************

bool CDatabaseReport::SaveLine( CArray<int,int>& arrayHeader, CSQLRowAccountFull& RowAccount )
{
	CCSV csv ( '\t' );

	// add start of line data sort by data
	m_DbReporter.AddOrderByData (&csv, RowAccount );			
	
	// hotlist report
	if ( m_strReportLabel == "R001" )					
	{
		CSSDateTime dtPurse1LastUsed (RowAccount.GetPurse1LastSpendDate(), RowAccount.GetPurse1LastSpendTime() );
		CSSDateTime dtPurse2LastUsed (RowAccount.GetPurse2LastSpendDate(), RowAccount.GetPurse2LastSpendTime() );
		CSSDateTime dtCashLastUsed (RowAccount.GetCashLastSpendDate(), RowAccount.GetCashLastSpendTime());
		CSSDateTime dtHotlisted (RowAccount.GetHotlistDate(), RowAccount.GetHotlistTime() );

		if (RowAccount.IsCardHotlisted() == TRUE && ( (dtPurse1LastUsed > dtHotlisted) || (dtPurse2LastUsed > dtHotlisted) || (dtCashLastUsed > dtHotlisted)) )
		{
			csv.Add ( "* Used after hotlisted" );
			++m_nWarnings[nREPORT_GROUP];
			++m_nWarnings[nREPORT_TOTAL];
		}
		else
		{
			csv.Add("");
		}
	}

	if ( m_DbReporter.GetGroupSummary() == FALSE )		// no need to print if summary report
	{
		m_DbReporter.CreateReportLine ( arrayHeader, &csv, RowAccount );
		m_Report.WriteLine ( csv );
	}

	++m_nCount[nREPORT_GROUP];
	++m_nCount[nREPORT_TOTAL];	

	return TRUE;
}

//**********************************************************************

void CDatabaseReport::SaveTotals ( CArray<int,int>& arrayHeader, int nIndex, int nGroupNo )
{
	CString strText1;
	CString strText2;

	if (nIndex == nREPORT_GROUP)
	{
		CString strGroupName = "";

		CSQLRowGroup RowGroup;
		RowGroup.SetGroupNo(nGroupNo);

		CSQLRepositoryGroup repoGroup;
		if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			strGroupName = RowGroup.GetGroupName();
		}

		if (strGroupName == "")
		{
			strGroupName.Format("Group %2.2d", nGroupNo);
		}

		strText1.Format("<..>%s Total", (const char*) strGroupName);
		strText2.Format("<..>%s Warnings", (const char*)strGroupName);
	}
	else
	{
		strText1 = "<..>Report Total";
		strText2 = "<..>Total Warnings";
	}

	CCSV csvLine1 ( '\t' );
	CCSV csvLine2 ( '\t' );
	CCSV csvTabLine ( '\t' );

	m_DbReporter.AddOrderByTotalText ( strText1, &csvLine1, &csvTabLine, m_nCount[nIndex] );// add start of line total text

	if (m_strReportLabel == "R001")					// hotlist report
	{
		csvTabLine.Add("");							// Warning column
	}

	if ( m_DbReporter.GetGroupSummary() == FALSE )
	{
		if (m_strReportLabel == "R001")					// hotlist report
		{
			csvLine1.Add("");								// Warning column
		}
		m_DbReporter.CreateTotalLine ( arrayHeader, &csvLine1, &csvTabLine, nIndex );
		m_Report.WriteLine ( csvTabLine );
		m_Report.WriteLine ( csvLine1 );

		if ( m_nWarnings[nIndex] > 0 )
		{
			csvTabLine.RemoveAll();
			m_DbReporter.AddOrderByTotalText ( strText2, &csvLine2, &csvTabLine, m_nWarnings[nIndex] );// add start of line total text
			m_Report.WriteLine ( csvLine2 );
		}

		m_Report.WriteLine ( "" );
	}
	else
	{
		if (nIndex == nREPORT_TOTAL)
		{
			m_Report.WriteLine(csvTabLine);
		}

		m_Report.WriteLine ( csvLine1 );

		if (m_nWarnings[nIndex] > 0)
		{
			m_Report.WriteLine(csvLine2);
		}
	}
}

//**********************************************************************
