/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "DateRangeHelpers.h"
#include "FilterTypes.h"
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
#include "SalesHistoryDateRangePluDlg.h"
/**********************************************************************/

CSalesHistoryDateRangePluDlg::CSalesHistoryDateRangePluDlg(CWnd* pParent /*=NULL*/)
	: CSSDialog(CSalesHistoryDateRangePluDlg::IDD, pParent), 
		m_FilterArray ( FILTERTYPE_BROWSE_PLU ),
		m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CSalesHistoryDateRangePluDlg)
	//}}AFX_DATA_INIT
	m_nLocationComboDbIdx = -1;
}

/**********************************************************************/

void CSalesHistoryDateRangePluDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesHistoryDateRangePluDlg)
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_COMBO_ZEROSKIP, m_comboZeroSkip);
	DDX_Control(pDX, IDC_COMBO_REPORT, m_comboReport);
	DDX_Control(pDX, IDC_BUTTON_TAXBAND, m_buttonTaxBand);
	DDX_Control(pDX, IDC_COMBO_TAXBAND, m_comboTaxBand);
	DDX_Control(pDX, IDC_BUTTON_GRAPH, m_buttonGraph);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_TO, m_buttonBrowseTo);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_FROM, m_buttonBrowseFrom);
	DDX_Control(pDX, IDC_COMBO_PLUTIME, m_comboPluTime);
	DDX_Control(pDX, IDC_CHECK_SPECIFY, m_checkSpecify);
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_FROM, m_strPluNoFrom);
	DDX_IntegerString(pDX, IDC_EDIT_PLU_TO, m_strPluNoTo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesHistoryDateRangePluDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesHistoryDateRangePluDlg)
	ON_WM_MEASUREITEM()
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
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CSalesHistoryDateRangePluDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_FROM ); }
CEdit* CSalesHistoryDateRangePluDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_TO ); }
/**********************************************************************/

BOOL CSalesHistoryDateRangePluDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DeptAndPluComboHelpers.SetControlPointers( NULL, &m_LocationSelector, &m_DepartmentSelector );

	m_buttonTaxBand.SetWindowText( EcrmanOptions.GetTaxBandString() );

	SubclassEdit ( IDC_EDIT_PLU_FROM, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_TO, SS_NUM, Super.MaxPluLen(), "%s" );

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(30);
	arrayStops.Add(130);
	m_comboTaxBand.SetTabStops( arrayStops );
	
	CWordArray arrayTypes;

	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:
		arrayTypes.Add ( NODE_LOCATION );
		arrayTypes.Add ( NODE_DATABASE );
		arrayTypes.Add ( NODE_LOCATIONSET );
		arrayTypes.Add ( NODE_REPORTPOINT );
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
	m_LocationSelector.FillLocationCombo ( &m_comboLocation, NULL );

	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DeptAndPluComboHelpers.UpdateCombos();
	
	FillTaxBandComboForLocation();
	m_comboTaxBand.SetCurSel(0);
	
	CDialog::OnInitDialog();
	
	m_comboReport.AddString ( "LY Sales Comparison" );
	m_comboReport.AddString ( "Plu Item Summary" );
	m_comboReport.AddString ( "Plu Item Summary by Price Band" );
	m_comboReport.AddString ( "Department Summary" );
	m_comboReport.SetItemData( 0, SALESHISTORY_DATERANGE_LYCOMPARE );
	m_comboReport.SetItemData( 1, SALESHISTORY_DATERANGE_TRADING_PLU );
	m_comboReport.SetItemData( 2, SALESHISTORY_DATERANGE_TRADING_BAND );
	m_comboReport.SetItemData( 3, SALESHISTORY_DATERANGE_TRADING_DEPT );
	m_comboReport.SetCurSel(0);
	OnSelectReport();

	m_comboPluTime.AddString( "Transaction completion time" );
	m_comboPluTime.AddString( "Item sale time" );
	m_comboPluTime.SetCurSel( ( EcrmanOptions.GetReportsSalesTimeType() == 1 ) ? 1 : 0 );

	m_comboZeroSkip.AddString( "Show all items" );
	m_comboZeroSkip.AddString( "Items with no sales" );
	m_comboZeroSkip.AddString( "Price levels with no sales" );
	m_comboZeroSkip.SetCurSel(0);

	for ( int nIndex = 0; nIndex < DataManagerNonDb.ReportSession.GetSize(); nIndex++ )
		m_comboSession.AddString( DataManagerNonDb.ReportSession.GetName( nIndex ) );

	m_comboSession.AddString( "Adhoc Session" );

	if ( m_comboSession.GetCount() < 2 )
		m_comboSession.InsertString ( 0, "00:00 to 23:59" );

	if ( m_comboSession.GetCount() < 2 )
		m_comboSession.EnableWindow( FALSE );
	
	m_comboSession.SetCurSel(0);

	m_arrayDateRangeNames.Add ( szRANGE_TODAY );
	m_arrayDateRangeNames.Add ( szRANGE_YESTERDAY );
	m_arrayDateRangeNames.Add ( szRANGE_THIS_WEEK );
	m_arrayDateRangeNames.Add ( szRANGE_THIS_MONTH );
	
	if ( EcrmanOptions.GetReportsSalesTaxType() == 1 )
		m_arrayDateRangeNames.Add ( szRANGE_THIS_GST_QUARTER );
	else
		m_arrayDateRangeNames.Add ( szRANGE_THIS_VAT_QUARTER );

	m_arrayDateRangeNames.Add ( szRANGE_THIS_YEAR );
	m_arrayDateRangeNames.Add ( szRANGE_THIS_FINANCIAL_YEAR );
	m_arrayDateRangeNames.Add ( szRANGE_LAST_WEEK );
	m_arrayDateRangeNames.Add ( szRANGE_LAST_MONTH );
	
	if ( EcrmanOptions.GetReportsSalesTaxType() == 1 )
		m_arrayDateRangeNames.Add ( szRANGE_LAST_GST_QUARTER );
	else
		m_arrayDateRangeNames.Add ( szRANGE_LAST_VAT_QUARTER );

	m_arrayDateRangeNames.Add ( szRANGE_LAST_YEAR );
	m_arrayDateRangeNames.Add ( szRANGE_LAST_FINANCIAL_YEAR );
	m_arrayDateRangeNames.Add ( szRANGE_LAST_12_MONTHS );
	m_arrayDateRangeNames.Add ( szRANGE_ALL );

	m_listDateRange.SubclassDlgItem ( IDC_LIST, this );
	m_listDateRange.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listDateRange.InsertColumn ( 0, "", LVCFMT_LEFT, 290 );
	m_listDateRange.SetLockItemZeroFlag( FALSE );

	for ( int n = 0; n < m_arrayDateRangeNames.GetSize(); n++ )
	{
		CSSListTaggedSelectItem item;
		item.m_strText = m_arrayDateRangeNames.GetAt(n);
		item.m_nData = 0;
		item.SetTagState(0);
		m_listDateRange.AddItemToList( item );
	}

	OnTogglePluRange();
	return TRUE;  
}

/**********************************************************************/

void CSalesHistoryDateRangePluDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listDateRange.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

bool CSalesHistoryDateRangePluDlg::GetAdhocReportSession()
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

void CSalesHistoryDateRangePluDlg::OnSelectDepartment() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CSalesHistoryDateRangePluDlg::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CSalesHistoryDateRangePluDlg::OnSelectLocation() 
{
	m_DeptAndPluComboHelpers.UpdateCombos();
	FillTaxBandComboForLocation();
	ReloadDatabase();	
}

/**********************************************************************/

void CSalesHistoryDateRangePluDlg::OnTogglePluRange() 
{
	bool bEnable = ( m_checkSpecify.GetCheck() != 0 );
	GetEditPluFrom() -> EnableWindow ( bEnable );
	GetEditPluTo() -> EnableWindow ( bEnable );
	m_buttonBrowseFrom.EnableWindow ( bEnable );
	m_buttonBrowseTo.EnableWindow ( bEnable );	
}

/**********************************************************************/

void CSalesHistoryDateRangePluDlg::OnKillFocusPluFrom() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoFrom = BarcodePluNoTable.ConvertPlu( m_strPluNoFrom );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CSalesHistoryDateRangePluDlg::OnKillFocusPluTo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoTo = BarcodePluNoTable.ConvertPlu( m_strPluNoTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CSalesHistoryDateRangePluDlg::OnButtonBrowseFrom() 
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

void CSalesHistoryDateRangePluDlg::OnButtonBrowseTo() 
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

void CSalesHistoryDateRangePluDlg::FillTaxBandComboForLocation()
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

void CSalesHistoryDateRangePluDlg::OnButtonTaxBand() 
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

void CSalesHistoryDateRangePluDlg::OnSelectReport() 
{
	int nSel = m_comboReport.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboReport.GetCount() )
	{
		switch ( m_comboReport.GetItemData( nSel ) )
		{
		case SALESHISTORY_DATERANGE_TRADING_DEPT:	
			m_buttonGraph.EnableWindow( TRUE );
			break;

		default:							
			m_buttonGraph.EnableWindow( FALSE );
			break;
		}
	}
}

/**********************************************************************/

void CSalesHistoryDateRangePluDlg::OnButtonDisplay() 
{
	CreateReport( FALSE );
}

/**********************************************************************/

void CSalesHistoryDateRangePluDlg::OnButtonGraph() 
{
	CreateReport( TRUE );
}

/**********************************************************************/

void CSalesHistoryDateRangePluDlg::CreateReport( bool bGraph )
{
	int nSel = m_comboReport.GetCurSel();
	if ( nSel < 0 || nSel >= m_comboReport.GetCount() )
		return;

	OnKillFocusDeptNo();
	OnKillFocusPluFrom();
	OnKillFocusPluTo();

	int nReportType = m_comboReport.GetItemData( nSel );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CDateRangeOrRegionColumnInfo RegionInfo;
	RegionInfo.m_nDbIdx = LocSelEntity.GetDbIdx();
	RegionInfo.m_nTableIdx = LocSelEntity.GetEntityIdx();
	RegionInfo.m_nType = LocSelEntity.GetConType();

	CArray<CDateRangeInfo,CDateRangeInfo> DateInfo;

	for ( int n = 0; n < m_listDateRange.GetItemCount(); n++ )
	{
		if ( m_listDateRange.GetItemTagState(n) == 0 )
			continue;

		CString strName = m_arrayDateRangeNames.GetAt(n);
		
		COleDateTime dateStart, dateEnd;
		CDateRangeHelpers DateRangeHelpers;
		DateRangeHelpers.GetOleDateRange( strName, dateStart, dateEnd );

		CDateRangeInfo info;
		info.m_strName = strName;
		info.m_dateStart = dateStart;
		info.m_dateEnd = dateEnd;
		DateInfo.Add( info );
	}

	if ( DateInfo.GetSize() == 0 )
	{
		Prompter.Error ( "You must select at least one date range\nbefore you can create this report." );
		return;
	}

	int n = m_comboSession.GetCurSel();
	if ( ( n >= 0 ) && ( n < DataManagerNonDb.ReportSession.GetSize() ) )
	{
		CSessionCSVRecord Session;
		DataManagerNonDb.ReportSession.GetAt ( n, m_ReportSession );
	}
	else
		m_ReportSession.ClearRecord();

	if ( n == m_comboSession.GetCount() - 1 )
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

	if ( nReportType == SALESHISTORY_DATERANGE_LYCOMPARE )
	{
		for ( int n = DateInfo.GetSize() - 1; n >= 0; n-- )
		{
			CDateRangeInfo info = DateInfo.GetAt(n);
			
			CDateRangeHelpers DateRangeHelpers;
			DateRangeHelpers.GetLYOleDateRange( info.m_strName, info.m_dateStart, info.m_dateEnd );
		
			DateInfo.InsertAt( n + 1, info );
		}
	}

	int nDeptFilter = m_DepartmentSelector.GetDepartmentFilter();	
	if ( nDeptFilter == 0 ) nDeptFilter = FIRST_DEPARTMENTSET_FILTER - 1;
	
	int nTaxFilter = 0;
	int nTaxSel = m_comboTaxBand.GetCurSel();
	if ( nTaxSel >= 0 && nTaxSel < m_comboTaxBand.GetCount() )
		nTaxFilter = m_comboTaxBand.GetItemData( nTaxSel );

	bool bItemRange = ( m_checkSpecify.GetCheck() != 0 );
	bool bPriceBand = ( SALESHISTORY_DATERANGE_TRADING_BAND == nReportType );

	m_Report.SetReportType( nReportType );
	m_Report.SetZeroSkipType( m_comboZeroSkip.GetCurSel() );
	m_Report.SetSaleTimeFlag( m_comboPluTime.GetCurSel() ? 1 : 0 );
	m_Report.Configure( RegionInfo, DateInfo, nDeptFilter, nTaxFilter, bItemRange, _atoi64(m_strPluNoFrom), _atoi64(m_strPluNoTo), bPriceBand, m_ReportSession );
	m_Report.Consolidate();

	if ( bGraph == FALSE )
	{
		if ( m_Report.CreateReport() == FALSE )
			Prompter.Error ( "Unable to create report" );
		else
		{
			CFilenameUpdater FnUp ( SysFiles::RegionHistPrm );
			
			CString strKey;
			strKey.Format ( "2%2.2d%3.3d", nReportType, DateInfo.GetSize() );
			
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

void CSalesHistoryDateRangePluDlg::ReloadDatabase()
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
}

/**********************************************************************/







