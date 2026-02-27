/**********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_BioRegister\SQLRepositoryBioRegister.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "DbExportEditor.h"
#include "BirthdayReport.h"
/**********************************************************************/

CBirthdayReport::CBirthdayReport ( CWnd* pParent )
{
	m_pParent = pParent;
}

//*******************************************************************

void CBirthdayReport::AddToSorter ( bool bExpiry, CSQLRowAccountFull& RowAccount )
{
	bool bReqd = FALSE;

	CString strDate = "";
	if (TRUE == bExpiry)
	{
		strDate = RowAccount.GetExpiryDate();
	}
	else
	{
		strDate = RowAccount.GetDOB();
	}

	CSSDate date ( strDate );
	if ( date.IsSet() == TRUE )
	{
		switch ( date.GetMonth() )
		{
		case	1:	bReqd = ( m_dlgBirthday.m_bCheckMonth1 != 0 );	break;
		case	2:	bReqd = ( m_dlgBirthday.m_bCheckMonth2 != 0 );	break;
		case	3:	bReqd = ( m_dlgBirthday.m_bCheckMonth3 != 0 );	break;
		case	4:	bReqd = ( m_dlgBirthday.m_bCheckMonth4 != 0 );	break;
		case	5:	bReqd = ( m_dlgBirthday.m_bCheckMonth5 != 0 );	break;
		case	6:	bReqd = ( m_dlgBirthday.m_bCheckMonth6 != 0 );	break;
		case	7:	bReqd = ( m_dlgBirthday.m_bCheckMonth7 != 0 );	break;
		case	8:	bReqd = ( m_dlgBirthday.m_bCheckMonth8 != 0 );	break;
		case	9:	bReqd = ( m_dlgBirthday.m_bCheckMonth9 != 0 );	break;
		case	10:	bReqd = ( m_dlgBirthday.m_bCheckMonth10 != 0 );	break;
		case	11:	bReqd = ( m_dlgBirthday.m_bCheckMonth11 != 0 );	break;
		case	12:	bReqd = ( m_dlgBirthday.m_bCheckMonth12 != 0 );	break;
		}
	}

	if ( bReqd == TRUE )
	{
		CString strLabel;
		strLabel.Format ( "%2.2d%2.2d%s", 
			date.GetMonth(), 
			date.GetDay(), 
			(const char*) m_DbReporter.CreateSortLabel(RowAccount) );
		
		CSortedInt64ByString item;
		item.m_strKey = strLabel;
		item.m_nData = RowAccount.GetUserID();
		m_Sorter.Consolidate(item);
	}
}

//*******************************************************************
// Compile & show report : return of FALSE = fatal error exit calling routine

bool CBirthdayReport::Show()
{
	CString strReportLabel = "R002";

	if (m_dlgBirthday.DoModal() == IDCANCEL)
	{
		return FALSE;
	}

	CDbExportEditor editor{};
	if (editor.EditExportLabel(strReportLabel) == FALSE)
	{
		return FALSE;
	}

	if (m_DbReporter.Read(strReportLabel) == FALSE)
	{
		return FALSE;
	}

	m_Sorter.RemoveAll();

	CString strReportFile = m_DbReporter.GetReportFilename();

	CRepmanHandler repman ( m_DbReporter.GetReportTitle(), m_pParent );

	int nErrorNo; 
	if ( ( nErrorNo = Create ( strReportFile ) ) == nREPORT_NOERROR )
	{
		repman.SetupSaveAs ( m_DbReporter.GetSaveAsType(), m_DbReporter.GetSaveAsFile() );
		nErrorNo = repman.DisplayReport ( strReportFile, m_DbReporter.GetParamsFilename(), m_DbReporter.GetReportKey() );
	}

	return repman.DisplayError ( nErrorNo );
}

//*******************************************************************

int CBirthdayReport::Create ( const char* szReportFile )
{
	if (m_Report.Open(szReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}
	
	CWorkingDlg progress ( szMSG_CREATINGREPORT );
	progress.Create();
	bool bCancelButton = FALSE;

	{
		CString strWhere = "";
		CSQLBuilderBind BuilderBind{};
		CSQLRepositoryAccount RepoAccount;
		RepoAccount.PrepareExportWhere(m_DbReporter, -1, strWhere, BuilderBind);

		CSQLRecordSetAccountFull RecordSet(nullptr, RSParams_AccountFull_ByParams{ &BuilderBind, strWhere });
		RecordSet.m_strSort = m_DbReporter.GetSQLOrderBy();
		
		int nCount = 0;

		CSQLRowAccountFull RowAccount;

		while (RecordSet.StepSelectAll(RowAccount) == TRUE)
		{
			if (progress.SetCaption2RecordsChecked(++nCount) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}

			if (m_DbReporter.NonSQLFilter(RowAccount) == TRUE)
			{
				if (m_dlgBirthday.m_bDobReqd == TRUE)
				{
					AddToSorter(FALSE, RowAccount);
				}

				if (m_dlgBirthday.m_bExpiryReqd == TRUE)
				{
					AddToSorter(TRUE, RowAccount);
				}
			}
		}
	}

	if ( bCancelButton == FALSE && m_Sorter.GetSize() > 0 )
	{
		m_Report.SetStyle1 ( m_DbReporter.m_strTitle );
		AddColumnText();

		CSSDate dateToday;
		dateToday.SetCurrentDate();

		int nCount				= 0;
		int nCurrentMonth		= 0;
		bool bShowMonthBanner = FALSE;
		bool bShowSeparator	  = FALSE;

		for ( int nIndex = 0 ; nIndex < m_Sorter.GetSize() ; nIndex++ )
		{
			if (progress.SetCaption2PercentDone(50 + ((nIndex * 50L) / m_Sorter.GetSize())) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}

			CSortedInt64ByString item;	
			m_Sorter.GetAt(nIndex,item);

			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(item.m_nData);

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
			{
				CString strLabel = item.m_strKey;		// label used in sorter
				int nDobMonth = atoi ( (const char*)strLabel.Left(2) );

				if ( nDobMonth > nCurrentMonth )						// see if month changed
				{
					ShowMonthTotal ( nCount );				// show previous month totals
					m_DbReporter.ClearTotals ( nREPORT_GROUP );				// use nGROUP total for month totals

					nCurrentMonth = nDobMonth;
					bShowMonthBanner = TRUE;
					nCount = 0;
				}

// see if current date passed

				if ( bShowSeparator == FALSE )
				{
					if (nDobMonth > dateToday.GetMonth())
					{
						bShowSeparator = TRUE;
					}
					else if ( nDobMonth == dateToday.GetMonth() )
					{
						int nDobDay = atoi ( (const char*)strLabel.Mid(2,2) );

						if (nDobDay > dateToday.GetDay())
						{
							bShowSeparator = TRUE;
						}
					}

					if (bShowSeparator == TRUE && nIndex > 0)
					{
						m_Report.WriteLine("<LI>\t");
					}
				}

// see if month has changed

				if ( bShowMonthBanner == TRUE )
				{
					SaveBanner ( nCurrentMonth );
					bShowMonthBanner = FALSE;
				}

// display report line

				SaveLine(RowAccount);
				++nCount;
			}
		}
		ShowMonthTotal ( nCount);
	}

	m_Report.Close();
	
// exit creation

	if ( bCancelButton == TRUE )	return nREPORT_CANCEL;
	if ( m_Sorter.GetSize() == 0 )	return nREPORT_NOMATCH;
	
	return nREPORT_NOERROR;
}

//**********************************************************************

void CBirthdayReport::AddColumnText( )
{
	m_DbReporter.AddOrderByColumnText();					// add OrderBy column headings
	m_DbReporter.CreateReportHeader( &m_Report );			// add additional fields
}

/**********************************************************************/

bool CBirthdayReport::SaveBanner ( int nMonth )
{
	CString strText;

	switch ( nMonth )
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

bool CBirthdayReport::SaveLine(CSQLRowAccountFull& RowAccount)
{
	CCSV csv ( '\t' );
	m_DbReporter.AppendDbFieldsToCSVLine ( RowAccount, &csv );
	return m_Report.WriteLine ( csv );
}

//**********************************************************************

void CBirthdayReport::ShowMonthTotal ( int nCount )
{
	if (nCount == 0)
	{
		return;
	}

	CCSV csvLine ( '\t' );
	CCSV csvTabLine ( '\t' );

	//m_DbReporter.AddSortOnTotalText ( "Total", &csvLine, &csvTabLine, nCount );
	m_DbReporter.CreateTotalLine ( 0,"Total", & csvLine, &csvTabLine, nREPORT_GROUP, nCount);
	m_Report.WriteLine ( csvTabLine );
	m_Report.WriteLine ( csvLine );
	m_Report.WriteLine ( "" );
}

//**********************************************************************
