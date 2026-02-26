/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "DateRangeHelpers.h"
#include "FilterTypes.h"
#include "LocationCSVArray.h"
#include "LocationSetCSVArray.h"
#include "MaxLengths.h"
#include "NodeTypes.h"
#include "Password.h"
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
#include "..\SPosStockManager5\PluFilterDlgStockman.h"
#include "RepSpawn.h"
#include "SalesHistoryFields.h"
#include "SalesHistoryTaxDlg.h"
#include "SessionEditDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryRegionPluDlg.h"
/**********************************************************************/

CSalesHistoryRegionListItem::CSalesHistoryRegionListItem()
{
	Reset();
}

/**********************************************************************/

void CSalesHistoryRegionListItem::Reset()
{
	m_nDbIdx = 0;
	m_nLineNo = 0;
	m_nConType = NODE_DATABASE;
	m_nEntityIdx = 0;
	m_bSelected = FALSE;
}

/**********************************************************************/

void CSalesHistoryRegionListItem::Add ( CSalesHistoryRegionListItem& source )
{
}

/**********************************************************************/

int CSalesHistoryRegionListItem::Compare( CSalesHistoryRegionListItem& source, int nHint )
{
	if ( m_nDbIdx != source.m_nDbIdx )
		return ( m_nDbIdx > source.m_nDbIdx ? 1 : -1 );

	if ( m_nLineNo != source.m_nLineNo )
		return ( m_nLineNo > source.m_nLineNo ? 1 : -1 );

	if ( m_nConType != source.m_nConType )
		return ( m_nConType > source.m_nConType ? 1 : -1 );
	
	if ( m_nEntityIdx != source.m_nEntityIdx )
		return ( m_nEntityIdx > source.m_nEntityIdx ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSalesHistoryRegionPluDlg::CSalesHistoryRegionPluDlg(CWnd* pParent /*=NULL*/)
	: CSSDialog(CSalesHistoryRegionPluDlg::IDD, pParent), 
		m_FilterArray ( FILTERTYPE_BROWSE_PLU ),
		m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
		m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CSalesHistoryRegionPluDlg)
	//}}AFX_DATA_INIT
	m_nRegionListStartIdx = 0;
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesHistoryRegionPluDlg)
	DDX_Control(pDX, IDC_CHECK_LOCATION, m_checkLocation);
	DDX_Control(pDX, IDC_COMBO_REPORT, m_comboReport);
	DDX_Control(pDX, IDC_BUTTON_TAXBAND, m_buttonTaxBand);
	DDX_Control(pDX, IDC_COMBO_TAXBAND, m_comboTaxBand);
	DDX_Control(pDX, IDC_COMBO_ZEROSKIP, m_comboZeroSkip);
	DDX_Control(pDX, IDC_COMBO_ROUND, m_comboRound);
	DDX_Control(pDX, IDC_BUTTON_GRAPH, m_buttonGraph);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_TO, m_buttonBrowseTo);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_FROM, m_buttonBrowseFrom);
	DDX_Control(pDX, IDC_COMBO_PLUTIME, m_comboPluTime);
	DDX_Control(pDX, IDC_CHECK_SPECIFY, m_checkSpecify);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	DDX_Control(pDX, IDC_STATIC_DATABASE, m_staticDatabase);
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_FROM, m_strPluNoFrom);
	DDX_IntegerString(pDX, IDC_EDIT_PLU_TO, m_strPluNoTo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesHistoryRegionPluDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesHistoryRegionPluDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_FROM, OnButtonBrowseFrom)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_TO, OnButtonBrowseTo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_FROM, OnKillFocusPluFrom)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_TO, OnKillFocusPluTo)
	ON_BN_CLICKED(IDC_CHECK_SPECIFY, OnTogglePluRange)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDepartment)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH, OnButtonGraph)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_BN_CLICKED(IDC_BUTTON_TAXBAND, OnButtonTaxBand)
	ON_CBN_SELCHANGE(IDC_COMBO_REPORT, OnSelectReport)
	ON_CBN_SELCHANGE(IDC_COMBO_DATABASE, OnSelectDatabase)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CSalesHistoryRegionPluDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_FROM ); }
CEdit* CSalesHistoryRegionPluDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_TO ); }
/**********************************************************************/

BOOL CSalesHistoryRegionPluDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_buttonTaxBand.SetWindowText( EcrmanOptions.GetTaxBandString() );

	SubclassEdit ( IDC_EDIT_PLU_FROM, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_TO, SS_NUM, Super.MaxPluLen(), "%s" );

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(30);
	arrayStops.Add(130);
	m_comboTaxBand.SetTabStops( arrayStops );
			
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
	
	m_comboSession.SetCurSel(0);

	if ( m_comboSession.GetCount() < 2 )
		m_comboSession.EnableWindow( FALSE );
	
	m_comboRound.AddString ( "Dates as selected" );
	m_comboRound.AddString ( "Nearest full week" );
	m_comboRound.AddString ( "Nearest full month" );
	m_comboRound.SetCurSel(0);

	m_comboPluTime.AddString( "Transaction completion time" );
	m_comboPluTime.AddString( "Item sale time" );
	m_comboPluTime.SetCurSel( ( EcrmanOptions.GetReportsSalesTimeType() == 1 ) ? 1 : 0 );

	m_comboReport.AddString ( "Plu Item by Region" );
	m_comboReport.AddString ( "Plu Item by Region and Price Level" );
	m_comboReport.AddString ( "Department by Region" );
	m_comboReport.SetItemData( 0, SALESHISTORY_REGION_PLU );
	m_comboReport.SetItemData( 1, SALESHISTORY_REGION_BAND );
	m_comboReport.SetItemData( 2, SALESHISTORY_REGION_DEPT );
	m_comboReport.SetCurSel(0);
	OnSelectReport();

	m_comboZeroSkip.AddString( "Show all items" );
	m_comboZeroSkip.AddString( "Items with no sales" );
	m_comboZeroSkip.AddString( "Price levels with no sales" );
	m_comboZeroSkip.SetCurSel(0);

	m_listLocations.SubclassDlgItem ( IDC_LIST, this );
	m_listLocations.InsertColumn ( 0, "", LVCFMT_LEFT, 55 );
	m_listLocations.InsertColumn ( 1, "Regions", LVCFMT_LEFT, 210 );
	
	m_listLocations.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );
	m_ImageList.Create ( IDB_ICONS_TREE, 31, 0, COLORREF(0xFFFFFF) );
	m_listLocations.SetImageList ( &m_ImageList, LVSIL_SMALL );

	CWordArray arrayTypes;
	arrayTypes.Add( NODE_LOCATION );

	if ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_LOC )
	{
		arrayTypes.Add( NODE_DATABASE );
		arrayTypes.Add( NODE_LOCATIONSET );
	}

	DealerFlags.GetLocationSelectorOrder( arrayTypes );

	int nLineNo = 0;
	for ( int n = 0; n < arrayTypes.GetSize(); n++ )
	{
		switch( arrayTypes.GetAt(n) )
		{
		case NODE_DATABASE:		RegionArrayAddDatabases( nLineNo );		break;
		case NODE_LOCATIONSET:	RegionArrayAddLocationSets( nLineNo );	break;
		case NODE_LOCATION:		RegionArrayAddLocations( nLineNo );		break;
		}
	}

	{
		int nFirstDbIdx = 0;
		int nLastDbIdx = 0;
		
		int nRegionCount = m_arrayRegionInfo.GetSize();
		if ( nRegionCount != 0 )
		{
			CSalesHistoryRegionListItem item1;
			m_arrayRegionInfo.GetAt(0, item1 );
			nFirstDbIdx = item1.m_nDbIdx;

			CSalesHistoryRegionListItem item2;
			m_arrayRegionInfo.GetAt(nRegionCount - 1, item2 );
			nLastDbIdx = item2.m_nDbIdx;
		}

		if ( nFirstDbIdx == nLastDbIdx )
		{
			ShowAndEnableWindow( &m_staticDatabase, FALSE );
			ShowAndEnableWindow( &m_comboDatabase, FALSE );
			MoveControl( &m_listLocations, 251, 7, 210, 203 );
		}
	
		CWordArray arrayTypes;
		arrayTypes.Add( NODE_DATABASE );

		if ( nFirstDbIdx == nLastDbIdx )
			m_LocationSelector.SetSingleDbIdx( nFirstDbIdx );

		m_LocationSelector.ExcludeWebSales();
		m_LocationSelector.BuildList( arrayTypes );
		m_LocationSelector.FillLocationCombo ( &m_comboDatabase, NULL );
	}

	UpdateRegionList();

	m_DeptAndPluComboHelpers.SetControlPointers( NULL, &m_LocationSelector, &m_DepartmentSelector );

	m_editDeptNo.LimitText( MAX_LENGTH_DEPTNO );
	m_DeptAndPluComboHelpers.UpdateCombos();
	
	FillTaxBandComboForLocation();
	m_comboTaxBand.SetCurSel(0);
	
	OnTogglePluRange();
	return TRUE;  
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::RegionArrayAddDatabases( int& nLineNo )
{
	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
		{
			CSalesHistoryRegionListItem item;
			item.m_nDbIdx = nDbIdx;
			item.m_nLineNo = nLineNo++;
			item.m_nConType = NODE_DATABASE;
			item.m_nEntityIdx = nDbIdx;
			m_arrayRegionInfo.Consolidate( item );
		}
	}
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::RegionArrayAddLocationSets( int& nLineNo )
{
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			int nStart, nEnd;
			dbLocationSet.GetLsIdxRange( nDbIdx, nStart, nEnd );
			
			for ( int nLsIdx = nStart; nLsIdx <= nEnd; nLsIdx++ )
				RegionArrayAddLocationSet( nLineNo, nDbIdx, nLsIdx );
		}
	}
	else
	{	
		CWordArray arrayLsIdx;
		dbLocationSet.SortByName( arrayLsIdx );

		for ( int x = 0; x < arrayLsIdx.GetSize(); x++ )
		{
			int nLsIdx = arrayLsIdx.GetAt(x);

			int nDbIdx;
			int nDbNo = dbLocationSet.GetDbNo( nLsIdx );
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
				RegionArrayAddLocationSet( nLineNo, nDbIdx, nLsIdx );
		}
	}
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::RegionArrayAddLocationSet( int& nLineNo, int nDbIdx, int nLsIdx )
{
	if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
	{
		CSalesHistoryRegionListItem item;
		item.m_nDbIdx = nDbIdx;
		item.m_nLineNo = nLineNo++;
		item.m_nConType = NODE_LOCATIONSET;
		item.m_nEntityIdx = nLsIdx;
		m_arrayRegionInfo.Consolidate( item );
	}
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::RegionArrayAddLocations( int& nLineNo )
{
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			CArray<int,int> Set;
			dbLocation.GetDatabaseLocIdxSet( nDbIdx, Set );
			
			for ( int n = 0; n < Set.GetSize(); n++ )
			{
				int nLocIdx = Set.GetAt(n);
				RegionArrayAddLocation( nLineNo, nDbIdx, nLocIdx );
			}
		}
	}
	else
	{
		CArray<int,int> arrayLocIdx;
		dbLocation.SortByName( arrayLocIdx );

		for ( int x = 0; x < arrayLocIdx.GetSize(); x++ )
		{
			int nLocIdx = arrayLocIdx.GetAt(x);

			int nDbIdx;
			int nDbNo = dbLocation.GetDbNo( nLocIdx );
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
				RegionArrayAddLocation( nLineNo, nDbIdx, nLocIdx );
		}
	}
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::RegionArrayAddLocation( int& nLineNo, int nDbIdx, int nLocIdx )
{
	if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == TRUE )
	{
		CSalesHistoryRegionListItem item;
		item.m_nDbIdx = nDbIdx;
		item.m_nLineNo = nLineNo++;
		item.m_nConType = NODE_LOCATION;
		item.m_nEntityIdx = nLocIdx;
		m_arrayRegionInfo.Consolidate( item );
	}
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::UpdateRegionList()
{
	for ( int n = 0; n < m_listLocations.GetItemCount(); n++ )
	{
		int nIdx = m_nRegionListStartIdx + n;

		if ( ( nIdx >= 0 ) && ( nIdx < m_arrayRegionInfo.GetSize() ) )
		{
			CSalesHistoryRegionListItem item;
			m_arrayRegionInfo.GetAt( nIdx, item );
			item.m_bSelected = ( m_listLocations.GetCheck(n) != 0 );
			m_arrayRegionInfo.SetAt( nIdx, item );
		}
	}

	m_listLocations.DeleteAllItems();

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	int nDbIdx = LocSelEntity.GetDbIdx();

	int nStartIdx, nEndIdx;

	CSalesHistoryRegionListItem item;
	item.m_nDbIdx = nDbIdx;
	item.m_nLineNo = 0;
	m_arrayRegionInfo.Find( item, nStartIdx );

	item.m_nDbIdx++;
	m_arrayRegionInfo.Find( item, nEndIdx );
	nEndIdx--;

	m_nRegionListStartIdx = nStartIdx;

	int nListPos = 0;
	for ( int nIdx = nStartIdx; nIdx <= nEndIdx; nIdx++ )
	{
		m_arrayRegionInfo.GetAt( nIdx, item );

		switch( item.m_nConType )
		{
		case NODE_DATABASE:
			m_listLocations.InsertItem ( nListPos, "", NODE_ICON_DATABASE );
			m_listLocations.SetItem ( nListPos, 1, LVIF_TEXT, dbDatabase.GetName ( item.m_nEntityIdx ), 0, 0, 0, NULL );
			m_listLocations.SetCheck( nListPos, item.m_bSelected );
			nListPos++;
			break;

		case NODE_LOCATIONSET:
			m_listLocations.InsertItem ( nListPos, "", NODE_ICON_LOCATION );
			m_listLocations.SetItem ( nListPos, 1, LVIF_TEXT, dbLocationSet.GetName ( item.m_nEntityIdx ), 0, 0, 0, NULL );
			m_listLocations.SetCheck( nListPos, item.m_bSelected );
			nListPos++;
			break;

		case NODE_LOCATION:
			m_listLocations.InsertItem ( nListPos, "", NODE_ICON_LOCATION );
			m_listLocations.SetItem ( nListPos, 1, LVIF_TEXT, dbLocation.GetName ( item.m_nEntityIdx ), 0, 0, 0, NULL );
			m_listLocations.SetCheck( nListPos, item.m_bSelected );
			nListPos++;
			break;
		}
	}
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::OnSelectDatabase() 
{
	UpdateRegionList();
	m_DeptAndPluComboHelpers.UpdateCombos();
	FillTaxBandComboForLocation();
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::OnSelectDepartment() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::GetReportDates()
{
	COleDateTime dateFrom,dateTo;
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

	m_OleDateFrom = dateFrom;
	m_OleDateTo = dateTo;

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

bool CSalesHistoryRegionPluDlg::GetAdhocReportSession()
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

void CSalesHistoryRegionPluDlg::OnTogglePluRange() 
{
	bool bEnable = ( m_checkSpecify.GetCheck() != 0 );
	GetEditPluFrom() -> EnableWindow ( bEnable );
	GetEditPluTo() -> EnableWindow ( bEnable );
	m_buttonBrowseFrom.EnableWindow ( bEnable );
	m_buttonBrowseTo.EnableWindow ( bEnable );	
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::OnKillFocusPluFrom() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoFrom = BarcodePluNoTable.ConvertPlu( m_strPluNoFrom );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::OnKillFocusPluTo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoTo = BarcodePluNoTable.ConvertPlu( m_strPluNoTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::OnButtonBrowseFrom() 
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

void CSalesHistoryRegionPluDlg::OnButtonBrowseTo() 
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

void CSalesHistoryRegionPluDlg::FillTaxBandComboForLocation()
{
	ReloadDatabase();

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

void CSalesHistoryRegionPluDlg::OnButtonTaxBand() 
{
	CSalesHistoryTaxDlg dlg ( this );
	
	if ( dlg.DoModal() == IDOK )
	{
		CString strItem = ",";
		strItem += SysInfo.GetCustomTaxFilterString();

		int nPos = m_comboTaxBand.GetCount() - 1;
		m_comboTaxBand.DeleteString( nPos );
		m_comboTaxBand.AddItem ( strItem );
		m_comboTaxBand.SetItemData( nPos, SysInfo.GetCustomTaxFilter() );
		m_comboTaxBand.SetCurSel( m_comboTaxBand.GetCount() - 1 );
	}
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::OnSelectReport() 
{
	int nSel = m_comboReport.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboReport.GetCount() )
	{
		switch ( m_comboReport.GetItemData( nSel ) )
		{
		case SALESHISTORY_REGION_PLU:	m_buttonGraph.EnableWindow( FALSE );	break;
		case SALESHISTORY_REGION_BAND:	m_buttonGraph.EnableWindow( FALSE );	break;
		case SALESHISTORY_REGION_DEPT:	m_buttonGraph.EnableWindow( TRUE );		break;
		default:						m_buttonGraph.EnableWindow( FALSE );	break;
		}
	}
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::OnButtonDisplay() 
{
	CreateReport( FALSE );
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::OnButtonGraph() 
{
	CreateReport( TRUE );
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::CreateReport( bool bGraph )
{
	int nSel = m_comboReport.GetCurSel();
	if ( nSel < 0 || nSel >= m_comboReport.GetCount() )
		return;

	OnKillFocusDeptNo();
	OnKillFocusPluFrom();
	OnKillFocusPluTo();

	int nReportType = m_comboReport.GetItemData( nSel );

	bool bByLocation = ( m_checkLocation.GetCheck() != 0 ) && ( bGraph == FALSE );

	CArray<CDateRangeOrRegionColumnInfo,CDateRangeOrRegionColumnInfo> arrayColumns;

	for ( int n = 0; n < m_listLocations.GetItemCount(); n++ )
	{
		if ( m_listLocations.GetCheck(n) != 0 )
		{
			int nIdx = m_nRegionListStartIdx + n;

			if ( nIdx < m_arrayRegionInfo.GetSize() )
			{
				CSalesHistoryRegionListItem infoRegion;
				m_arrayRegionInfo.GetAt( nIdx, infoRegion );

				CDateRangeOrRegionColumnInfo infoColumn;
				infoColumn.m_nDbIdx = infoRegion.m_nDbIdx;
				infoColumn.m_nTableIdx = infoRegion.m_nEntityIdx;
				infoColumn.m_nType = infoRegion.m_nConType;
				arrayColumns.Add( infoColumn );
			}
		}
	}

	if ( arrayColumns.GetSize() == 0 )
	{
		Prompter.Error ( "You must select at least one location before you can create this report." );
		return;
	}

	if ( ( arrayColumns.GetSize() > 3 ) && ( bGraph == TRUE ) )
	{
		Prompter.Error ( "You can only select a maximum of three regions for each graph." );
		return;
	}

	GetReportDates();

	if ( m_comboSession.GetCurSel() == m_comboSession.GetCount() - 1 )
	{
		if ( GetAdhocReportSession() == FALSE )
			return;
	}
	else
	{
		CString str;
		m_comboSession.GetWindowText( str );
		m_ReportSession.SetName( str );
	}

	int nDeptFilter = m_DepartmentSelector.GetDepartmentFilter();	
	if ( nDeptFilter == 0 ) nDeptFilter = FIRST_DEPARTMENTSET_FILTER - 1;

	int nTaxFilter = 0;
	int nTaxSel = m_comboTaxBand.GetCurSel();
	if ( nTaxSel >= 0 && nTaxSel < m_comboTaxBand.GetCount() )
		nTaxFilter = m_comboTaxBand.GetItemData( nTaxSel );

	bool bItemRange = ( m_checkSpecify.GetCheck() != 0 );
	bool bPriceBandSplit = ( SALESHISTORY_REGION_BAND == nReportType );

	m_Report.SetSaleTimeFlag( m_comboPluTime.GetCurSel() == 1 );
	m_Report.SetReportType( nReportType );
	m_Report.SetZeroSkipType( m_comboZeroSkip.GetCurSel() );
	m_Report.Configure( m_OleDateFrom, m_OleDateTo, arrayColumns, bByLocation, nDeptFilter, nTaxFilter, bItemRange, _atoi64(m_strPluNoFrom), _atoi64(m_strPluNoTo), bPriceBandSplit, m_ReportSession );
	m_Report.Consolidate();

	if ( bGraph == FALSE )
	{
		if ( m_Report.CreateReport() == FALSE )
			Prompter.Error ( "Unable to create report" );
		else
		{
			CFilenameUpdater FnUp ( SysFiles::RegionHistPrm );
		
			CString strKey;
			strKey.Format ( "1%2.2d%3.3d", nReportType, arrayColumns.GetSize() );
			
			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, this );
		}
	}
	else
	{
		switch ( m_Report.CreateGraph() )
		{
		case 1:
			Prompter.Error ( "Unable to create graph" );
			break;

		case 2:
			Prompter.Error ( "No sales data was found in the specified range" );
			break;

		default:
			{
				CReportManager ReportManager;
				ReportManager.DisplayGraph( Super.ReportFilename() );
			}
			break;
		}
	}
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CSalesHistoryRegionPluDlg::ReloadDatabase()
{
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( 0, info, FALSE );
}

/**********************************************************************/
