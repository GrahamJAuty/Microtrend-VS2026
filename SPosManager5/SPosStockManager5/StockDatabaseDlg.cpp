/**********************************************************************/
#include "..\CommonEcrStock\ColumnListDlg.h"
#include "..\CommonEcrStock\CustomExportManager.h"
#include "DatabaseCSVArray.h"
#include "DatabaseImportHandlerStockman.h"
#include "ExportDate.h"
#include "FileRemover.h"
#include "FilterTypes.h"
#include "ImportDefines.h"
#include "..\CommonEcrStock\KeyboardHelpers.h"
#include "MessageLogger.h"
#include "OpeningStockAuditer.h"
#include "PluImportStatsDlgStockman.h"
#include "NodeTypes.h"
#include "PropertySheetStock.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockCodeDlg.h"
#include "StockCustomListDlg.h"
#include "StockDefineFilterDlg.h"
#include "StockDeleteSingleDlg.h"
#include "StockExportDlg.h"
#include "StockOptionsIni.h"
#include "StockPluArray.h"
#include "StockPrefsDlg.h"
#include "StockRangeDlg.h"
#include "StockRangeCustomFieldDlg.h"
#include "StockRangeUnitsDlg.h"
#include "StockReportSupplierProfitDlg.h"
#include "StockReportSupplierProfitTickDlg.h"
#include "StockTreeReportNewProfit.h"
#include "SupplierSetCSVArray.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "StockDatabaseDlg.h"
/**********************************************************************/

CStockDatabaseDlg::CStockDatabaseDlg( int nDbIdx, CWnd* pParent)
	: CDialog(CStockDatabaseDlg::IDD, pParent), 
	m_FilterArray ( FILTERTYPE_DATABASE ), 
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CStockDatabaseDlg)
	//}}AFX_DATA_INIT
	m_nCacheIndex = -1;
	m_strDbName = dbDatabase.GetName( nDbIdx );
	m_bReadOnly = FALSE;
	m_bInitDialog = FALSE;
	m_nApparentComboPos = 0;
	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_bModifiedStock = FALSE;
	
	bool bApparent = ( DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE );
	m_FilterArray.SetCheckApparentFlag( bApparent );
}

/**********************************************************************/

void CStockDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockDatabaseDlg)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockDatabaseDlg, CDialog)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CStockDatabaseDlg)
	ON_WM_SIZE()
	ON_COMMAND(IDM_STOCK_ADD, OnStockAdd)
	ON_COMMAND(IDM_STOCK_DELETE_CURRENT, OnStockDeleteCurrent)
	ON_COMMAND(IDM_STOCK_DELETE_RANGE, OnStockDeleteRange)
	ON_COMMAND(IDM_STOCK_EDIT, OnStockEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_COMMAND(IDM_REPORTS_PROFIT, OnReportProfit)
	ON_COMMAND(IDM_NOFILTER_STOCKRECORDS, OnNoFilterStockRecords)
	ON_COMMAND(IDM_FILTER_STOCKRECORDS, OnFilterStockRecords)
	ON_COMMAND(IDM_STOCK_RANGE_PROGRAM, OnStockRangeProgram)
	ON_COMMAND(IDM_STOCK_EXPORT_CSV, OnStockExportCsv)
	ON_COMMAND(IDM_STOCK_IMPORT_CSV, OnStockImportCsv)
	ON_COMMAND(IDM_COLUMN_EDIT, OnColumnEdit)
	ON_COMMAND(IDM_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM_COLUMN_DEFAULT, OnColumnDefault)
	ON_COMMAND(IDM_STOCK_RANGE_UNITS, OnStockRangeUnits)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	ON_COMMAND(IDM_EXIT, OnCancel)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnStockEdit)
	ON_COMMAND(IDM_REPORTS_CUSTOM1, OnReportsCustom1)
	ON_COMMAND(IDM_REPORTS_CUSTOM2, OnReportsCustom2)
	ON_COMMAND(IDM_REPORTS_CUSTOM3, OnReportsCustom3)
	ON_COMMAND(IDM_REPORTS_CUSTOM4, OnReportsCustom4)
	ON_COMMAND(IDM_REPORTS_CUSTOM5, OnReportsCustom5)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_USER_BUTTON, OnUserButton )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockDatabaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );

	m_listStock.SubclassDlgItem ( IDC_LIST, this );
	m_listStock.EnableColour();

	m_ColumnManager.Reset( TableNames::Stock );
	m_ColumnManager.AddColumn( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 115, 115, COLUMN_STATUS_LOCKED ); 	
	m_ColumnManager.AddColumn( ColumnNames::Desc, "Description", LVCFMT_LEFT, 135, 170, COLUMN_STATUS_SHOWN ); 	
	m_ColumnManager.AddColumn( ColumnNames::Category, "Category", LVCFMT_LEFT, 80, 140, COLUMN_STATUS_SHOWN ); 	
	m_ColumnManager.AddColumn( ColumnNames::StockUnit, "Unit", LVCFMT_LEFT, 100, 100, COLUMN_STATUS_SHOWN );

	if ( DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE )
		m_ColumnManager.AddColumn( ColumnNames::Apparent, "Apparent", LVCFMT_RIGHT, 60, 80, COLUMN_STATUS_SHOWN ); 	
	
	m_ColumnManager.AddColumn( ColumnNames::Supplier, "Supplier", LVCFMT_LEFT, 100, 100, COLUMN_STATUS_HIDDEN );
	m_ColumnManager.AddColumn( ColumnNames::DUCost, "DU Cost", LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_HIDDEN );
	m_ColumnManager.AddColumn( ColumnNames::SUCost, "SU Cost", LVCFMT_RIGHT, 55, 70, COLUMN_STATUS_HIDDEN );
	m_ColumnManager.LoadSettings();

	m_MenuChanger.EatMenuTopLevel( GetMenu() );

	if ( StockOptions.GetPluCSVManualImportStockFlag() == FALSE )
		m_MenuChanger.KillItem ( IDM_STOCK_IMPORT_CSV );

	if ( StockOptions.GetPluCSVManualExportStockFlag() == FALSE )
		m_MenuChanger.KillItem ( IDM_STOCK_EXPORT_CSV );

	if ( StockOptions.GetStockSubUnitsFlag() == FALSE )
		m_MenuChanger.KillItem ( IDM_STOCK_RANGE_UNITS );

	if ( m_bReadOnly == TRUE )
	{
		m_MenuChanger.KillItem( IDM_STOCK_IMPORT_CSV );
		m_MenuChanger.KillItem( IDM_STOCK_ADD );
		m_MenuChanger.SetItemText( IDM_STOCK_EDIT, "View" );
		m_MenuChanger.KillItem( IDM_STOCK_DELETE_CURRENT );
		m_MenuChanger.KillParent( IDM_STOCK_RANGE_PROGRAM );
	}

	if ( DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_READWRITE )
	{
		m_MenuChanger.KillItem( IDM_STOCK_DELETE_CURRENT );
		m_MenuChanger.KillItem( IDM_STOCK_DELETE_RANGE );
	}

	SetCustomListMenuNames();

	m_MenuChanger.BuildMenu( GetMenu() );

	m_listStock.SetItemCountEx ( m_FilterArray.GetSize() );
	
	m_bInitDialog = TRUE;
	m_ColumnManager.RedrawList( &m_listStock, this );
	CenterWindow();

	CDWordArray dw;
	dw.Add( DataManager.Plu.GetSize() );
	dw.Add( DataManager.Stock.GetSize() );
	MessageLogger.LogSystem( 3, 1, TRUE, dw );

	CFileRemover FileRemover;
	CFilenameUpdater FnUp( SysFiles::OpeningTemp, DataManager.GetActiveDbIdx() );
	FileRemover.RemoveFile( FnUp.GetFilenameToUse() );
	
	DisplayScreen ( 0 );
	return TRUE;  
}

/**********************************************************************/

void CStockDatabaseDlg::SetCustomListMenuNames()
{
	CString strName;
	CStockTreeReportNewCustomList::GetCSVReportName( 1, strName );
	m_MenuChanger.SetItemText( IDM_REPORTS_CUSTOM1, strName );
	CStockTreeReportNewCustomList::GetCSVReportName( 2, strName );
	m_MenuChanger.SetItemText( IDM_REPORTS_CUSTOM2, strName );
	CStockTreeReportNewCustomList::GetCSVReportName( 3, strName );
	m_MenuChanger.SetItemText( IDM_REPORTS_CUSTOM3, strName );
	CStockTreeReportNewCustomList::GetCSVReportName( 4, strName );
	m_MenuChanger.SetItemText( IDM_REPORTS_CUSTOM4, strName );
	CStockTreeReportNewCustomList::GetCSVReportName( 5, strName );
	m_MenuChanger.SetItemText( IDM_REPORTS_CUSTOM5, strName );
}

/**********************************************************************/

void CStockDatabaseDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listStock.MoveWindow ( 0, 0, cx, cy );
	}
}

/**********************************************************************/

void CStockDatabaseDlg::DisplayScreen ( int nIndex )
{
	CString strText;
	int nSize = m_FilterArray.GetSize();
	CString strFiltered = ( m_FilterArray.GetFilteredFlag() == TRUE ) ? ", Filtered" : "";

	if ( SysInfo.GetMaxDatabases() > 1 )
	{
		strText.Format ( "Stock Database (%s, %d of %d records%s)",
			(const char*) m_strDbName,
			nSize,
			DataManager.Stock.GetSize(),
			(const char*) strFiltered );
	}
	else
	{
		strText.Format ( "Stock Database (%d of %d records%s)",
			nSize,
			DataManager.Stock.GetSize(),
			(const char*) strFiltered );
	}

	if ( m_bReadOnly == TRUE ) strText += " (READ ONLY)";
	SetWindowText ( strText );

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listStock.SetCurSel ( nIndex );
	else
		m_listStock.SetCurSel ( nSize - 1 );

	m_listStock.Invalidate();
}

/**********************************************************************/

void CStockDatabaseDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ( nIndex != m_nCacheIndex  )
		{
			m_CacheCSV.ParseLine( m_FilterArray.GetColumnManagerDisplayLine ( m_ColumnManager, nIndex ) );
			m_nCacheIndex = nIndex;
		}

		lstrcpy ( pDispInfo->item.pszText, m_CacheCSV.GetString ( pDispInfo->item.iSubItem ) );

		if ( m_FilterArray.GetNegativeStockFlag() )
			m_listStock.SetColour( nIndex, 0x0C );
		else if ( m_FilterArray.GetBelowMinStockFlag() )
			m_listStock.SetColour( nIndex, COLORREF ( StockOptions.GetStockBelowMinimumColour() ) );
		else if ( m_FilterArray.GetAboveMaxStockFlag() )
			m_listStock.SetColour( nIndex, COLORREF ( StockOptions.GetStockAboveMaximumColour() ) );
		else
			m_listStock.SetColour( nIndex, 0x00 );
	}
	*pResult = 0;
}

/**********************************************************************/

void CStockDatabaseDlg::OnFindByCode()
{
	CStockCodeDlg dlg ( "Find Stock Code", m_StockCodeSearch, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		CString strStockCode = m_StockCodeSearch.m_strStockCodeToUse;

		int nIndex;
		if ( m_FilterArray.FindStockItemByCode ( strStockCode, nIndex ) == FALSE )
		{
			CString strMsg;
			strMsg.Format ( "Cannot find Stock Code\n%s", (const char*) strStockCode );
			Prompter.Error ( strMsg );
		}	
		m_listStock.SetCurSel ( nIndex );
	}
}

/**********************************************************************/

void CStockDatabaseDlg::BuildRecordList()
{
	m_FilterArray.BuildRecordList();
	m_listStock.SetItemCountEx ( m_FilterArray.GetSize() );
}

/**********************************************************************/

void CStockDatabaseDlg::OnStockAdd() 
{
	CStockCodeSearch StockCodeSearch;
	StockCodeSearch.m_bSearchFromStartTick = FALSE;

	CStockCodeDlg dlg ( "Add Stock Code", StockCodeSearch, this );
	if ( dlg.DoModal() != IDOK )
		return;

	CString strStockCode = StockCodeSearch.m_strStockCodeToUse;

	if ( strStockCode == "" )
		return;

	int nIndex;

	if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nIndex ) == TRUE )
	{
		CString strMessage;
		strMessage.Format ( "Stock code %s is already in use", ( const char*) strStockCode );
		Prompter.Error ( strMessage );
		return;
	}

	CStockCSVRecord NewRecord;
	NewRecord.SetStockCode ( strStockCode );

	NewRecord.SetCategory ( m_FilterArray.GetDefaultCategory() );
	int nInsertionPosition = m_FilterArray.InsertRecord ( NewRecord );
	m_listStock.SetItemCountEx ( m_FilterArray.GetSize() );
	DisplayScreen ( nInsertionPosition );
	m_FilterArray.SetArrayIdx ( nInsertionPosition );
	EditStock( nInsertionPosition );
	m_bModifiedStock = TRUE;
}

/**********************************************************************/

void CStockDatabaseDlg::OnStockEdit() 
{
	int nIndex = m_listStock.GetCurSel();

	if ( nIndex >= 0 && nIndex < m_FilterArray.GetSize() )
		EditStock( nIndex );
}

/**********************************************************************/

void CStockDatabaseDlg::OnStockDeleteCurrent() 
{
	int nCurrent = m_listStock.GetCurSel();

	if ( nCurrent < 0 || nCurrent >= m_FilterArray.GetSize() )
		return;

	int nStockIdx = m_FilterArray.GetStockItemIndexAt ( nCurrent );
	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );

	CString strMsg = "";

	CStockLevelsCSVRecord StockLevels;
	DataManager.Stock.GetDatabaseTotals( nStockIdx, StockRecord, StockLevels );

	CStockDeleteSingleDlg dlgDelete( StockRecord, StockLevels, this );
	if ( dlgDelete.DoModal() != IDOK )
		return;

	m_bModifiedStock = TRUE;
	DataManager.Stock.RemoveAt ( nStockIdx );
	m_FilterArray.RemoveIndex ( nCurrent );

	m_listStock.SetItemCountEx ( m_FilterArray.GetSize() );
	DisplayScreen ( nCurrent );
}

/**********************************************************************/

void CStockDatabaseDlg::OnStockDeleteRange() 
{
	int nOldSize = m_FilterArray.GetSize();

	if ( nOldSize == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}
	
	int nCurrent = m_listStock.GetCurSel();
	const char* szTitle = "Range Delete";
	
	CStockRangeDlg dlg ( szTitle, this );

	if ( nCurrent >= 0 )
	{
		int nStockIdx = m_FilterArray.GetStockItemIndexAt ( nCurrent );
		CString strStockCode = DataManager.Stock.GetStockCode ( nStockIdx );
		dlg.m_strStockCodeFrom = strStockCode; 
		dlg.m_strStockCodeTo = strStockCode;
	}
	else
	{
		dlg.m_strStockCodeFrom = "";
		dlg.m_strStockCodeTo = "";
	}
		
	if ( dlg.DoModal() == IDOK )
	{
		int nSkip = m_FilterArray.DeleteStockItemRange ( dlg.m_strStockCodeFrom, dlg.m_strStockCodeTo );

		m_listStock.SetItemCountEx ( m_FilterArray.GetSize() );
		DisplayScreen ( nCurrent );

		if ( nSkip >= 1 )
		{
			CString strMsg = "Some of the items from this range could not\n";
			strMsg += "be deleted automatically as they still have\n";
			strMsg += "outstanding orders to be delivered.";
			Prompter.Info ( strMsg );
		}
	}

	if ( nOldSize != m_FilterArray.GetSize() )
	{
		m_bModifiedStock = TRUE;

		CDWordArray dw;
		dw.Add( nOldSize );
		dw.Add( m_FilterArray.GetSize() );

		CStringArray str;
		str.Add( dlg.m_strStockCodeFrom );
		str.Add( dlg.m_strStockCodeTo );

		MessageLogger.LogSystem( 3, 8, FALSE, dw,str );
	}
}

/**********************************************************************/

void CStockDatabaseDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnStockEdit();
	*pResult = 0;
}

/**********************************************************************/

void CStockDatabaseDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CStockDatabaseDlg::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	int nIndex = ( int ) wIndex;
	int nType = m_ColumnManager.GetListColumnType( nIndex );

	switch( nType )
	{
	case COLUMN_STOCKCODE:
		if ( m_FilterArray.GetSize() > 0 )
			OnFindByCode();
		break;

	case COLUMN_DESC:
	case COLUMN_CATEGORY:
		OnFilterStockRecords();
		break;
	}
	
	return 0L;
}

/**********************************************************************/

void CStockDatabaseDlg::EditStock ( int nIndex )
{
	m_FilterArray.SetArrayIdx ( nIndex );
	
	CPropertySheetStockInfo info;
	info.m_bEnableSpin = TRUE;
	info.m_wSpinID = IDC_SPIN;
	info.m_pStockFilter = &m_FilterArray;
	info.m_bPluLinkTab = TRUE;
	info.m_bSupplierEdit = TRUE;
	info.m_bStockLevelTab = ( DealerFlags.GetCurrentStockLevelsAccessType() == STOCKLEVELS_ACCESS_READWRITE );
	info.m_nDefaultSuppNo = 0;
	info.m_bReadOnly = m_bReadOnly;
	info.m_bApparentTab = ( DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE );
	info.m_nApparentComboPos = m_nApparentComboPos;

	CPropertySheetStock propSheet ( info, this );

	if ( propSheet.DoModal() != IDOK )
		propSheet.UndoChanges();
	
	m_bModifiedStock |= propSheet.GetModifiedFlag();
	m_bModifiedStock |= propSheet.GetChangeLevelFlag();
	
	DisplayScreen ( m_FilterArray.GetArrayIdx() );
	m_nApparentComboPos = propSheet.GetApparentComboPos();
}

/**********************************************************************/

void CStockDatabaseDlg::OnReportProfit() 
{
	CWordArray wSuppSelectArray;

	bool bAlphaSort = FALSE;

	if ( m_FilterArray.GetSupplierFilter() == 0 )
	{
		CStockReportSupplierProfitDlg dlg ( m_wSuppSelectArray, this );
		if ( dlg.DoModal() == IDCANCEL )
			return;

		if ( m_wSuppSelectArray.GetSize() < 1 )
		{
			Prompter.Error ( "You must select at least one supplier\nbefore you can view this report." );
			return;
		}

		for ( int nIndex = 0; nIndex < m_wSuppSelectArray.GetSize(); nIndex++ )
			wSuppSelectArray.Add ( m_wSuppSelectArray.GetAt ( nIndex ) );

		bAlphaSort = ( dlg.m_bAlphaSort != 0 );
	}
	else
	{
		DataManager.SupplierSet.CreateSuppSelectArray ( m_FilterArray.GetSupplierFilter(), wSuppSelectArray );

		CStockReportSupplierProfitTickDlg dlg ( this );
		if ( dlg.DoModal() == IDCANCEL )
			return;

		bAlphaSort = ( dlg.m_bAlphaSort != 0 );
	}

	CStockLevelTreeNew tree;
	
	CWordArray arraySortOrder;
	arraySortOrder.Add( SH_FIELD_SUPP + SH_FIELD_HEADER );

	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetDatabaseNode( DataManager.GetActiveDbIdx() );

	tree.Configure( 2, &m_FilterArray, LocSelEntity, 0, arraySortOrder, TRUE, 0, "", "", FALSE, FALSE, wSuppSelectArray, TRUE, FALSE, FALSE );

	tree.SetStockFilterArray( &m_FilterArray );
	tree.Consolidate();

	CStockTreeReportNewProfit report( tree );
	report.PrepareArrays();
	report.SetConLevelText( LocSelEntity.GetEntityName() );
	report.PrepareReport();
	report.CreateReport();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), report.GetReportParamsFilename(), report.GetReportParamsKey(), this );
}

/**********************************************************************/
void CStockDatabaseDlg::OnReportsCustom1(){ DoCustomListReport(1); } 
void CStockDatabaseDlg::OnReportsCustom2(){ DoCustomListReport(2); } 
void CStockDatabaseDlg::OnReportsCustom3(){ DoCustomListReport(3); } 
void CStockDatabaseDlg::OnReportsCustom4(){ DoCustomListReport(4); } 
void CStockDatabaseDlg::OnReportsCustom5(){ DoCustomListReport(5); } 
/**********************************************************************/

void CStockDatabaseDlg::DoCustomListReport( int nRepNum )
{
	int nStartSpIdx, nEndSpIdx;
	dbStockpoint.GetSpIdxRange( DataManager.GetActiveDbIdx(), nStartSpIdx, nEndSpIdx );

	if ( nStartSpIdx > nEndSpIdx )
		Prompter.Error ( "You must add a stockpoint to this database\nbefore you can create a custom list report." );
	else
	{
		CStockCustomListDlg dlg( m_FilterArray, nRepNum, this );
		m_FilterArray.GetDefaultRange ( m_listStock.GetCurSel(), dlg.m_strStockCodeFrom, dlg.m_strStockCodeTo );
		dlg.DoModal();

		SetCustomListMenuNames();
		m_MenuChanger.BuildMenu( GetMenu() );
	}
}

/**********************************************************************/

void CStockDatabaseDlg::OnFilterStockRecords() 
{
	CStockDefineFilterDlg dlg ( m_FilterArray, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		BuildRecordList();
		DisplayScreen ( 0 );
	}
}

/**********************************************************************/

void CStockDatabaseDlg::OnNoFilterStockRecords() 
{
	int nStockIdx = 0;
	int nSelection = m_listStock.GetCurSel();

	if ( nSelection >= 0 && nSelection < m_FilterArray.GetSize() )
		nStockIdx = m_FilterArray.GetStockItemIndexAt ( nSelection );

	m_FilterArray.SetDescriptionFilter ( "", 0, FALSE );
	m_FilterArray.SetCategoryFilter(0);
	m_FilterArray.SetSupplierFilter(0);
	m_FilterArray.SetMinMaxFilter( STOCKFILTER_MINMAX_ALL );
	m_FilterArray.SetNoPluLinkFlag( TRUE );
	m_FilterArray.SetSinglePluLinkFlag( TRUE );
	m_FilterArray.SetMultiPluLinkFlag( TRUE );
	
	BuildRecordList();
	DisplayScreen(nStockIdx);
}

/**********************************************************************/

bool CStockDatabaseDlg::ContextMenuSort( CPoint& point )
{
	HWND hWnd = HWND( m_listStock );

	HDHITTESTINFO hitTest;
	hitTest.pt.x = point.x;
	hitTest.pt.y = point.y;

	::ScreenToClient( hWnd, &hitTest.pt );

	CHeaderCtrl* headerCtrl = m_listStock.GetHeaderCtrl();
	int nIndex = headerCtrl -> SendMessage( HDM_HITTEST, 0, (LPARAM) &hitTest );

	int nType = m_ColumnManager.GetListColumnType( nIndex );

	switch( nType )
	{
	case COLUMN_STOCKCODE:
		SortListInternal( STOCKFILTER_SORT_FORWARD_STOCKCODE );
		return TRUE;
	
	case COLUMN_DESC:
		SortListInternal( STOCKFILTER_SORT_FORWARD_DESCRIPTION );
		return TRUE;
	
	case COLUMN_CATEGORY:
		SortListInternal( STOCKFILTER_SORT_FORWARD_CATEGORY );
		return TRUE;
	}
	
	return FALSE;
}

/**********************************************************************/

void CStockDatabaseDlg::OnContextMenu(CWnd*, CPoint point)
{
	if ( ContextMenuSort( point ) == TRUE )
		return;

	if ( m_bReadOnly == TRUE )
		return;

	//NOTHING TO DO IF NO RECORDS IN DATABASE
	if ( DataManager.Stock.GetSize() == 0 )
		return;

	if (point.x == -1 && point.y == -1)
	{
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_STOCK_DATABASE));

	if ( DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_READWRITE )
		menu.DeleteMenu( IDM_STOCK_DELETE_CURRENT, MF_BYCOMMAND );

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

/**********************************************************************/

void CStockDatabaseDlg::OnStockRangeProgram() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}

	int nCurrent = m_listStock.GetCurSel();
	CStockRangeCustomFieldDlg dlg ( m_FilterArray, this  );
	m_FilterArray.GetDefaultRange ( nCurrent, dlg.m_strStockCodeFrom, dlg.m_strStockCodeTo );
	dlg.DoModal();
	m_bModifiedStock |= dlg.GetModifiedFlag();
	BuildRecordList();
	DisplayScreen ( nCurrent );
}

/**********************************************************************/

void CStockDatabaseDlg::OnStockRangeUnits() 
{
	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}

	int nCurrent = m_listStock.GetCurSel();
	CStockRangeUnitsDlg dlg ( m_FilterArray, this  );
	m_FilterArray.GetDefaultRange ( nCurrent, dlg.m_strStockCodeFrom, dlg.m_strStockCodeTo );
	dlg.DoModal();
	m_bModifiedStock |= dlg.GetModifiedFlag();
	BuildRecordList();
	DisplayScreen ( nCurrent );
}

/**********************************************************************/

void CStockDatabaseDlg::OnStockImportCsv() 
{
	CString strImportFolder = "";
	CFilenameUpdater FnUp( SysFiles::StockImport, DataManager.GetActiveDbIdx() );
	
	{
		CSSFile fileImportFolder;
		if ( fileImportFolder.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
			fileImportFolder.ReadString( strImportFolder );
	}

	CString strTitle = "Import Stock CSV File";
	CFileDialog dlgFilename ( TRUE, "csv", "stock", OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFilename.m_ofn.lpstrTitle = strTitle;
	dlgFilename.m_ofn.lpstrInitialDir = strImportFolder;
	
	if ( dlgFilename.DoModal() != IDOK )
		return;

	CString strFilename = dlgFilename.GetPathName();

	CSSFile fileImport;
	if ( fileImport.Open ( strFilename, "rb" ) == FALSE )
	{
		Prompter.Error ( "Unable to open import file." );
		return;
	}

	CString strBuffer;
	fileImport.ReadString ( strBuffer );
	bool bComplex = ( ( strBuffer.GetLength() >= 1 ) && strBuffer.Left(1) == "#" );
	fileImport.Close();

	if ( bComplex == TRUE )
	{
		Prompter.Error ( "The selected import file is not\ncompatible with this software." );
		return;
	}

	CStockPrefsDlg dlgMethod ( this );
	dlgMethod.m_nImportMethod = IMPORT_METHOD_ALL;

	if ( dlgMethod.DoModal() != IDOK )
		return;

	CDataManagerInfo info;
	if ( DataManager.OpenSupplierBuffer( info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	for ( int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
	{
		if ( DataManager.OpenCustomFieldValueBuffer( nFieldNo, info ) == FALSE )
		{
			Prompter.ShareError( info );
			DataManager.CloseSupplierBuffer( info );

			for ( int x = 1; x < nFieldNo; x++ )
				DataManager.CloseCustomFieldValueBuffer( x, info );

			return;
		}
	}

	CDatabaseImportHandlerStockman ImportHandler;
	ImportHandler.SetStockImportMethod ( dlgMethod.m_nImportMethod );
	int nResult = ImportHandler.ImportStockFile ( strFilename );

	if ( nResult == IMPORTERROR_NONE )
	{
		if ( DataManager.WriteSupplierBuffer( info ) == FALSE )
			Prompter.WriteError( info );

		for ( int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
			if ( DataManager.WriteCustomFieldValueBuffer( nFieldNo, info ) == FALSE )
				Prompter.WriteError( info );
	}

	DataManager.CloseSupplierBuffer( info );

	for ( int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
		DataManager.CloseCustomFieldValueBuffer( nFieldNo, info );

	if ( nResult != IMPORTERROR_NONE )
	{
		CString strMsg;
		strMsg.Format ( "Unable to process import file\n(error code %d)", nResult );
		Prompter.Error ( strMsg );
		return;
	}

	m_FilterArray.SetDescriptionFilter ( "", 0, FALSE );
	m_FilterArray.SetCategoryFilter(0);
	m_FilterArray.SetSupplierFilter(0);
	BuildRecordList();
	DisplayScreen(0);			

	int nStockAdded = ImportHandler.GetImportStockAdded();
	int nStockModified = ImportHandler.GetImportStockModified();
	if ( nStockAdded != 0 || nStockModified != 0 )
		m_bModifiedStock = TRUE;

	CDWordArray dw;
	dw.Add( ImportHandler.GetImportTotalLines() );
	dw.Add( ImportHandler.GetImportIgnoredLines() );
	dw.Add( nStockAdded );
	dw.Add( nStockModified );
	dw.Add( ImportHandler.GetImportSupplierAdded() );
	MessageLogger.LogSystem( 3, 2, FALSE, dw );

	CPluImportStatsDlgStockman dlgInfo ( TRUE, this );
	dlgInfo.m_nTotalLines = ImportHandler.GetImportTotalLines();
	dlgInfo.m_nIgnoredLines = ImportHandler.GetImportIgnoredLines();
	dlgInfo.m_nPluAdded = nStockAdded;
	dlgInfo.m_nPluModified = nStockModified;
	dlgInfo.m_nStockAdded = ImportHandler.GetImportSupplierAdded();	
	dlgInfo.DoModal();	
	
	CString strPath = dlgFilename.GetPathName();
	CString strFile = dlgFilename.GetFileName();

	if ( strPath.GetLength() >= strFile.GetLength() )
	{
		strPath = strPath.Left ( strPath.GetLength() - strFile.GetLength() );

		CSSFile fileImportFolder;
		if ( fileImportFolder.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
			fileImportFolder.WriteLine( strPath );
	}
}

/**********************************************************************/

void CStockDatabaseDlg::OnStockExportCsv() 
{
	CPtrArray CustomFieldPtrArray;
	DataManager.GetCustomFieldPtrArray( CustomFieldPtrArray );
	
	ExportDate.Read();

	if ( m_FilterArray.GetSize() == 0 )
	{
		Prompter.Error ( "You must select some records before\nyou can use this option." );
		return;
	}
	
	CStockExportDlg dlgFields( m_FilterArray, m_listStock.GetCurSel(), this);
	if ( dlgFields.DoModal() == IDCANCEL )
		return;

	int nStartIdx, nRange;
	m_FilterArray.GetArrayIdxRange( dlgFields.m_strStockFrom, dlgFields.m_strStockTo, nStartIdx, nRange );

	if ( nRange == 0 )
	{
		Prompter.Error ( "There are no stock items in the selected range" );
		return;
	}

	CString strPathName = ExportDate.GetLastStockFile();
	
	int nSupplierCount = StockOptions.GetStockMaxSuppliersPerItem();
	CString strDatabaseHeader = CStockCSVRecord::GetHeader( nSupplierCount );
	CString strExportHeader = dlgFields.GetStockHeaderLine();

	CSSExportFilterFile fileExportStock ( strDatabaseHeader, strExportHeader );
	if ( fileExportStock.Open ( strPathName, "wb" ) == FALSE )
	{
		CString strMsg;
		strMsg.Format ( "Unable to create export file %s", (const char*) strPathName );
		Prompter.Error ( strMsg );
		return;
	}

	int nStockExported = 0;
	
	//LOOP THROUGH SELECTED RANGE OF STOCK RECORDS
	StatusProgress.Lock( TRUE, "Exporting stock data" );
	for ( int nProgress = 0; nProgress < nRange; nProgress++ )
	{
		StatusProgress.SetPos ( nProgress, nRange );	
		
		CStockCSVRecord StockRecord;
		int nStockIdx = m_FilterArray.GetStockItemIndexAt ( nStartIdx + nProgress );
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );

		CString strStockCode = StockRecord.GetStockCode();
		if ( ( strStockCode < dlgFields.m_strStockFrom ) || ( strStockCode > dlgFields.m_strStockTo ) )
			continue;

		//EXPORT THE STOCK RECORD
		CCSV csv( ',', '"', FALSE, FALSE, TRUE);
		StockRecord.ConvertToCSVSimpleExport ( csv, DataManager.Supplier, CustomFieldPtrArray );
		fileExportStock.WriteData ( csv.GetLine() );
		nStockExported++;
	}
	StatusProgress.Unlock();
	fileExportStock.Close();

	ExportDate.Write();

	//SHOW THE RESULTS
	CString strMsg;
	switch ( nStockExported )
	{
	case 0:	strMsg = "No stock items were exported.";	break;
	case 1:	strMsg = "1 stock item was exported.";	break;
	default:	strMsg.Format ( "%d stock items were exported.", nStockExported );	break;
	}

	Prompter.Info ( strMsg );
}

/**********************************************************************/

void CStockDatabaseDlg::OnColumnEdit() 
{
	m_ColumnManager.RememberCurrentLine( &m_listStock, this );
	CArray<CColumnStatusInfo,CColumnStatusInfo> arrayPos;
	m_ColumnManager.GetListColumnDetails( arrayPos );

	CColumnListDlg dlg( "Stock Database", arrayPos, this );
	if ( dlg.DoModal() == IDOK )
	{
		m_ColumnManager.SetListColumnDetails( arrayPos );
		m_ColumnManager.RedrawList( &m_listStock, this );
		m_ColumnManager.SaveCurrentLine( &m_listStock, this );
	}
}

/**********************************************************************/

void CStockDatabaseDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listStock, this );
}

/**********************************************************************/

void CStockDatabaseDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CStockDatabaseDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CStockDatabaseDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/

void CStockDatabaseDlg::OnCancel() 
{
	if ( m_bReadOnly == TRUE )
	{
		EndDialog( IDCANCEL );
		return;
	}

	CDWordArray dw;
	dw.Add( DataManager.Plu.GetSize() );
	dw.Add( DataManager.Stock.GetSize() );

	if ( m_bModifiedStock == TRUE  )
	{
		int nChoice = Prompter.YesNoCancel ( "Stock database has been modified - save changes" );
		
		switch ( nChoice )
		{
		case IDYES:
			{
				CWaitCursor wait;
				DataManager.Plu.UpdatePrices();
				DataManager.Plu.RemoveDeadStockLinks();
				
				CDataManagerInfo info;
				DataManager.WriteStock( info, TRUE, TRUE );
				DataManager.Plu.UpdateSalesManCostPrice();
				DataManager.WritePlu( info );

				COpeningStockAuditer OpeningStockAuditer;
				
				CCustomExportManagerPlu CustomExport ( DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE );
				MessageLogger.LogSystem( 3, 5, FALSE, dw );
				break;
			}
			
		case IDNO:
			MessageLogger.LogSystem( 3, 6, FALSE, dw );
			break;

		case IDCANCEL:
			return;
		}
	}
	else
		MessageLogger.LogSystem( 3, 7, FALSE );
	
	EndDialog ( IDCANCEL );
}

/**********************************************************************/

void CStockDatabaseDlg::SortListInternal( int nType )
{
	CString strStockCode = "";
	
	{
		int nIndex = m_listStock.GetCurSel();
		if ( ( nIndex >= 0 ) && ( nIndex < m_FilterArray.GetSize() ) )
			strStockCode = DataManager.Stock.GetStockCode( m_FilterArray.GetStockItemIndexAt( nIndex ) );
	}

	m_FilterArray.ChangeSortOrder( nType, TRUE );
	m_listStock.SetItemCountEx ( m_FilterArray.GetSize() );

	if ( strStockCode == "" )
		DisplayScreen(0);
	else
	{
		int nSel;
		m_FilterArray.FindStockItemByCode( strStockCode, nSel );
		DisplayScreen( nSel );
	}
}

/**********************************************************************/
