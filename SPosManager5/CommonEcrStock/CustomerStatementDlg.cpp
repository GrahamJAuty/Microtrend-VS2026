 /**********************************************************************/
#include "CommsHandshaker.h"
#include "EmailOptions.h"
#include "EposReportCustomerStatement.h"
#include "FileRemover.h"
#include "Password.h"
#include "RepSpawn.h"
#include "SessionCSVArray.h"
#include "SMTPEmailHelpers.h"
#include "SMTPResultsDlg.h"
/**********************************************************************/
#include "CustomerStatementDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CCustomerStatementDlg, CDialog)
/**********************************************************************/

CCustomerStatementDlg::CCustomerStatementDlg(CWnd* pParent)
	: CSSDialog(CCustomerStatementDlg::IDD, pParent),
	m_DateSelectorRange( m_comboOther, m_DatePickerFrom, m_DatePickerTo )
{
	DataManagerNonDb.CustomerNameTable.BuildList();
	DataManagerNonDb.CustomerNameTable.BuildSortOrder();
	m_nSortType = CUSTOMER_SORT_LASTNAME_INC;
	m_strFilter = "";
}

/**********************************************************************/

CCustomerStatementDlg::~CCustomerStatementDlg()
{
}

/**********************************************************************/

void CCustomerStatementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_OTHER, m_radioOther);
	DDX_Control(pDX, IDC_RADIO_MONTH, m_radioMonth);
	DDX_Control(pDX, IDC_COMBO_MONTH, m_comboMonth);
	DDX_Control(pDX, IDC_COMBO_OTHER, m_comboOther);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_COMBO_ROUND, m_comboRound);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_EDIT_LINE1, m_editLine1);
	DDX_Control(pDX, IDC_EDIT_LINE2, m_editLine2);
	DDX_Control(pDX, IDC_CHECK_BALANCE, m_checkBalance);
	DDX_Control(pDX, IDC_EDIT_FILTER, m_editFilter);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCustomerStatementDlg, CDialog)
	ON_BN_CLICKED( IDC_RADIO_MONTH, OnRadioMonth )
	ON_BN_CLICKED( IDC_RADIO_OTHER, OnRadioOther )
	ON_CBN_SELCHANGE(IDC_COMBO_MONTH, OnSelectMonth)
	ON_CBN_SELCHANGE(IDC_COMBO_OTHER, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_NONE, OnButtonSelectNone)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_EMAIL, OnButtonEmail)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_EN_KILLFOCUS( IDC_EDIT_FILTER, OnKillFocusFilter )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCustomerStatementDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.AddCustomerStatementRanges();
	m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );

	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	m_nThisYear = timeNow.GetYear();
	m_nThisMonth = timeNow.GetMonth();

	m_comboMonth.AddString( "This Month" );

	int nYear = m_nThisYear;
	int nMonth = m_nThisMonth;
	for ( int n = 0; n < 12; n++ )
	{
		nMonth--;

		if ( nMonth <= 0 )
		{
			nMonth = 12;
			nYear--;
		}

		COleDateTime timeMonth( nYear, nMonth, 1, 0, 0, 0 );
		CString strMonth = timeMonth.Format( "%B %Y" );
		m_comboMonth.AddString( strMonth );
	}

	m_comboMonth.SetCurSel( 0 );
	m_radioMonth.SetCheck( TRUE );
	m_radioOther.SetCheck( FALSE );
	OnRadioMonth();

	m_comboRound.AddString ( "Dates as selected" );
	m_comboRound.AddString ( "Nearest full week" );
	m_comboRound.AddString ( "Nearest full month" );
	m_comboRound.SetCurSel(0);

	CWordArray arrayTypes;

	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:
		arrayTypes.Add ( NODE_LOCATION );
		arrayTypes.Add ( NODE_DATABASE );
		arrayTypes.Add ( NODE_LOCATIONSET );
		arrayTypes.Add ( NODE_SYSTEM );
		break;

	case LOCATION_ACCESS_DB:
		arrayTypes.Add ( NODE_LOCATION );
		arrayTypes.Add ( NODE_DATABASE );
		arrayTypes.Add ( NODE_REPORTPOINT );
		m_LocationSelector.SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add ( NODE_LOCATION );
		m_LocationSelector.SetSingleLocIdx( PasswordArray.GetLocationAccessLocIdx() );
		break;
	}

	m_LocationSelector.ExcludeWebSales();
	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboLocation, NULL );
	
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	m_editLine1.LimitText( CUSTOMER_STATEMENT_HEADER_LINE_MAXLEN );
	m_editLine2.LimitText( CUSTOMER_STATEMENT_HEADER_LINE_MAXLEN );
	
	CDataManagerInfo info;
	DataManagerNonDb.OpenCustomerHeader( DB_READONLY, info );
	m_editLine1.SetWindowText( DataManagerNonDb.CustomerHeader.GetLine(0) );
	m_editLine2.SetWindowText( DataManagerNonDb.CustomerHeader.GetLine(1) );
	DataManagerNonDb.CloseCustomerHeader( info );

	m_listCustomers.SubclassDlgItem ( IDC_LIST, this );
	m_listCustomers.InsertColumn ( 0, "", LVCFMT_LEFT, 25 );
	m_listCustomers.InsertColumn ( 1, "Location", LVCFMT_LEFT, 140 );
	m_listCustomers.InsertColumn ( 2, "Code", LVCFMT_LEFT, 80 );
	m_listCustomers.InsertColumn ( 3, "First Name", LVCFMT_LEFT, 130 );
	m_listCustomers.InsertColumn ( 4, "Last Name", LVCFMT_LEFT, 130 );
	m_listCustomers.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );

	m_editFilter.LimitText( 20 );

	UpdateCustomerList();

	return TRUE;  
}

/**********************************************************************/

void CCustomerStatementDlg::OnRadioMonth()
{
	m_comboMonth.EnableWindow( TRUE );
	m_comboOther.EnableWindow( FALSE );
	OnSelectMonth();
}

/**********************************************************************/

void CCustomerStatementDlg::OnRadioOther()
{
	m_comboMonth.EnableWindow( FALSE );
	m_comboOther.EnableWindow( TRUE );
	OnSelectDate();
}

/**********************************************************************/

void CCustomerStatementDlg::OnSelectMonth() 
{
	int nYear = m_nThisYear;
	int nMonth = m_nThisMonth;

	nMonth -= m_comboMonth.GetCurSel();

	while( nMonth < 1 )
	{
		nMonth += 12;
		nYear--;
	}

	m_DateSelectorRange.UpdateDateControlsByMonth( nYear, nMonth );
	m_DatePickerFrom.EnableWindow( FALSE );
	m_DatePickerTo.EnableWindow( FALSE );
}

/**********************************************************************/

void CCustomerStatementDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CCustomerStatementDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CCustomerStatementDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CCustomerStatementDlg::GetReportDates( COleDateTime& dateFrom, COleDateTime& dateTo )
{
	m_DatePickerFrom.GetTime( dateFrom );
	m_DatePickerTo.GetTime( dateTo );

	switch ( m_comboRound.GetCurSel() )
	{
	case 1:	//ROUND TO WEEK
		{
			int nWeekStartDay = EcrmanOptions.GetReportsWeekStartDay();
				
			int nWeekDayFrom = dateFrom.GetDayOfWeek() - 1;
			int nOffsetFrom = nWeekDayFrom - nWeekStartDay;
			if ( nOffsetFrom < 0 ) nOffsetFrom += 7;
			dateFrom -= COleDateTimeSpan ( nOffsetFrom, 0, 0, 0 );

			int nWeekDayTo = dateTo.GetDayOfWeek() - 1;
			int nOffsetTo = ( nWeekStartDay - nWeekDayTo ) - 1;
			if ( nOffsetTo < 0 ) nOffsetTo += 7;
			dateTo += COleDateTimeSpan ( nOffsetTo, 0, 0, 0 );
		}
		break;

	case 2:	//ROUND TO MONTH
		{
			int nYearFrom = dateFrom.GetYear();
			int nMonthFrom = dateFrom.GetMonth();
			dateFrom = COleDateTime( nYearFrom, nMonthFrom, 1, 0, 0, 0 );
		
			int nYearTo = dateTo.GetYear();
			int nMonthTo = dateTo.GetMonth();
				
			nMonthTo++;
			if ( nMonthTo > 12 )
			{
				nYearTo++;
				nMonthTo = 1;
			}

			dateTo = COleDateTime( nYearTo, nMonthTo, 1, 0, 0, 0 );
			dateTo -= COleDateTimeSpan ( 1, 0, 0, 0 );
		}

	default:
		break;
	}
}

/**********************************************************************/

void CCustomerStatementDlg::OnSelectLocation() 
{
	for ( int n = 0; n < m_listCustomers.GetItemCount(); n++ )
	{
		int nIdx = m_listCustomers.GetItemData(n);

		CCustomerSelectionListAccountInfo info;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nIdx, info );
		info.m_bIsSelectedTemp = ( m_listCustomers.GetCheck( n ) != 0 );
		DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nIdx, info );
	}

	m_editFilter.GetWindowText( m_strFilter );
	UpdateCustomerList();
}

/**********************************************************************/

void CCustomerStatementDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nColumn = pNMListView->iSubItem;
	
	int nNewSortType = -1;

	switch( nColumn )
	{
	case 2:
		nNewSortType = ( CUSTOMER_SORT_ID_INC == m_nSortType ) ? CUSTOMER_SORT_ID_DEC : CUSTOMER_SORT_ID_INC;
		break;

	case 3:
		nNewSortType = ( CUSTOMER_SORT_FIRSTNAME_INC == m_nSortType ) ? CUSTOMER_SORT_FIRSTNAME_DEC : CUSTOMER_SORT_FIRSTNAME_INC;
		break;

	case 4:
		nNewSortType = ( CUSTOMER_SORT_LASTNAME_INC == m_nSortType ) ? CUSTOMER_SORT_LASTNAME_DEC : CUSTOMER_SORT_LASTNAME_INC;
		break;
	}

	if ( nNewSortType != -1 )
	{
		for ( int n = 0; n < m_listCustomers.GetItemCount(); n++ )
		{
			int nIdx = m_listCustomers.GetItemData(n);

			CCustomerSelectionListAccountInfo info;
			DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nIdx, info );
			info.m_bIsSelectedTemp = ( m_listCustomers.GetCheck( n ) != 0 );
			DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nIdx, info );
		}

		DataManagerNonDb.CustomerNameTable.Account.BuildSortOrder( nNewSortType );
		m_nSortType = nNewSortType;

		m_editFilter.GetWindowText( m_strFilter );
		UpdateCustomerList();
	}

	*pResult = 0;
}

/**********************************************************************/

void CCustomerStatementDlg::OnKillFocusFilter()
{
	CString strFilter;
	m_editFilter.GetWindowText( strFilter );

	if ( strFilter != m_strFilter )
	{
		m_strFilter = strFilter;

		for ( int nIndex = 0; nIndex < m_listCustomers.GetItemCount(); nIndex++ )
		{
			int nArrayIdx = m_listCustomers.GetItemData(nIndex);

			CCustomerSelectionListAccountInfo info;
			DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nArrayIdx, info );
			info.m_bIsSelectedTemp = ( m_listCustomers.GetCheck(nIndex) != 0 );
			DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nArrayIdx, info );
		}

		UpdateCustomerList();
	}
}

/**********************************************************************/

void CCustomerStatementDlg::UpdateCustomerList()
{
	int nFilter = 0;
	CString strFilter = m_strFilter;
	strFilter.MakeUpper();
	
	if ( ( strFilter.GetLength() <= 8 ) && ( ::TestNumeric( strFilter ) == TRUE ) )
	{
		nFilter = atoi( strFilter );
		strFilter = "";
	}

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	
	CEposSelectArray SelectArray;
	LocSelEntity.GetEPOSSelectArray( SelectArray );

	m_listCustomers.DeleteAllItems();

	int nListPos = 0;
	for ( int nSortOrderIdx = 0; nSortOrderIdx < DataManagerNonDb.CustomerNameTable.Account.GetSortOrderSize(); nSortOrderIdx++ )
	{
		int nArrayIdx = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderItem(nSortOrderIdx);

		CCustomerSelectionListAccountInfo info;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nArrayIdx, info );

		bool bAccept = TRUE;

		if ( SelectArray.GotTerminals( info.m_nLocIdx ) == FALSE )
			bAccept = FALSE;
		else if ( ( nFilter != 0 ) && ( nFilter != info.m_nCustomerID ) )
			bAccept = FALSE;
		else if ( strFilter != "" )
		{
			CString strNameTest = "";
			strNameTest += info.m_strName1;
			strNameTest += " ";
			strNameTest += info.m_strName2;
			strNameTest.MakeUpper();
			bAccept = ( strNameTest.Find( strFilter ) != -1 );
		}

		if ( TRUE == bAccept )
		{
			CString strCode;
			strCode.Format( "%8.8d", info.m_nCustomerID );

			m_listCustomers.InsertItem ( nListPos, "" );
			m_listCustomers.SetItem ( nListPos, 1, LVIF_TEXT, dbLocation.GetName( info.m_nLocIdx ), 0, 0, 0, NULL );
			m_listCustomers.SetItem ( nListPos, 2, LVIF_TEXT, strCode, 0, 0, 0, NULL );
			m_listCustomers.SetItem ( nListPos, 3, LVIF_TEXT, info.m_strName1, 0, 0, 0, NULL );
			m_listCustomers.SetItem ( nListPos, 4, LVIF_TEXT, info.m_strName2, 0, 0, 0, NULL );
			m_listCustomers.SetCheck ( nListPos, info.m_bIsSelectedTemp );
		
			m_listCustomers.SetItemData( nListPos, nArrayIdx );
			nListPos++;	
		}
	}

	m_listCustomers.SetCurSel(0);
}

/**********************************************************************/

void CCustomerStatementDlg::OnButtonSelectAll() 
{
	for ( int nIndex = 0; nIndex < m_listCustomers.GetItemCount(); nIndex++ )
		m_listCustomers.SetCheck ( nIndex, TRUE );

	for ( int nArrayIdx = 0; nArrayIdx < DataManagerNonDb.CustomerNameTable.Account.GetSize(); nArrayIdx++ )
	{
		CCustomerSelectionListAccountInfo info;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nArrayIdx, info );
		info.m_bIsSelectedTemp = TRUE;
		DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nArrayIdx, info );
	}
}

/**********************************************************************/

void CCustomerStatementDlg::OnButtonSelectNone() 
{
	for ( int nIndex = 0; nIndex < m_listCustomers.GetItemCount(); nIndex++ )
		m_listCustomers.SetCheck ( nIndex, FALSE );

	for ( int nArrayIdx = 0; nArrayIdx < DataManagerNonDb.CustomerNameTable.Account.GetSize(); nArrayIdx++ )
	{
		CCustomerSelectionListAccountInfo info;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nArrayIdx, info );
		info.m_bIsSelectedTemp = FALSE;
		DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nArrayIdx, info );
	}
}

/**********************************************************************/

void CCustomerStatementDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonView();
	*pResult = 0;
}

/**********************************************************************/

void CCustomerStatementDlg::OnButtonView()
{
	OnKillFocusFilter();

	{
		CWaitCursor wait;
		CommsHandshaker.ProcessSales();
	}

	for ( int nArrayIdx = 0; nArrayIdx < DataManagerNonDb.CustomerNameTable.Account.GetSize(); nArrayIdx++ )
	{
		CCustomerSelectionListAccountInfo info;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nArrayIdx, info );
		info.m_bIsSelected = info.m_bIsSelectedTemp;
		DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nArrayIdx, info );
	}

	m_arrayErrors.RemoveAll();

	int nSel = m_listCustomers.GetCurSel();

	if ( ( nSel >= 0 ) && ( nSel < m_listCustomers.GetItemCount() ) )
	{
		int nNameTableIdx = m_listCustomers.GetItemData(nSel);

		int nResult = 0;
		CString strError = "";
		CreateStatement( nNameTableIdx, CUSTOMER_STATEMENT_DISPLAY, nResult, strError );
		
		if ( nResult != CUSTOMER_STATEMENT_ERROR_NONE )
		{
			CCustomerStatementErrorInfo infoError;
			infoError.m_nNameTableIdx = nNameTableIdx;
			infoError.m_nStatementError = nResult;
			infoError.m_nSMTPError = SMTP_ERROR_NONE;
			infoError.m_strErrorText = strError;
			m_arrayErrors.Add( infoError );
		}

		ShowErrors( CUSTOMER_STATEMENT_DISPLAY, 1 );
		m_listCustomers.SetFocus();
		m_listCustomers.Invalidate();
	}	
}

/**********************************************************************/

void CCustomerStatementDlg::OnButtonPrint()
{
	OnKillFocusFilter();

	{
		CWaitCursor wait;
		CommsHandshaker.ProcessSales();
	}

	for ( int nArrayIdx = 0; nArrayIdx < DataManagerNonDb.CustomerNameTable.Account.GetSize(); nArrayIdx++ )
	{
		CCustomerSelectionListAccountInfo info;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nArrayIdx, info );
		info.m_bIsSelected = info.m_bIsSelectedTemp;
		DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nArrayIdx, info );
	}

	int nErrorPos = 0;
	m_arrayErrors.RemoveAll();

	int nAttempted = 0;
	for ( int nListIdx = 0; nListIdx < m_listCustomers.GetItemCount(); nListIdx++ )
	{
		if ( m_listCustomers.GetCheck( nListIdx ) != 0 )
		{
			nAttempted++;

			int nNameTableIdx = m_listCustomers.GetItemData(nListIdx);
			
			int nResult = 0;
			CString strError = "";
			CreateStatement( nNameTableIdx, CUSTOMER_STATEMENT_PRINT, nResult, strError );
		
			if ( nResult != CUSTOMER_STATEMENT_ERROR_NONE )
			{
				CCustomerStatementErrorInfo infoError;
				infoError.m_nNameTableIdx = nNameTableIdx;
				infoError.m_nStatementError = nResult;
				infoError.m_nSMTPError = SMTP_ERROR_NONE;
				infoError.m_strErrorText = strError;
				m_arrayErrors.InsertAt( nErrorPos++, infoError );
			}
			else
			{
				CCustomerStatementErrorInfo infoError;
				infoError.m_nNameTableIdx = nNameTableIdx;
				infoError.m_nStatementError = CUSTOMER_STATEMENT_ERROR_NONE;
				infoError.m_nSMTPError = SMTP_ERROR_NONE;
				infoError.m_strErrorText = strError;
				m_arrayErrors.Add( infoError );
			}
		}
	}

	ShowErrors( CUSTOMER_STATEMENT_PRINT, nAttempted );
	m_listCustomers.SetFocus();
	m_listCustomers.Invalidate();
}

/**********************************************************************/

void CCustomerStatementDlg::OnButtonEmail()
{
	OnKillFocusFilter();

	{
		CWaitCursor wait;
		CommsHandshaker.ProcessSales();
	}

	for ( int nArrayIdx = 0; nArrayIdx < DataManagerNonDb.CustomerNameTable.Account.GetSize(); nArrayIdx++ )
	{
		CCustomerSelectionListAccountInfo info;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nArrayIdx, info );
		info.m_bIsSelected = info.m_bIsSelectedTemp;
		DataManagerNonDb.CustomerNameTable.Account.SetCustomerStatus( nArrayIdx, info );
	}

	if (EmailOptions.GetEmailMethodToUse() != 0)
	{
		EmailSMTP();
	}
	else
	{
		EmailMAPI();
	}

	m_listCustomers.SetFocus();
	m_listCustomers.Invalidate();
}

/**********************************************************************/

void CCustomerStatementDlg::EmailMAPI()
{
	int nErrorCount = 0;
	int nRedirectCount = 0;
	m_arrayErrors.RemoveAll();

	CString strEmailFolder = "Email";
	::GetSyssetProgramPath( strEmailFolder );
	::CreateSubdirectory( strEmailFolder );
	strEmailFolder += "\\Statement";
	::CreateSubdirectory( strEmailFolder );
	strEmailFolder += "\\";

	int nAttempted = 0;
	for ( int nListIdx = 0; nListIdx < m_listCustomers.GetItemCount(); nListIdx++ )
	{
		if ( m_listCustomers.GetCheck( nListIdx ) != 0 )
		{
			nAttempted++;

			int nNameTableIdx = m_listCustomers.GetItemData(nListIdx);

			CCustomerSelectionListAccountInfo infoCustomer;
			DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nNameTableIdx, infoCustomer );

			if ( TRUE == infoCustomer.m_bEmailRedirect )
			{
				int nResult = 0;
				CString strError = "";
				CreateStatement( nNameTableIdx, CUSTOMER_STATEMENT_PRINT, nResult, strError );
		
				if ( nResult != CUSTOMER_STATEMENT_ERROR_NONE )
				{
					CCustomerStatementErrorInfo infoError;
					infoError.m_nNameTableIdx = nNameTableIdx;
					infoError.m_nStatementError = nResult;
					infoError.m_nSMTPError = SMTP_ERROR_NONE;
					infoError.m_strErrorText = strError;
					m_arrayErrors.InsertAt( nErrorCount++, infoError );
				}
				else
				{
					CCustomerStatementErrorInfo infoError;
					infoError.m_nNameTableIdx = nNameTableIdx;
					infoError.m_nStatementError = CUSTOMER_STATEMENT_ERROR_REDIRECT;
					infoError.m_nSMTPError = SMTP_ERROR_NONE;
					infoError.m_strErrorText = "Redirected to printer";
					m_arrayErrors.InsertAt( nErrorCount + nRedirectCount++, infoError );
				}
			}
			else if ( infoCustomer.m_strEmail == "" )
			{
				CCustomerStatementErrorInfo infoError;
				infoError.m_nNameTableIdx = nNameTableIdx;
				infoError.m_nStatementError = CUSTOMER_STATEMENT_ERROR_NOADDRESS;
				infoError.m_nSMTPError = SMTP_ERROR_NONE;
				infoError.m_strErrorText = "";
				m_arrayErrors.InsertAt( nErrorCount++, infoError );
			}
			else
			{
				int nResult = 0;
				CString strError = "";
				CreateStatement( nNameTableIdx, CUSTOMER_STATEMENT_EMAIL, nResult, strError );

				if ( nResult != CUSTOMER_STATEMENT_ERROR_NONE )
				{
					CCustomerStatementErrorInfo infoError;
					infoError.m_nNameTableIdx = nNameTableIdx;
					infoError.m_nStatementError = nResult;
					infoError.m_nSMTPError = SMTP_ERROR_NONE;
					infoError.m_strErrorText = strError;
					m_arrayErrors.Add( infoError );
				}
				else
				{
					CString strEmailFilename;

					strEmailFilename.Format( "%3.3d_%6.6d_%6.6d_%d.HTM",
						Sysset.GetInstanceNo(),
						dbLocation.GetNetworkNo( infoCustomer.m_nLocIdx ),
						dbLocation.GetLocNo( infoCustomer.m_nLocIdx ),
						infoCustomer.m_nCustomerID );

					int nResult = 0;
					CString strError = "";
					CreateStatement( nNameTableIdx, CUSTOMER_STATEMENT_EMAIL, nResult, strError );

					if ( nResult != CUSTOMER_STATEMENT_ERROR_NONE )
					{
						CCustomerStatementErrorInfo infoError;
						infoError.m_nNameTableIdx = nNameTableIdx;
						infoError.m_nStatementError = nResult;
						infoError.m_nSMTPError = SMTP_ERROR_NONE;
						infoError.m_strErrorText = strError;
						m_arrayErrors.Add( infoError );
					}
					else
					{	
						CString strEmailFilepath = strEmailFolder + "\\";
						strEmailFilepath += strEmailFilename;

						CReportFileDoc ReportFileDoc ( Super.ReportFilename() );
						ReportFileDoc.SaveAsHTML( strEmailFilepath );

						char cBuffer [ _MAX_PATH ];
						cBuffer[0] = 0;
						strcat_s ( cBuffer, strEmailFilepath );

						CSSMapi32 EmailHandler;
						bool bResult = EmailHandler.SendMessage( infoCustomer.m_strEmail, "Customer Statement", cBuffer, TRUE );

						if ( bResult == FALSE )
						{
							CCustomerStatementErrorInfo infoError;
							infoError.m_nNameTableIdx = nNameTableIdx;
							infoError.m_nStatementError = CUSTOMER_STATEMENT_ERROR_EMAIL;
							infoError.m_nSMTPError = SMTP_ERROR_NONE;
							infoError.m_strErrorText = EmailHandler.GetError();
							m_arrayErrors.InsertAt( nErrorCount++, infoError );
						}
						else
						{
							CCustomerStatementErrorInfo infoError;
							infoError.m_nNameTableIdx = nNameTableIdx;
							infoError.m_nStatementError = CUSTOMER_STATEMENT_ERROR_NONE;
							infoError.m_nSMTPError = SMTP_ERROR_NONE;
							infoError.m_strErrorText = EmailHandler.GetError();
							m_arrayErrors.Add( infoError );
						}
					}
				}
			}
		}
	}

	ShowErrors( CUSTOMER_STATEMENT_EMAIL, nAttempted );
}

/**********************************************************************/

void CCustomerStatementDlg::EmailSMTP()
{
	int nErrorCount = 0;
	int nRedirectCount = 0;
	m_arrayErrors.RemoveAll();

	CSMTPEmailHelpers SMTPEmailHelpers;
	SMTPEmailHelpers.OpenSession( 1 );

	int nAttempted = 0;
	for ( int nListIdx = 0; nListIdx < m_listCustomers.GetItemCount(); nListIdx++ )
	{
		if ( m_listCustomers.GetCheck( nListIdx ) != 0 )
		{
			nAttempted++;

			int nNameTableIdx = m_listCustomers.GetItemData(nListIdx);

			CCustomerSelectionListAccountInfo infoCustomer;
			DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nNameTableIdx, infoCustomer );

			CCSV csvFriendlyText;
			csvFriendlyText.Add( dbLocation.GetName( infoCustomer.m_nLocIdx ) );
			csvFriendlyText.Add( infoCustomer.GetReportName() );
			CString strFriendlyText = csvFriendlyText.GetLine();

			if ( TRUE == infoCustomer.m_bEmailRedirect )
			{
				int nResult = 0;
				CString strError = "";
				CreateStatement( nNameTableIdx, CUSTOMER_STATEMENT_PRINT, nResult, strError );
		
				if (nResult != CUSTOMER_STATEMENT_ERROR_NONE)
				{
					SMTPEmailHelpers.FlagUnsendableEmail(strFriendlyText, nResult);
				}
				else
				{
					SMTPEmailHelpers.FlagRedirectedEmail(strFriendlyText);
				}
			}
			else if (infoCustomer.m_strEmail == "")
			{
				SMTPEmailHelpers.FlagUnsendableEmail(strFriendlyText, CUSTOMER_STATEMENT_ERROR_NOADDRESS);
			}
			else
			{
				int nResult = 0;
				CString strError = "";
				CreateStatement( nNameTableIdx, CUSTOMER_STATEMENT_EMAIL, nResult, strError );

				if (nResult != CUSTOMER_STATEMENT_ERROR_NONE)
				{
					SMTPEmailHelpers.FlagUnsendableEmail(strFriendlyText, nResult);
				}
				else
				{
					CString strEmailFilename;
					strEmailFilename.Format( "%6.6d_%d",
						infoCustomer.m_nLocIdx,
						infoCustomer.m_nCustomerID );
				
					CReportFileDoc ReportFileDoc ( Super.ReportFilename() );
					SMTPEmailHelpers.CreatePendingEmail( infoCustomer.m_strEmail, "Customer Statement", csvFriendlyText.GetLine(), ReportFileDoc, strEmailFilename );
				}
			}
		}
	}

	SMTPEmailHelpers.CreateIndexFile();
	CSMTPResultsDlg dlg( 1, SMTPEmailHelpers, this );
	dlg.DoModal();
	SMTPEmailHelpers.DeleteSession();
}

/**********************************************************************/

void CCustomerStatementDlg::CreateStatement( int nNameTableIdx, int nMode, int& nResult, CString& strError )
{
	CString strLine1, strLine2;
	m_editLine1.GetWindowText( strLine1 );
	m_editLine2.GetWindowText( strLine2 );

	nResult = CUSTOMER_STATEMENT_ERROR_NONE;
	strError = "";

	COleDateTime dateFrom, dateTo;
	GetReportDates( dateFrom, dateTo );

	CCustomerSelectionListAccountInfo infoCustomer;
	DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nNameTableIdx, infoCustomer );

	CEposSelectArray SelectArray;
	SelectArray.SelectLocation( infoCustomer.m_nLocIdx );

	CEposReportCustomerStatement Report ( SelectArray, nNameTableIdx );

	CSessionCSVRecord DummySession;
	Report.SetDates ( dateFrom, dateTo, DummySession );
	Report.SetConLevelTitle( dbLocation.GetName( infoCustomer.m_nLocIdx ) );
	Report.SetSessionTitle( "" );
	Report.SetItemServerFlag( FALSE );
	Report.SetConLevel( NODE_LOCATION );
	Report.SetPMSPendingFlag( FALSE );
	Report.SetPMSActiveFlag( FALSE );
	Report.SetPMSCompleteFlag( FALSE );
	Report.SetPMSCancelledFlag( FALSE );
	Report.SetPMSEposFlag( TRUE );
	Report.SetPMSDateMode( PMS_DATEMODE_ACTUAL );
	Report.SetHeaderLine1( strLine1 );
	Report.SetHeaderLine2( strLine2 );
	Report.SetShowRollingBalanceFlag( IsTicked( m_checkBalance ) );

	Report.Consolidate();
	nResult = Report.GetConsolidationError();
	if (CUSTOMER_STATEMENT_ERROR_NONE != nResult)
	{
		return;
	}

	if ( Report.CreateReport() == FALSE )
	{
		nResult = CUSTOMER_STATEMENT_ERROR_CREATE;
		return;
	}
	
	switch( nMode )
	{
	case CUSTOMER_STATEMENT_PRINT:
		{
			CReportManager ReportManager;
			if (ReportManager.PrintSilent(Super.ReportFilename(), Report.GetParamsFilename(), "", m_pParentWnd) == FALSE)
			{
				nResult = CUSTOMER_STATEMENT_ERROR_PRINT;
			}
		}
		break;

	case CUSTOMER_STATEMENT_DISPLAY:
		{
			CReportManager ReportManager;
			if (ReportManager.DisplayReport(Super.ReportFilename(), Report.GetParamsFilename(), "", m_pParentWnd) == FALSE)
			{
				nResult = CUSTOMER_STATEMENT_ERROR_DISPLAY;
			}
		}
		break;
	}
}

/**********************************************************************/

void CCustomerStatementDlg::ShowErrors( int nType, int nAttempted )
{
	if ( m_arrayErrors.GetSize() != 0 )
	{
		CCustomerStatementErrorDlg dlg( nType, nAttempted, m_arrayErrors, this );
		dlg.DoModal();
	}
}

/**********************************************************************/

void CCustomerStatementDlg::OnCancel()
{
	SaveHeaderLines();
}

/**********************************************************************/

void CCustomerStatementDlg::OnOK()
{
	SaveHeaderLines();
}

/**********************************************************************/

void CCustomerStatementDlg::SaveHeaderLines()
{
	CDataManagerInfo info;
	DataManagerNonDb.OpenCustomerHeader( DB_READWRITE, info );
	
	CString str1, str2;
	m_editLine1.GetWindowText( str1 );
	m_editLine2.GetWindowText( str2 );

	DataManagerNonDb.CustomerHeader.SetLine( 0, str1 );
	DataManagerNonDb.CustomerHeader.SetLine( 1, str2 );
	
	DataManagerNonDb.WriteCustomerHeader( info );
	DataManagerNonDb.CloseCustomerHeader( info );

	EndDialog( IDOK );
}

/**********************************************************************/
