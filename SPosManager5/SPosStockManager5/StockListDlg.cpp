/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "StockCodeDlg.h"
#include "StockDefineFilterDlg.h"
#include "StockImportSheetDlg.h"
#include "StockOptionsIni.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "StockListDlg.h"
/**********************************************************************/
static const int TOOLBAR_SPACE = 28;
/**********************************************************************/

CStockListDlg::CStockListDlg ( CWnd* pParent ) : CDialog(CStockListDlg::IDD, pParent),
	m_CacheCSV ( '\t' )
{
	m_pFilterArray = NULL;
	m_nCategory = 0;

	m_nCacheIndex = -1;

	m_bInitDialog = FALSE;
	m_bImported = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_nReportType = 0;
	m_nExceptionType = 0;
	m_nSpIdx = 0;

	DataManager.StockPlu.Invalidate();

	m_bStockWriteError = FALSE;
}

/**********************************************************************/

CStockListDlg::CStockListDlg( int nReportType, int nExceptionType, int nSpIdx, CWnd* pParent)
	: CDialog(CStockListDlg::IDD, pParent),
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CStockListDlg)
	//}}AFX_DATA_INIT
	m_nCacheIndex = -1;

	m_pFilterArray = NULL;
	m_nCategory = 0;

	m_nSpIdx = nSpIdx;
	m_bInitDialog = FALSE;
	m_bImported = FALSE;

	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_nReportType = nReportType;
	m_nExceptionType = nExceptionType;
	
	dbStockpoint.RemoveTemporaryAudits( m_nSpIdx );

	DataManager.StockPlu.Invalidate();

	m_bStockWriteError = FALSE;
}

/**********************************************************************/

CStockListDlg::~CStockListDlg()
{
	if (m_pFilterArray != NULL)
	{
		delete m_pFilterArray;
	}

	dbStockpoint.RemoveTemporaryAudits( m_nSpIdx );
}

/**********************************************************************/

void CStockListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockListDlg)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockListDlg, CDialog)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CStockListDlg)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_COMMAND(IDM_EDIT_STOCKLEVEL, OnEditStockLevel)
	ON_COMMAND(IDM_IMPORT_CREATESHEET, OnCreateImportSheet)
	ON_COMMAND(IDM_IMPORT_STOCKLEVEL, OnImport)
	ON_COMMAND(IDM_IMPORT_WRAPPER, OnImportWrapper)
	ON_COMMAND(IDM_IMPORT_IMPORTSHEET, OnImportSheetFile)
	ON_COMMAND(IDM_IMPORT_IMPORTSHEET_FOLDER, OnImportSheetFolder)
	ON_COMMAND(IDM_FILTER_STOCKRECORDS, OnFilterStockRecords)
	ON_COMMAND(IDM_NOFILTER_STOCKRECORDS, OnNofilterStockRecords)
	ON_COMMAND(IDM_RANDOM_STOCKLEVEL, OnEditStockLevelRandom)
	ON_COMMAND(IDM_COPY_LEVELS, OnCopyLevels)
	ON_COMMAND(IDM_PRINT_SESSION, OnPrintSession)
	ON_COMMAND(IDM_COPY_PLU, OnCopyPlu)
	ON_COMMAND(IDM_COPY_SUPPLIER, OnCopySupplier)
	ON_COMMAND(IDM_REMOVE_ZEROS, OnRemoveZeros)
	ON_COMMAND(IDM_RANGE_ACTIVE, OnRangeActive)
	ON_COMMAND(IDM_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM_COLUMN_DEFAULT, OnColumnDefault)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	ON_COMMAND(IDM_EXIT, OnCancel)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnEditStockLevel)
	ON_COMMAND(IDM_EDIT_STOCKLEVEL2, OnEditStockLevel)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_USER_BUTTON, OnUserButton )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );
	m_listStock.SubclassDlgItem ( IDC_LIST, this );
	m_listStock.EnableColour();
	
	SetFilterArray();

	m_listStock.SetItemCountEx ( m_pFilterArray -> GetSize() );

	m_MenuChanger.EatMenuTopLevel( GetMenu() );
	PrepareMenu();
	m_MenuChanger.BuildMenu ( GetMenu() );
	DrawMenuBar();

	m_bInitDialog = TRUE;
	SetColumns();
	m_ColumnManager.RedrawList( &m_listStock, this );
	CenterWindow();
	DisplayScreen ( 0 );
	RemoveAudit();
	return TRUE;
}

/**********************************************************************/

void CStockListDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listStock.MoveWindow ( 0, 0, cx, cy );
	}
}

/**********************************************************************/

void CStockListDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EditStockLevel();
	*pResult = 0;
}

/**********************************************************************/

void CStockListDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

void CStockListDlg::OnContextMenu(CWnd*, CPoint point)
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
	case COLUMN_STOCKCODE2:	SortListInternal( STOCKFILTER_SORT_FORWARD_NEWSTOCKCODE );	break;
	case COLUMN_DESC:		SortListInternal( STOCKFILTER_SORT_FORWARD_DESCRIPTION );	break;
	case COLUMN_CATEGORY:	SortListInternal( STOCKFILTER_SORT_FORWARD_CATEGORY );		break;
	}
}

/**********************************************************************/

void CStockListDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ( nIndex != m_nCacheIndex  )
		{
			m_CacheCSV.ParseLine( m_pFilterArray -> GetDisplayLine ( nIndex ) );
			m_nCacheIndex = nIndex;
		}

		lstrcpy ( pDispInfo->item.pszText, m_CacheCSV.GetString ( pDispInfo->item.iSubItem ) );

		if ( m_pFilterArray -> GetNegativeStockFlag() )
			m_listStock.SetColour( nIndex, 0x0C );
		else if ( m_pFilterArray -> GetBelowMinStockFlag() )
			m_listStock.SetColour( nIndex, COLORREF ( StockOptions.GetStockBelowMinimumColour() ) );
		else if ( m_pFilterArray -> GetAboveMaxStockFlag() )
			m_listStock.SetColour( nIndex, COLORREF ( StockOptions.GetStockAboveMaximumColour() ) );
		else
			m_listStock.SetColour( nIndex, 0x00 );
	}
	*pResult = 0;
}

/**********************************************************************/

void CStockListDlg::OnCreateImportSheet()
{
	HandleCreateImportSheet();
}

/**********************************************************************/

void CStockListDlg::OnFindByCode()
{
	CStockCodeDlg dlg ( "Find Stock Code", m_StockCodeSearch, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		CString strStockCode = m_StockCodeSearch.m_strStockCodeToUse;

		int nIndex;
		if ( m_pFilterArray -> FindStockItemByCode ( strStockCode, nIndex ) == FALSE )
		{
			CString strMsg;
			strMsg.Format ( "Cannot find Stock Code\n%s", (const char*) strStockCode );
			Prompter.Error ( strMsg );
		}	
		m_listStock.SetCurSel ( nIndex );
	}
}

/**********************************************************************/

void CStockListDlg::OnEditStockLevel() 
{
	EditStockLevel();
}

/**********************************************************************/

void CStockListDlg::OnEditStockLevelRandom() 
{
	EditStockLevelRandom();
}

/**********************************************************************/

void CStockListDlg::DisplayScreen ( int nIndex )
{
	int nSize = m_pFilterArray -> GetSize();
	CString strFiltered = ( m_pFilterArray -> GetFilteredFlag() == TRUE ) ? ", Filtered" : "";

	CString strInactive = "";
	if ( m_pFilterArray -> GetInactiveExcludeCount() != 0 )
		strInactive.Format ( ", %d inactive", m_pFilterArray -> GetInactiveExcludeCount() );

	CString strText;
	strText.Format ( "%s (%s%d of %d records%s%s)",
		GetTitle(),
		GetSpName(),
		nSize,
		DataManager.Stock.GetSize(),
		(const char*) strInactive,
		(const char*) strFiltered );
	
	SetWindowText ( strText );

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listStock.SetCurSel ( nIndex );
	else
		m_listStock.SetCurSel ( nSize - 1 );

	m_listStock.Invalidate();
}

/**********************************************************************/

void CStockListDlg::BuildRecordList()
{
	m_pFilterArray -> BuildRecordList ();
	m_listStock.SetItemCountEx ( m_pFilterArray -> GetSize() );
}

/**********************************************************************/

void CStockListDlg::OnCancel() 
{
	HandleCancel();
}

/**********************************************************************/

void CStockListDlg::HandleCancel()
{
	int nExit = IDOK;

	if ( DataManager.Stock.IsModified() == TRUE || m_bImported == TRUE )
	{
		int nChoice = Prompter.YesNoCancel ( "Stock database has been modified - save changes" );	
		nExit = IDCANCEL;

		switch ( nChoice )
		{
		case IDYES:
			{
				CWaitCursor wait;
				
				CDataManagerInfo info;
				if ( DataManager.WriteStockLevels( m_nSpIdx, info ) == FALSE )
				{
					Prompter.WriteError( info );
					m_bStockWriteError = TRUE;
				}
				else
				{
					AppendAudit();
			
					for ( int nIndex = 0; nIndex < m_ImportFilesArray.GetSize(); nIndex++ )
					{
						CString strFile = m_ImportFilesArray.GetAt ( nIndex );
						CFileRemover FileRemover ( strFile ); 
					}

					nExit = IDOK;
				}
			}	
			break;

		case IDNO:
			RemoveAudit();
			break;

		case IDCANCEL:
			return;
		}
	}
	EndDialog ( nExit );
}

/**********************************************************************/

long CStockListDlg::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	int nIndex = ( int ) wIndex;
	int nType = m_ColumnManager.GetListColumnType( nIndex );

	switch( nType )
	{
	case COLUMN_STOCKCODE:
		if ( m_pFilterArray -> GetSize() > 0 )
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

void CStockListDlg::RemoveAudit()
{
	dbStockpoint.RemoveTemporaryAudits( m_nSpIdx );
}

/**********************************************************************/

void CStockListDlg::OnImport() 
{
	if ( UpdateData ( TRUE ) == TRUE )
		HandleImport();
}

/**********************************************************************/

void CStockListDlg::OnImportSheetFile()
{
	if (UpdateData(TRUE) == TRUE)
	{
		HandleImportSheetFile();
	}
}

/**********************************************************************/

void CStockListDlg::OnImportSheetFolder()
{
	if (UpdateData(TRUE) == TRUE)
	{
		HandleImportSheetFolder();
	}
}

/**********************************************************************/

void CStockListDlg::OnImportWrapper() 
{
	if ( UpdateData ( TRUE ) == TRUE )
		HandleImportWrapper();
}

/**********************************************************************/

void CStockListDlg::OnPrintSession() 
{
	if ( UpdateData ( TRUE ) == TRUE )
		HandlePrintSession();
}

/**********************************************************************/

void CStockListDlg::OnCopyLevels() 
{
	if ( UpdateData ( TRUE ) == TRUE )
		HandleCopyLevels();
}

/**********************************************************************/

void CStockListDlg::OnFilterStockRecords() 
{
	CStockDefineFilterDlg dlg ( *m_pFilterArray, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		BuildRecordList();
		DisplayScreen ( 0 );
	}
}

/**********************************************************************/

void CStockListDlg::OnNofilterStockRecords() 
{
	int nStockIdx = 0;
	int nSelection = m_listStock.GetCurSel();

	if ( nSelection >= 0 && nSelection < m_pFilterArray -> GetSize() )
		nStockIdx = m_pFilterArray -> GetStockItemIndexAt ( nSelection );

	m_pFilterArray -> SetDescriptionFilter ( "", 0, FALSE );
	m_pFilterArray -> SetCategoryFilter(0);
	m_pFilterArray -> SetSupplierFilter(0);
	m_pFilterArray -> SetNoPluLinkFlag( TRUE );
	m_pFilterArray -> SetSinglePluLinkFlag( TRUE );
	m_pFilterArray -> SetMultiPluLinkFlag( TRUE );
	
	BuildRecordList();
	DisplayScreen(nStockIdx);
}

/**********************************************************************/

void CStockListDlg::OnCopySupplier() 
{
	HandleCopySupplierStockCodes();
}

/**********************************************************************/

void CStockListDlg::OnCopyPlu() 
{
	HandleCopyPluNumbers();	
}

/**********************************************************************/

void CStockListDlg::OnRemoveZeros() 
{
	HandleRemoveStockZeros();
}

/**********************************************************************/

void CStockListDlg::OnRangeActive() 
{
	HandleRangeActive();
}

/**********************************************************************/

const char* CStockListDlg::GetSpName()
{
	m_strSpName = "";
	if ( dbStockpoint.GetSize() >= 2 )
	{
		m_strSpName = dbStockpoint.GetName( m_nSpIdx );
		m_strSpName += ", ";
	}
	return m_strSpName;
}

/**********************************************************************/

void CStockListDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listStock, this );
}

/**********************************************************************/

void CStockListDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CStockListDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CStockListDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/

void CStockListDlg::SortListInternal( int nType )
{
	CString strStockCode = "";
	
	{
		int nIndex = m_listStock.GetCurSel();
		if ( ( nIndex >= 0 ) && ( nIndex < m_pFilterArray -> GetSize() ) )
			strStockCode = DataManager.Stock.GetStockCode( m_pFilterArray -> GetStockItemIndexAt( nIndex ) );
	}
	
	bool bRefresh = FALSE;
	switch( nType )
	{
	case STOCKFILTER_SORT_FORWARD_NEWSTOCKCODE:
	case STOCKFILTER_SORT_REVERSE_NEWSTOCKCODE:
		bRefresh = TRUE;
		break;
	}

	m_pFilterArray -> ChangeSortOrder( nType, bRefresh );
	m_listStock.SetItemCountEx ( m_pFilterArray -> GetSize() );

	if ( strStockCode == "" )
		DisplayScreen(0);
	else
	{
		int nSel;
		m_pFilterArray -> FindStockItemByCode( strStockCode, nSel );
		DisplayScreen( nSel );
	}
}

/**********************************************************************/
