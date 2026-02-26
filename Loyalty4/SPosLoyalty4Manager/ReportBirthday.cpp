/**********************************************************************/
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/**********************************************************************/
#include "DbExportEditor.h"
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
#include "WorkingDlg.h"
/**********************************************************************/
#include "ReportBirthday.h"
/**********************************************************************/

CBirthdayReport::CBirthdayReport ( CWnd* pParent )
	: m_pParent(pParent)
{
}

//*******************************************************************

void CBirthdayReport::AddToSorter ( CSQLRowAccountFull& RowAccount, bool bExpiry )
{
	bool bAcceptMonth = FALSE;
	bool bAcceptDate = FALSE;

	CString strDate = "";
	if (FALSE == bExpiry)
	{
		strDate = RowAccount.GetDOB();
	}
	else
	{
		strDate = RowAccount.GetExpiryDate();
	}

	CSSDate date ( strDate );
	if ( date.IsSet() == TRUE )
	{
		bAcceptMonth = TRUE;
		bAcceptDate = TRUE;

		if ( ( FALSE == bExpiry ) || ( m_ReportInfo.GetExpiryMonthFilterFlag() == TRUE ) )
		{
			int nMonth = date.GetMonth();
			if ((nMonth >= 1) && (nMonth <= 12))
			{
				bAcceptMonth = m_ReportInfo.GetMonthFlag(nMonth - 1);
			}
			else
			{
				bAcceptMonth = FALSE;
			}
		}
		
		if ( TRUE == bExpiry )
		{
			bAcceptDate = ( date >= m_ReportInfo.GetDateFrom() ) && ( date <= m_ReportInfo.GetDateTo() ); 
		}		
	}

	if ( ( TRUE == bAcceptDate ) && ( TRUE == bAcceptMonth ) )
	{
		CSortedDbReportItem SortItem;
		m_DbReporter.GetSortItem(RowAccount, SortItem, TRUE);

		CBirthdayReportItem item;
		item.m_nMonth = date.GetMonth();
		item.m_nDay = date.GetDay();
		item.m_strSortLabel = SortItem.m_strSortField;
		item.m_nGroupNo = SortItem.m_nGroupNo;
		item.m_nUserID = RowAccount.GetUserID();
		m_ConsolArray.Consolidate(item);
	}
}

//*******************************************************************
// Compile & show report : return of FALSE = fatal error exit calling routine

bool CBirthdayReport::Show()
{
	CString strReportLabel = "R002";

	m_dlgBirthday.SetReportInfo( &m_ReportInfo );
	if (m_dlgBirthday.DoModal() == IDCANCEL)
	{
		return FALSE;
	}

	CDbExportEditor editor ( FALSE, NULL );
	if (editor.EditLabel(strReportLabel) == FALSE)
	{
		return FALSE;
	}

	if (m_DbReporter.Read(strReportLabel) == FALSE)
	{
		return TRUE;
	}

	m_ConsolArray.RemoveAll();

	CString strReportFile = m_DbReporter.GetReportFilename();

	CRepmanHandler repman ( m_DbReporter.GetReportTitle(), m_pParent );

	int nErrorNo = 0; 
	if ( ( nErrorNo = Create ( strReportFile ) ) == nREPORT_NOERROR )
	{
		repman.SetupSaveAs ( m_DbReporter.GetSaveAsType(), m_DbReporter.GetSaveAsFile() );
		nErrorNo = repman.DisplayReport( strReportFile, m_DbReporter.GetParamsFilename(), m_DbReporter.GetReportKey() );
	}

	return repman.DisplayError ( nErrorNo );
}

//*******************************************************************

int CBirthdayReport::Create(const char* szReportFile)
{
	if (m_Report.Open(szReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CWorkingDlg WorkingDlg ( "Creating Report" );
	WorkingDlg.Create();
	bool bCancelButton = FALSE;

	CString strWhere = "";
	CSQLBuilderBind BuilderBind{};
	CSQLRepositoryAccount repoAccount;
	repoAccount.PrepareExportWhere(m_DbReporter, -1, strWhere, BuilderBind);

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ExportByParams{ &BuilderBind, strWhere });

	int nChecked = 0;
	CSQLRowAccountFull RowAccount;

	while ( RecordSet.StepSelectAll(RowAccount) == TRUE )
	{
		if (WorkingDlg.SetCaption2RecordsChecked(++nChecked) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		if ( m_DbReporter.NonSQLFilter(RowAccount) == TRUE )
		{
			if (m_ReportInfo.GetDOBFlag() == TRUE)
			{
				AddToSorter(RowAccount, FALSE);
			}

			if (m_ReportInfo.GetExpiryFlag() == TRUE)
			{
				AddToSorter(RowAccount, TRUE);
			}
		}
	}

	if ((FALSE == bCancelButton) && (m_ConsolArray.GetSize() > 0))
	{
		m_Report.SetStyle1(m_DbReporter.m_strTitle);
		AddColumnText(&m_Report);

		CArray<int, int> arrayHeader;
		LoyaltyHeaderHelpers.CreateNumericHeader(m_DbReporter.m_strHeader, arrayHeader);

		CSSDate dateToday;
		dateToday.SetCurrentDate();

		int nCount = 0;
		int nCurrentMonth = 0;
		bool bShowMonthBanner = FALSE;
		bool bShowSeparator = FALSE;

		int nSize = m_ConsolArray.GetSize();
		for (int nIndex = 0; nIndex < nSize; nIndex++)
		{
			{
				int nPercent = (50 + ((nIndex * 50L) / nSize));
				if (WorkingDlg.SetCaption2PercentDone(nPercent) == TRUE)
				{
					bCancelButton = TRUE;
					break;
				}
			}

			CBirthdayReportItem item;
			m_ConsolArray.GetAt(nIndex, item);

			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(item.m_nUserID);

			if (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
			{
				int nDOBMonth = item.m_nMonth;

				if (nDOBMonth > nCurrentMonth)						// see if month changed
				{
					ShowMonthTotal(arrayHeader, nCount);				// show previous month totals
					m_DbReporter.ClearTotals(nREPORT_GROUP);			// use nGROUP total for month totals

					nCurrentMonth = nDOBMonth;
					bShowMonthBanner = TRUE;
					nCount = 0;
				}

				// see if current date passed

				if (bShowSeparator == FALSE)
				{
					if (nDOBMonth > dateToday.GetMonth())
					{
						bShowSeparator = TRUE;
					}
					else if (nDOBMonth == dateToday.GetMonth())
					{
						int nDOBDay = item.m_nDay;

						if (nDOBDay > dateToday.GetDay())
						{
							bShowSeparator = TRUE;
						}
					}

					if ( ( TRUE == bShowSeparator ) && (nIndex > 0))
					{
						m_Report.WriteLine("<LI>\t");
					}
				}

				// see if month has changed

				if (bShowMonthBanner == TRUE)
				{
					SaveBanner(nCurrentMonth);
					bShowMonthBanner = FALSE;
				}

				// display report line

				SaveLine(arrayHeader, RowAccount);
				++nCount;
			}
		}
		ShowMonthTotal(arrayHeader, nCount);
	}

	m_Report.Close();

// exit creation

	if (bCancelButton == TRUE)
	{
		return nREPORT_CANCEL;
	}

	if (m_ConsolArray.GetSize() == 0)
	{
		return nREPORT_NOMATCH;
	}

	return nREPORT_NOERROR;
}

//**********************************************************************

void CBirthdayReport::AddColumnText(CReportFile* pReport)
{
	m_DbReporter.AddOrderByColumnText(pReport);			// add OrderBy column headings
	m_DbReporter.CreateReportHeader(pReport);			// add additional fields
}

/**********************************************************************/

bool CBirthdayReport::SaveBanner ( int nMonth )
{
	CString strText;

	switch (nMonth)
	{
	case	1:	strText = "January";	break;
	case	2:	strText = "February";	break;
	case	3:	strText = "March";		break;
	case	4:	strText = "April";		break;
	case	5:	strText = "May";		break;
	case	6:	strText = "June";		break;
	case	7:	strText = "July";		break;
	case	8:	strText = "August";		break;
	case	9:	strText = "September";	break;
	case	10:	strText = "October";	break;
	case	11:	strText = "November";	break;
	case	12:	strText = "December";	break;
	}

	strText = "<..>" + strText;
	m_Report.WriteLine ( strText );
	return m_Report.WriteLine ( "<LI>" );
}

//**********************************************************************

bool CBirthdayReport::SaveLine(CArray<int, int>& arrayHeader, CSQLRowAccountFull& RowAccount)
{
	CCSV csv('\t');

	// add start of line data sort by data
	m_DbReporter.AddOrderByData(&csv, RowAccount);
	m_DbReporter.CreateReportLine(arrayHeader, &csv, RowAccount);

	return m_Report.WriteLine(csv);
}

//**********************************************************************

void CBirthdayReport::ShowMonthTotal(CArray<int, int>& arrayHeader, int nCount)
{
	if (0 == nCount)
	{
		return;
	}

	CCSV csvLine('\t');
	CCSV csvTabLine('\t');

	m_DbReporter.AddOrderByTotalText("Total", &csvLine, &csvTabLine, nCount);
	m_DbReporter.CreateTotalLine(arrayHeader, &csvLine, &csvTabLine, nREPORT_GROUP);

	m_Report.WriteLine(csvTabLine);
	m_Report.WriteLine(csvLine);
	m_Report.WriteLine("");
}

//**********************************************************************
