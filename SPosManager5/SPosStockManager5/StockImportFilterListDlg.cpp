/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "NodeTypes.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockAuditBaseReport.h"
#include "StockImportFilterCloseDlg.h"
#include "StockImportFilterPrintDlg.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "StockImportFilterListDlg.h"
/**********************************************************************/

CStockImportFilterListDlg::CStockImportFilterListDlg( int nSpIdx, int nNodeType, int nAction, CStockImportFilterArray& arrayImport, CWnd* pParent )
	: CDialog(CStockImportFilterListDlg::IDD, pParent), 
	m_arrayImport( arrayImport ),
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CStockImportFilterListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nSpIdx = nSpIdx;
	m_nNodeType = nNodeType;
	m_nAction = nAction;

	m_nCacheIndex = -1;
	
	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	
	m_bInitDialog = FALSE;

	m_strTitle = "Filter ";
	
	switch( m_nNodeType )
	{
	case NODE_CLOSING:
		m_strTitle += "Closing Stock";
		break;

	case NODE_OPENING:
		m_strTitle += "Opening Stock";
		break;
	}

	m_strTitle += " Import";
}	
	
/**********************************************************************/

void CStockImportFilterListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockImportFilterListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockImportFilterListDlg, CDialog)
	//{{AFX_MSG_MAP(CStockImportFilterListDlg)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_COMMAND(IDM_STOCK_SELECT_ALL, OnStockSelectAll)
	ON_COMMAND(IDM_STOCK_SELECT_NONE, OnStockSelectNone)
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

BOOL CStockImportFilterListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );

	m_listStock.SubclassDlgItem ( IDC_LIST, this );

	m_ColumnManager.Reset( TableNames::ImportStock );
	m_ColumnManager.AddColumn( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 115, 115 );
	m_ColumnManager.AddColumn( ColumnNames::Desc, "Description", LVCFMT_LEFT, 135, 170 );
	m_ColumnManager.AddColumn( ColumnNames::Category, "Category", LVCFMT_LEFT, 80, 140 );
	m_ColumnManager.AddColumn( ColumnNames::CurrentQty, "Current Level", LVCFMT_RIGHT, 60, 100 );
	m_ColumnManager.AddColumn( ColumnNames::ImportQty, "Imported", LVCFMT_RIGHT, 60, 100 );
	m_ColumnManager.AddColumn( ColumnNames::NewQty, "New Level", LVCFMT_RIGHT, 60, 100 );
	m_ColumnManager.AddColumn( ColumnNames::Import, "Import", LVCFMT_LEFT, 60, 60 );
	m_ColumnManager.LoadSettings();
	
	m_listStock.SetItemCountEx ( m_arrayImport.GetSize() );

	m_bInitDialog = TRUE;
	m_ColumnManager.RedrawList( &m_listStock, this );
	CenterWindow();
	DisplayScreen ( 0 );
	return TRUE;
}

/**********************************************************************/

void CStockImportFilterListDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listStock.MoveWindow ( 0,0,cx,cy );
	}
}

/**********************************************************************/

void CStockImportFilterListDlg::DisplayScreen ( int nIndex )
{
	CString strMsg;
	int nSelected = m_arrayImport.GetSelectionCount();
	int nSize = m_arrayImport.GetSize();

	strMsg.Format ( "%s - (%s, selected %d of %d)",
		(const char*) m_strTitle,
		dbStockpoint.GetName( m_nSpIdx ),
		nSelected,
		nSize );

	SetWindowText ( strMsg );

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listStock.SetCurSel ( nIndex );
	else
		m_listStock.SetCurSel ( nSize - 1 );

	m_listStock.Invalidate();
}

/**********************************************************************/

void CStockImportFilterListDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ( nIndex != m_nCacheIndex  )
		{
			m_CacheCSV.RemoveAll();

			CStockImportFilterItem item;
			m_arrayImport.GetItem( nIndex, item );

			int nStockIdx = item.m_nStockIdx;

			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( nStockIdx, StockRecord );

			CString strCurrent = DataManager.Stock.GetStockLevelString( StockRecord, item.m_dCurrentSUQty );
			CString strImport = DataManager.Stock.GetStockLevelString( StockRecord, item.m_dImportSUQty );
			CString strNew = DataManager.Stock.GetStockLevelString( StockRecord, item.GetNewStockLevel( m_nAction ) );

			m_CacheCSV.Add( StockRecord.GetStockCode() );
			m_CacheCSV.Add( StockRecord.GetDescription() );
			m_CacheCSV.Add( DataManager.Category.GetText( StockRecord.GetCategory() ) );
			m_CacheCSV.Add( strCurrent );
			m_CacheCSV.Add( strImport );
			m_CacheCSV.Add( strNew );
			m_CacheCSV.Add( item.m_bSelected ? "Yes" : "" );

			m_nCacheIndex = nIndex;
		}

		lstrcpy ( pDispInfo->item.pszText, m_CacheCSV.GetString ( pDispInfo->item.iSubItem ) );
	}
	
	*pResult = 0;
}

/**********************************************************************/

void CStockImportFilterListDlg::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ToggleFlag();	
	*pResult = 0;
}

/**********************************************************************/

void CStockImportFilterListDlg::OnDefaultButton() 
{
	ToggleFlag();	
}

/**********************************************************************/

void CStockImportFilterListDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CStockImportFilterListDlg::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	int nIndex = ( int ) wIndex;
	int nType = m_ColumnManager.GetListColumnType( nIndex );

	/*
	if ( strType == ColumnNames::StockCode.Label )
	{
		if ( m_FilterArray.GetSize() > 0 )
			OnFindByCode();
	}
	*/
	
	return 0L;
}

/**********************************************************************/

void CStockImportFilterListDlg::ToggleFlag()
{
	int nIndex = m_listStock.GetCurSel();

	if ( nIndex >= 0 && nIndex < m_arrayImport.GetSize() )
	{
		CStockImportFilterItem item;
		m_arrayImport.GetItem( nIndex, item );

		if ( item.m_bSelected == TRUE )
			item.m_bSelected = FALSE;
		else
			item.m_bSelected = TRUE;
		
		m_arrayImport.SetItem( nIndex, item );
		DisplayScreen ( nIndex );
	}
}

/**********************************************************************/

void CStockImportFilterListDlg::OnStockSelectAll() 
{
	if ( Prompter.YesNo ( "Are you sure you wish to accept all of these import lines." ) == IDYES )
		SelectAll ( 1 );
}

/**********************************************************************/

void CStockImportFilterListDlg::OnStockSelectNone() 
{
	if ( Prompter.YesNo ( "Are you sure you wish to reject all of these import lines." ) == IDYES )
		SelectAll ( 0 );
}

/**********************************************************************/

void CStockImportFilterListDlg::SelectAll ( int nFlag )
{
	int nSelection = m_listStock.GetCurSel();
	int nCount = m_arrayImport.GetSize();

	StatusProgress.Lock( TRUE, "Updating selection list" );
	for ( int nIdx = 0; nIdx < nCount; nIdx++ )
	{
		StatusProgress.SetPos ( nIdx, nCount ); 
		
		CStockImportFilterItem item;
		m_arrayImport.GetItem( nIdx, item );
		item.m_bSelected = ( nFlag == 1 );
		m_arrayImport.SetItem( nIdx, item );
	}
	StatusProgress.Unlock();

	DisplayScreen( nSelection );	
}

/**********************************************************************/

void CStockImportFilterListDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listStock, this );
}

/**********************************************************************/

void CStockImportFilterListDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CStockImportFilterListDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CStockImportFilterListDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/

void CStockImportFilterListDlg::OnPrint()
{
	BOOL bLines = TRUE;
	BOOL bMarked = TRUE;

	CSSFile fileOptions;
	CFilenameUpdater FnUp( SysFiles::StockImportDef );
	if ( fileOptions.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		CString strBuffer;
		fileOptions.ReadString( strBuffer );
		CCSV csv( strBuffer );

		if ( csv.GetInt(0) == 1 )
		{
			bLines = csv.GetBool(1);
			bMarked = csv.GetBool(2);
		}
	}

	while( TRUE )
	{
		CStockImportFilterPrintDlg dlg( this );

		dlg.m_bLines = bLines;
		dlg.m_bMarked = bMarked;
	
		if ( dlg.DoModal() != IDOK )
			return;

		bLines = dlg.m_bLines;
		bMarked = dlg.m_bMarked;
		
		CReportFile ReportFile;
		if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		{
			Prompter.Error( "Unable to create report" );
			continue;
		}
	
		ReportFile.SetStyle1 ( m_strTitle );
		
		CCSV csvColumns;
		AddReportColumn( ReportFile, csvColumns, "Code", TA_LEFT, 380 );
		AddReportColumn( ReportFile, csvColumns, "Description", TA_LEFT, 300 );
		AddReportColumn( ReportFile, csvColumns, "Category", TA_LEFT, 300 );
		AddReportColumn( ReportFile, csvColumns, "Current Level", TA_RIGHT, 300 );
		AddReportColumn( ReportFile, csvColumns, "Imported", TA_RIGHT, 300 );
		AddReportColumn( ReportFile, csvColumns, "New Level", TA_RIGHT, 300 );
		AddReportColumn( ReportFile, csvColumns, "Import", TA_LEFT, 200 );
				
		for ( int nIdx = 0; nIdx < m_arrayImport.GetSize(); nIdx++ )
		{
			CStockImportFilterItem item;
			m_arrayImport.GetItem( nIdx, item );

			if ( bMarked == 0 )
				if ( FALSE == item.m_bSelected )
					continue;

			CStockCSVRecord StockRecord;
			CStockLevelsCSVRecord StockLevels;	
			DataManager.Stock.GetAt ( item.m_nStockIdx, StockRecord );
			
			double dNewQty = 0.0;

			switch( m_nAction )
			{
			case SUBTRACT_STOCK_LEVELS:
				dNewQty = item.m_dCurrentSUQty - item.m_dImportSUQty;
				break;

			case OVERWRITE_STOCK_LEVELS:
				dNewQty = item.m_dImportSUQty;
				break;

			case ADD_STOCK_LEVELS:
			default:
				dNewQty = item.m_dCurrentSUQty + item.m_dImportSUQty;
				break;
			}

			CString strCurrent = DataManager.Stock.GetStockLevelString( StockRecord, item.m_dCurrentSUQty );
			CString strImport = DataManager.Stock.GetStockLevelString( StockRecord, item.m_dImportSUQty );
			CString strNew = DataManager.Stock.GetStockLevelString( StockRecord, dNewQty );
		
			CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );

			CReportHelpers ReportHelpers;
			csv.Add ( ReportHelpers.GetDisplayStockCode ( StockRecord.GetStockCode() ) );
			csv.Add ( StockRecord.GetDescription() );		
			csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
			csv.Add ( strCurrent );
			csv.Add ( strImport );
			csv.Add ( strNew );

			if ( TRUE == item.m_bSelected )
				csv.Add( "Yes" );
				
			ReportFile.WriteLine ( csv.GetLine() );

			if ( bLines != 0 )
				ReportFile.WriteLine( "<LI>" );
		}
		ReportFile.Close();
	
		CReportManager ReportManager;
		CFilenameUpdater FnUp ( SysFiles::StockImportPrm );
		CString strKey = csvColumns.GetLine();
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, this );
	}
}

/**********************************************************************/

void CStockImportFilterListDlg::AddReportColumn( CReportFile& ReportFile, CCSV& csv, const char* szType, UINT nAlign, int nWidth )
{
	ReportFile.AddColumn( szType, nAlign, nWidth );
	csv.Add( szType );
}

/**********************************************************************/

void CStockImportFilterListDlg::OnCancel()
{
	if ( m_arrayImport.IsSelectionModified() == FALSE )
		EndDialog( IDOK );
	else
	{
		CStockImportFilterCloseDlg dlg( m_strTitle, this );
		
		dlg.DoModal();

		switch( dlg.m_nAction )
		{
		case STOCKIMPORT_YES:
			EndDialog( IDOK );
			break;

		case STOCKIMPORT_NO:
			EndDialog( IDCANCEL );
			break;

		case STOCKIMPORT_CANCEL:
		default:
			break;
		}
	}
}

/**********************************************************************/
