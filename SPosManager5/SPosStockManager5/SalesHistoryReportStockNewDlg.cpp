/**********************************************************************/
#include <process.h>
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "..\CommonEcrStock\DefEditDlg.h"
#include "FilterTypes.h"
#include "..\CommonEcrStock\ListDataPresetDlg.h"
#include "ListDataSalesHistoryStockDlg.h"
#include "NodeTypes.h"
#include "Password.h"
#include "..\CommonEcrStock\PresetReportManager.h"
#include "RepSpawn.h"
#include "SalesHistoryCSVRecordEditStockDlg.h"
#include "..\CommonEcrStock\SelectPresetActionDlg.h"
#include "..\CommonEcrStock\SessionEditDlg.h"
#include "StockFilterDlg.h"
#include "StockOptionsIni.h"
#include "StockReportSupplierDlg.h"
/**********************************************************************/
#include "SalesHistoryReportStockNewDlg.h"
/**********************************************************************/

CSalesHistoryReportStockNewDlg::CSalesHistoryReportStockNewDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent /*=NULL*/)
	: CSSDialog(CSalesHistoryReportStockNewDlg::IDD, pParent), 
		m_FilterArray ( FILTERTYPE_BROWSE_PLU ),
		m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo ),
		m_CategorySelector( m_comboCategory )
{
	//{{AFX_DATA_INIT(CSalesHistoryReportStockNewDlg)
	m_IDStaticCustomField[0] = IDC_STATIC_CUSTOM_FIELD1;
	m_IDStaticCustomField[1] = IDC_STATIC_CUSTOM_FIELD2;
	m_IDStaticCustomField[2] = IDC_STATIC_CUSTOM_FIELD3;
	m_IDStaticCustomField[3] = IDC_STATIC_CUSTOM_FIELD4;
	m_IDStaticCustomField[4] = IDC_STATIC_CUSTOM_FIELD5;

	m_IDComboCustomField[0] = IDC_COMBO_CUSTOM_FIELD1;
	m_IDComboCustomField[1] = IDC_COMBO_CUSTOM_FIELD2;
	m_IDComboCustomField[2] = IDC_COMBO_CUSTOM_FIELD3;
	m_IDComboCustomField[3] = IDC_COMBO_CUSTOM_FIELD4;
	m_IDComboCustomField[4] = IDC_COMBO_CUSTOM_FIELD5;

	m_bByMonth = FALSE;
	m_bEposSales = TRUE;
	m_bImportSales = TRUE;
	m_bManualSales = TRUE;
	m_bCatSort = FALSE;
	m_bPercent = FALSE;
	m_strStockCodeFrom = _T("");
	m_strStockCodeTo = _T("");
	m_bEstimatedProfit = FALSE;
	//}}AFX_DATA_INIT
	m_bChangeTop100 = FALSE;
	m_nPresetNo = nPresetNo;
	m_nPresetError = 0;
	m_bAutoRunPreset = bAutoRunPreset;

	CDataManagerInfo info;
	DataManagerNonDb.OpenReportSession( DB_READONLY, info );
	DataManagerNonDb.OpenSalesHistoryStock( DB_READONLY, info );
	PresetReportManager.Open( NODE_PRESET_STOCK_HIST, DB_READONLY, info );
	DataManagerNonDb.OpenCustomFieldNames( DB_READONLY, info );
}

/**********************************************************************/

CSalesHistoryReportStockNewDlg::~CSalesHistoryReportStockNewDlg()
{
	CDataManagerInfo info;
	DataManagerNonDb.CloseReportSession( info );
	DataManagerNonDb.CloseSalesHistoryStock( info );
	PresetReportManager.Close( NODE_PRESET_STOCK_HIST, info );
	DataManagerNonDb.CloseCustomFieldNames( info );
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesHistoryReportStockNewDlg)
	
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_COMBO_ROUND, m_comboRound);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_CHECK_EPOSSALES, m_checkEposSales);
	DDX_Control(pDX, IDC_CHECK_MANUALSALES, m_checkManualSales);
	DDX_Control(pDX, IDC_CHECK_IMPORTSALES, m_checkImportSales);
	
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Control(pDX, IDC_COMBO_CATFILTERTYPE, m_comboCatFilterType);
	
	for ( int n = 0; n < 5; n++ )
	{
		DDX_Control(pDX, m_IDStaticCustomField[n], m_staticCustomField[n] );
		DDX_Control(pDX, m_IDComboCustomField[n], m_comboCustomField[n] );
	}

	DDX_Control(pDX, IDC_CHECK_STOCK_FILTER, m_checkStockFilter);
	DDX_Control(pDX, IDC_EDIT_STOCK_FROM, m_editStockCodeFrom);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_FROM, m_buttonBrowseFrom);
	DDX_Control(pDX, IDC_EDIT_STOCK_TO, m_editStockCodeTo);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_TO, m_buttonBrowseTo);
	
	DDX_Control(pDX, IDC_STATIC_PRESET, m_staticPreset);
	DDX_Control(pDX, IDC_BUTTON_PRESET, m_buttonPreset);
	DDX_Control(pDX, IDC_COMBO_PRESET, m_comboPreset);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_buttonSelect);
	DDX_Control(pDX, IDC_COMBO_REPORTTYPE, m_comboReportType);
	DDX_Control(pDX, IDC_CHECK_BYMONTH, m_checkByMonth);
	DDX_Control(pDX, IDC_CHECK_CATSORT, m_checkCatSort);
	DDX_Control(pDX, IDC_CHECK_PERCENT, m_checkPercent);
	DDX_Control(pDX, IDC_CHECK_ESTPROFIT, m_checkEstimatedProfit);
	DDX_Control(pDX, IDC_CHECK_APPARENT, m_checkApparent);

	DDX_Control(pDX, IDC_BUTTON_GRAPH, m_buttonGraph);

	DDX_Check(pDX, IDC_CHECK_EPOSSALES, m_bEposSales);
	DDX_Check(pDX, IDC_CHECK_MANUALSALES, m_bManualSales);
	DDX_Check(pDX, IDC_CHECK_IMPORTSALES, m_bImportSales);
	
	DDX_Text(pDX, IDC_EDIT_STOCK_FROM, m_strStockCodeFrom);
	DDX_Text(pDX, IDC_EDIT_STOCK_TO, m_strStockCodeTo);
	
	DDX_Check(pDX, IDC_CHECK_BYMONTH, m_bByMonth);
	DDX_Check(pDX, IDC_CHECK_CATSORT, m_bCatSort);
	DDX_Check(pDX, IDC_CHECK_PERCENT, m_bPercent);
	DDX_Check(pDX, IDC_CHECK_ESTPROFIT, m_bEstimatedProfit);
	//}}AFX_DATA_MAP	
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesHistoryReportStockNewDlg, CDialog)
	
	//{{AFX_MSG_MAP(CSalesHistoryReportStockNewDlg)

	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)
	
	ON_BN_CLICKED(IDC_CHECK_STOCK_FILTER, OnToggleStockRange)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCK_FROM, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_FROM, OnButtonBrowseFrom)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCK_TO, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_TO, OnButtonBrowseTo)

	ON_BN_CLICKED(IDC_BUTTON_PRESET, OnButtonPreset)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET, OnSelectPreset)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_CBN_SELCHANGE(IDC_COMBO_REPORTTYPE, OnSelectReportType)
	
	ON_BN_CLICKED(IDC_BUTTON_GRAPH, OnButtonGraph)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)

	//}}AFX_MSG_MAP

	ON_MESSAGE (WM_APP, OnReportMessage)

END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSalesHistoryReportStockNewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_CategoryComboHelpers.SetControlPointers( &m_LocationSelector, &m_CategorySelector );
	m_CategoryComboHelpers.EnableCustomFields();
	
	CWordArray arrayTypes;

	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:
		arrayTypes.Add ( NODE_STOCKPOINT );
		arrayTypes.Add ( NODE_SITE );
		arrayTypes.Add ( NODE_DATABASE );
		arrayTypes.Add ( NODE_SYSTEM );
		break;

	case LOCATION_ACCESS_DB:
		arrayTypes.Add ( NODE_STOCKPOINT );
		arrayTypes.Add ( NODE_SITE );
		arrayTypes.Add ( NODE_DATABASE );
		m_LocationSelector.SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add ( NODE_STOCKPOINT );
		m_LocationSelector.SetSingleSpIdx( PasswordArray.GetLocationAccessSpIdx() );
		break;
	}

	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboLocation, NULL );
	
	m_nCategoryDbIdx = -1;
	m_nCustomFieldDbIdx = -1;
	FillReportCombo(0);
	
	PresetReportManager.StockHist.SetPasswordAccess();
	FillPresetCombo(-1);
	
	InitialiseCustomFieldCombos();
	m_CategoryComboHelpers.UpdateCombos();
	FillSupplierCombo();
	
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

#ifndef STOCKMAN_SYSTEM
	m_checkEcrSales.SetCheck( TRUE );
	m_checkManualSales.SetCheck( FALSE );
	m_checkImportSales.SetCheck( FALSE );
	m_checkEcrSales.ShowWindow ( SW_HIDE );
	m_checkManualSales.ShowWindow ( SW_HIDE );
	m_checkImportSales.ShowWindow ( SW_HIDE );
#endif

	m_comboRound.AddString ( "Dates as selected" );
	m_comboRound.AddString ( "Nearest full week" );
	m_comboRound.AddString ( "Nearest full month" );
	m_comboRound.SetCurSel(0);

	m_comboCatFilterType.AddString( "By category at time of sale" );
	m_comboCatFilterType.AddString( "By category in database" );
	m_comboCatFilterType.SetCurSel(0);

	if ( m_nPresetNo != 0 )
	{
		m_nPresetError = PresetReportManager.SelectFromCombo( NODE_PRESET_STOCK_HIST, m_comboPreset, m_nPresetNo );

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

void CSalesHistoryReportStockNewDlg::FillSupplierCombo()
{
	//ITEM DATA 0 =          CUSTOM SUPPLIERS
	//ITEM DATA 1 =		UNSPECIFIED SUPPLIER
	//ITEM DATA 2 =		ALL SUPPLIERS
	//ITEM DATA <= 9999  =	SUPPLIER NUMBER + 2
	//ITEM DATA >= 10000 =	SUPPLIER SET + 10000

	for ( int nIndex = 0; nIndex < DataManager.Supplier.GetSize(); nIndex++ )
	{
		int nInsertPos = m_comboSupplier.AddString ( DataManager.Supplier.GetName ( nIndex ) );
		m_comboSupplier.SetItemData ( nInsertPos, DataManager.Supplier.GetSuppNo ( nIndex ) + 2 );
	}

	m_comboSupplier.InsertString ( 0, "All" );
	m_comboSupplier.SetItemData ( 0, 2 );

	int nSupplierPos = 1;

	for ( int nIndex = 0; nIndex < DataManager.SupplierSet.GetSize(); nIndex++ )
	{
		m_comboSupplier.InsertString ( nSupplierPos, DataManager.SupplierSet.GetName ( nIndex ) );
		m_comboSupplier.SetItemData ( nSupplierPos, FIRST_SUPPLIERSET_FILTER + nIndex );
		nSupplierPos++;
	}

	m_comboSupplier.InsertString ( nSupplierPos, "Unspecified" );
	m_comboSupplier.SetItemData ( nSupplierPos, 1 );
	nSupplierPos = m_comboSupplier.InsertString ( -1, "Custom" );
	m_comboSupplier.SetItemData ( nSupplierPos, 0 );
	m_comboSupplier.SetCurSel(0);
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::InitialiseCustomFieldCombos()
{
	DataManager.CustomFieldManager.Initialise();

	for ( int nSlot = 0; nSlot < 5; nSlot++ )
	{
		if ( nSlot < DataManager.CustomFieldManager.GetSlotCount() )
			m_staticCustomField[nSlot].SetWindowText( DataManager.CustomFieldManager.GetFieldNameBySlot( nSlot ) );
		else
		{
			m_staticCustomField[nSlot].ShowWindow( SW_HIDE );
			m_comboCustomField[nSlot].ShowWindow( SW_HIDE );
		}
	}

	int nOffset = 20 * DataManager.CustomFieldManager.GetSlotCount();

	MoveControl( &m_checkStockFilter, 229, 97 + nOffset );
	MoveControl( &m_editStockCodeFrom, 287, 97 + nOffset );
	MoveControl( &m_editStockCodeTo, 287, 116 + nOffset );
	MoveControl( &m_buttonBrowseFrom, 373, 97 + nOffset );
	MoveControl( &m_buttonBrowseTo, 373, 116 + nOffset );
	
	DataManager.CustomFieldManager.SetIncludeAllFlag( TRUE );
	DataManager.CustomFieldManager.SetIncludeNAFlag( TRUE );
	DataManager.CustomFieldManager.SetIncludeSetsFlag( TRUE );

	for ( int nSlot = 0; nSlot < 5; nSlot++ )
		DataManager.CustomFieldManager.SetComboSelector( nSlot, &m_comboCustomField[ nSlot ] );
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::FillPresetCombo( int nPresetIdxSel )
{
	PresetReportManager.FillPresetCombo( NODE_PRESET_STOCK_HIST, m_comboPreset, nPresetIdxSel );
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::FillReportCombo( int nSelection )
{
	m_comboReportType.ResetContent();

	int n = 0;
	int nCurSel = 0;
	for ( int nPos = 0; nPos < DataManagerNonDb.SalesHistoryStock.GetSize(); nPos++ )
	{
		CSalesHistoryCSVRecordStock ReportRecord;
		DataManagerNonDb.SalesHistoryStock.GetAt( nPos, ReportRecord );

		int nCheck = n;

		switch ( ReportRecord.GetReportType() )
		{
		case SH_CUSTOM:
			m_comboReportType.AddString ( ReportRecord.GetListName() );
			m_comboReportType.SetItemData( n++, 100 + nPos );
			break;

		case SH_STK_ITEM:
		case SH_STK_TOP100:
		case SH_STK_UNSOLD:
		case SH_STK_CAT:
		case SH_STK_CAT_BYWDAY:
		case SH_STK_SP_BYWDAY:
		case SH_STK_MONTHLY:
		case SH_STK_WEEKLY:
		case SH_STK_DAILY:
		case SH_STK_DAILY_BYWEEK:	
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

void CSalesHistoryReportStockNewDlg::SelectReportByItemData( int n )
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

void CSalesHistoryReportStockNewDlg::GetReportDates()
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

bool CSalesHistoryReportStockNewDlg::GetAdhocReportSession()
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

bool CSalesHistoryReportStockNewDlg::GetSupplierArray( CWordArray& wSuppSelectArray )
{
	int nSelection = m_comboSupplier.GetCurSel();

	if ( nSelection < 0 || nSelection >= m_comboSupplier.GetCount() )
		return FALSE;

	int nSupplierFilter = m_comboSupplier.GetItemData( nSelection );
	
	//CUSTOM LIST OF SUPPLIERS
	if ( nSupplierFilter == 0 )
	{
		CStockReportSupplierDlg dlg ( m_wSuppSelectArray, this );
		if ( dlg.DoModal() != IDOK )	return FALSE;

		for ( int nIndex = 0; nIndex < m_wSuppSelectArray.GetSize(); nIndex++ )
			wSuppSelectArray.Add ( m_wSuppSelectArray.GetAt ( nIndex ) );
	}
	//ALL SUPPLIERS OR SELECTED SUPPLIER
	else 
	{
		if ( nSupplierFilter < FIRST_SUPPLIERSET_FILTER ) 
			nSupplierFilter -= 2;

		DataManager.SupplierSet.CreateSuppSelectArray ( nSupplierFilter, wSuppSelectArray );	
	}

	if ( wSuppSelectArray.GetSize() < 1 )
	{
		Prompter.Error ( "You must select at least one supplier\nbefore you can view this report." );
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnSelectLocation() 
{	
	m_CategoryComboHelpers.UpdateCombos();
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnToggleStockRange() 
{
	bool bEnable = ( m_checkStockFilter.GetCheck() != 0 );
	m_editStockCodeFrom.EnableWindow ( bEnable );
	m_editStockCodeTo.EnableWindow ( bEnable );
	m_buttonBrowseFrom.EnableWindow ( bEnable );
	m_buttonBrowseTo.EnableWindow ( bEnable );
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnKillFocusStockCode() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_strStockCodeFrom = BarcodePluNoTable.ConvertStockCode ( m_strStockCodeFrom );
		m_strStockCodeTo = BarcodePluNoTable.ConvertStockCode ( m_strStockCodeTo );
		UpdateData ( FALSE );
	}
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnButtonBrowseFrom() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		{
			CWaitCursor wait;

			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
		}
	
		CWaitCursor wait;
		CStockFilterArray StockFilterArray ( FILTERTYPE_BROWSE_STOCK );
		StockFilterArray.SetSupplierFilter ( 0 );
		StockFilterArray.SetCategoryFilter ( m_CategorySelector.GetCategoryFilter() );
		
		CStockFilterDlg dlgBrowse ( StockFilterArray, this );
		
		if ( dlgBrowse.DoModal() != IDOK )
			return;

		int nStockIdx = dlgBrowse.GetStockIdx();

		if ( nStockIdx < 0 )
			return;

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );
		m_strStockCodeFrom = StockRecord.GetStockCode ();	
		UpdateData ( FALSE );
		m_editStockCodeFrom.SetFocus();
		m_editStockCodeFrom.SetSel ( 0, -1 );
	}	
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnButtonBrowseTo() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		{
			CWaitCursor wait;
			
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
		}
	
		CWaitCursor wait;
		CStockFilterArray StockFilterArray ( FILTERTYPE_BROWSE_STOCK );
		StockFilterArray.SetSupplierFilter ( 0 );
		StockFilterArray.SetCategoryFilter ( m_CategorySelector.GetCategoryFilter() );
	
		CStockFilterDlg dlgBrowse ( StockFilterArray, this );
		
		if ( dlgBrowse.DoModal() != IDOK )
			return;

		int nStockIdx = dlgBrowse.GetStockIdx();

		if ( nStockIdx < 0 )
			return;

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );
		m_strStockCodeTo = StockRecord.GetStockCode ();	
		UpdateData ( FALSE );
		m_editStockCodeTo.SetFocus();
		m_editStockCodeTo.SetSel ( 0, -1 );
	}	
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnButtonPreset() 
{
	CPresetReportInfo Preset;
	CreatePreset( Preset );
	PresetReportManager.PresetButton( NODE_PRESET_STOCK_HIST, m_comboPreset, Preset, m_pParentWnd );
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::CreatePreset( CPresetReportInfo& Base )
{
	CPresetReportInfoStockHist Preset;

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

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	Base.SetPresetConLevel( LocSelEntity.GetConType() );

	int nEntityNo1, nEntityNo2;
	CPresetReportArray::GetLocSelEntityNo( LocSelEntity, nEntityNo1, nEntityNo2 );
	Base.SetPresetEntityNo1( nEntityNo1 );
	Base.SetPresetEntityNo2( nEntityNo2 );

	Preset.SetEposSalesFlag( m_checkEposSales.GetCheck() != 0 );
	Preset.SetManualSalesFlag( m_checkManualSales.GetCheck() != 0 );
	Preset.SetImportSalesFlag( m_checkImportSales.GetCheck() != 0 );

	Preset.SetStockRangeFlag( m_checkStockFilter.GetCheck() != 0 );

	CString strStockFrom;
	m_editStockCodeFrom.GetWindowText( strStockFrom );
	Preset.SetStockFrom( strStockFrom );

	CString strStockTo;
	m_editStockCodeTo.GetWindowText( strStockTo );
	Preset.SetStockTo( strStockTo );
	
	Preset.SetCategoryFilter( m_CategorySelector.GetCategoryFilter() );
	Preset.SetCategoryFilterType( m_comboCatFilterType.GetCurSel() );

	for ( int nSlot = 0; nSlot < DataManager.CustomFieldManager.GetSlotCount(); nSlot++ )
	{
		int nFieldNo = DataManager.CustomFieldManager.GetFieldNoBySlot( nSlot );
		int nFilter = DataManager.CustomFieldManager.GetComboSelection( nSlot );
		Preset.SetCustomFieldFilter( nFieldNo, nFilter );
	}
	
	int nSel = m_comboSupplier.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboSupplier.GetCount() )
		Preset.SetSupplierFilter( m_comboSupplier.GetItemData(nSel) );
	
	CString strReportName;
	m_comboReportType.GetWindowText( strReportName );
	Preset.SetReportName( strReportName );

	Preset.SetByMonthFlag( m_checkByMonth.GetCheck() != 0 );
	Preset.SetCatSortFlag( m_checkCatSort.GetCheck() != 0 );
	Preset.SetPercentSalesFlag( m_checkPercent.GetCheck() != 0 );
	Preset.SetEstProfitFlag( m_checkEstimatedProfit.GetCheck() != 0 );
	Preset.SetApparentFlag( m_checkApparent.GetCheck() != 0 );

	CString str;
	Preset.ToString( str );
	Base.SetOtherFields( str );
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnSelectPreset() 
{
	int nSel = m_comboPreset.GetCurSel() - 1;
	if ( nSel < 0 || nSel >= PresetReportManager.StockHist.GetSize() )
		return;

	CPresetReportInfo Base;
	PresetReportManager.StockHist.GetAt( nSel, Base );

	CPresetReportInfoStockHist Preset;
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

	//LOCATION GROUP
	int nConLevel = Base.GetPasswordConLevel();
	int nEntity1 = Base.GetPasswordEntityNo1();
	int nEntity2 = Base.GetPasswordEntityNo2();
	
	int nEntityIdx = 0;
	CPresetReportArray::GetEntityIdx( nConLevel, nEntity1, nEntity2, nEntityIdx );	
	m_LocationSelector.ForceSelection( nConLevel, nEntityIdx );

	m_checkEposSales.SetCheck( Preset.GetEposSalesFlag() );
	m_checkManualSales.SetCheck( Preset.GetManualSalesFlag() );
	m_checkImportSales.SetCheck( Preset.GetImportSalesFlag() );

	OnSelectLocation();

	//FILTER GROUP
	m_checkStockFilter.SetCheck( Preset.GetStockRangeFlag() );
	OnToggleStockRange();

	CString strStockFrom = Preset.GetStockFrom();
	if ( strStockFrom.GetLength() <= Stock::StockCode.Max )
		m_editStockCodeFrom.SetWindowText( strStockFrom );

	CString strStockTo = Preset.GetStockTo();
	if ( strStockTo.GetLength() <= Stock::StockCode.Max )
		m_editStockCodeTo.SetWindowText( strStockTo );

	m_CategorySelector.UpdateCategoryCombo( Preset.GetCategoryFilter() );

	nType = Preset.GetCategoryFilterType();
	if ( nType >= 0 && nType < m_comboCatFilterType.GetCount() )
		m_comboCatFilterType.SetCurSel( nType );

	for ( int nSlot = 0; nSlot < DataManager.CustomFieldManager.GetSlotCount(); nSlot++ )
	{
		int nFieldNo = DataManager.CustomFieldManager.GetFieldNoBySlot( nSlot );
		int nFilter = Preset.GetCustomFieldFilter( nFieldNo );
		DataManager.CustomFieldManager.ForceComboSelection( nSlot, nFilter );
	}

	nType = Preset.GetSupplierFilter();
	for ( int n = 0; n < m_comboSupplier.GetCount(); n++ )
	{
		if ( m_comboSupplier.GetItemData(n) == (WORD) nType )
		{
			m_comboSupplier.SetCurSel(n);
			break;
		}
	}

	//REPORT GROUP
	m_checkByMonth.SetCheck( Preset.GetByMonthFlag() );
	m_checkCatSort.SetCheck( Preset.GetCatSortFlag() );
	m_checkPercent.SetCheck( Preset.GetPercentSalesFlag() );
	m_checkEstimatedProfit.SetCheck( Preset.GetEstProfitFlag() );
	m_checkApparent.SetCheck( Preset.GetApparentFlag() );

	nResult = m_comboReportType.FindStringExact( 0, Preset.GetReportName() );
		
	if ( nResult != CB_ERR )
		m_comboReportType.SetCurSel( nResult );
	
	OnSelectReportType();
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnButtonSelect() 
{
	int nSel = m_comboReportType.GetCurSel();
	if ( nSel < 0 || nSel >= m_comboReportType.GetCount() )
		return;

	//SELECT FOR ADHOC REPORT SHOULD JUST GO THROUGH DISPLAY ROUTINE
	if ( nSel == m_comboReportType.GetCount() - 1 )
	{
		OnButtonDisplay();
		return;
	}

	int nOldSelect = m_comboReportType.GetItemData( nSel );

	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenSalesHistoryStockBuffer( info ) == FALSE )
		Prompter.ShareError( info );
	else
	{
		CListDataSalesHistoryStockDlg dlg ( nOldSelect, this );
	
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteSalesHistoryStockBuffer( info ) == FALSE )
			{
				DataManagerNonDb.CloseSalesHistoryStockBuffer( info );
				Prompter.WriteError( info );
				FillReportCombo( 0 );
				SelectReportByItemData( nOldSelect );
				OnSelectReportType();
			}
			else
			{
				FillReportCombo( dlg.GetListSelection() );
				DataManagerNonDb.CloseSalesHistoryStockBuffer( info );
			}
		}
		else
		{
			DataManagerNonDb.CloseSalesHistoryStockBuffer( info );
			FillReportCombo( 0 );
			SelectReportByItemData( nOldSelect );
			OnSelectReportType();
		}
	}	
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnSelectReportType() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	int nSel = m_comboReportType.GetCurSel();
	if ( nSel < 0 || nSel >= m_comboReportType.GetCount() )
		return;

	CSalesHistoryCSVRecordStock ReportRecord;
	
	if ( nSel < DataManagerNonDb.SalesHistoryStock.GetSize() )
		DataManagerNonDb.SalesHistoryStock.GetAt( nSel, ReportRecord );
	else
		DataManagerNonDb.SalesHistoryStock.LoadAdhocReport( ReportRecord );

	m_checkByMonth.EnableWindow( ReportRecord.CanDoMonthlyBreakdown() );

	bool bEnableStockRange = ( m_checkStockFilter.GetCheck() != 0 );
	m_editStockCodeFrom.EnableWindow( bEnableStockRange );
	m_editStockCodeTo.EnableWindow( bEnableStockRange );
	m_buttonBrowseTo.EnableWindow( bEnableStockRange );
	m_buttonBrowseFrom.EnableWindow( bEnableStockRange );

	m_checkCatSort.EnableWindow( ReportRecord.CanDoCategorySort() );
	m_checkEstimatedProfit.EnableWindow ( ReportRecord.CanDoEstimatedProfit() );
	m_checkPercent.EnableWindow( ReportRecord.CanDoPercentSale() );
	m_checkApparent.EnableWindow( ReportRecord.CanDoApparent() );
	
	m_buttonGraph.EnableWindow( ReportRecord.CanDoGraph() );
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnButtonGraph() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	if ( m_bEposSales == FALSE && m_bManualSales == FALSE && m_bImportSales == FALSE )
	{
		Prompter.Error ( "You must select at least one type of\nsales to include in the report." );
		return;
	}

	CWordArray wSuppSelectArray;
	if ( GetSupplierArray ( wSuppSelectArray ) == FALSE )
		return;

	//DataManager.CustomFieldManager.UpdateValueMapsFromComboSelectors();

	CWaitCursor wait;	
	GetReportDates();

	if ( m_comboSession.GetCurSel() == m_comboSession.GetCount() - 1 )
		if ( GetAdhocReportSession() == FALSE )
			return;

	//WORK OUT FILES WE NEED TO READ FOR REPORT
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );

	m_ReportRecord.Reset();
	
	int nSel = m_comboReportType.GetCurSel();
	if ( nSel < 0 || nSel >= DataManagerNonDb.SalesHistoryStock.GetSize() )
		return;

	DataManagerNonDb.SalesHistoryStock.GetAt( nSel, m_ReportRecord );
	if ( m_ReportRecord.CanDoGraph() == FALSE )
		return;

	CString strConTitle = LocSelEntity.GetConsolidationTitle();
	
	m_HistoryReport.Reset( m_ReportRecord, LocSelEntity, (m_bEposSales != 0), (m_bManualSales != 0), (m_bImportSales != 0) );
	m_HistoryReport.SetConLevelTitleForGraph( strConTitle );
	m_HistoryReport.SetDates ( m_oleDateFrom, m_oleDateTo, m_ReportSession );

	CString strSessionTitle;
	m_comboSession.GetWindowText( strSessionTitle );
	m_HistoryReport.SetSessionTitle( strSessionTitle );

	{
		int nConType = LocSelEntity.GetConType();
		m_HistoryReport.m_ReportFilters.CategoryFilter.SetConsolidationType( nConType );
		m_HistoryReport.m_ReportFilters.CategoryFilter.SetCategoryFilterNo( m_CategorySelector.GetCategoryFilter() );
	}
	
	m_HistoryReport.SetCategoryByItemFlag( m_comboCatFilterType.GetCurSel() == 1 );
	m_HistoryReport.SetSupplierFilter( wSuppSelectArray );

	m_HistoryReport.PrepareFields();
	m_HistoryReport.Consolidate();

	switch ( m_HistoryReport.CreateGraph() )
	{
	case 0:
		{
			CReportManager ReportManager;
			ReportManager.DisplayGraph( Super.ReportFilename() );
		}
		break;
			
	case 2:	
		Prompter.Error ( "No sales data was found in the specified range." );
		break;

	case 1:
	default:	
		Prompter.Error ( "Unable to create graph" );	break;
		break;
	}
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::OnButtonDisplay() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	OnKillFocusStockCode();

	if ( m_bEposSales == FALSE && m_bManualSales == FALSE && m_bImportSales == FALSE )
	{
		Prompter.Error ( "You must select at least one type of\nsales to include in the report." );
		return;
	}

	CWordArray wSuppSelectArray;
	if ( GetSupplierArray ( wSuppSelectArray ) == FALSE )
		return;

	//DataManager.CustomFieldManager.UpdateValueMapsFromComboSelectors();

	CWaitCursor wait;
	GetReportDates();

	if ( m_comboSession.GetCurSel() == m_comboSession.GetCount() - 1 )
		if ( GetAdhocReportSession() == FALSE )
			return;

	//WORK OUT FILES WE NEED TO READ FOR REPORT
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );

	m_ReportRecord.Reset();
	
	int nSel = m_comboReportType.GetCurSel();
	if ( nSel >= 0 && nSel < DataManagerNonDb.SalesHistoryStock.GetSize() )
		DataManagerNonDb.SalesHistoryStock.GetAt( nSel, m_ReportRecord );
	else
	{
		DataManagerNonDb.SalesHistoryStock.LoadAdhocReport( m_ReportRecord );
	
		CSalesHistoryCSVRecordEditStockDlg dlg( m_ReportRecord, TRUE, this );
		if ( dlg.DoModal() != IDOK )
			return;
		
		DataManagerNonDb.SalesHistoryStock.SaveAdhocReport( m_ReportRecord );
	}

	int nReportType = m_ReportRecord.GetReportType();
	if ( nReportType == SH_STK_TOP100 )
		m_ReportRecord.SetReportMode( SALESHISTORY_REPORTMODE_CHART );

	if ( LocSelEntity.GetConType() == NODE_SYSTEM )
	{
		switch( nReportType )
		{
		case SH_STK_UNSOLD:
			Prompter.Error( "The Unsold Items report cannot be consolidated at a system level" );
			return;
		}
	}
	
	if ( m_bCatSort && m_bByMonth )
	{
		switch ( nReportType )
		{
		case SH_STK_ITEM:		nReportType = SH_STK_ITEM_MONTH_CAT;	break;
		case SH_STK_TOP100:		nReportType = SH_STK_TOP100_MONTH_CAT;	break;
		case SH_STK_UNSOLD:		nReportType = SH_STK_UNSOLD_MONTH_CAT;	break;
		}
	}
	else if ( m_bByMonth )
	{
		switch ( nReportType )
		{
		case SH_STK_ITEM:		nReportType = SH_STK_ITEM_MONTH;		break;
		case SH_STK_TOP100:		nReportType = SH_STK_TOP100_MONTH;		break;
		case SH_STK_UNSOLD:		nReportType = SH_STK_UNSOLD_MONTH;		break;
		case SH_STK_CAT:		nReportType = SH_STK_CAT_MONTH;		break;
		}
	}
	else if ( m_bCatSort )
	{
		switch ( nReportType )
		{
		case SH_STK_ITEM:		nReportType = SH_STK_ITEM_CAT;		break;
		case SH_STK_TOP100:		nReportType = SH_STK_TOP100_CAT;		break;
		case SH_STK_UNSOLD:		nReportType = SH_STK_UNSOLD_CAT;		break;
		}
	}
	m_ReportRecord.SetReportType( nReportType );
	m_HistoryReport.Reset( m_ReportRecord, LocSelEntity, (m_bEposSales != 0), (m_bManualSales != 0), (m_bImportSales != 0) );
	m_HistoryReport.SetDates ( m_oleDateFrom, m_oleDateTo, m_ReportSession );

	CString strSessionTitle;
	m_comboSession.GetWindowText( strSessionTitle );
	m_HistoryReport.SetSessionTitle( strSessionTitle );
	
	if ( m_checkStockFilter.GetCheck() != 0 )
	{
		m_HistoryReport.SetItemRangeFlag ( TRUE );
		m_HistoryReport.SetItemFrom ( m_strStockCodeFrom );
		m_HistoryReport.SetItemTo ( m_strStockCodeTo );
	}

	m_bReportShowsPercent = FALSE;
	m_bReportShowsProfit = FALSE;
	m_bReportShowsApparent = FALSE;

	if ( m_ReportRecord.CanDoPercentSale() )
		m_bReportShowsPercent = ( m_checkPercent.GetCheck() != 0 );

	if ( m_ReportRecord.CanDoEstimatedProfit() )
		m_bReportShowsProfit = ( m_checkEstimatedProfit.GetCheck() != 0 );

	if ( m_ReportRecord.CanDoApparent() )
		m_bReportShowsApparent = ( m_checkApparent.GetCheck() != 0 );

	m_HistoryReport.SetPercentSaleFlag( m_bReportShowsPercent );
	m_HistoryReport.SetEstimatedProfitFlag( m_bReportShowsProfit );
	m_HistoryReport.SetApparentFlag( m_bReportShowsApparent );

	{
		int nConType = LocSelEntity.GetConType();
		m_HistoryReport.m_ReportFilters.CategoryFilter.SetConsolidationType( nConType );
		m_HistoryReport.m_ReportFilters.CategoryFilter.SetCategoryFilterNo( m_CategorySelector.GetCategoryFilter() );
	}

	m_HistoryReport.SetCategoryByItemFlag( m_comboCatFilterType.GetCurSel() == 1 );
	m_HistoryReport.SetSupplierFilter( wSuppSelectArray );

	if ( m_HistoryReport.PrepareFields() == FALSE )
	{
		Prompter.Error ( "You have not selected any fields\nto include in this report" );
		return;
	}

	if ( m_ReportRecord.GetReportMode() == SALESHISTORY_REPORTMODE_CHART )
	{
		CDataManagerInfo info;
		if ( DataManagerNonDb.OpenSalesHistoryStockBuffer( info ) == FALSE )
			Prompter.ShareError( info );
		else
		{
			CStockReportTop100Dlg dlgReport ( TOP100_SALESHISTORY_STOCK, m_ReportRecord.GetActualName(), m_ReportRecord, this );
		
			m_bChangeTop100 = FALSE;
			dlgReport.DoModal();
			
			if ( m_bChangeTop100 )
				if ( DataManagerNonDb.WriteSalesHistoryStockBuffer( info ) == FALSE )
					Prompter.WriteError( info );
				
			DataManagerNonDb.CloseSalesHistoryStockBuffer( info );
		}
	}
	else
	{
		m_HistoryReport.Consolidate();
		
		if ( m_HistoryReport.CreateReport() == FALSE )
			Prompter.Error ( "Unable to create report" );
		else
		{
			CFilenameUpdater FnUp( SysFiles::CustomHistPrm );

			CString strKey = m_ReportRecord.GetKey( m_bReportShowsPercent, m_bReportShowsProfit, m_bReportShowsApparent );
			m_HistoryReport.ShowStockpoints();
			
			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, this );
		}
	}
}

/**********************************************************************/

long CSalesHistoryReportStockNewDlg::OnReportMessage( WPARAM wIndex, LPARAM lParam )
{ 
	switch ( wIndex )
	{
	case TOP100_MESSAGE:
		{
			CStockReportTop100Dlg* pDlg = (CStockReportTop100Dlg*) lParam;
			HandleTop100Report( pDlg );
		}
		break;
	}

	return 0L;
}

/**********************************************************************/

void CSalesHistoryReportStockNewDlg::HandleTop100Report( CStockReportTop100Dlg* pDlg )
{
	m_ReportRecord.SetReportMode( SALESHISTORY_REPORTMODE_CHART );
	pDlg -> GetChartInfo( m_ReportRecord );
	
	//SAVE CHANGES BACK TO RECORD
	int nSel = m_comboReportType.GetCurSel();
	if ( nSel >= 0 && nSel < DataManagerNonDb.SalesHistoryStock.GetSize() )
	{
		CSalesHistoryCSVRecordStock ArrayRecord;
		DataManagerNonDb.SalesHistoryStock.GetAt( nSel, ArrayRecord );

		bool bCanChange = FALSE;
		if ( ( ArrayRecord.GetBaseReportType() == SH_STK_TOP100 ) && ( m_ReportRecord.GetBaseReportType() == SH_STK_TOP100 ) )
			bCanChange = TRUE;

		if ( ( ArrayRecord.GetReportType() == SH_CUSTOM ) && ( ArrayRecord.GetReportMode() == SALESHISTORY_REPORTMODE_CHART ) && ( m_ReportRecord.GetReportType() == SH_CUSTOM ) )
			bCanChange = TRUE;

		if ( bCanChange )
		{
			pDlg -> GetChartInfo( ArrayRecord );
			DataManagerNonDb.SalesHistoryStock.SetAt( nSel, ArrayRecord );
			m_bChangeTop100 = TRUE;
		}
	}
	else if ( nSel == DataManagerNonDb.SalesHistoryStock.GetSize() )
	{
		CSalesHistoryCSVRecordStock AdhocRecord;
		DataManagerNonDb.SalesHistoryStock.LoadAdhocReport( AdhocRecord );
		pDlg -> GetChartInfo( AdhocRecord );
		DataManagerNonDb.SalesHistoryStock.SaveAdhocReport( AdhocRecord );
	}

	m_HistoryReport.SetChartDetails( m_ReportRecord );
	m_HistoryReport.Consolidate();

	if ( m_HistoryReport.CreateReport() == FALSE )
		Prompter.Error ( "Unable to create report" );
	else
	{
		CFilenameUpdater FnUp( SysFiles::CustomHistPrm );

		CString strKey = m_ReportRecord.GetKey( m_bReportShowsPercent, m_bReportShowsProfit, m_bReportShowsApparent );
		m_HistoryReport.ShowStockpoints();
		
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, this );
	}
}

/**********************************************************************/
