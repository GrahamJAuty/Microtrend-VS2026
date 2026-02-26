/**********************************************************************/
#include <process.h>
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "CommsHandshaker.h"
#include "CustomerSelectDlg.h"
#include "..\CommonEcrStock\DefEditDlg.h"
#include "FilterTypes.h"
#include "ListDataPresetDlg.h"
#include "ListDataSalesHistoryCustomerDlg.h"
#include "MaxLengths.h"
#include "NodeTypes.h"
#include "Password.h"
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
#include "..\SPosStockManager5\PluFilterDlgStockman.h"
#include "PresetReportManager.h"
#include "RepSpawn.h"
#include "SalesHistoryCSVRecordEditCustomerDlg.h"
#include "SalesHistoryPriceLevelDlg.h"
#include "SalesHistoryTaxDlg.h"
#include "SelectPresetActionDlg.h"
#include "SessionEditDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryReportCustomerDlg.h"
/**********************************************************************/

CSalesHistoryReportCustomerDlg::CSalesHistoryReportCustomerDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent /*=NULL*/)
	: CSSDialog(CSalesHistoryReportCustomerDlg::IDD, pParent), 
		m_FilterArray ( FILTERTYPE_BROWSE_PLU ),
		m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
		m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CSalesHistoryReportCustomerDlg)
	//}}AFX_DATA_INIT
	m_nLocationComboDbIdx = -1;
	m_strCustomerFilter = "";
	m_nPresetNo = nPresetNo;
	m_nPresetError = 0;
	m_bAutoRunPreset = bAutoRunPreset;
	DataManagerNonDb.CustomerNameTable.BuildList();
	DataManagerNonDb.CustomerNameTable.BuildSortOrder();
	DataManagerNonDb.ServerNameTable.BuildList();
}

/**********************************************************************/

CSalesHistoryReportCustomerDlg::~CSalesHistoryReportCustomerDlg()
{
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesHistoryReportCustomerDlg)
	DDX_Control(pDX, IDC_STATIC_PRESET, m_staticPreset);
	DDX_Control(pDX, IDC_BUTTON_PRESET, m_buttonPreset);
	DDX_Control(pDX, IDC_COMBO_PRESET, m_comboPreset);
	DDX_Control(pDX, IDC_COMBO_DEPTFILTERTYPE, m_comboDeptFilterType);
	DDX_Control(pDX, IDC_BUTTON_TAXBAND, m_buttonTaxBand);
	DDX_Control(pDX, IDC_COMBO_TAXBAND, m_comboTaxBand);
	DDX_Control(pDX, IDC_COMBO_PRICELEVEL, m_comboPriceLevel);
	DDX_Control(pDX, IDC_COMBO_ROUND, m_comboRound);
	DDX_Control(pDX, IDC_COMBO_TIMESLICE, m_comboTimeSlice);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_buttonSelect);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_TO, m_buttonBrowseTo);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_FROM, m_buttonBrowseFrom);
	DDX_Control(pDX, IDC_CHECK_SPECIFY, m_checkSpecify);
	DDX_Control(pDX, IDC_COMBO_REPORTTYPE, m_comboReportType);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_COMBO_TERMINAL, m_comboTerminal);
	DDX_Control(pDX, IDC_CHECK_SHOW_SALES, m_checkSales);
	DDX_Control(pDX, IDC_CHECK_SHOW_TAX, m_checkTax);
	DDX_Control(pDX, IDC_CHECK_SHOW_RA, m_checkRA);
	DDX_Control(pDX, IDC_CHECK_SHOW_CHARGES, m_checkCharges);
	DDX_Control(pDX, IDC_CHECK_SHOW_PAYMETH, m_checkPayMeth);
	DDX_Control(pDX, IDC_CHECK_CUSTOMER, m_checkCustomerList);
	DDX_Control(pDX, IDC_CHECK_PAGE, m_checkCustomerPage);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_FROM, m_strPluNoFrom);
	DDX_IntegerString(pDX, IDC_EDIT_PLU_TO, m_strPluNoTo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesHistoryReportCustomerDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesHistoryReportCustomerDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_FROM, OnButtonBrowseFrom)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_TO, OnButtonBrowseTo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_FROM, OnKillFocusPluFrom)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_TO, OnKillFocusPluTo)
	ON_BN_CLICKED(IDC_CHECK_SPECIFY, OnTogglePluRange)
	ON_CBN_SELCHANGE(IDC_COMBO_REPORTTYPE, OnSelectReportType)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDepartment)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_TAXBAND, OnButtonTaxBand)
	ON_BN_CLICKED(IDC_BUTTON_PRICELEVEL, OnButtonPriceLevel)
	ON_BN_CLICKED(IDC_CHECK_SHOW_RA, OnToggleActivity)
	ON_BN_CLICKED(IDC_CHECK_SHOW_SALES, OnToggleActivity)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	ON_BN_CLICKED(IDC_BUTTON_PRESET, OnButtonPreset)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET, OnSelectPreset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CSalesHistoryReportCustomerDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_FROM ); }
CEdit* CSalesHistoryReportCustomerDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_TO ); }
/**********************************************************************/

BOOL CSalesHistoryReportCustomerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DeptAndPluComboHelpers.SetControlPointers( NULL, &m_LocationSelector, &m_DepartmentSelector );

	m_buttonTaxBand.SetWindowText( EcrmanOptions.GetTaxBandString() );
	m_checkTax.SetWindowText( EcrmanOptions.GetTaxString( "Show item %T" ) );
	
	SubclassEdit ( IDC_EDIT_PLU_FROM, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_TO, SS_NUM, Super.MaxPluLen(), "%s" );

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(30);
	m_comboPriceLevel.SetTabStops( arrayStops );

	arrayStops.Add(130);
	m_comboTaxBand.SetTabStops( arrayStops );

	FillReportCombo(0);
	FillTimeSliceCombo(0);

	PresetReportManager.CustHist.SetPasswordAccess();
	FillPresetCombo(-1);
	
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

	m_editDeptNo.LimitText( MAX_LENGTH_DEPTNO );
	m_DeptAndPluComboHelpers.UpdateCombos();
	
	FillTaxBandComboForLocation();
	FillPriceLevelCombo();

	m_comboTaxBand.SetCurSel(0);
	
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

	if ( PasswordArray.GetEnable( PasswordTicks::SetupMenu ) == FALSE )
		m_buttonSelect.EnableWindow( FALSE );

	m_comboRound.AddString ( "Dates as selected" );
	m_comboRound.AddString ( "Nearest full week" );
	m_comboRound.AddString ( "Nearest full month" );
	m_comboRound.SetCurSel(0);

	m_comboDeptFilterType.AddString( "By department at time of sale" );
	m_comboDeptFilterType.AddString( "By department in database" );
	m_comboDeptFilterType.SetCurSel(0);

	m_checkSales.SetCheck( TRUE );
	m_checkTax.SetCheck( FALSE );
	m_checkRA.SetCheck( TRUE );
	m_checkCharges.SetCheck( TRUE );
	m_checkPayMeth.SetCheck( TRUE );

	if ( m_nPresetNo != 0 )
	{
		m_nPresetError = PresetReportManager.SelectFromCombo( NODE_PRESET_CUST_HIST, m_comboPreset, m_nPresetNo );

		if ( 0 == m_nPresetError )
		{
			OnSelectPreset();

			if ( TRUE == m_bAutoRunPreset )
				OnButtonDisplay();
		}

		if ( TRUE == m_bAutoRunPreset )
		{
			EndDialog( IDCANCEL );
			return FALSE;
		}			
	}

	bool bPreset = PasswordArray.CheckLocationAccessSystem();
	ShowAndEnableWindow( &m_buttonPreset, bPreset );
	ShowAndEnableWindow( &m_staticPreset, FALSE == bPreset );
	
	return TRUE;  
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::FillReportCombo( int nSelection )
{	
	m_comboReportType.ResetContent();

	int n = 0;
	int nCurSel = 0;
	for ( int nPos = 0; nPos < DataManagerNonDb.SalesHistoryCustomer.GetSize(); nPos++ )
	{
		CSalesHistoryCSVRecordCustomer ReportRecord;
		DataManagerNonDb.SalesHistoryCustomer.GetAt( nPos, ReportRecord );

		int nCheck = n;

		switch ( ReportRecord.GetReportType() )
		{
		case SH_CUSTOM:
			m_comboReportType.AddString ( ReportRecord.GetListName() );
			m_comboReportType.SetItemData( n++, 100 + nPos );
			break;

		case SH_CUST_SUMMARY:
		case SH_CUST_DETAIL:
			m_comboReportType.AddString ( ReportRecord.GetListName() );
			m_comboReportType.SetItemData( n++, ReportRecord.GetReportType() );
			break;
		}

		if ( ( nPos == nSelection ) && ( nCheck != n ) )
			nCurSel = nCheck;
	}

	m_comboReportType.AddString ( "Adhoc Report" );
	m_comboReportType.SetItemData( n++, SH_CUSTOM );
	
	m_comboReportType.SetCurSel( nCurSel );
	m_comboReportType.EnableWindow( m_comboReportType.GetCount() > 1 );
		
	OnSelectReportType();
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::SelectReportByItemData( int n )
{
	for ( int x = 0; x < m_comboReportType.GetCount(); x++ )
	{
		if ( m_comboReportType.GetItemData(x) == (DWORD) n )
		{
			m_comboReportType.SetCurSel(x);
			break;
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::FillTimeSliceCombo( int nSelection )
{
	m_comboTimeSlice.ResetContent();

	int n = 0;
	int nCurSel = 0;
	for ( int nPos = 0; nPos < DataManagerNonDb.TimeSliceList.GetSize(); nPos++ )
	{
		CTimeSliceMapCSVRecord ReportRecord;
		DataManagerNonDb.TimeSliceList.GetAt( nPos, ReportRecord );

		int nCheck = n;

		switch ( ReportRecord.GetReportType() )
		{
		case SH_CUSTOM:
			m_comboTimeSlice.AddString ( ReportRecord.GetListName() );
			m_comboTimeSlice.SetItemData( n++, 100 + nPos );
			break;

		case SH_TIMESLICE_SECOND:
		case SH_TIMESLICE_MINUTE:
		case SH_TIMESLICE_5MIN:
		case SH_TIMESLICE_10MIN:
		case SH_TIMESLICE_15MIN:
		case SH_TIMESLICE_30MIN:
		case SH_TIMESLICE_60MIN:
			m_comboTimeSlice.AddString ( ReportRecord.GetListName() );
			m_comboTimeSlice.SetItemData( n++, ReportRecord.GetReportType() );
			break;
		}

		if ( ( nPos == nSelection ) && ( nCheck != n ) )
			nCurSel = nCheck;
	}

	m_comboTimeSlice.SetCurSel( nCurSel );
	if ( m_comboTimeSlice.GetCount() <= 1 )
		m_comboTimeSlice.EnableWindow( FALSE );
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::FillPresetCombo( int nPresetIdxSel )
{
	PresetReportManager.FillPresetCombo( NODE_PRESET_CUST_HIST, m_comboPreset, nPresetIdxSel );
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnSelectDepartment() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnSelectLocation() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_DeptAndPluComboHelpers.UpdateCombos();
		FillTaxBandComboForLocation();
	}
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::GetReportDates()
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

bool CSalesHistoryReportCustomerDlg::GetAdhocReportSession()
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

void CSalesHistoryReportCustomerDlg::OnToggleActivity()
{
	bool bSales = ( m_checkSales.GetCheck() != 0 );
	bool bRA = ( m_checkRA.GetCheck() != 0 );

	if ( bSales && bRA )
	{
		m_checkPayMeth.EnableWindow( TRUE );
		m_checkTax.EnableWindow( TRUE );
	}
	else
	{
		m_checkPayMeth.EnableWindow( FALSE );
		m_checkPayMeth.SetCheck( FALSE );
		m_checkTax.EnableWindow( FALSE );
		m_checkTax.SetCheck( FALSE );
	}
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnTogglePluRange() 
{
	bool bEnable = ( m_checkSpecify.GetCheck() != 0 );
	GetEditPluFrom() -> EnableWindow ( bEnable );
	GetEditPluTo() -> EnableWindow ( bEnable );
	m_buttonBrowseFrom.EnableWindow ( bEnable );
	m_buttonBrowseTo.EnableWindow ( bEnable );	
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnKillFocusPluFrom() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoFrom = BarcodePluNoTable.ConvertPlu( m_strPluNoFrom );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnKillFocusPluTo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoTo = BarcodePluNoTable.ConvertPlu( m_strPluNoTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnSelectReportType() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	int nSel = m_comboReportType.GetCurSel();
	if ( nSel < 0 || nSel >= m_comboReportType.GetCount() )
		return;

	CSalesHistoryCSVRecordCustomer ReportRecord;
	
	if ( nSel < DataManagerNonDb.SalesHistoryCustomer.GetSize() )
		DataManagerNonDb.SalesHistoryCustomer.GetAt( nSel, ReportRecord );
	else
		DataManagerNonDb.SalesHistoryCustomer.LoadAdhocReport( ReportRecord );

	bool bEnablePluRange = ( m_checkSpecify.GetCheck() != 0 );
	GetEditPluFrom() -> EnableWindow( bEnablePluRange );
	GetEditPluTo() -> EnableWindow( bEnablePluRange );
	m_buttonBrowseTo.EnableWindow( bEnablePluRange );
	m_buttonBrowseFrom.EnableWindow( bEnablePluRange );
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnButtonBrowseFrom() 
{
	UpdateData( TRUE, FALSE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_FilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK == nDlgResult )
	{
		if ( nPluIdx >= 0 && nPluIdx < DataManager.Plu.GetSize() )
		{
			m_strPluNoFrom = DataManager.Plu.GetPluNoString ( nPluIdx );
			UpdateData ( FALSE );
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnButtonBrowseTo() 
{
	UpdateData( TRUE, FALSE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_FilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK == nDlgResult )
	{
		if ( nPluIdx >= 0 && nPluIdx < DataManager.Plu.GetSize() )
		{
			m_strPluNoTo = DataManager.Plu.GetPluNoString ( nPluIdx );
			UpdateData ( FALSE );
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::FillDepartmentComboForLocation()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( LocSelEntity.GetDbIdx() == m_nLocationComboDbIdx )
		return;

	if ( ( EcrmanOptions.GetGlobalDepartmentFlag() == TRUE ) && ( m_nLocationComboDbIdx >= 0 ) )
		return;

	int nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();
	if ( nDepartmentFilter == 0 ) nDepartmentFilter = FIRST_DEPARTMENTSET_FILTER - 1;
		
	m_nLocationComboDbIdx = LocSelEntity.GetDbIdx();

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );

	//FIDDLE : MAY BE SYSTEM
	m_DepartmentSelector.FillDepartmentComboDatabase( nDepartmentFilter, DEPARTMENT_SELECTOR_REPORT );
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::FillTaxBandComboForLocation()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	
	int nSel = m_comboTaxBand.GetCurSel();

	m_comboTaxBand.ResetContent();
	m_comboTaxBand.AddItem ( ",All" );
	m_comboTaxBand.SetItemData (0, TAXBAND_FILTER_ALL );

	int nMask = 2;
	CString strLetter = "A";
	for ( int nTaxBand = 1; nTaxBand <= MAX_TAX_BANDS; nTaxBand++ )
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( nTaxBand, TaxRateInfo );

		strLetter.SetAt( 0, 'A' + nTaxBand - 1 );

		CString strRate;
		strRate.Format ( "%.4lf", TaxRateInfo.m_dTaxRate );

		CCSV csv;
		csv.Add( strLetter );
		csv.Add( TaxRateInfo.m_strReportText );
		csv.Add( strRate );
		m_comboTaxBand.AddItem( csv.GetLine() );
		m_comboTaxBand.SetItemData( nTaxBand, nMask );
		
		nMask *= 2;
	}

	CCSV csv;
	csv.Add( "" );
	csv.Add( SysInfo.GetCustomTaxFilterString() );
	m_comboTaxBand.AddItem( csv.GetLine() );
	m_comboTaxBand.SetItemData( MAX_TAX_BANDS + 1, SysInfo.GetCustomTaxFilter() );
	
	if ( ( nSel >= 0 ) && ( nSel < m_comboTaxBand.GetCount() ) )
		m_comboTaxBand.SetCurSel( nSel );
	else
		m_comboTaxBand.SetCurSel( 0 );
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::FillPriceLevelCombo()
{
	m_comboPriceLevel.ResetContent();
	m_comboPriceLevel.AddItem ( ",All" );
	m_comboPriceLevel.SetItemData (0, PRICELEVEL_FILTER_ALL );

	int nMask = 1;
	for ( int nLevel = 0; nLevel < 10; nLevel++ )
	{
		CString strNum;
		strNum.Format( "%2.2d", nLevel + 1 );

		CCSV csv;
		csv.Add( strNum );
		csv.Add( DataManager.PriceText.GetReportText( nLevel ) );
		m_comboPriceLevel.AddItem( csv.GetLine() );
		m_comboPriceLevel.SetItemData ( nLevel + 1, nMask );
		nMask *= 2;
	}

	CCSV csv;
	csv.Add( "" );
	csv.Add( SysInfo.GetCustomPriceLevelFilterString() );
	m_comboPriceLevel.AddItem( csv.GetLine() );
	m_comboPriceLevel.SetItemData( 11, SysInfo.GetCustomPriceLevelFilter() );
	
	m_comboPriceLevel.SetCurSel( 0 );
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnButtonSelect() 
{
	int nCount = m_comboReportType.GetCount();

	int nSel = m_comboReportType.GetCurSel();
	if ( nSel < 0 || nSel >= nCount )
		return;

	//SELECT FOR ADHOC REPORT SHOULD JUST GO THROUGH DISPLAY ROUTINE
	if ( ( nCount > 1 ) && ( nSel == nCount - 1 ) )
	{
		OnButtonDisplay();
		return;
	}

	int nOldSelect = m_comboReportType.GetItemData( nSel );

	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenSalesHistoryCustomerBuffer( info ) == FALSE )
		Prompter.ShareError( info );
	else
	{
		CListDataSalesHistoryCustomerDlg dlg ( nOldSelect, this );
	
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteSalesHistoryCustomerBuffer( info ) == FALSE )
			{
				DataManagerNonDb.CloseSalesHistoryCustomerBuffer( info );
				Prompter.WriteError( info );
				FillReportCombo( 0 );
				SelectReportByItemData( nOldSelect );
				OnSelectReportType();
			}
			else
			{
				FillReportCombo( dlg.GetListSelection() );
				DataManagerNonDb.CloseSalesHistoryCustomerBuffer( info );
			}
		}
		else
		{
			DataManagerNonDb.CloseSalesHistoryCustomerBuffer( info );
			FillReportCombo( 0 );
			SelectReportByItemData( nOldSelect );
			OnSelectReportType();
		}
	}	
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnButtonPriceLevel() 
{
	CSalesHistoryPriceLevelDlg dlg ( this );
	
	if ( dlg.DoModal() == IDOK )
	{
		UpdateCustomPriceLevelString();
		m_comboPriceLevel.SetCurSel( m_comboPriceLevel.GetCount() - 1 );
	}
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::UpdateCustomPriceLevelString() 
{
	int nSel = m_comboPriceLevel.GetCurSel();

	CString strItem = ",";
	strItem += SysInfo.GetCustomPriceLevelFilterString();

	int nPos = m_comboPriceLevel.GetCount() - 1;
	m_comboPriceLevel.DeleteString( nPos );
	m_comboPriceLevel.AddItem ( strItem );
	m_comboPriceLevel.SetItemData( nPos, SysInfo.GetCustomPriceLevelFilter() );

	m_comboPriceLevel.SetCurSel( nSel );
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnButtonTaxBand() 
{
	CSalesHistoryTaxDlg dlg ( this );
	
	if ( dlg.DoModal() == IDOK )
	{
		UpdateCustomTaxString();
		m_comboTaxBand.SetCurSel( m_comboTaxBand.GetCount() - 1 );
	}
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::UpdateCustomTaxString()
{
	int nSel = m_comboTaxBand.GetCurSel();

	CString strItem = ",";
	strItem += SysInfo.GetCustomTaxFilterString();

	int nPos = m_comboTaxBand.GetCount() - 1;
	m_comboTaxBand.DeleteString( nPos );
	m_comboTaxBand.AddItem ( strItem );
	m_comboTaxBand.SetItemData( nPos, SysInfo.GetCustomTaxFilter() );

	m_comboTaxBand.SetCurSel( nSel );
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnButtonDisplay() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	OnKillFocusDeptNo();
	OnKillFocusPluFrom();
	OnKillFocusPluTo();

	CWaitCursor wait;
	CommsHandshaker.ProcessSales();
	
	GetReportDates();

	if ( m_comboSession.GetCurSel() == m_comboSession.GetCount() - 1 )
		if ( GetAdhocReportSession() == FALSE )
			return;

	//WORK OUT FILES WE NEED TO READ FOR REPORT
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	
	//GET SERVER LIST
	//MUST DO THIS BEFORE WE FINALISE ECR SELECT ARRAY
	if ( m_checkCustomerList.GetCheck() != 0 )
	{	
		CEposSelectArray SelectArray;
		LocSelEntity.GetEPOSSelectArray( SelectArray );

		CCustomerSelectDlg dlg( m_strCustomerFilter, SelectArray, this );
		if ( dlg.DoModal() != IDOK )
			return;
	}
		
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );

	m_ReportRecord.Reset();
	
	int nSel = m_comboReportType.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.SalesHistoryCustomer.GetSize() ) )
		DataManagerNonDb.SalesHistoryCustomer.GetAt( nSel, m_ReportRecord );
	else
	{
		DataManagerNonDb.SalesHistoryCustomer.LoadAdhocReport( m_ReportRecord );
	
		CSalesHistoryCSVRecordEditCustomerDlg dlg( m_ReportRecord, TRUE, this );
		if ( dlg.DoModal() != IDOK )
			return;
		
		DataManagerNonDb.SalesHistoryCustomer.SaveAdhocReport( m_ReportRecord );
	}

	int nReportType = m_ReportRecord.GetReportType();
	m_ReportRecord.SetReportType( nReportType );

	CTimeSliceMapCSVRecord TimeSliceRecord;
	nSel = m_comboTimeSlice.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.TimeSliceList.GetSize() ) )
		DataManagerNonDb.TimeSliceList.GetAt( nSel, TimeSliceRecord );
	
	m_ReportRecord.SetReportType( nReportType );
	m_HistoryReport.Reset( m_ReportRecord, LocSelEntity );
	m_HistoryReport.SetDates ( m_oleDateFrom, m_oleDateTo, m_ReportSession, TimeSliceRecord );
	
	CString strSessionTitle;
	if ( m_comboSession.GetCurSel() < m_comboSession.GetCount() - 1 )
		m_comboSession.GetWindowText( strSessionTitle );
	else
		strSessionTitle = m_ReportSessionAdhoc.GetName();

	m_HistoryReport.SetSessionTitle( strSessionTitle );

	if ( m_checkSpecify.GetCheck() != 0 )
	{
		m_HistoryReport.SetPluRangeFlag ( TRUE );
		m_HistoryReport.SetPluFrom ( _atoi64 ( m_strPluNoFrom ) );
		m_HistoryReport.SetPluTo ( _atoi64 ( m_strPluNoTo ) );
	}

	{
		int nConType = LocSelEntity.GetConType();
	
		int nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();	
		if ( nDepartmentFilter == 0 ) nDepartmentFilter = FIRST_DEPARTMENTSET_FILTER - 1;

		m_HistoryReport.m_ReportFilters.DeptFilter.SetConsolidationType( nConType );
		m_HistoryReport.m_ReportFilters.DeptFilter.SetDeptFilterNo( nDepartmentFilter );
	}

	m_HistoryReport.SetDepartmentByItemFlag( m_comboDeptFilterType.GetCurSel() == 1 );

	int nTaxFilter = m_comboTaxBand.GetItemData( m_comboTaxBand.GetCurSel() );
	m_HistoryReport.SetTaxFilter( nTaxFilter );

	int nPriceLevelFilter = m_comboPriceLevel.GetItemData( m_comboPriceLevel.GetCurSel() );
	m_HistoryReport.SetPriceLevelFilter( nPriceLevelFilter );

	if ( m_HistoryReport.PrepareFields() == FALSE )
	{
		Prompter.Error ( "You have not selected any fields\nto include in this report" );
		return;
	}

	bool bSales = ( m_checkSales.GetCheck() != 0 );
	bool bRA = ( m_checkRA.GetCheck() != 0 );
	bool bTax = ( m_checkTax.GetCheck() != 0 );
	bool bCharges = ( m_checkCharges.GetCheck() != 0 );
	bool bPayMeth = ( m_checkPayMeth.GetCheck() != 0 );

	if ( ( FALSE == bSales ) && ( FALSE == bRA ) && ( FALSE == bCharges ) )
	{
		Prompter.Error ( "You have not selected any customer activities\nto include in this report" );
		return;
	}

	m_HistoryReport.SetShowSalesFlag( bSales );
	m_HistoryReport.SetShowRAFlag( bRA );
	m_HistoryReport.SetShowTaxFlag( bTax );
	m_HistoryReport.SetShowChargesFlag( bCharges );
	m_HistoryReport.SetShowPayMethFlag( bSales & bRA & bPayMeth );
	m_HistoryReport.SetNewPagePerCustomerFlag( m_checkCustomerPage.GetCheck() != 0 );

	m_HistoryReport.Consolidate();

	if ( m_HistoryReport.CreateReport() == FALSE )
		Prompter.Error ( "Unable to create report" );
	else
	{
		CFilenameUpdater FnUp( SysFiles::CustomHistPrm );
		
		CString strKey = m_ReportRecord.GetKey();
		m_HistoryReport.WriteTerminalList();
		
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, this );
	}
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnButtonPreset() 
{
	CPresetReportInfo Preset;
	CreatePreset( Preset );
	PresetReportManager.PresetButton( NODE_PRESET_CUST_HIST, m_comboPreset, Preset, m_pParentWnd );
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::CreatePreset( CPresetReportInfo& Base )
{
	CPresetReportInfoCustHist Preset;

	CString strDateType;
	m_comboDates.GetWindowText( strDateType );
	Preset.SetDateType( strDateType );

	CString strDateFrom;
	COleDateTime dateFrom;
	m_DatePickerFrom.GetTime( dateFrom );
	::GetStringFromOleDate( dateFrom, strDateFrom );
	Preset.SetDateFrom( strDateFrom );
	
	CString strDateTo;
	COleDateTime dateTo;
	m_DatePickerTo.GetTime( dateTo );
	::GetStringFromOleDate( dateTo, strDateTo );
	Preset.SetDateTo( strDateTo );
	
	Preset.SetRoundToType( m_comboRound.GetCurSel() );

	CString strSession;
	m_comboSession.GetWindowText( strSession );
	Preset.SetSessionName( strSession );

	CString strTimeSlice;
	m_comboTimeSlice.GetWindowText( strTimeSlice );
	Preset.SetTimeSliceName( strTimeSlice );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	Base.SetPresetConLevel( LocSelEntity.GetConType() );

	int nEntityNo1, nEntityNo2;
	CPresetReportArray::GetLocSelEntityNo( LocSelEntity, nEntityNo1, nEntityNo2 );
	Base.SetPresetEntityNo1( nEntityNo1 );
	Base.SetPresetEntityNo2( nEntityNo2 );

	Base.SetTerminalIdx( LocSelEntity.GetTermIdx() );
	Preset.SetPluRangeFlag( m_checkSpecify.GetCheck() != 0 );

	CString strPluFrom;
	GetEditPluFrom() -> GetWindowText( strPluFrom );
	Preset.SetPluFrom( strPluFrom );

	CString strPluTo;
	GetEditPluTo() -> GetWindowText( strPluTo );
	Preset.SetPluTo( strPluTo );

	Preset.SetDepartmentFilter( m_DepartmentSelector.GetDepartmentFilter() );
	Preset.SetDepartmentFilterType( m_comboDeptFilterType.GetCurSel() );
	Preset.SetTaxFilter( m_comboTaxBand.GetCurSel() );
	Preset.SetPriceLevelFilter( m_comboPriceLevel.GetCurSel() );
	Preset.SetTaxCustomFilter( SysInfo.GetCustomTaxFilter() );
	Preset.SetPriceLevelCustomFilter( SysInfo.GetCustomPriceLevelFilter() );
	
	Preset.SetSalesFlag( m_checkSales.GetCheck() != 0 );
	Preset.SetRAFlag( m_checkRA.GetCheck() != 0 );
	Preset.SetTaxFlag( m_checkTax.GetCheck() != 0 );
	Preset.SetChargesFlag( m_checkCharges.GetCheck() != 0 );
	Preset.SetCustomerListFlag( m_checkCustomerList.GetCheck() != 0 );
	Preset.SetPayMethFlag( m_checkPayMeth.GetCheck() != 0 );
	Preset.SetCustomerPageFlag( m_checkCustomerPage.GetCheck() != 0 );

	CString strReportName;
	m_comboReportType.GetWindowText( strReportName );
	Preset.SetReportName( strReportName );

	CString str;
	Preset.ToString( str );
	Base.SetOtherFields( str );
}

/**********************************************************************/

void CSalesHistoryReportCustomerDlg::OnSelectPreset() 
{
	int nSel = m_comboPreset.GetCurSel() - 1;
	if ( nSel < 0 || nSel >= PresetReportManager.CustHist.GetSize() )
		return;

	CPresetReportInfo Base;
	PresetReportManager.CustHist.GetAt( nSel, Base );

	CPresetReportInfoCustHist Preset;
	{
		CString str = Base.GetOtherFields();
		Preset.FromString( str );
	}

	//DATE GROUP
	int nResult = m_comboDates.FindStringExact( 0, Preset.GetDateType() );
		
	if ( nResult != CB_ERR )
		m_comboDates.SetCurSel( nResult );
	
	CString strDateFrom = Preset.GetDateFrom();
	CString strDateTo = Preset.GetDateTo();

	COleDateTime dateFrom, dateTo;
	
	if ( ::GetOleDateFromString( strDateFrom, dateFrom ) == TRUE )
		m_DatePickerFrom.SetTime( dateFrom );

	if ( ::GetOleDateFromString( strDateTo, dateTo ) == TRUE )
		m_DatePickerTo.SetTime( dateTo );

	OnSelectDate();

	int nType = Preset.GetRoundToType();
	if ( nType >= 0 && nType < m_comboRound.GetCount() )
		m_comboRound.SetCurSel( nType );

	nResult = m_comboSession.FindStringExact( 0, Preset.GetSessionName() );
		
	if ( nResult != CB_ERR )
		m_comboSession.SetCurSel( nResult );
	
	nResult = m_comboTimeSlice.FindStringExact( 0, Preset.GetTimeSliceName() );
		
	if ( nResult != CB_ERR )
		m_comboTimeSlice.SetCurSel( nResult );
	
	//LOCATION GROUP
	int nConLevel = Base.GetPasswordConLevel();
	int nEntity1 = Base.GetPasswordEntityNo1();
	int nEntity2 = Base.GetPasswordEntityNo2();
	int nTerminalIdx = Base.GetTerminalIdx();

	int nEntityIdx = 0;
	CPresetReportArray::GetEntityIdx( nConLevel, nEntity1, nEntity2, nEntityIdx );
	
	if ( nConLevel == NODE_LOCATION_TERMINAL )
	{
		if ( ( nTerminalIdx >= 0 ) && ( nTerminalIdx < m_comboTerminal.GetCount() ) )
			m_comboTerminal.SetCurSel(nTerminalIdx);
	}
	
	m_LocationSelector.ForceSelection( nConLevel, nEntityIdx );
	OnSelectLocation();

	//FILTER GROUP
	FillDepartmentComboForLocation();
	FillTaxBandComboForLocation();

	m_checkSpecify.SetCheck( Preset.GetPluRangeFlag() );
	OnTogglePluRange();

	CString strPluFrom = Preset.GetPluFrom();
	if ( ::TestNumeric( strPluFrom ) == TRUE )
		if ( strPluFrom.GetLength() <= Super.MaxPluLen() )
			GetEditPluFrom() -> SetWindowText( strPluFrom );

	CString strPluTo = Preset.GetPluTo();
	if ( ::TestNumeric( strPluTo ) == TRUE )
		if ( strPluTo.GetLength() <= Super.MaxPluLen() )
			GetEditPluTo() -> SetWindowText( strPluTo );

	m_DepartmentSelector.UpdateDepartmentCombo( Preset.GetDepartmentFilter() );
	m_DepartmentSelector.UpdateDepartmentEditBox();

	nType = Preset.GetDepartmentFilterType();
	if ( nType >= 0 && nType < m_comboDeptFilterType.GetCount() )
		m_comboDeptFilterType.SetCurSel( nType );

	nType = Preset.GetTaxFilter();
	if ( nType >= 0 && nType < m_comboTaxBand.GetCount() )
		m_comboTaxBand.SetCurSel( nType );

	nType = Preset.GetPriceLevelFilter();
	if ( nType >= 0 && nType < m_comboPriceLevel.GetCount() )
		m_comboPriceLevel.SetCurSel( nType );

	nType = Preset.GetTaxCustomFilter();
	if ( nType != 0 )
	{
		SysInfo.SetCustomTaxFilter( nType );
		UpdateCustomTaxString();
	}

	nType = Preset.GetPriceLevelCustomFilter();
	if ( nType != 0 )
	{
		SysInfo.SetCustomPriceLevelFilter( nType );
		UpdateCustomPriceLevelString();
	}

	m_checkSales.SetCheck( Preset.GetSalesFlag() );
	m_checkRA.SetCheck( Preset.GetRAFlag() );
	m_checkTax.SetCheck( Preset.GetTaxFlag() );
	m_checkCharges.SetCheck( Preset.GetChargesFlag() );
	m_checkPayMeth.SetCheck( Preset.GetPayMethFlag() );
	m_checkCustomerList.SetCheck( Preset.GetCustomerListFlag() );
	m_checkCustomerPage.SetCheck( Preset.GetCustomerPageFlag() );

	nResult = m_comboReportType.FindStringExact( 0, Preset.GetReportName() );
		
	if ( nResult != CB_ERR )
		m_comboReportType.SetCurSel( nResult );
	
	OnSelectReportType();
	OnToggleActivity();
}

/**********************************************************************/
