/**********************************************************************/
#include "..\CommonEcrStock\CustomExportManager.h"
#include "DatabaseCSVArray.h"
#include "FilterTypes.h"
#include "..\CommonEcrStock\KeyboardHelpers.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockCodeDlg.h"
#include "..\CommonEcrStock\StockDeleteCloseDlg.h"
#include "StockDeletePrintDlg.h"
#include "StockDeleteRangeSetDlg.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "StockDeleteDatabaseDlg.h"
/**********************************************************************/

CStockDeleteDatabaseDlg::CStockDeleteDatabaseDlg( bool bKeepOnOrder, bool bKeepApparent, bool bKeepSales, CWnd* pParent )
	: CDialog(CStockDeleteDatabaseDlg::IDD, pParent), 
	m_FilterArray ( FILTERTYPE_STOCK_DELETION ),
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CStockDeleteDatabaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bDoneDelete = FALSE;

	m_nCacheIndex = -1;

	m_strDatabaseName = "";
	int nStockDbIdx = DataManager.GetActiveDbIdx();

	if ( nStockDbIdx >= 0 && nStockDbIdx < dbDatabase.GetSize() )
		m_strDatabaseName = dbDatabase.GetName( nStockDbIdx );
	
	m_nDeletionCount = 0;
	m_FilterArray.QuickBuild ( bKeepOnOrder, bKeepApparent, bKeepSales );
	
	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_strStockCodeSearch = "";
	m_bStockCodeSearchFromStart = FALSE;
	m_bInitDialog = FALSE;
	m_strTitle = "Delete Unlinked Stock Records";
	m_bUnusedMode = FALSE;
}	
	

/**********************************************************************/

CStockDeleteDatabaseDlg::CStockDeleteDatabaseDlg( CByteArray& FlagArray, bool bKeepOnOrder, bool bKeepApparent, bool bKeepSales, CWnd* pParent )
	: CDialog(CStockDeleteDatabaseDlg::IDD, pParent), 
	m_FilterArray ( FILTERTYPE_STOCK_DELETION ),
	m_CacheCSV ( '\t' )
{
	m_bDoneDelete = FALSE;

	m_nCacheIndex = -1;

	m_strDatabaseName = "";
	int nStockDbIdx = DataManager.GetActiveDbIdx();

	if ( nStockDbIdx >= 0 && nStockDbIdx < dbDatabase.GetSize() )
		m_strDatabaseName = dbDatabase.GetName( nStockDbIdx );
	
	m_nDeletionCount = 0;
	m_FilterArray.QuickBuild ( FlagArray, bKeepOnOrder, bKeepApparent, bKeepSales );
	
	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_strStockCodeSearch = "";
	m_bStockCodeSearchFromStart = FALSE;
	m_bInitDialog = FALSE;
	m_strTitle = "Delete Unsold Stock Records";
	m_bUnusedMode = TRUE;
}	
	

/**********************************************************************/

void CStockDeleteDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockDeleteDatabaseDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockDeleteDatabaseDlg, CDialog)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CStockDeleteDatabaseDlg)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_COMMAND(IDM_STOCK_DELETE_RANGE, OnStockDeleteRange)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_COMMAND(IDM_STOCK_DELETE_ALL, OnStockDeleteAll)
	ON_COMMAND(IDM_STOCK_DELETE_NONE, OnStockDeleteNone)
	ON_COMMAND(IDM_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM_COLUMN_DEFAULT, OnColumnDefault)
	ON_COMMAND(IDM_PRINT_SESSION, OnPrint)
	ON_COMMAND(IDM_EXIT, OnCancel)
	ON_COMMAND(IDM_FILE_EXIT, OnCancel)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_USER_BUTTON, OnUserButton )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockDeleteDatabaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );

	m_listStock.SubclassDlgItem ( IDC_LIST, this );

	m_ColumnManager.Reset( TableNames::DeleteStock );
	m_ColumnManager.AddColumn( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 115, 115 );
	m_ColumnManager.AddColumn( ColumnNames::Desc, "Description", LVCFMT_LEFT, 135, 170 );
	m_ColumnManager.AddColumn( ColumnNames::Category, "Category", LVCFMT_LEFT, 80, 140 );
	m_ColumnManager.AddColumn( ColumnNames::OnOrder, "On Order", LVCFMT_RIGHT, 60, 80 );
	m_ColumnManager.AddColumn( ColumnNames::Apparent, "Apparent", LVCFMT_RIGHT, 60, 80 );
	m_ColumnManager.AddColumn( ColumnNames::Sales, "Sales", LVCFMT_RIGHT, 60, 80 );
	m_ColumnManager.AddColumn( ColumnNames::Delete, "Delete", LVCFMT_LEFT, 60, 60 );
	m_ColumnManager.LoadSettings();
	
	m_listStock.SetItemCountEx ( m_FilterArray.GetSize() );

	m_bInitDialog = TRUE;
	m_ColumnManager.RedrawList( &m_listStock, this );
	CenterWindow();
	DisplayScreen ( 0 );

	return TRUE;
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listStock.MoveWindow ( 0,0,cx,cy );
	}
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::DisplayScreen ( int nIndex )
{
	CString strMsg;
	int nSize = m_FilterArray.GetSize();

	CString strFiltered = ( m_FilterArray.GetFilteredFlag() == TRUE ) ? ", Filtered" : "";

	strMsg.Format ( "%s - (%s, selected %d of %d)",
		(const char*) m_strTitle,
		(const char*) m_strDatabaseName, 
		m_nDeletionCount,
		m_FilterArray.GetSize() );

	SetWindowText ( strMsg );

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listStock.SetCurSel ( nIndex );
	else
		m_listStock.SetCurSel ( nSize - 1 );

	m_listStock.Invalidate();
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ( nIndex != m_nCacheIndex  )
		{
			m_CacheCSV.ParseLine( m_FilterArray.GetDisplayLine ( nIndex ) );
			m_nCacheIndex = nIndex;
		}

		lstrcpy ( pDispInfo->item.pszText, m_CacheCSV.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ToggleFlag();	
	*pResult = 0;
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnDefaultButton() 
{
	ToggleFlag();	
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CStockDeleteDatabaseDlg::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	int nIndex = ( int ) wIndex;
	int nType = m_ColumnManager.GetListColumnType( nIndex );

	switch( nType )
	{
	case COLUMN_STOCKCODE:
		if ( m_FilterArray.GetSize() > 0 )
			OnFindByCode();
		break;
	}
	
	return 0L;
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnFindByCode()
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

void CStockDeleteDatabaseDlg::OnContextMenu( CWnd* pWnd, CPoint point )
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
	case COLUMN_STOCKCODE:	SortListInternal( STOCKFILTER_SORT_FORWARD_STOCKCODE );		break;
	case COLUMN_DESC:		SortListInternal( STOCKFILTER_SORT_FORWARD_DESCRIPTION );	break;
	case COLUMN_CATEGORY:	SortListInternal( STOCKFILTER_SORT_FORWARD_CATEGORY );		break;
	}
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::ToggleFlag()
{
	int nIndex = m_listStock.GetCurSel();

	if ( nIndex >= 0 && nIndex < m_FilterArray.GetSize() )
	{
		if ( m_FilterArray.GetDeletionFlagAt( nIndex ) == FALSE )
		{
			m_FilterArray.SetDeletionFlagAt( nIndex, TRUE );
			m_nDeletionCount++;
		}
		else
		{
			m_FilterArray.SetDeletionFlagAt( nIndex, FALSE );
			m_nDeletionCount--;
		}

		DisplayScreen ( nIndex );
	}
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnStockDeleteRange() 
{
	int nSelection = m_listStock.GetCurSel();

	CStockDeleteRangeSetDlg dlg ( this );

	if ( nSelection >= 0 )
	{
		int nStockIdx = m_FilterArray.GetStockItemIndexAt ( nSelection );
		CString strStockCode = DataManager.Stock.GetStockCode ( nStockIdx );
		dlg.m_strStockCodeFrom = strStockCode; 
		dlg.m_strStockCodeTo = strStockCode;
	}
	else
	{
		dlg.m_strStockCodeFrom = "";
		dlg.m_strStockCodeTo = "";
	}
		
	dlg.m_nCategoryFilter = 0;

	if ( dlg.DoModal() == IDOK )
	{
		int nStartIdx, nRange;
		m_FilterArray.GetArrayIdxRange( dlg.m_strStockCodeFrom, dlg.m_strStockCodeTo, nStartIdx, nRange );
		
		CCategorySetCSVRecord CategorySet;
		CategorySet.CreateCategorySet ( dlg.m_nCategoryFilter );

		StatusProgress.Lock( TRUE, "Updating deletion list" );

		int nCount = 0;
		for ( int nOffset = 0; nOffset < nRange; nOffset++ )
		{
			StatusProgress.SetPos ( nOffset, nRange ); 

			CStockCSVRecord StockRecord;
			int nStockIdx = m_FilterArray.GetStockItemIndexAt ( nStartIdx + nOffset );
			
			DataManager.Stock.GetAt ( nStockIdx, StockRecord );
			
			CString strStockCode = StockRecord.GetStockCode();
			if ( ( strStockCode < dlg.m_strStockCodeFrom ) || ( strStockCode > dlg.m_strStockCodeTo ) )
				continue;
	
			if ( CategorySet.GetCategoryStatus ( StockRecord.GetCategory() ) == FALSE )
				continue;

			if ( 0 == dlg.m_nDelete ) // 0 == DELETE
			{
				if ( m_FilterArray.GetDeletionFlagAt( nStartIdx + nOffset ) == FALSE )
				{
					m_FilterArray.SetDeletionFlagAt(  nStartIdx + nOffset , TRUE );
					m_nDeletionCount++;
				}
			}
			else
			{
				if ( m_FilterArray.GetDeletionFlagAt(  nStartIdx + nOffset  ) == TRUE )
				{
					m_FilterArray.SetDeletionFlagAt(  nStartIdx + nOffset , FALSE );
					m_nDeletionCount--;
				}
			}
			
			nCount++;
		}
		StatusProgress.Unlock();
		
		DisplayScreen( nSelection );
		
		CString strBuf;

		switch ( nCount )
		{
		case 0:
			Prompter.Info ( "No records were matched" );
			break;

		case 1:
			Prompter.Info ( "1 record was matched" );
			break;

		default:
			strBuf.Format ( "%d records were matched", nCount );
			Prompter.Info ( strBuf );
			break;
		}
	}
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnStockDeleteAll() 
{
	if ( Prompter.YesNo ( "Are you sure you wish to select all these records for deletion." ) == IDYES )
		SelectAll ( 1 );
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnStockDeleteNone() 
{
	if ( Prompter.YesNo ( "Are you sure you wish to save all these records from deletion." ) == IDYES )
		SelectAll ( 0 );
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::SelectAll ( int nFlag )
{
	int nSelection = m_listStock.GetCurSel();
	int nCount = m_FilterArray.GetSize();

	StatusProgress.Lock( TRUE, "Updating deletion list" );
	for ( int nIdx = 0; nIdx < nCount; nIdx++ )
		m_FilterArray.SetDeletionFlagAt( nIdx, ( 1 == nFlag ) );

	m_nDeletionCount = ( 1 == nFlag ) ? nCount : 0;

	StatusProgress.Unlock();
	
	DisplayScreen( nSelection );	
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listStock, this );
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnPrint()
{
	BOOL bLines = TRUE;
	BOOL bMarked = TRUE;
	BOOL bOnOrder = TRUE;
	BOOL bApparent = TRUE;
	BOOL bSales = TRUE;
	BOOL bValue = TRUE;
	BOOL bEmpty = TRUE;

	CSSFile fileOptions;
	CFilenameUpdater FnUp( SysFiles::StockDeleteDef );
	if ( fileOptions.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		CString strBuffer;
		fileOptions.ReadString( strBuffer );
		CCSV csv( strBuffer );

		if ( csv.GetInt(0) == 1 )
		{
			bLines = csv.GetBool(1);
			bMarked = csv.GetBool(2);
			bOnOrder = csv.GetBool(3);
			bApparent = csv.GetBool(4);
			bSales = csv.GetBool(5);
			bValue = csv.GetBool(6);
			bEmpty = csv.GetBool(7);
		}
	}

	while( TRUE )
	{
		CStockDeletePrintDlg dlg( this );

		dlg.m_bLines = bLines;
		dlg.m_bMarked = bMarked;
		dlg.m_bOnOrder = bOnOrder;
		dlg.m_bApparent = bApparent;
		dlg.m_bSales = bSales;
		dlg.m_bValue = bValue;
		dlg.m_bEmpty = bEmpty;

		if ( dlg.DoModal() != IDOK )
			return;

		bLines = dlg.m_bLines;
		bMarked = dlg.m_bMarked;
		bOnOrder = dlg.m_bOnOrder;
		bApparent = dlg.m_bApparent;
		bSales = dlg.m_bSales;
		bValue = dlg.m_bValue;
		bEmpty = dlg.m_bEmpty;

		CReportFile ReportFile;
		if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		{
			Prompter.Error( "Unable to create report" );
			continue;
		}
	
		ReportFile.SetStyle1 ( "Stock Deletion List" );
		
		CCSV csvColumns;
		AddReportColumn( ReportFile, csvColumns, "Code", TA_LEFT, 380 );
		AddReportColumn( ReportFile, csvColumns, "Description", TA_LEFT, 300 );
		AddReportColumn( ReportFile, csvColumns, "Category", TA_LEFT, 300 );
		
		if ( bOnOrder != 0 )
			AddReportColumn( ReportFile, csvColumns, "On Order", TA_RIGHT, 200 );
		
		if ( bApparent != 0 )
			AddReportColumn( ReportFile, csvColumns, "Apparent", TA_RIGHT, 200 );
		
		if ( bSales != 0 )
			AddReportColumn( ReportFile, csvColumns, "Sales", TA_RIGHT, 200 );

		if ( bValue != 0 )
			AddReportColumn( ReportFile, csvColumns, "Value", TA_RIGHT, 200 );

		if ( bEmpty != 0 )
			AddReportColumn( ReportFile, csvColumns, "", TA_LEFT, 450 );
		
		AddReportColumn( ReportFile, csvColumns, "Delete", TA_LEFT, 200 );
				
		for ( int nIdx = 0; nIdx < m_FilterArray.GetSize(); nIdx++ )
		{
			int nStockIdx = m_FilterArray.GetStockItemIndexAt( nIdx );
		
			if ( bMarked == 0 )
				if ( m_FilterArray.GetDeletionFlagAt ( nIdx ) == FALSE )
					continue;

			if ( ( nStockIdx >= 0 ) && ( nStockIdx < DataManager.Stock.GetSize() ) )
			{
				CStockCSVRecord StockRecord;
				CStockLevelsCSVRecord DbStockLevels;	
				DataManager.Stock.GetAt ( nStockIdx, StockRecord );
				DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, DbStockLevels );
		
				CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );

				CReportHelpers ReportHelpers;
				csv.Add ( ReportHelpers.GetDisplayStockCode ( StockRecord.GetStockCode() ) );
				csv.Add ( StockRecord.GetDescription() );		
				csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
			
				if ( bOnOrder != 0 )
					csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, DbStockLevels.GetQtyOnOrder() ) );
				
				if ( bApparent != 0 )
					csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, DbStockLevels.GetApparentQty() ) );
				
				if ( bSales != 0 )
					csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, DbStockLevels.GetTotalSalesQty() ) );
		
				if ( bValue != 0 )
				{
					double dUnitCost = StockRecord.GetUnitCost(0);
					csv.Add ( dUnitCost * DbStockLevels.GetApparentQty(), 5 );
				}

				if ( bEmpty != 0 )
					csv.Add ( "" );

				if ( m_FilterArray.GetDeletionFlagAt ( nIdx ) == TRUE )
					csv.Add ( "Yes" );

				ReportFile.WriteLine ( csv.GetLine() );

				if ( bLines != 0 )
					ReportFile.WriteLine( "<LI>" );
			}
		}
		ReportFile.Close();
	
		CReportManager ReportManager;
		CFilenameUpdater FnUp ( SysFiles::StockDeletePrm );
		CString strKey = csvColumns.GetLine();
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, this );
	}
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::AddReportColumn( CReportFile& ReportFile, CCSV& csv, const char* szType, UINT nAlign, int nWidth )
{
	ReportFile.AddColumn( szType, nAlign, nWidth );
	csv.Add( szType );
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::OnCancel()
{
	CDWordArray dw;
	dw.Add( m_nDeletionCount );

	if ( m_nDeletionCount == 0 )
	{
		Prompter.Info ( "No records were deleted." );
		MessageLogger.LogSystem( 4, 4, FALSE );
		EndDialog ( IDOK );
		return;
	}

	CStockDeleteCloseDlg dlg( NODE_STOCK, m_nDeletionCount, this );
	dlg.DoModal();

	if ( STOCKDELETE_CANCEL == dlg.m_nAction )
	{
		Prompter.Info ( "No records were deleted." );
		MessageLogger.LogSystem( 4, 5, FALSE, dw );
		EndDialog ( IDOK );
		return;
	}

	if ( STOCKDELETE_CONFIRM != dlg.m_nAction )
		return;

	DataManager.StockPlu.Invalidate();

	m_FilterArray.SetSortType( STOCKFILTER_SORT_FORWARD_STOCKCODE );
	m_FilterArray.QuickSort();

	int nProgress = 0;
	int nCount = m_FilterArray.GetSize();

	StatusProgress.Lock( TRUE, "Deleting stock items" );			
	for ( int nFilterIdx = nCount - 1; nFilterIdx >= 0; nFilterIdx-- )
	{
		StatusProgress.SetPos ( nProgress++, nCount );

		if ( m_FilterArray.GetDeletionFlagAt ( nFilterIdx ) == 1 )
		{
			int nStockIdx = m_FilterArray.GetStockItemIndexAt( nFilterIdx );
			DataManager.Stock.RemoveAt ( nStockIdx );
		}
	}
	StatusProgress.Unlock();
	
	if ( m_bUnusedMode == TRUE )
	{
		DataManager.Plu.RemoveDeadStockLinks();
		
		CDataManagerInfo info;
		DataManager.WritePlu( info );
		
		CCustomExportManagerPlu CustomExport( TRUE );
	}

	CDataManagerInfo info;
	if ( DataManager.WriteStock( info, TRUE, TRUE ) == FALSE )
	{
		MessageLogger.LogSystem( 4, 7, FALSE, dw );
		Prompter.WriteError ( info );
	}
	else
	{
		m_bDoneDelete = TRUE;
		MessageLogger.LogSystem( 4, 6, FALSE, dw );
	}

	EndDialog ( IDOK );
}

/**********************************************************************/

void CStockDeleteDatabaseDlg::SortListInternal( int nType )
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
