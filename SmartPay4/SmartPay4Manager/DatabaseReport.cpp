/**********************************************************************/
// UR?? - Database user defined reports
// R010 - Never used
/**********************************************************************/
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_BioRegister\SQLRepositoryBioRegister.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLDb.h"
//**********************************************************************
#include "DatabaseReport.h"
/**********************************************************************/

CDatabaseReport::CDatabaseReport ( const char* szLabel, CWnd* pParent )
{
	m_strReportLabel = szLabel;
	m_pParent = pParent;
	m_strSaveAsFilename = "";
	m_strReportTitle = "";								// gets filled out later
}

//*******************************************************************

void CDatabaseReport::ClearTotals ( int nIndex )
{
	m_nCount[nIndex] = 0;
}

//*******************************************************************

bool CDatabaseReport::Show ( bool bEditReqd )
{
	if ( bEditReqd == TRUE )
	{
		CDbExportEditor editor{};
		if (editor.EditExportLabel(m_strReportLabel) == FALSE)
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

	ClearTotals ( nREPORT_SORTON );
	ClearTotals ( nREPORT_GROUP );
	ClearTotals ( nREPORT_TOTAL );

	CRepmanHandler repman ( m_strReportTitle, m_pParent );
	int nErrorNo; 
	if ( ( nErrorNo = Create ( strReportFile ) ) == nREPORT_NOERROR )
	{
		m_strSaveAsFilename = m_DbReporter.GetSaveAsFile();
		repman.SetupSaveAs ( m_DbReporter.GetSaveAsType(), m_strSaveAsFilename );
		nErrorNo = repman.DisplayReport ( strReportFile, m_DbReporter.GetParamsFilename(), m_DbReporter.GetReportKey() );
	}

	return repman.DisplayError ( nErrorNo );
}

//*******************************************************************

bool CDatabaseReport::Show( CDatabaseReportInfo& info )
{
	m_DbReporter.CopyFromReportInfo(info);

	// ok to continue & create report file

	CString strReportFile = m_DbReporter.GetReportFilename();
	m_strReportTitle = m_DbReporter.GetReportTitle();

	ClearTotals(nREPORT_SORTON);
	ClearTotals(nREPORT_GROUP);
	ClearTotals(nREPORT_TOTAL);

	CRepmanHandler repman(m_strReportTitle, m_pParent);
	int nErrorNo;
	if ((nErrorNo = Create(strReportFile)) == nREPORT_NOERROR)
	{
		m_strSaveAsFilename = m_DbReporter.GetSaveAsFile();
		repman.SetupSaveAs(m_DbReporter.GetSaveAsType(), m_strSaveAsFilename);
		nErrorNo = repman.DisplayReport(strReportFile, m_DbReporter.GetParamsFilename(), m_DbReporter.GetReportKey());
	}

	return repman.DisplayError(nErrorNo);
}

//*******************************************************************

int CDatabaseReport::Create ( const char* szReportFile )
{
	if (m_Report.Open(szReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CString strWhere = "";
	CSQLBuilderBind BuilderBind{};
	CSQLRepositoryAccount RepoAccount;
	RepoAccount.PrepareExportWhere(m_DbReporter, -1, strWhere, BuilderBind);

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ByParams{ &BuilderBind, strWhere });
	RecordSet.m_strSort = m_DbReporter.GetSQLOrderBy();
	
	m_Report.SetStyle1(m_strReportTitle);
	AddColumnText();

	CString strLastSortOn = "";
	bool bFirstLine = TRUE;
	int nGroupNo = 1;
	
	CSQLRowAccountFull RowAccount;
	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (m_DbReporter.NonSQLFilter(RowAccount) == TRUE)
		{
			bool bReqd = TRUE;
			if (m_strReportLabel == "R010")						// never used report
			{
				CSSDate date(RowAccount.GetPurse1LastSpendDate());
				if (date.IsSet() == TRUE)
				{
					bReqd = FALSE;
				}
			}

			if (TRUE == bReqd)
			{
				if ((m_DbReporter.GetSortOn() != nSORTONUSERID) && (m_DbReporter.GetTotalise() == TRUE))
				{
					CString strSortOn = m_DbReporter.GetSortFieldText(RowAccount);

					if (TRUE == bFirstLine)
					{
						strLastSortOn = strSortOn;							// only need for first entry
					}

					if (strLastSortOn != strSortOn)
					{
						SaveSortOnTotal(strLastSortOn);
						strLastSortOn = strSortOn;
						m_DbReporter.ClearTotals(nREPORT_SORTON);			// clear nSortOn totals
						ClearTotals(nREPORT_SORTON);
					}
				}

				if (m_DbReporter.GetSortByGroup() == TRUE)
				{
					if (nGroupNo != RowAccount.GetGroupNo())
					{
						SaveGroupTotal(nGroupNo);
						nGroupNo = RowAccount.GetGroupNo();
						m_DbReporter.ClearTotals(nREPORT_GROUP);
						ClearTotals(nREPORT_GROUP);
					}
				}

				SaveLine(RowAccount);
				bFirstLine = FALSE;
			}
		}
	}
	
	if ((m_DbReporter.GetSortOn() != nSORTONUSERID) && (m_DbReporter.GetTotalise() == TRUE))
	{
		SaveSortOnTotal(strLastSortOn);
	}

	if (m_DbReporter.GetSortByGroup() == TRUE)
	{
		SaveGroupTotal(nGroupNo);
	}

	SaveReportTotal();

	m_Report.Close();

	if (TRUE == bFirstLine)
	{
		return nREPORT_NOMATCH;
	}

	return nREPORT_NOERROR;
}

/**********************************************************************/

void CDatabaseReport::AddColumnText()
{
	m_DbReporter.AddOrderByColumnText();		// add OrderBy column headings

	if (m_DbReporter.GetGroupSummary() == FALSE)
	{
		m_DbReporter.CreateReportHeader(&m_Report);		// add additional fields
	}
}

//**********************************************************************

bool CDatabaseReport::SaveLine( CSQLRowAccountFull& AccountRow)
{
	CCSV csv ( '\t' );

	if ( m_DbReporter.GetGroupSummary() == FALSE )		// no need to print if summary report
	{
		m_DbReporter.AppendDbFieldsToCSVLine ( AccountRow, &csv );
		m_Report.WriteLine ( csv );
	}

	++m_nCount[nREPORT_SORTON];
	++m_nCount[nREPORT_GROUP];
	++m_nCount[nREPORT_TOTAL];	

	return TRUE;
}

//**********************************************************************

void CDatabaseReport::SaveSortOnTotal ( const char* szSortOnText )
{
	if ( m_nCount[nREPORT_SORTON] >  0 )
	{
		CString strSortOnText = szSortOnText;
		strSortOnText.TrimLeft ( ' ' );
		strSortOnText.TrimRight ( ' ' );

		SaveTotals ( nREPORT_SORTON, strSortOnText );
	}
}

//**********************************************************************

void CDatabaseReport::SaveGroupTotal ( int nGroupNo )
{
	if ( m_nCount[nREPORT_GROUP] > 0 )
	{
		CString strGroupName;
		CSQLRowGroupFull RowGroup;
		RowGroup.SetGroupNo(nGroupNo);

		CSQLRepositoryGroup RepoGroup;
		if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			strGroupName.Format("Group %2.2d", nGroupNo);
		}
		else
		{
			strGroupName = RowGroup.GetGroupName();
		}

		SaveTotals ( nREPORT_GROUP, strGroupName );
	}
}

//**********************************************************************

void CDatabaseReport::SaveReportTotal()
{
	SaveTotals ( nREPORT_TOTAL, "Report" );
}

//**********************************************************************

void CDatabaseReport::SaveTotals(int nIndex, const char* szText)
{
	CCSV csvLine('\t');
	CCSV csvTabLine('\t');

	CString strTotalText = "";
	strTotalText.Format("%s Total", szText);
	m_DbReporter.CreateTotalLine(0,strTotalText, &csvLine, &csvTabLine, nIndex, m_nCount[nIndex]);

	//m_DbReporter.AddSortOnTotalText ( strTotalText, &csvLine, &csvTabLine, m_nCount[nIndex] );	// add start of line total text

	if (m_DbReporter.GetGroupSummary() == FALSE)
	{
		m_Report.WriteLine(csvTabLine);
		m_Report.WriteLine(csvLine);
		m_Report.WriteLine("");
	}
	else
	{
		if (nIndex == nREPORT_TOTAL)
		{
			m_Report.WriteLine(csvTabLine);
		}

		m_Report.WriteLine(csvLine);
	}
}

//**********************************************************************


