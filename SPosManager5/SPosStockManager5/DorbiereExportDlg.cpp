/**********************************************************************/
#include "CommsHandshaker.h"
#include "EposReportGeneralDorbiere.h"
#include "Password.h"
#include "ReportTypes.h"
#include "RepSpawn.h"
#include "..\CommonEcrStock\SessionEditDlg.h"
/**********************************************************************/
#include "DorbiereExportDlg.h"
/**********************************************************************/

CDorbiereExportDlg::CDorbiereExportDlg( CWnd* pParent)
	: CSSDialog(CDorbiereExportDlg::IDD, pParent), 
		m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CDorbiereExportDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CDorbiereExportDlg::~CDorbiereExportDlg()
{
}

/**********************************************************************/

void CDorbiereExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDorbiereExportDlg)
	DDX_Control(pDX, IDC_COMBO_ROUND, m_comboRound);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_COMBO_TERMINAL, m_comboTerminal);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDorbiereExportDlg, CDialog)
	//{{AFX_MSG_MAP(CDorbiereExportDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDorbiereExportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CWordArray arrayTypes;
	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:
		arrayTypes.Add ( NODE_LOCATION );
		arrayTypes.Add ( NODE_DATABASE );
		arrayTypes.Add ( NODE_LOCATIONSET );
		arrayTypes.Add ( NODE_REPORTPOINT );
		arrayTypes.Add ( NODE_SYSTEM );
		break;

	case LOCATION_ACCESS_DB:
		arrayTypes.Add ( NODE_LOCATION );
		arrayTypes.Add ( NODE_DATABASE );
		arrayTypes.Add ( NODE_LOCATIONSET );
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
	m_LocationSelector.FillLocationCombo ( &m_comboLocation, &m_comboTerminal );
	
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( LocSelEntity.GetConType() != NODE_DATABASE )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	}
	else
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( 0, info, FALSE );
	}

	CDialog::OnInitDialog();
	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.AddStandardReportRanges();
	m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );

	{
		int nResult = m_comboDates.FindStringExact( 0, EcrmanOptions.GetReportsDefaultDateRange() );
			
		if ( nResult != CB_ERR )
			m_comboDates.SetCurSel( nResult );
		else
			m_comboDates.SetCurSel( 0 );

		OnSelectDate();
	}

	for ( int nIndex = 0; nIndex < DataManagerNonDb.ReportSession.GetSize(); nIndex++ )
		m_comboSession.AddString( DataManagerNonDb.ReportSession.GetName( nIndex ) );

	m_comboSession.AddString( "Adhoc Session" );

	if ( m_comboSession.GetCount() < 2 )
		m_comboSession.InsertString ( 0, "00:00 to 23:59" );
	
	m_comboSession.EnableWindow( m_comboSession.GetCount() > 1 );
	m_comboSession.SetCurSel(0);

	m_comboRound.AddString ( "Dates as selected" );
	m_comboRound.AddString ( "Nearest full week" );
	m_comboRound.AddString ( "Nearest full month" );
	m_comboRound.SetCurSel(0);
	
	return TRUE;  
}

/**********************************************************************/

void CDorbiereExportDlg::OnSelectLocation() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		m_LocationSelector.FillEposCombo();	
		
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	}
}

/**********************************************************************/

void CDorbiereExportDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CDorbiereExportDlg::GetReportDates()
{
	m_DatePickerFrom.GetTime( m_oleDateFrom );
	m_DatePickerTo.GetTime( m_oleDateTo );

	switch ( m_comboRound.GetCurSel() )
	{
	case 1:	//ROUND TO WEEK
		{
			int nWeekStartDay = EcrmanOptions.GetReportsWeekStartDay();
				
			int nWeekDayFrom = m_oleDateFrom.GetDayOfWeek() - 1;
			int nOffsetFrom = nWeekDayFrom - nWeekStartDay;
			if ( nOffsetFrom < 0 ) nOffsetFrom += 7;
			m_oleDateFrom -= COleDateTimeSpan ( nOffsetFrom, 0, 0, 0 );

			int nWeekDayTo = m_oleDateTo.GetDayOfWeek() - 1;
			int nOffsetTo = ( nWeekStartDay - nWeekDayTo ) - 1;
			if ( nOffsetTo < 0 ) nOffsetTo += 7;
			m_oleDateTo += COleDateTimeSpan ( nOffsetTo, 0, 0, 0 );
		}
		break;

	case 2:	//ROUND TO MONTH
		{
			int nYearFrom = m_oleDateFrom.GetYear();
			int nMonthFrom = m_oleDateFrom.GetMonth();
			m_oleDateFrom = COleDateTime( nYearFrom, nMonthFrom, 1, 0, 0, 0 );
		
			int nYearTo = m_oleDateTo.GetYear();
			int nMonthTo = m_oleDateTo.GetMonth();
				
			nMonthTo++;
			if ( nMonthTo > 12 )
			{
				nYearTo++;
				nMonthTo = 1;
			}

			m_oleDateTo = COleDateTime( nYearTo, nMonthTo, 1, 0, 0, 0 );
			m_oleDateTo -= COleDateTimeSpan ( 1, 0, 0, 0 );
		}

	default:
		break;
	}

	int n = m_comboSession.GetCurSel();
	if ( n >= 0 && n < DataManagerNonDb.ReportSession.GetSize() )
	{
		CSessionCSVRecord Session;
		DataManagerNonDb.ReportSession.GetAt ( n, m_ReportSession );
	}
	else
		m_ReportSession.ClearRecord();
}

/**********************************************************************/

bool CDorbiereExportDlg::GetAdhocReportSession()
{
	CSessionEditDlg dlg( m_ReportSessionAdhoc, TRUE, this );

	if ( dlg.DoModal() != IDOK )
		return FALSE;
	
	CCSV csv;
	m_ReportSessionAdhoc.ConvertToCSV( csv );
	m_ReportSession.ConvertFromCSV( csv );
	return TRUE;
}

/**********************************************************************/

void CDorbiereExportDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CDorbiereExportDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CDorbiereExportDlg::OnButtonDisplay() 
{
	CreateReport( FALSE );
}

/**********************************************************************/

void CDorbiereExportDlg::OnButtonExport() 
{
	CreateReport( TRUE );
}

/**********************************************************************/

bool CDorbiereExportDlg::CreateReport( bool bExport ) 
{
	CWaitCursor wait;
	CommsHandshaker.ProcessSales();

	GetReportDates();

	if ( m_comboSession.GetCurSel() == m_comboSession.GetCount() - 1 )
		if ( GetAdhocReportSession() == FALSE )
			return FALSE;

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	LocSelEntity.GetEPOSSelectArray ( m_SelectArray );

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );

	CString strTitle = LocSelEntity.GetConsolidationTitle();
	
	CString strSessionTitle;
	switch( m_comboSession.GetCount() - m_comboSession.GetCurSel() )
	{
	case 1:
		strSessionTitle = m_ReportSessionAdhoc.GetName();
		break;

	default:
		m_comboSession.GetWindowText( strSessionTitle );
		break;
	}

	CEposReportGeneralDorbiere DorbiereReport( m_SelectArray );
	DorbiereReport.SetPreferConsolType( EPOSREPORT_CONSOL_ADHOC );
	
	COleDateTime dateTo = m_oleDateTo;
	COleDateTime dateFrom = m_oleDateFrom;
	
	DorbiereReport.SetDates ( dateFrom, dateTo, m_ReportSession );
	DorbiereReport.SetSessionTitle( strSessionTitle );

	DorbiereReport.SetConLevelTitle( strTitle );
	DorbiereReport.SetItemServerFlag( FALSE );
	DorbiereReport.SetConLevel( LocSelEntity.GetConType() );

	DorbiereReport.SetPMSPendingFlag( FALSE );
	DorbiereReport.SetPMSActiveFlag( FALSE );
	DorbiereReport.SetPMSCompleteFlag( FALSE );
	DorbiereReport.SetPMSCancelledFlag( FALSE );
	DorbiereReport.SetPMSEposFlag( TRUE );
	DorbiereReport.SetPMSDateMode( PMS_DATEMODE_ACTUAL );
	
	DorbiereReport.m_ReportFilters.DeptFilter.SetDeptFilterNo( DEPARTMENTSET_ALL );
	DorbiereReport.m_ReportFilters.PluFilter.SetMasterFilterNo( 0 );
	
	DorbiereReport.SetSaleTimeFlag( FALSE );
	DorbiereReport.Consolidate();
	
	bool bResult = FALSE;

	if ( FALSE == bExport )
		bResult = DorbiereReport.CreateReport();
	else
		bResult = DorbiereReport.CreateExport();
	
	if ( DorbiereReport.GetCashRSPVersionCheckerFailFlag() == TRUE )
	{
		DorbiereReport.ShowCashRSPVersionCheckerFailMessage();
		return FALSE;
	}

	if ( FALSE == bResult )
	{
		if ( FALSE == bExport )	
			Prompter.Error( "Unable to create report." );
		else
			Prompter.Error( "Unable to create export." );

		return FALSE;
	}

	if ( FALSE == bExport )
	{
		CReportManager ReportManager;
		bResult = ReportManager.DisplayReport( Super.ReportFilename(), DorbiereReport.GetParamsFilename(), "", m_pParentWnd );
	}
	else
		Prompter.Info( "The export was created as requested." );

	return bResult;
}

/**********************************************************************/


