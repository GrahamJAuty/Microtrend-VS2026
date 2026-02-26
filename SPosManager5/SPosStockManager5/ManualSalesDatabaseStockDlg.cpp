/**********************************************************************/
#include "Append.h"
#include "DeliveryCloseDlg.h"
#include "FileRemover.h"
#include "ManualSalesEditStockDlg.h"
#include "NodeTypes.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "Resource.h"
#include "SalesHistoryFields.h"
#include "SalesTypes.h"
#include "StockAuditExceptions.h"
#include "StockAuditSales.h"
#include "StockImportBasicWrapperSalesDlg.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "ManualSalesDatabaseStockDlg.h"
/**********************************************************************/

CManualSalesDatabaseStockDlg::CManualSalesDatabaseStockDlg( int nSpIdx, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
	: CDialog(CManualSalesDatabaseStockDlg::IDD, pParent),
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CManualSalesDatabaseStockDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	

	m_pStocktake = pStocktake;
	m_nStkTakeNo = ( pStocktake != NULL ) ? pStocktake -> GetStocktakeNo() : -1;

	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_bInitDialog = FALSE;

	m_strPreviousSession = "";

	m_nSpIdx = nSpIdx;
	m_nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );
	m_nCacheIndex = -1;
	
	m_strTitle.Format ( "Manual Stock Sales (%s)",
		dbStockpoint.GetName( m_nSpIdx ) );
		
	m_pManualStockSalesEditDlg = NULL;
	m_bSaveChanges = FALSE;
}	
	
/**********************************************************************/

CManualSalesDatabaseStockDlg::~CManualSalesDatabaseStockDlg()
{
	dbStockpoint.RemoveTemporaryAudits( m_nSpIdx );

	if ( m_pManualStockSalesEditDlg != NULL )
		delete m_pManualStockSalesEditDlg;
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualSalesDatabaseStockDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CManualSalesDatabaseStockDlg, CDialog)
	//{{AFX_MSG_MAP(CManualSalesDatabaseStockDlg)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_COMMAND(IDM_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM_COLUMN_DEFAULT, OnColumnDefault)
	ON_COMMAND(IDM_ADD, OnAdd)
	ON_COMMAND(IDM_PREVIEW, OnPreview)
	ON_COMMAND(IDM_DELETE, OnDelete)
	ON_COMMAND(IDM_EDIT, OnEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_EXIT, OnCancel)
	ON_COMMAND(IDM_FILE_EXIT, OnCancel)
	ON_COMMAND(IDM_IMPORT_WRAPPER, OnImportWrapper)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_USER_BUTTON, OnUserButton )
	ON_MESSAGE ( WM_APP, OnProcessReturn)
	ON_MESSAGE ( WM_APP + 1, OnInitialAddItem)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CManualSalesDatabaseStockDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );

	m_listStock.SubclassDlgItem ( IDC_LIST, this );

	m_ColumnManager.Reset( TableNames::Returns );
	m_ColumnManager.AddColumn( ColumnNames::PluNo, "Plu No", LVCFMT_LEFT, 110, 110 );
	m_ColumnManager.AddColumn( ColumnNames::Reference, "Supp Ref", LVCFMT_LEFT, 80, 100 );
	m_ColumnManager.AddColumn( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 115, 115 );
	m_ColumnManager.AddColumn( ColumnNames::Desc, "Description", LVCFMT_LEFT, 130, 130 );
	m_ColumnManager.AddColumn( ColumnNames::DUQty, "DU Qty", LVCFMT_RIGHT, 50, 80 );
	m_ColumnManager.AddColumn( ColumnNames::Yield, "Yield", LVCFMT_RIGHT, 60, 90 );
	m_ColumnManager.AddColumn( ColumnNames::SUQty, "SU Qty", LVCFMT_RIGHT, 50, 80 );
	m_ColumnManager.AddColumn( ColumnNames::Cost, "Cost", LVCFMT_RIGHT, 50, 60 );
	m_ColumnManager.LoadSettings();
	
	m_bInitDialog = TRUE;
	m_ColumnManager.RedrawList( &m_listStock, this );
	CenterWindow();
	
	CMenuChanger MenuChanger;
	MenuChanger.EatMenuTopLevel( GetMenu() );
	MenuChanger.KillItem( IDM_IMPORT );

	if ( StockOptions.GetImportSalesWrapperFlag() == FALSE )
		MenuChanger.KillParent( IDM_IMPORT );

	MenuChanger.BuildMenu ( GetMenu() );
	DrawMenuBar();

	if ( m_strPreviousSession != "" )
	{
		CString strFilename = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_MANUALSALE_STOCK );
		strFilename += "\\";
		strFilename += m_strPreviousSession;

		CSSFile filePrevious;
		if ( filePrevious.Open( strFilename, "rb" ) == TRUE )
		{
			bool bFirstLine = TRUE;
			CString strBuffer;
			
			while( filePrevious.ReadString( strBuffer ) == TRUE )
			{
				CCSV csv ( strBuffer );
				
				if ( FALSE == bFirstLine )
					m_arraySales.Add( strBuffer );
				else if ( csv.GetInt(0) != 1 )
					break;

				bFirstLine = FALSE;
			}
		}

		m_listStock.SetItemCountEx( m_arraySales.GetSize() );
	}

	DisplayScreen ( 0 );

	PostMessage ( WM_APP + 1, 0, 0 );

	return TRUE;
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listStock.MoveWindow ( 0,0,cx,cy );
	}
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::DisplayScreen ( int nIndex )
{
	int nSize = m_arraySales.GetSize();
	
	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listStock.SetCurSel ( nIndex );
	else
		m_listStock.SetCurSel ( nSize - 1 );

	SetWindowText( m_strTitle );
	m_listStock.Invalidate();
}

/**********************************************************************/

const char* CManualSalesDatabaseStockDlg::GetDisplayLine( int nIndex )
{
	m_strDisplayLine = "";
	if ( nIndex >= 0 && nIndex < m_arraySales.GetSize() )
	{
		CCSVMiscStockLevels csvIn ( m_arraySales.GetAt( nIndex ) );
		CCSV csvOut ( '\t' );

		if ( csvIn.GetItemLookupMethod() != ENTRY_MODE_WRAPPER )
		{
			csvOut.Add( csvIn.GetPluNoString() );	
			csvOut.Add( csvIn.GetSuppRef() );
		}
		else
		{
			csvOut.Add( "WRAPPER" );
			csvOut.Add( csvIn.GetWrapperCode() );
		}

		csvOut.Add( csvIn.GetStockCode() );
		csvOut.Add( csvIn.GetDescription() );
	
		if ( csvIn.GetUseDUFlag() == TRUE )
		{
			csvOut.Add ( csvIn.GetDUQty(), 3 );

			CString strYield;
			strYield.Format ( "%d x %.3f", csvIn.GetDUItems(), csvIn.GetDUItemSize() );
			csvOut.Add ( strYield );
		}
		else
		{
			csvOut.Add ( "" );
			csvOut.Add ( "" );
		}

		csvOut.Add ( csvIn.GetSUQty(), SysInfo.GetDPQty() );
		csvOut.Add ( csvIn.GetTotalCost(), SysInfo.GetDPValue() );

		m_strDisplayLine = csvOut.GetLine();
	}
	return m_strDisplayLine;
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ( nIndex != m_nCacheIndex  )
		{
			m_CacheCSV.ParseLine( GetDisplayLine ( nIndex ) );
			m_nCacheIndex = nIndex;
		}

		lstrcpy ( pDispInfo->item.pszText, m_CacheCSV.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEdit();
	*pResult = 0;
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CManualSalesDatabaseStockDlg::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	return 0L;
}

/**********************************************************************/

long CManualSalesDatabaseStockDlg::OnProcessReturn( WPARAM wIndex, LPARAM lParam )
{
	CManualSalesEditStockDlg* pDlg = (CManualSalesEditStockDlg*) lParam;
	m_arraySales.Add( pDlg -> m_strOutputLine );
	m_listStock.SetItemCountEx( m_arraySales.GetSize() );
	m_listStock.SetCurSel( m_arraySales.GetSize() - 1 );
	m_listStock.Invalidate();
	return 0L;
}
	
/**********************************************************************/

long CManualSalesDatabaseStockDlg::OnInitialAddItem( WPARAM wIndex, LPARAM lParam )
{
	OnAdd();
	return 0L;
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listStock, this );
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnAdd() 
{
	if ( m_pManualStockSalesEditDlg == NULL )
	{
		m_pManualStockSalesEditDlg = new CManualSalesEditStockDlg( m_nSpIdx, m_SuppRefFinder, &m_pManualStockSalesEditDlg, m_nStkTakeNo, this );
		m_pManualStockSalesEditDlg -> Create( IDD_MANUAL_SALES_EDIT_STOCK, this );
	}
	
	m_pManualStockSalesEditDlg -> SetFocus();
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnImportWrapper() 
{
	CString strTitle = "Import Sales by Wrapper Code";
	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );

	CString strImportFile = StockOptions.GetImportSalesWrpName();
	if ( ( strImportFile.GetLength() == 0 ) || ( strImportFile.GetLength() > 25 ) )
		strImportFile = "sales";

	CFileDialog dlgFile ( TRUE, "csv", strImportFile, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFile.m_ofn.lpstrTitle = strTitle;
	dlgFile.m_ofn.lpstrInitialDir = strPath;
	
	if ( dlgFile.DoModal() == IDCANCEL )
		return;

	CString strImportPath = dlgFile.GetPathName();
	
	for ( int nIndex = 0; nIndex < m_ImportFilesArray.GetSize(); nIndex++ )
	{
		if ( strImportPath == m_ImportFilesArray.GetAt ( nIndex ) )
		{
			Prompter.Error ( "You have already imported the selected file." );
			return;
		}
	}
	
	m_SuppRefFinder.Rebuild( 0 );

	CStockImportBasicWrapperSalesDlg dlgImport ( m_nSpIdx, strImportPath, m_SuppRefFinder, m_pStocktake, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No sales data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing sales" );
	ImportWrapperInternal ( strImportPath, dlgImport.GetAction() );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::ImportWrapperInternal( CString& strImportPath, int nAction )
{
	CSSFile fileImport;
	if ( fileImport.Open ( strImportPath, "rb" ) == FALSE )
		return;
			
	CString strBuffer;
	int nLines = 0;
	while ( fileImport.ReadString ( strBuffer ) == TRUE )  
	{
		StatusProgress.SetPos ( fileImport );
		
		CCSV csvIn ( strBuffer );
		CString strWrapper = csvIn.GetString ( 0 );
		double dDUQty = csvIn.GetDouble( 1 );
		double dTotalRetail = csvIn.GetDouble( 2 );

		int nStockIdx, nStockSuppIdx;
		if ( m_SuppRefFinder.FindItemByWrapper( strWrapper, nStockIdx, nStockSuppIdx ) == FALSE )
			continue;
		
		CStockCSVRecord StockRecord;
		CStockLevelsCSVRecord StockLevels;
		if ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, m_nSpNo, m_nStkTakeNo ) != STOCKLEVEL_IMPORT_ACCEPT )
			continue;

		CString strStockCode = StockRecord.GetStockCode();
		CString strSuppRef = StockRecord.GetSuppRefExternal ( nStockSuppIdx, DealerFlags.GetSuppRefFlag() );

		double dRetailPerDU = 0.0;

		switch( DealerFlags.GetPluCSVImportType() )
		{
		case 0:
		case 2:
			dRetailPerDU = ( dDUQty != 0.0 ) ? dTotalRetail / dDUQty : 0.0;
			break;

		default:
			dRetailPerDU = StockRecord.GetRetail( nStockSuppIdx );
			dTotalRetail = dRetailPerDU * dDUQty;
			break;
		}

		double dSUQty = dDUQty * StockRecord.GetYield( nStockSuppIdx );
		int nDUItems = StockRecord.GetDUItems( nStockSuppIdx );
		double dDUItemSize = StockRecord.GetDUItemSize( nStockSuppIdx );

		if ( nAction == SUBTRACT_STOCK_LEVELS )
		{
			dDUQty = -dDUQty;
			dSUQty = -dSUQty;
			dTotalRetail = -dTotalRetail;
		}

		CCSVStockAudit csvOut;
		csvOut.Add ( "" );
		csvOut.Add ( strSuppRef );
		csvOut.Add ( strStockCode );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( FALSE );				//FREE STOCK
		csvOut.Add ( TRUE );				//USE DU FLAG
		csvOut.AddStockAuditQty ( dDUQty );	//DU QTY
		csvOut.Add ( nDUItems );				//ITEMS PER DU
		csvOut.AddStockAuditQty ( dDUItemSize);	//DU ITEM SIZE
		csvOut.AddStockAuditQty ( dSUQty );
		csvOut.AddStockAuditVal ( dRetailPerDU );	//COST PER DU
		csvOut.AddStockAuditVal ( dTotalRetail );
		csvOut.Add ( ENTRY_MODE_WRAPPER );
		csvOut.Add ( StockRecord.GetSuppNo( nStockSuppIdx ) );
		csvOut.Add ( strWrapper );			//WRAPPER CODE
		csvOut.Add ( SALES_TYPE_IMPORT );
		
		m_arraySales.Add( csvOut.GetLine() );
	}
	fileImport.Close();

	COleDateTime date;
	CString strPrevEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV_EX );
	CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
	::Append ( strPrevEx, strTempEx );
	
	CFileRemover FileRemover;
	FileRemover.RemoveFile ( strPrevEx );

	m_ImportFilesArray.Add ( strImportPath );

	m_listStock.SetItemCountEx( m_arraySales.GetSize() );
	m_listStock.SetCurSel( m_arraySales.GetSize() - 1 );
	m_listStock.Invalidate();
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnPreview() 
{
	COleDateTime date;
	
	if ( NULL == m_pStocktake )
		date = COleDateTime::GetCurrentTime();
	else
		date = m_pStocktake -> GetOleDateTime();

	AuditSales( date, FALSE );

	CStockAuditSales Audit;
	Audit.SetReportType( AUDIT_SALES );
	Audit.SetConLevel( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_TEMP );
	Audit.SetManualSalesFlag( TRUE );
	Audit.SetImportSalesFlag( TRUE );
	Audit.SetE1SalesFlag( FALSE );
	Audit.SetHidePluNoFlag( TRUE );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::AuditSales( COleDateTime date, bool bUpdateStock )
{
	StatusProgress.Lock( TRUE, "Auditing sales" ); 
	AuditSalesInternal ( date, bUpdateStock );
	StatusProgress.Unlock(); 
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::AuditSalesInternal( COleDateTime date, bool bUpdateStock )
{
	CSSFile fileTemp;
	CString strPath = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
	if ( fileTemp.Open ( strPath, "wb" ) == FALSE )
		return;

	bool bWrittenHeader = FALSE;
	CStockAuditSales Audit;
	Audit.SetReportType( AUDIT_SALES );

	CStockAuditHeaderInfo info;
	info.m_date = date;
	CString strHeader = Audit.GetHeaderLine ( info );
	
	CString strHistoryMonth, strTime;
	strHistoryMonth = date.Format ( "%Y%m" );
	strTime = date.Format ( "%H%M" );
	int nMDay = date.GetDay();

	CString strHistoryFolder = dbStockpoint.GetFolderPathHistory( m_nSpIdx );
	::MakeSubdirectory ( strHistoryFolder );
		
	CString strHistoryFile;
	strHistoryFile.Format ( "%s\\St%s.002",
		(const char*) strHistoryFolder,
		(const char*) strHistoryMonth );

	CSSFile fileHistory;
	if ( bUpdateStock )	
		if ( fileHistory.Open ( strHistoryFile, "ab" ) == FALSE )
			return;
	
	int nCount = m_arraySales.GetSize();
	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, nCount );
		CString strSale = m_arraySales.GetAt ( nIndex );
		CCSVMiscStockLevels csvSale ( strSale );
		
		int nStockIdx;
		CString strStockCode = csvSale.GetStockCode();
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
			continue;

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );
		int nSubUnits = DataManager.Stock.GetSubUnits( StockRecord );

		int nSalesType = csvSale.GetSalesType();
		int nAuditSalesType = ( SALES_TYPE_IMPORT == nSalesType ) ? SALES_TYPE_IMPORT_STOCK : SALES_TYPE_MANUAL_STOCK;
		int nStockSuppIdx = csvSale.GetStockSuppIdx();

		if ( bWrittenHeader == FALSE )
		{
			fileTemp.WriteLine( strHeader );
			bWrittenHeader = TRUE;
		}

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_SALES );
		csvOut.Add ( 3 );
		csvOut.Add ( nAuditSalesType );			
		csvOut.Add ( strStockCode );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( ( -1 == nStockSuppIdx ) ? 0 : 1 );

		if ( -1 == nStockSuppIdx )
		{
			csvOut.Add( "" );
			csvOut.Add( "" );
			csvOut.Add( "" );
			csvOut.Add( "" );
			csvOut.Add( "" );
		}
		else
		{
			csvOut.Add( StockRecord.GetCase( nStockSuppIdx ) );

			if ( csvSale.GetItemLookupMethod() == ENTRY_MODE_WRAPPER )
				csvOut.Add( csvSale.GetWrapperCode() );
			else
				csvOut.Add( csvSale.GetSuppRef() );

			csvOut.AddStockAuditQty( StockRecord.GetYield( nStockSuppIdx ) );
			csvOut.AddStockAuditQty( csvSale.GetDUQty() );
			csvOut.AddStockAuditVal( csvSale.GetDUCost() );
		}

		csvOut.Add ( nSubUnits );
		csvOut.AddStockAuditQty ( csvSale.GetSUQty() );		
		csvOut.AddStockAuditVal ( csvSale.GetTotalCost() );
		fileTemp.WriteLine ( csvOut.GetLine() );
				
		if ( bUpdateStock )
		{		
			csvOut.RemoveAll();
			csvOut.Add ( nMDay );
			csvOut.Add ( strTime );
			csvOut.Add ( csvSale.GetStockCode() );
			csvOut.Add ( nSalesType );
			csvOut.AddStockAuditQty ( csvSale.GetSUQty() );
			csvOut.AddStockAuditVal ( csvSale.GetTotalCost() );
			csvOut.Add ( DataManager.Stock.GetCategory(nStockIdx) );
			csvOut.Add ( "" );
			csvOut.Add ( 0 );
			fileHistory.WriteLine ( csvOut.GetLine() );
				
			double dStockQty = csvSale.GetSUQty();
			double dStockValue = csvSale.GetTotalCost();

			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt ( nStockIdx, StockRecord );
						
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, m_nSpNo, StockLevels );					
	
			if ( nAuditSalesType == SALES_TYPE_IMPORT_STOCK )
				StockLevels.AddImportSales ( dStockQty, dStockValue, ( m_pStocktake != NULL ) );
			else
				StockLevels.AddManualSales ( dStockQty, dStockValue, ( m_pStocktake != NULL ) );

			DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, m_nSpNo, StockLevels );
			m_bSaveChanges = TRUE;
		}
	}
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnEdit() 
{
	int nSel = m_listStock.GetCurSel();
	if ( nSel >= 0 && nSel < m_arraySales.GetSize() )
	{
		bool bFromAdd = FALSE;
		if ( m_pManualStockSalesEditDlg != NULL )
		{
			 delete m_pManualStockSalesEditDlg;
			 m_pManualStockSalesEditDlg = NULL;
			 bFromAdd = TRUE;
		}
		  
		CManualSalesEditStockDlg dlg ( m_nSpIdx, m_SuppRefFinder, m_nStkTakeNo, this );
		dlg.m_strInputLine = m_arraySales.GetAt( nSel );

		if ( dlg.DoModal() == IDOK )
		{
			m_arraySales.SetAt( nSel, dlg.m_strOutputLine );
			m_listStock.Invalidate();
		}

		if ( bFromAdd == TRUE )
			OnAdd();
	}
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnDelete() 
{
	int nSel = m_listStock.GetCurSel();
	if ( nSel >= 0 && nSel < m_arraySales.GetSize() )
	{
		if ( Prompter.YesNo ( "Are you sure you wish to delete this sale" ) == IDYES )
		{
			m_arraySales.RemoveAt( nSel );
			m_listStock.SetItemCountEx( m_arraySales.GetSize() );

			if ( nSel < m_listStock.GetItemCount() )
				m_listStock.SetCurSel ( nSel );
			else
				m_listStock.SetCurSel ( nSel - 1 );

			m_listStock.Invalidate();
		}
	}
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//NOTHING TO DO IF NO LINES IN LIST
	if ( m_arraySales.GetSize() == 0 )
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
	VERIFY(menu.LoadMenu(IDR_POPUP_STOCK_LIST));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnCancel()
{
	if ( m_arraySales.GetSize() == 0 )
	{
		EndDialog ( IDOK );
		return;
	}

	CDeliveryCloseDlg dlg ( NODE_MANUALSALE_STOCK, ( m_pStocktake != NULL ), this );
	dlg.DoModal();
		
	switch ( dlg.m_nAction )
	{
	case SESSION_FINALISE:
		{
			DeleteSessionFile();
			DeleteImportFiles();

			COleDateTime date;
	
			if ( NULL == m_pStocktake )
				date = COleDateTime::GetCurrentTime();
			else
				date = m_pStocktake -> GetOleDateTime();

			AuditSales ( date, TRUE );

			CString strAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_NORMAL );
			CString strTempAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
			CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
			
			::Append ( strTempAudit, strAudit );
		
			CSSFile fileAudit;
			if ( fileAudit.Open ( strAudit, "ab" ) == TRUE )
			{
				CStockAuditExceptions Audit( AUDIT_EX_SALES );
				
				CStockAuditHeaderInfo info;
				info.m_date = date;
				fileAudit.WriteLine ( Audit.GetHeaderLine( info ) );

				fileAudit.Close();
				::Append ( strTempEx, strAudit );
			}
			
			EndDialog( IDOK );
		}
		break;

	case SESSION_DISCARD:
		{
			if ( "" == m_strPreviousSession )
				WriteSessionFile( FALSE );
			
			EndDialog ( IDCANCEL );
		}
		break;

	case SESSION_ABANDON:
		{
			DeleteSessionFile();
			EndDialog ( IDCANCEL );
		}
		break;

	case SESSION_FILE:
		{
			DeleteImportFiles();
			WriteSessionFile( TRUE );
			EndDialog( IDCANCEL );
		}
		break;

	case SESSION_CONTINUE:
	default:
		return;
	}
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::DeleteImportFiles()
{
	for ( int nIndex = 0; nIndex < m_ImportFilesArray.GetSize(); nIndex++ )
	{
		CFileRemover FileRemover;
		CString strFile = m_ImportFilesArray.GetAt( nIndex );
		FileRemover.RemoveFile( strFile );
	}
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::DeleteSessionFile()
{		
	if (  "" != m_strPreviousSession ) 
	{
		CString strFolder = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_MANUALSALE_STOCK );
		strFolder += "\\";
		CFileRemover FileRemover( strFolder + m_strPreviousSession );
	}
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::WriteSessionFile( bool bDetail )
{
	::CreateSubdirectory( dbStockpoint.GetFolderPathSession( m_nSpIdx, -1 ) );
	::CreateSubdirectory( dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_MANUALSALE_STOCK ) );
			
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strFilename = m_strPreviousSession;
			
	if ( "" == strFilename )
	{
		strFilename.Format( "%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond() );
	}

	CString strFolder = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_MANUALSALE_STOCK );
	strFolder += "\\";

	CSSFile fileSession;
	if ( fileSession.Open( strFolder + strFilename, "wb" ) == TRUE )
	{
		CCSV csv;
		csv.Add( 1 );
		fileSession.WriteLine( csv.GetLine() );

		if ( TRUE == bDetail )
		{
			for ( int n = 0; n < m_arraySales.GetSize(); n++ )
				fileSession.WriteLine( m_arraySales.GetAt(n) );
		}
	}
}

/**********************************************************************/

void CManualSalesDatabaseStockDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/
