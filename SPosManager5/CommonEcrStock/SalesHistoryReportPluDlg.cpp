/**********************************************************************/
#include <process.h>
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "CommsHandshaker.h"
#include "FilterTypes.h"
#include "ListDataSalesHistoryPluDlg.h"
#include "MaxLengths.h"
#include "Password.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "PresetReportManager.h"
#include "SalesHistoryCSVRecordEditPluDlg.h"
#include "SalesHistoryPriceLevelDlg.h"
#include "SalesHistoryModifierDlg.h"
#include "SalesHistoryTaxDlg.h"
#include "SessionEditDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryReportPluDlg.h"
/**********************************************************************/

CSalesHistoryReportPluDlg::CSalesHistoryReportPluDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent /*=NULL*/)
	: CSSDialog(CSalesHistoryReportPluDlg::IDD, pParent), 
		m_FilterArray ( FILTERTYPE_BROWSE_PLU ),
		m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
		m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo ),
		m_DateSelectorRange2( m_comboDates2, m_DatePickerFrom2, m_DatePickerTo2 )
{
	//{{AFX_DATA_INIT(CSalesHistoryReportPluDlg)
	//}}AFX_DATA_INIT
	m_bChangeTop100 = FALSE;
	m_nPresetNo = nPresetNo;
	m_nPresetError = 0;
	m_bAutoRunPreset = bAutoRunPreset;

	DataManagerNonDb.ServerNameTable.BuildList();
	DataManagerNonDb.CustomerNameTable.BuildList();
	DataManagerNonDb.EODDateTimeFilterArray.PrepareForUse();
	DataManagerNonDb.SessionDateTimeFilter.Reset();
}

/**********************************************************************/

CSalesHistoryReportPluDlg::~CSalesHistoryReportPluDlg()
{
	DataManagerNonDb.ServerNameTable.ClearList();
	DataManagerNonDb.EODDateTimeFilterArray.ClearAfterUse();
	DataManagerNonDb.SessionDateTimeFilter.Reset();
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesHistoryReportPluDlg)
	
	//FIRST DATE RANGE
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_COMBO_ROUND, m_comboRound);
	//TIME
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	DDX_Control(pDX, IDC_COMBO_TIMESLICE, m_comboTimeSlice);
	//PLU SALE TIME
	DDX_Control(pDX, IDC_COMBO_PLUTIME, m_comboPluTime);
	//SECOND DATE RANGE
	DDX_Control(pDX, IDC_CHECK_DATE_COMPARE, m_checkDateCompare);
	DDX_Control(pDX, IDC_COMBO_AUTODATE2, m_comboAutoDate2);
	DDX_Control(pDX, IDC_COMBO_DATES2, m_comboDates2);
	DDX_Control(pDX, IDC_DATEPICKER_TO2, m_DatePickerTo2);
	DDX_Control(pDX, IDC_DATEPICKER_FROM2, m_DatePickerFrom2);
	DDX_Control(pDX, IDC_COMBO_ROUND2, m_comboRound2);
	//PLU RANGE
	DDX_Control(pDX, IDC_CHECK_SPECIFY, m_checkSpecify);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_TO, m_buttonBrowseTo);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_FROM, m_buttonBrowseFrom);
	DDX_Control(pDX, IDC_COMBO_PLUFILTER, m_comboPluFilter);
	//OTHER ITEM FILTERS
	DDX_Control(pDX, IDC_COMBO_MODIFIER, m_comboModifier);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_DEPTFILTERTYPE, m_comboDeptFilterType);
	DDX_Control(pDX, IDC_BUTTON_TAXBAND, m_buttonTaxBand);
	DDX_Control(pDX, IDC_COMBO_TAXBAND, m_comboTaxBand);
	DDX_Control(pDX, IDC_COMBO_PRICELEVEL, m_comboPriceLevel);
	//LOCATION
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_COMBO_TERMINAL, m_comboTerminal);
	DDX_Control(pDX, IDC_COMBO_TERMDATE, m_comboTermDate);
	DDX_Control(pDX, IDC_STATIC_TABLE, m_staticTable);
	DDX_Control(pDX, IDC_COMBO_TABLE, m_comboTable);
	DDX_Control(pDX, IDC_CHECK_GUEST, m_checkGuest);
	DDX_Control(pDX, IDC_CHECK_EPOSSALES, m_checkEposSales);
	DDX_Control(pDX, IDC_CHECK_MANUALSALES, m_checkManualSales);
	DDX_Control(pDX, IDC_CHECK_IMPORTSALES, m_checkImportSales);
	//REPORT TYPE
	DDX_Control(pDX, IDC_STATIC_PRESET, m_staticPreset);
	DDX_Control(pDX, IDC_BUTTON_PRESET, m_buttonPreset);
	DDX_Control(pDX, IDC_COMBO_PRESET, m_comboPreset);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_buttonSelect);
	DDX_Control(pDX, IDC_COMBO_REPORTTYPE, m_comboReportType);
	DDX_Control(pDX, IDC_CHECK_BYMONTH, m_checkByMonth);
	DDX_Control(pDX, IDC_CHECK_DEPTSORT, m_checkDeptSort);
	DDX_Control(pDX, IDC_CHECK_PERCENT, m_checkPercent);
	DDX_Control(pDX, IDC_CHECK_ESTPROFIT, m_checkEstimatedProfit);
	DDX_Control(pDX, IDC_CHECK_CUSTOMER, m_checkCustomer);
	DDX_Control(pDX, IDC_CHECK_VIPSALES, m_checkVIPSales);
	//GRAPH BUTTON
	DDX_Control(pDX, IDC_BUTTON_GRAPH, m_buttonGraph);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_FROM, m_strPluNoFrom);
	DDX_IntegerString(pDX, IDC_EDIT_PLU_TO, m_strPluNoTo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesHistoryReportPluDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesHistoryReportPluDlg)
	
	//FIRST DATE RANGE
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	//SECOND DATE RANGE
	ON_BN_CLICKED(IDC_CHECK_DATE_COMPARE, OnToggleDateCompare)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTODATE2, OnSelectAutoDate2)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES2, OnSelectDate2)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM2, OnChangeDateFrom2)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO2, OnChangeDateTo2)
	//PLU RANGE
	ON_BN_CLICKED(IDC_CHECK_SPECIFY, OnTogglePluRange)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_FROM, OnKillFocusPluFrom)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_FROM, OnButtonBrowseFrom)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_TO, OnKillFocusPluTo)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_TO, OnButtonBrowseTo)
	//OTHER ITEM FILTERS
	ON_BN_CLICKED(IDC_BUTTON_MODIFIER, OnButtonModifier)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDepartment)
	ON_BN_CLICKED(IDC_BUTTON_TAXBAND, OnButtonTaxBand)
	ON_BN_CLICKED(IDC_BUTTON_PRICELEVEL, OnButtonPriceLevel)
	//LOCATION
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)
	//REPORT TYPE
	ON_BN_CLICKED(IDC_BUTTON_PRESET, OnButtonPreset)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET, OnSelectPreset)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_CBN_SELCHANGE(IDC_COMBO_REPORTTYPE, OnSelectReportType)
	//ACTIONS
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH, OnButtonGraph)
	//}}AFX_MSG_MAP
	ON_MESSAGE (WM_APP, OnReportMessage)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSalesHistoryReportPluDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_buttonTaxBand.SetWindowText( EcrmanOptions.GetTaxBandString() );

	m_DeptAndPluComboHelpers.SetControlPointers( &m_comboPluFilter, &m_LocationSelector, &m_DepartmentSelector );
	
	SubclassEdit ( IDC_EDIT_PLU_FROM, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_TO, SS_NUM, Super.MaxPluLen(), "%s" );

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(30);
	m_comboPriceLevel.SetTabStops( arrayStops );
	m_comboModifier.SetTabStops( arrayStops );

	arrayStops.Add(130);
	m_comboTaxBand.SetTabStops( arrayStops );

	FillReportCombo(0);
	FillTimeSliceCombo(0);
	
	PresetReportManager.PluHist.SetPasswordAccess();
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

	m_LocationSelector.BuildList( arrayTypes );

	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		m_LocationSelector.FillLocationCombo ( &m_comboLocation, &m_comboTerminal, &m_comboTable, &m_TableReportHelpers );
	}
	else
	{
		m_LocationSelector.FillLocationCombo ( &m_comboLocation, &m_comboTerminal );
		ShowAndEnableWindow( &m_staticTable, FALSE );
		ShowAndEnableWindow( &m_comboTable, FALSE );
		MoveControl( &m_checkGuest, 480, 87 );
		MoveControl( &m_checkEposSales, 480, 101 );
		MoveControl( &m_checkManualSales, 480, 115 );
		MoveControl( &m_checkImportSales, 480, 129 );
	}

	//TERM DATE
	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(35);
		m_comboTermDate.SetTabStops(arrayStops);
	}

	m_comboTermDate.AddItem(",Date info by terminal");
	m_comboTermDate.AddItem(",Date info by location");
	m_comboTermDate.AddItem(",Date info summary");
	m_comboTermDate.SetCurSel(0);

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	if ( LocSelEntity.GetConType() != NODE_DATABASE )
	{
		m_checkEposSales.EnableWindow ( FALSE );
		m_checkManualSales.EnableWindow ( FALSE );
		m_checkImportSales.EnableWindow ( FALSE );
	
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
	FillDatabaseSpecificCombos();
	
	m_comboTaxBand.SetCurSel(0);
	
	//MAIN DATE
	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.AddStandardReportRanges();
	m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );

	{
		int nResult = m_comboDates.FindStringExact( 0, EcrmanOptions.GetReportsDefaultDateRange() );
			
		if (nResult != CB_ERR)
		{
			m_comboDates.SetCurSel(nResult);
		}
		else
		{
			m_comboDates.SetCurSel(0);
		}

		OnSelectDate();
	}

	//SECOND DATE
	m_comboAutoDate2.AddString( "Free Selection" );
	m_comboAutoDate2.AddString( "Back 1 week" );
	m_comboAutoDate2.AddString( "Back 4 weeks" );
	m_comboAutoDate2.AddString( "Back 13 weeks" );
	m_comboAutoDate2.AddString( "Back 52 weeks" );
	m_comboAutoDate2.SetItemData(0,0);
	m_comboAutoDate2.SetItemData(1,1);
	m_comboAutoDate2.SetItemData(2,2);
	m_comboAutoDate2.SetItemData(3,3);
	m_comboAutoDate2.SetItemData(4,4);
	m_comboAutoDate2.SetCurSel(0);

	m_DateSelectorRange2.InitialiseDateControls();
	m_DateSelectorRange2.AddStandardReportRanges();
	m_DateSelectorRange2.SetComboSelection( 0, szRANGE_TODAY );

	{
		int nResult = m_comboDates2.FindStringExact( 0, EcrmanOptions.GetReportsDefaultDateRange() );
			
		if (nResult != CB_ERR)
		{
			m_comboDates2.SetCurSel(nResult);
		}
		else
		{
			m_comboDates2.SetCurSel(0);
		}

		OnSelectDate2();
	}

	m_checkDateCompare.SetCheck(FALSE);
	OnToggleDateCompare();

	for (int nIndex = 0; nIndex < DataManagerNonDb.ReportSession.GetSize(); nIndex++)
	{
		m_comboSession.AddString(DataManagerNonDb.ReportSession.GetName(nIndex));
	}

	if (EcrmanOptions.GetReportsEODSessionFlag() == TRUE)
	{
		m_comboSession.AddString("EOD Business Day");
	}

	m_comboSession.AddString( "Adhoc Session" );
	
	if (m_comboSession.GetCount() < 2)
	{
		m_comboSession.InsertString(0, "00:00 to 23:59");
	}

	m_comboSession.EnableWindow( m_comboSession.GetCount() > 1 );
	m_comboSession.SetCurSel(0);

	if (PasswordArray.GetEnable(PasswordTicks::SetupMenu) == FALSE)
	{
		m_buttonSelect.EnableWindow(FALSE);
	}

	m_checkEposSales.SetCheck( TRUE );
	m_checkManualSales.SetCheck( FALSE );
	m_checkImportSales.SetCheck( FALSE );
	
#ifndef STOCKMAN_SYSTEM
	m_checkEposSales.ShowWindow ( SW_HIDE );
	m_checkManualSales.ShowWindow ( SW_HIDE );
	m_checkImportSales.ShowWindow ( SW_HIDE );
#endif

	m_comboRound.AddString ( "Dates as selected" );
	m_comboRound.AddString ( "Nearest full week" );
	m_comboRound.AddString ( "Nearest full month" );
	m_comboRound.SetCurSel(0);

	m_comboRound2.AddString ( "Dates as selected" );
	m_comboRound2.AddString ( "Nearest full week" );
	m_comboRound2.AddString ( "Nearest full month" );
	m_comboRound2.SetCurSel(0);

	m_comboPluTime.AddString( "Transaction completion time" );
	m_comboPluTime.AddString( "Item sale time" );
	m_comboPluTime.SetCurSel( ( EcrmanOptions.GetReportsSalesTimeType() == 1 ) ? 1 : 0 );

	m_comboDeptFilterType.AddString( "By department at time of sale" );
	m_comboDeptFilterType.AddString( "By department in database" );
	m_comboDeptFilterType.SetCurSel(0);

	if (EcrmanOptions.GetReportsVIPType() == 0)
	{
		m_checkVIPSales.ShowWindow(SW_HIDE);
	}

	if ( SysInfo.IsPMSSystem() == FALSE )
	{
		m_checkGuest.SetCheck( FALSE );
		m_checkGuest.EnableWindow( FALSE );
		m_checkGuest.ShowWindow( SW_HIDE );
	}

	if ( m_nPresetNo != 0 )
	{
		m_nPresetError = PresetReportManager.SelectFromCombo( NODE_PRESET_PLU_HIST, m_comboPreset, m_nPresetNo );

		if ( 0 == m_nPresetError )
		{
			OnSelectPreset();

			if (TRUE == m_bAutoRunPreset)
			{
				OnButtonDisplay();
			}
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
CEdit* CSalesHistoryReportPluDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_FROM ); }
CEdit* CSalesHistoryReportPluDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_TO ); }
/**********************************************************************/

void CSalesHistoryReportPluDlg::FillReportCombo( int nSelection )
{	
	m_comboReportType.ResetContent();

	int n = 0;
	int nCurSel = 0;
	for ( int nPos = 0; nPos < DataManagerNonDb.SalesHistoryPlu.GetSize(); nPos++ )
	{
		CSalesHistoryCSVRecordPlu ReportRecord;
		DataManagerNonDb.SalesHistoryPlu.GetAt( nPos, ReportRecord );

		int nCheck = n;

		switch ( ReportRecord.GetReportType() )
		{
		case SH_CUSTOM:
			m_comboReportType.AddString ( ReportRecord.GetListName() );
			m_comboReportType.SetItemData( n++, 100 + nPos );
			break;

		case SH_PLU_ITEM:
		case SH_PLU_TOP100:
		case SH_PLU_UNSOLD:
		case SH_PLU_DEPT:
		case SH_PLU_DEPT_BYWDAY:
		case SH_PLU_LOC_BYWDAY:
		case SH_PLU_MONTHLY:
		case SH_PLU_WEEKLY:
		case SH_PLU_DAILY:
		case SH_PLU_DAILY_BYWEEK:
		case SH_PLU_TAX:
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

void CSalesHistoryReportPluDlg::SelectReportByItemData( int n )
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
/**********************************************************************/
/**********************************************************************/

void CSalesHistoryReportPluDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
	AutoUpdateDate2();
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	AutoUpdateDate2();
	*pResult = 0;
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	AutoUpdateDate2();
	*pResult = 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CSalesHistoryReportPluDlg::FillTimeSliceCombo( int nSelection )
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
/**********************************************************************/
/**********************************************************************/

void CSalesHistoryReportPluDlg::OnToggleDateCompare()
{
	bool bEnable = ( m_checkDateCompare.GetCheck() != 0 );
	bool bAuto = ( m_comboAutoDate2.GetCurSel() != 0 );

	m_comboAutoDate2.EnableWindow( bEnable );
	m_comboDates2.EnableWindow( bEnable & !bAuto );
	m_DatePickerFrom2.EnableWindow( bEnable & !bAuto );
	m_DatePickerTo2.EnableWindow( bEnable & !bAuto );
	m_comboRound2.EnableWindow( bEnable & !bAuto );
	
	if (bEnable && (!bAuto))
	{
		m_DateSelectorRange2.UpdateDateControls();
	}
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnSelectAutoDate2() 
{
	AutoUpdateDate2();
	OnToggleDateCompare();
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::AutoUpdateDate2() 
{
	int nDaysBack = 0;
	switch ( m_comboAutoDate2.GetCurSel() )
	{
	case 1:	nDaysBack = 7;		break;
	case 2:	nDaysBack = 28;		break;
	case 3:	nDaysBack = 91;		break;
	case 4:	nDaysBack = 364;	break;
	}

	if ( nDaysBack != 0 )
	{
		COleDateTime dateFrom, dateTo;
		GetReportDates( m_comboRound.GetCurSel(), m_DatePickerFrom, m_DatePickerTo, dateFrom, dateTo );
		dateFrom -= COleDateTimeSpan( nDaysBack, 0, 0, 0 );
		dateTo -= COleDateTimeSpan( nDaysBack, 0, 0, 0 );

		int nResult = m_comboDates2.FindStringExact( 0, szRANGE_CUSTOM_RANGE );
			
		if ( nResult != CB_ERR )
		{
			m_comboDates2.SetCurSel( nResult );
			m_DatePickerFrom2.SetTime( dateFrom );
			m_DatePickerTo2.SetTime( dateTo );
			m_comboRound2.SetCurSel( m_comboRound.GetCurSel() );
		}
	}

	int nShow = ( 0 == nDaysBack ) ? SW_SHOW : SW_HIDE;
	m_comboRound2.ShowWindow( nShow );
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnSelectDate2() 
{
	m_DateSelectorRange2.UpdateDateControls();
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnChangeDateFrom2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange2.OnChangeFromDate();
	m_DateSelectorRange2.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnChangeDateTo2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange2.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CSalesHistoryReportPluDlg::GetReportDates( int nRoundType, CDateTimeCtrl& DatePickerFrom, CDateTimeCtrl& DatePickerTo, COleDateTime& DateFrom, COleDateTime& DateTo )
{
	DatePickerFrom.GetTime( DateFrom );
	DatePickerTo.GetTime( DateTo );

	switch ( nRoundType )
	{
	case 1:	//ROUND TO WEEK
		{
			int nWeekStartDay = EcrmanOptions.GetReportsWeekStartDay();
				
			int nWeekDayFrom = DateFrom.GetDayOfWeek() - 1;
			int nOffsetFrom = nWeekDayFrom - nWeekStartDay;
			if ( nOffsetFrom < 0 ) nOffsetFrom += 7;
			DateFrom -= COleDateTimeSpan ( nOffsetFrom, 0, 0, 0 );

			int nWeekDayTo = DateTo.GetDayOfWeek() - 1;
			int nOffsetTo = ( nWeekStartDay - nWeekDayTo ) - 1;
			if ( nOffsetTo < 0 ) nOffsetTo += 7;
			DateTo += COleDateTimeSpan ( nOffsetTo, 0, 0, 0 );
		}
		break;

	case 2:	//ROUND TO MONTH
		{
			int nYearFrom = DateFrom.GetYear();
			int nMonthFrom = DateFrom.GetMonth();
			DateFrom = COleDateTime( nYearFrom, nMonthFrom, 1, 0, 0, 0 );
		
			int nYearTo = DateTo.GetYear();
			int nMonthTo = DateTo.GetMonth();
				
			nMonthTo++;
			if ( nMonthTo > 12 )
			{
				nYearTo++;
				nMonthTo = 1;
			}

			DateTo = COleDateTime( nYearTo, nMonthTo, 1, 0, 0, 0 );
			DateTo -= COleDateTimeSpan ( 1, 0, 0, 0 );
		}

	default:
		break;
	}
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::GetReportDates()
{
	GetReportDates( m_comboRound.GetCurSel(), m_DatePickerFrom, m_DatePickerTo, m_oleReportDateFrom, m_oleReportDateTo );
	
	int nDaysBack = 0;
	switch( m_comboAutoDate2.GetCurSel() )
	{
	case 1:	nDaysBack = 7;		break;
	case 2:	nDaysBack = 28;		break;
	case 3:	nDaysBack = 91;		break;
	case 4:	nDaysBack = 364;	break;
	}

	if ( 0 == nDaysBack )
	{
		GetReportDates( m_comboRound2.GetCurSel(), m_DatePickerFrom2, m_DatePickerTo2, m_oleReportDateFrom2, m_oleReportDateTo2 );
	}
	else
	{
		m_oleReportDateFrom2 = m_oleReportDateFrom - COleDateTimeSpan( nDaysBack, 0, 0, 0 );
		m_oleReportDateTo2 = m_oleReportDateTo - COleDateTimeSpan( nDaysBack, 0, 0, 0 );
	}

	bool bSpecialSession = FALSE;
	switch( m_comboSession.GetCount() - m_comboSession.GetCurSel() )
	{
	case 2:
		if ( EcrmanOptions.GetReportsEODSessionFlag() == TRUE )
		{
			m_ReportSession.ClearRecord();
			m_ReportSession.SetEODFlag( TRUE );
			bSpecialSession = TRUE;
		}
		break;

	case 1:
		m_ReportSession.ClearRecord();
		m_ReportSession.SetEODFlag( FALSE );
		bSpecialSession = TRUE;
		break;
	}

	if ( FALSE == bSpecialSession )
	{
		int n = m_comboSession.GetCurSel();
		if ( ( n >= 0 ) && ( n < DataManagerNonDb.ReportSession.GetSize() ) )
			DataManagerNonDb.ReportSession.GetAt ( n, m_ReportSession );
		else
			m_ReportSession.ClearRecord();

		m_ReportSession.SetEODFlag( FALSE );
	}
}

/**********************************************************************/

bool CSalesHistoryReportPluDlg::GetAdhocReportSession()
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
/**********************************************************************/
/**********************************************************************/

void CSalesHistoryReportPluDlg::OnTogglePluRange() 
{
	bool bEnable = ( m_checkSpecify.GetCheck() != 0 );
	GetEditPluFrom() -> EnableWindow ( bEnable );
	GetEditPluTo() -> EnableWindow ( bEnable );
	m_buttonBrowseFrom.EnableWindow ( bEnable );
	m_buttonBrowseTo.EnableWindow ( bEnable );	
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnKillFocusPluFrom() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoFrom = BarcodePluNoTable.ConvertPlu( m_strPluNoFrom );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnButtonBrowseFrom() 
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

void CSalesHistoryReportPluDlg::OnKillFocusPluTo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoTo = BarcodePluNoTable.ConvertPlu( m_strPluNoTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnButtonBrowseTo() 
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
/**********************************************************************/
/**********************************************************************/

void CSalesHistoryReportPluDlg::OnButtonModifier() 
{
	CSalesHistoryModifierDlg dlg ( this );

	if ( dlg.DoModal() == IDOK )
	{
		UpdateCustomModifierString();
		m_comboModifier.SetCurSel( m_comboModifier.GetCount() - 1 );
	}
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::UpdateCustomModifierString()
{	
	int nSel = m_comboModifier.GetCurSel();

	CString strItem = ",";
	strItem += SysInfo.GetCustomModifierFilterString();

	int nPos = m_comboModifier.GetCount() - 1;
	m_comboModifier.DeleteString( nPos );
	m_comboModifier.AddItem ( strItem );
	m_comboModifier.SetItemData( nPos, SysInfo.GetCustomModifierFilter() );

	m_comboModifier.SetCurSel( nSel );
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnSelectDepartment() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnButtonTaxBand() 
{
	CSalesHistoryTaxDlg dlg ( this );
	
	if ( dlg.DoModal() == IDOK )
	{
		UpdateCustomTaxString();
		m_comboTaxBand.SetCurSel( m_comboTaxBand.GetCount() - 1 );
	}
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::UpdateCustomTaxString()
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

void CSalesHistoryReportPluDlg::OnButtonPriceLevel() 
{
	CSalesHistoryPriceLevelDlg dlg ( this );
	
	if ( dlg.DoModal() == IDOK )
	{
		UpdateCustomPriceLevelString();
		m_comboPriceLevel.SetCurSel( m_comboPriceLevel.GetCount() - 1 );
	}
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::UpdateCustomPriceLevelString() 
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
/**********************************************************************/
/**********************************************************************/

void CSalesHistoryReportPluDlg::OnSelectLocation() 
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	int nConType = LocSelEntity.GetConType();
	
	m_LocationSelector.FillEposCombo();
	m_LocationSelector.FillTableCombo();	
	m_DeptAndPluComboHelpers.UpdateCombos();
	FillDatabaseSpecificCombos();

	switch( nConType )
	{
	case NODE_DATABASE:
	case NODE_SYSTEM:
		m_checkEposSales.EnableWindow ( TRUE );
		m_checkManualSales.EnableWindow ( TRUE );
		m_checkImportSales.EnableWindow ( TRUE );
		break;

	default:
		m_checkEposSales.SetCheck ( TRUE );
		m_checkManualSales.SetCheck ( FALSE );
		m_checkImportSales.SetCheck ( FALSE );
		m_checkEposSales.EnableWindow ( FALSE );
		m_checkManualSales.EnableWindow ( FALSE );
		m_checkImportSales.EnableWindow ( FALSE );
		break;
	}

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::FillDatabaseSpecificCombos()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );

	{
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

	{
		int nSel = m_comboModifier.GetCurSel();

		m_comboModifier.ResetContent();
		m_comboModifier.AddItem ( ",All" );
		m_comboModifier.SetItemData (0, MODIFIER_FILTER_ALL );

		int nMask = 1;
		for ( int nMod = 0; nMod < 10; nMod++ )
		{
			if ( 0 == nMod )
			{
				m_comboModifier.AddItem( "00,Base Item" );
				m_comboModifier.SetItemData( nMod + 1, nMask );
			}
			else
			{
				CString strNum;
				strNum.Format( "%2.2d", nMod );

				CCSV csv;
				csv.Add( strNum );
				csv.Add( DataManager.Modifier.GetDisplayName( nMod ) );
				m_comboModifier.AddItem( csv.GetLine() );
				m_comboModifier.SetItemData ( nMod + 1, nMask );
			}
			nMask *= 2;
		}

		CCSV csv;
		csv.Add( "" );
		csv.Add( SysInfo.GetCustomModifierFilterString() );
		m_comboModifier.AddItem( csv.GetLine() );
		m_comboModifier.SetItemData( 11, SysInfo.GetCustomModifierFilter() );
		
		if ( ( nSel >= 0 ) && ( nSel < m_comboModifier.GetCount() ) )
			m_comboModifier.SetCurSel( nSel );
		else
			m_comboModifier.SetCurSel( 0 );
	}

	if ( m_comboPriceLevel.GetCount() == 0 )
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
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CSalesHistoryReportPluDlg::FillPresetCombo( int nPresetIdxSel )
{
	PresetReportManager.FillPresetCombo( NODE_PRESET_PLU_HIST, m_comboPreset, nPresetIdxSel );
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnButtonPreset() 
{
	CPresetReportInfo Preset;
	CreatePreset( Preset );
	PresetReportManager.PresetButton( NODE_PRESET_PLU_HIST, m_comboPreset, Preset, m_pParentWnd );
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnSelectPreset() 
{
	int nSel = m_comboPreset.GetCurSel() - 1;
	if ( nSel < 0 || nSel >= PresetReportManager.PluHist.GetSize() )
		return;

	CPresetReportInfo Base;
	PresetReportManager.PluHist.GetAt( nSel, Base );

	CPresetReportInfoPluHist Preset;
	{
		CString str = Base.GetOtherFields();
		Preset.FromString( str );
	}

	//DATE GROUP
	{
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
	}

	//DATE GROUP 2
	{
		m_checkDateCompare.SetCheck( Preset.GetSecondDateBlockFlag() );

		{
			int nSel = 0;
			for ( int x = 1; x < m_comboAutoDate2.GetCount(); x++ )
			{
				if ( m_comboAutoDate2.GetItemData(x) == Preset.GetSecondDateBlockAutoType() )
				{
					nSel = x;
					break;
				}
			}
			m_comboAutoDate2.SetCurSel(nSel);
		}
		
		int nResult = m_comboDates2.FindStringExact( 0, Preset.GetDateType2() );
			
		if ( nResult != CB_ERR )
			m_comboDates2.SetCurSel( nResult );
		
		CString strDateFrom = Preset.GetDateFrom2();
		CString strDateTo = Preset.GetDateTo2();

		COleDateTime dateFrom, dateTo;
		
		if ( ::GetOleDateFromString( strDateFrom, dateFrom ) == TRUE )
			m_DatePickerFrom2.SetTime( dateFrom );

		if ( ::GetOleDateFromString( strDateTo, dateTo ) == TRUE )
			m_DatePickerTo2.SetTime( dateTo );

		OnSelectDate2();
		AutoUpdateDate2();

		int nType = Preset.GetRoundToType2();
		if ( nType >= 0 && nType < m_comboRound2.GetCount() )
			m_comboRound2.SetCurSel( nType );
	}

	int nResult = m_comboSession.FindStringExact( 0, Preset.GetSessionName() );
		
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

#ifdef STOCKMAN_SYSTEM
	m_checkEposSales.SetCheck( Preset.GetEposSalesFlag() );
	m_checkManualSales.SetCheck( Preset.GetManualSalesFlag() );
	m_checkImportSales.SetCheck( Preset.GetImportSalesFlag() );
#endif

	OnSelectLocation();
	FillDatabaseSpecificCombos();

	{
		int nTermDateType = Preset.GetTermDateType();
		if ((nTermDateType < 0) || (nTermDateType > 2))
		{
			nTermDateType = 0;
		}

		m_comboTermDate.SetCurSel(nTermDateType);
	}

	{
		CTableFilter TableFilter;
		TableFilter.m_nType = Preset.GetTableFilterType();
		TableFilter.m_nLevel = Preset.GetTableFilterLevel();
		TableFilter.m_nEntityNo = Preset.GetTableFilterEntityNo();
		m_LocationSelector.SelectTableComboFilter( TableFilter );
	}

	//FILTER GROUP
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

	int nType = Preset.GetDepartmentFilterType();
	if ( nType >= 0 && nType < m_comboDeptFilterType.GetCount() )
		m_comboDeptFilterType.SetCurSel( nType );

	m_DeptAndPluComboHelpers.SetPluFilterNo( Preset.GetPluFilterNo() );
	m_comboPluTime.SetCurSel( ( Preset.GetSaleTimeFlag() ) ? 1 : 0 );

	nType = Preset.GetTaxFilter();
	if ( nType >= 0 && nType < m_comboTaxBand.GetCount() )
		m_comboTaxBand.SetCurSel( nType );

	nType = Preset.GetPriceLevelFilter();
	if ( nType >= 0 && nType < m_comboPriceLevel.GetCount() )
		m_comboPriceLevel.SetCurSel( nType );

	nType = Preset.GetModifierFilter();
	if ( nType >= 0 && nType < m_comboModifier.GetCount() )
		m_comboModifier.SetCurSel( nType );

	nType = Preset.GetTaxCustomFilter();
	if ( nType != 0 )
	{
		SysInfo.SetCustomTaxFilter( nType );
		UpdateCustomTaxString();
	}

	nType = Preset.GetModifierCustomFilter();
	if ( nType != 0 )
	{
		SysInfo.SetCustomModifierFilter( nType );
		UpdateCustomModifierString();
	}

	nType = Preset.GetPriceLevelCustomFilter();
	if ( nType != 0 )
	{
		SysInfo.SetCustomPriceLevelFilter( nType );
		UpdateCustomPriceLevelString();
	}

	//REPORT GROUP
	m_checkByMonth.SetCheck( Preset.GetByMonthFlag() );
	m_checkDeptSort.SetCheck( Preset.GetDeptSortFlag() );
	m_checkPercent.SetCheck( Preset.GetPercentSalesFlag() );
	m_checkEstimatedProfit.SetCheck( Preset.GetEstProfitFlag() );
	m_checkCustomer.SetCheck( Preset.GetCustomerFlag() );
	m_checkVIPSales.SetCheck( Preset.GetVIPSalesFlag() );
	m_checkGuest.SetCheck( Preset.GetGuestFlag() );

	nResult = m_comboReportType.FindStringExact( 0, Preset.GetReportName() );
		
	if ( nResult != CB_ERR )
		m_comboReportType.SetCurSel( nResult );
	
	OnSelectReportType();
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::CreatePreset( CPresetReportInfo& Base )
{
	CPresetReportInfoPluHist Preset;

	{
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
	}

	{
		Preset.SetSecondDateBlockFlag( m_checkDateCompare.GetCheck() != 0 );

		int nSel = m_comboAutoDate2.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_comboAutoDate2.GetCount() ) )
			Preset.SetSecondDateBlockAutoType( m_comboAutoDate2.GetItemData(nSel) );
		else
			Preset.SetSecondDateBlockAutoType(0);

		CString strDateType;
		m_comboDates2.GetWindowText( strDateType );
		Preset.SetDateType2( strDateType );

		CString strDateFrom;
		COleDateTime dateFrom;
		m_DatePickerFrom2.GetTime( dateFrom );
		::GetStringFromOleDate( dateFrom, strDateFrom );
		Preset.SetDateFrom2( strDateFrom );
		
		CString strDateTo;
		COleDateTime dateTo;
		m_DatePickerTo2.GetTime( dateTo );
		::GetStringFromOleDate( dateTo, strDateTo );
		Preset.SetDateTo2( strDateTo );
		
		Preset.SetRoundToType2( m_comboRound2.GetCurSel() );
	}

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

	Preset.SetTermDateType(m_comboTermDate.GetCurSel());
	Preset.SetEposSalesFlag( m_checkEposSales.GetCheck() != 0 );
	Preset.SetManualSalesFlag( m_checkManualSales.GetCheck() != 0 );
	Preset.SetImportSalesFlag( m_checkImportSales.GetCheck() != 0 );

	{
		CTableFilter TableFilter;
		m_TableReportHelpers.GetTableFilter( m_comboTable.GetCurSel(), TableFilter );
		Preset.SetTableFilterType( TableFilter.m_nType );
		Preset.SetTableFilterLevel( TableFilter.m_nLevel );
		Preset.SetTableFilterEntityNo( TableFilter.m_nEntityNo );
	}

	Preset.SetPluRangeFlag( m_checkSpecify.GetCheck() != 0 );

	CString strPluFrom;
	GetEditPluFrom() -> GetWindowText( strPluFrom );
	Preset.SetPluFrom( strPluFrom );

	CString strPluTo;
	GetEditPluTo() -> GetWindowText( strPluTo );
	Preset.SetPluTo( strPluTo );

	Preset.SetDepartmentFilter( m_DepartmentSelector.GetDepartmentFilter() );
	Preset.SetDepartmentFilterType( m_comboDeptFilterType.GetCurSel() );
	Preset.SetPluFilterNo( m_DeptAndPluComboHelpers.GetPluFilterNo() );
	Preset.SetSaleTimeFlag( m_comboPluTime.GetCurSel() == 1 );
	Preset.SetTaxFilter( m_comboTaxBand.GetCurSel() );
	Preset.SetPriceLevelFilter( m_comboPriceLevel.GetCurSel() );
	Preset.SetModifierFilter( m_comboModifier.GetCurSel() );
	Preset.SetTaxCustomFilter( SysInfo.GetCustomTaxFilter() );
	Preset.SetPriceLevelCustomFilter( SysInfo.GetCustomPriceLevelFilter() );
	Preset.SetModifierCustomFilter( SysInfo.GetCustomModifierFilter() );

	CString strReportName;
	m_comboReportType.GetWindowText( strReportName );
	Preset.SetReportName( strReportName );

	Preset.SetByMonthFlag( m_checkByMonth.GetCheck() != 0 );
	Preset.SetDeptSortFlag( m_checkDeptSort.GetCheck() != 0 );
	Preset.SetPercentSalesFlag( m_checkPercent.GetCheck() != 0 );
	Preset.SetEstProfitFlag( m_checkEstimatedProfit.GetCheck() != 0 );
	Preset.SetCustomerFlag( m_checkCustomer.GetCheck() != 0 );
	Preset.SetVIPSalesFlag( m_checkVIPSales.GetCheck() != 0 );
	Preset.SetGuestFlag( m_checkGuest.GetCheck() != 0 );

	CString str;
	Preset.ToString( str );
	Base.SetOtherFields( str );
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnButtonSelect() 
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
	if ( DataManagerNonDb.OpenSalesHistoryPluBuffer( info ) == FALSE )
		Prompter.ShareError( info );
	else
	{
		CListDataSalesHistoryPluDlg dlg ( nOldSelect, this );
	
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteSalesHistoryPluBuffer( info ) == FALSE )
			{
				DataManagerNonDb.CloseSalesHistoryPluBuffer( info );
				Prompter.WriteError( info );
				FillReportCombo( 0 );
				SelectReportByItemData( nOldSelect );
				OnSelectReportType();
			}
			else
			{
				FillReportCombo( dlg.GetListSelection() );
				DataManagerNonDb.CloseSalesHistoryPluBuffer( info );
			}
		}
		else
		{
			DataManagerNonDb.CloseSalesHistoryPluBuffer( info );
			FillReportCombo( 0 );
			SelectReportByItemData( nOldSelect );
			OnSelectReportType();
		}
	}	
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnSelectReportType() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	int nSel = m_comboReportType.GetCurSel();
	if ( nSel < 0 || nSel >= m_comboReportType.GetCount() )
		return;

	CSalesHistoryCSVRecordPlu ReportRecord;
	
	if ( nSel < DataManagerNonDb.SalesHistoryPlu.GetSize() )
		DataManagerNonDb.SalesHistoryPlu.GetAt( nSel, ReportRecord );
	else
		DataManagerNonDb.SalesHistoryPlu.LoadAdhocReport( ReportRecord );

	m_checkByMonth.EnableWindow( ReportRecord.CanDoMonthlyBreakdown() );

	bool bEnablePluRange = ( m_checkSpecify.GetCheck() != 0 );
	GetEditPluFrom() -> EnableWindow( bEnablePluRange );
	GetEditPluTo() -> EnableWindow( bEnablePluRange );
	m_buttonBrowseTo.EnableWindow( bEnablePluRange );
	m_buttonBrowseFrom.EnableWindow( bEnablePluRange );

	m_checkDeptSort.EnableWindow( ReportRecord.CanDoDeptSort() );
	m_checkEstimatedProfit.EnableWindow ( ReportRecord.CanDoEstimatedProfit() );
	m_checkPercent.EnableWindow( ReportRecord.CanDoPercentSale() );
	
	m_buttonGraph.EnableWindow( ReportRecord.CanDoGraph() );

	if ( ReportRecord.CanDoDateCompare() == TRUE )
	{
		m_checkDateCompare.EnableWindow( TRUE );
		OnToggleDateCompare();
	}
	else
	{
		m_checkDateCompare.EnableWindow( FALSE );
		m_comboDates2.EnableWindow( FALSE );
		m_DatePickerFrom2.EnableWindow( FALSE );
		m_DatePickerTo2.EnableWindow( FALSE );
		m_comboRound2.EnableWindow( FALSE );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

long CSalesHistoryReportPluDlg::OnReportMessage( WPARAM wIndex, LPARAM lParam )
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

void CSalesHistoryReportPluDlg::HandleTop100Report( CStockReportTop100Dlg* pDlg )
{
	m_ReportRecord.SetReportMode( SALESHISTORY_REPORTMODE_CHART );
	pDlg -> GetChartInfo( m_ReportRecord );
	
	//SAVE CHANGES BACK TO RECORD
	int nSel = m_comboReportType.GetCurSel();
	if ( nSel >= 0 && nSel < DataManagerNonDb.SalesHistoryPlu.GetSize() )
	{
		CSalesHistoryCSVRecordPlu ArrayRecord;
		DataManagerNonDb.SalesHistoryPlu.GetAt( nSel, ArrayRecord );

		bool bCanChange = FALSE;
		if ( ( ArrayRecord.GetBaseReportType() == SH_PLU_TOP100 ) && ( m_ReportRecord.GetBaseReportType() == SH_PLU_TOP100 ) )
			bCanChange = TRUE;

		if ( ( ArrayRecord.GetReportType() == SH_CUSTOM ) && ( ArrayRecord.GetReportMode() == SALESHISTORY_REPORTMODE_CHART ) && ( m_ReportRecord.GetReportType() == SH_CUSTOM ) )
			bCanChange = TRUE;

		if ( bCanChange )
		{
			pDlg -> GetChartInfo( ArrayRecord );
			DataManagerNonDb.SalesHistoryPlu.SetAt( nSel, ArrayRecord );
			m_bChangeTop100 = TRUE;
		}
	}
	else if ( nSel == DataManagerNonDb.SalesHistoryPlu.GetSize() )
	{
		CSalesHistoryCSVRecordPlu AdhocRecord;
		DataManagerNonDb.SalesHistoryPlu.LoadAdhocReport( AdhocRecord );
		pDlg -> GetChartInfo( AdhocRecord );
		DataManagerNonDb.SalesHistoryPlu.SaveAdhocReport( AdhocRecord );
	}

	m_HistoryReport.SetTermDateType(m_comboTermDate.GetCurSel());
	
	m_HistoryReport.SetChartDetails( m_ReportRecord );
	m_HistoryReport.Consolidate();

	if (m_HistoryReport.CreatePluSalesHistoryReport() == FALSE)
	{
		Prompter.Error("Unable to create report");
	}
	else
	{
		CString strKey = m_ReportRecord.GetKey( m_bReportShowsPercent, m_bReportShowsProfit );
		m_HistoryReport.WriteTerminalList();

		CReportManager ReportManager;
		CFilenameUpdater FnUp ( SysFiles::CustomHistPrm );
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, this );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CSalesHistoryReportPluDlg::OnButtonDisplay() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	OnKillFocusDeptNo();
	OnKillFocusPluFrom();
	OnKillFocusPluTo();

	if ( ( m_checkEposSales.GetCheck() == 0 ) && ( m_checkManualSales.GetCheck() == 0 ) && ( m_checkImportSales.GetCheck() == 0 ) )
	{
		Prompter.Error ( "You must select at least one type of\nsales to include in the report." );
		return;
	}

	CWaitCursor wait;
	CommsHandshaker.ProcessSales();
	
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
	if ( nSel >= 0 && nSel < DataManagerNonDb.SalesHistoryPlu.GetSize() )
		DataManagerNonDb.SalesHistoryPlu.GetAt( nSel, m_ReportRecord );
	else
	{
		DataManagerNonDb.SalesHistoryPlu.LoadAdhocReport( m_ReportRecord );
	
		CSalesHistoryCSVRecordEditPluDlg dlg( m_ReportRecord, TRUE, this );
		if ( dlg.DoModal() != IDOK )
			return;
		
		DataManagerNonDb.SalesHistoryPlu.SaveAdhocReport( m_ReportRecord );
	}

	int nReportType = m_ReportRecord.GetReportType();
	if ( nReportType == SH_PLU_TOP100 )
		m_ReportRecord.SetReportMode( SALESHISTORY_REPORTMODE_CHART );

	if ( LocSelEntity.GetConType() == NODE_SYSTEM )
	{
		switch( nReportType )
		{
		case SH_PLU_UNSOLD:
			Prompter.Error( "The Unsold Items report cannot be consolidated at a system level" );
			return;
		}
	}
	
	{
		bool bDeptSort = ( m_checkDeptSort.GetCheck() != 0 );
		bool bMonthSort = ( m_checkByMonth.GetCheck() != 0 );

		if ( bDeptSort && bMonthSort )
		{
			switch ( nReportType )
			{
			case SH_PLU_ITEM:	nReportType = SH_PLU_ITEM_MONTH_DEPT;	break;
			case SH_PLU_TOP100:	nReportType = SH_PLU_TOP100_MONTH_DEPT;	break;
			case SH_PLU_UNSOLD:	nReportType = SH_PLU_UNSOLD_MONTH_DEPT;	break;
			}
		}
		else if ( bMonthSort )
		{
			switch ( nReportType )
			{
			case SH_PLU_ITEM:	nReportType = SH_PLU_ITEM_MONTH;	break;
			case SH_PLU_TOP100:	nReportType = SH_PLU_TOP100_MONTH;	break;
			case SH_PLU_UNSOLD:	nReportType = SH_PLU_UNSOLD_MONTH;	break;
			case SH_PLU_DEPT:	nReportType = SH_PLU_DEPT_MONTH;	break;
			}
		}
		else if ( bDeptSort )
		{
			switch ( nReportType )
			{
			case SH_PLU_ITEM:	nReportType = SH_PLU_ITEM_DEPT;		break;
			case SH_PLU_TOP100:	nReportType = SH_PLU_TOP100_DEPT;	break;
			case SH_PLU_UNSOLD:	nReportType = SH_PLU_UNSOLD_DEPT;	break;
			}
		}
	}

	m_ReportRecord.SetReportType( nReportType );

	CTimeSliceMapCSVRecord TimeSliceRecord;
	nSel = m_comboTimeSlice.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.TimeSliceList.GetSize() ) )
		DataManagerNonDb.TimeSliceList.GetAt( nSel, TimeSliceRecord );
	
	if ( ( m_checkDateCompare.GetCheck() != 0 ) && ( m_ReportRecord.CanDoDateCompare() == TRUE ) )
		m_ReportRecord.SetReportMode( SALESHISTORY_REPORTMODE_TWOBLOCK );

	m_ReportRecord.SetReportType( nReportType );
	m_HistoryReport.Reset( m_ReportRecord, LocSelEntity, m_checkEposSales.GetCheck() != 0, m_checkManualSales.GetCheck() != 0, m_checkImportSales.GetCheck() != 0, m_checkGuest.GetCheck() != 0 );
	m_HistoryReport.SetReportDates ( m_oleReportDateFrom, m_oleReportDateTo, m_oleReportDateFrom2, m_oleReportDateTo2, m_ReportSession, TimeSliceRecord );
	
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

	m_bReportShowsProfit = FALSE;
	m_bReportShowsPercent = FALSE;

	if ( m_ReportRecord.CanDoPercentSale() )
		m_bReportShowsPercent = ( m_checkPercent.GetCheck() != 0 );

	if ( m_ReportRecord.CanDoEstimatedProfit() )
		m_bReportShowsProfit = ( m_checkEstimatedProfit.GetCheck() != 0 );

	m_HistoryReport.SetPercentSaleFlag( m_bReportShowsPercent );
	m_HistoryReport.SetEstimatedProfitFlag( m_bReportShowsProfit );
	m_HistoryReport.SetCustomerFlag( m_checkCustomer.GetCheck() != 0 );
	m_HistoryReport.SetVIPSalesFlag( m_checkVIPSales.GetCheck() != 0 );
		
	{
		int nConType = LocSelEntity.GetConType();

		int nPluFilterNo = m_DeptAndPluComboHelpers.GetPluFilterNo();
		m_HistoryReport.m_ReportFilters.PluFilter.SetConsolidationType( nConType );
		m_HistoryReport.m_ReportFilters.PluFilter.SetMasterFilterNo( nPluFilterNo );

		int nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();	
		if ( nDepartmentFilter == 0 ) nDepartmentFilter = FIRST_DEPARTMENTSET_FILTER - 1;
		m_HistoryReport.m_ReportFilters.DeptFilter.SetConsolidationType( nConType );
		m_HistoryReport.m_ReportFilters.DeptFilter.SetDeptFilterNo( nDepartmentFilter );
	}

	m_HistoryReport.SetDepartmentByItemFlag( m_comboDeptFilterType.GetCurSel() == 1 );
	
	m_HistoryReport.SetSaleTimeFlag( m_comboPluTime.GetCurSel() == 1 );

	int nTaxFilter = m_comboTaxBand.GetItemData( m_comboTaxBand.GetCurSel() );
	m_HistoryReport.SetTaxFilter( nTaxFilter );

	int nPriceLevelFilter = m_comboPriceLevel.GetItemData( m_comboPriceLevel.GetCurSel() );
	m_HistoryReport.SetPriceLevelFilter( nPriceLevelFilter );

	int nModifierFilter = m_comboModifier.GetItemData( m_comboModifier.GetCurSel() );
	m_HistoryReport.SetModifierFilter( nModifierFilter );

	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		CTableFilter TableFilter;
		m_TableReportHelpers.GetTableFilter( m_comboTable.GetCurSel(), TableFilter );
		m_HistoryReport.SetTableFilter( TableFilter );
	}

	if ( m_HistoryReport.PrepareFields() == FALSE )
	{
		Prompter.Error ( "You have not selected any fields\nto include in this report" );
		return;
	}

	if ( m_ReportRecord.GetReportMode() == SALESHISTORY_REPORTMODE_CHART )
	{
		CDataManagerInfo info;
		if ( DataManagerNonDb.OpenSalesHistoryPluBuffer( info ) == FALSE )
			Prompter.ShareError( info );
		else
		{
			CStockReportTop100Dlg dlgReport ( TOP100_SALESHISTORY_PLU, m_ReportRecord.GetActualName(), m_ReportRecord, this );
		
			m_bChangeTop100 = FALSE;
			dlgReport.DoModal();
			
			if ( m_bChangeTop100 )
				if ( DataManagerNonDb.WriteSalesHistoryPluBuffer( info ) == FALSE )
					Prompter.WriteError( info );
				
			DataManagerNonDb.CloseSalesHistoryPluBuffer( info );
		}
	}
	else
	{
		m_HistoryReport.SetTermDateType(m_comboTermDate.GetCurSel());

		m_HistoryReport.Consolidate();

		if (m_HistoryReport.CreatePluSalesHistoryReport() == FALSE)
		{
			Prompter.Error("Unable to create report");
		}
		else
		{
			CFilenameUpdater FnUp( SysFiles::CustomHistPrm );
			
			CString strKey = m_ReportRecord.GetKey( m_bReportShowsPercent, m_bReportShowsProfit );
			m_HistoryReport.WriteTerminalList();
			
			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, this );
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportPluDlg::OnButtonGraph() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	OnKillFocusDeptNo();

	if ( ( m_checkEposSales.GetCheck() == 0 ) && ( m_checkManualSales.GetCheck() == 0 ) && ( m_checkImportSales.GetCheck() == 0 ) )
	{
		Prompter.Error ( "You must select at least one type of\nsales to include in the report." );
		return;
	}

	CWaitCursor wait;
	CommsHandshaker.ProcessSales();
	
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
	if ( nSel < 0 || nSel >= DataManagerNonDb.SalesHistoryPlu.GetSize() )
		return;

	DataManagerNonDb.SalesHistoryPlu.GetAt( nSel, m_ReportRecord );
	if ( m_ReportRecord.CanDoGraph() == FALSE )
		return;

	CTimeSliceMapCSVRecord TimeSliceRecord;
	nSel = m_comboTimeSlice.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.TimeSliceList.GetSize() ) )
		DataManagerNonDb.TimeSliceList.GetAt( nSel, TimeSliceRecord );

	CString strConTitle = LocSelEntity.GetConsolidationTitle();
	
	m_HistoryReport.Reset( m_ReportRecord, LocSelEntity, m_checkEposSales.GetCheck() != 0, m_checkManualSales.GetCheck() != 0, m_checkImportSales.GetCheck() != 0, m_checkGuest.GetCheck() != 0 );
	m_HistoryReport.SetConLevelTitleForGraph( strConTitle );
	m_HistoryReport.SetReportDates ( m_oleReportDateFrom, m_oleReportDateTo, m_ReportSession, TimeSliceRecord );

	CString strSessionTitle;
	if ( m_comboSession.GetCurSel() < m_comboSession.GetCount() - 1 )
		m_comboSession.GetWindowText( strSessionTitle );
	else
		strSessionTitle = m_ReportSessionAdhoc.GetName();

	m_HistoryReport.SetSessionTitle( strSessionTitle );

	{
		int nConType = LocSelEntity.GetConType();

		int nPluFilterNo = m_DeptAndPluComboHelpers.GetPluFilterNo();
		m_HistoryReport.m_ReportFilters.PluFilter.SetConsolidationType( nConType );
		m_HistoryReport.m_ReportFilters.PluFilter.SetMasterFilterNo( nPluFilterNo );

		int nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();	
		if ( nDepartmentFilter == 0 ) nDepartmentFilter = FIRST_DEPARTMENTSET_FILTER - 1;
		m_HistoryReport.m_ReportFilters.DeptFilter.SetConsolidationType( nConType );
		m_HistoryReport.m_ReportFilters.DeptFilter.SetDeptFilterNo( nDepartmentFilter );
	}
	
	m_HistoryReport.SetDepartmentByItemFlag( m_comboDeptFilterType.GetCurSel() == 1 );

	m_HistoryReport.SetSaleTimeFlag( m_comboPluTime.GetCurSel() == 1 );

	int nTaxFilter = m_comboTaxBand.GetItemData( m_comboTaxBand.GetCurSel() );
	m_HistoryReport.SetTaxFilter( nTaxFilter );

	int nPriceLevelFilter = m_comboPriceLevel.GetItemData( m_comboPriceLevel.GetCurSel() );
	m_HistoryReport.SetPriceLevelFilter( nPriceLevelFilter );

	int nModifierFilter = m_comboModifier.GetItemData( m_comboModifier.GetCurSel() );
	m_HistoryReport.SetModifierFilter( nModifierFilter );

	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		CTableFilter TableFilter;
		m_TableReportHelpers.GetTableFilter( m_comboTable.GetCurSel(), TableFilter );
		m_HistoryReport.SetTableFilter( TableFilter );
	}

	m_HistoryReport.SetCustomerFlag( m_checkCustomer.GetCheck() != 0 );
	m_HistoryReport.SetVIPSalesFlag( m_checkVIPSales.GetCheck() != 0 );
	
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
		Prompter.Error ( "No sales data was found in\nthe specified range." );
		break;

	case 1:
	default:	
		Prompter.Error ( "Unable to create graph" );	break;
		break;
	}
}

/**********************************************************************/





