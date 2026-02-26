/**********************************************************************/
#include "Append.h"
#include "DeliveryCloseDlg.h"
#include "ExplodePlu.h"
#include "FileRemover.h"
#include "NodeTypes.h"
#include "PluSalesSorterNonEcr.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "ReportHelpers.h"
#include "ReportPluInfoFinder.h"
#include "Resource.h"
#include "SalesHistoryFields.h"
#include "SalesTypes.h"
#include "StatusProgress.h"
#include "StockAuditExceptionsSales.h"
#include "StockAuditSales.h"
#include "StockImportBasicSalesDlg.h"
#include "StockpointCSVArray.h"
#include "StockOptionsIni.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "ManualSalesDatabasePluDlg.h"
/**********************************************************************/

CManualSalesDatabasePluDlg::CManualSalesDatabasePluDlg( int nSpIdx, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
	: CDialog(CManualSalesDatabasePluDlg::IDD, pParent),
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CManualSalesDatabasePluDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pStocktake = pStocktake;
	m_nStkTakeNo = ( pStocktake != NULL ) ? pStocktake -> GetStocktakeNo() : -1;

	m_nCacheIndex = -1;

	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_bInitDialog = FALSE;

	m_strPreviousSession = "";
	m_strAutoImportFile = "";

	m_nSpIdx = nSpIdx;
	m_nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );

	m_strTitle.Format ( "Manual Sales (%s)",
		dbStockpoint.GetName( m_nSpIdx ) );
		
	m_bSaveChanges = FALSE;

	m_pManualSalesEditDlg = NULL;
}	
	
/**********************************************************************/

CManualSalesDatabasePluDlg::~CManualSalesDatabasePluDlg()
{
	dbStockpoint.RemoveTemporaryAudits( m_nSpIdx );

	if ( m_pManualSalesEditDlg != NULL )
		delete m_pManualSalesEditDlg;
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualSalesDatabasePluDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CManualSalesDatabasePluDlg, CDialog)
	//{{AFX_MSG_MAP(CManualSalesDatabasePluDlg)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_COMMAND(IDM_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM_COLUMN_DEFAULT, OnColumnDefault)
	ON_COMMAND(IDM_IMPORT, OnImport)
	ON_COMMAND(IDM_ADD, OnAdd)
	ON_COMMAND(IDM_PREVIEW, OnPreview)
	ON_COMMAND(IDM_DELETE, OnDelete)
	ON_COMMAND(IDM_EDIT, OnEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_EXIT, OnCancel)
	ON_COMMAND(IDM_FILE_EXIT, OnCancel)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_USER_BUTTON, OnUserButton )
	ON_MESSAGE ( WM_APP, OnProcessSalesItem)
	ON_MESSAGE ( WM_APP + 1, OnInitialAddItem)
	ON_MESSAGE ( WM_APP + 2, OnInitialImportPlu)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CManualSalesDatabasePluDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );

	m_listStock.SubclassDlgItem ( IDC_LIST, this );

	m_ColumnManager.Reset( TableNames::ManualSales );
	m_ColumnManager.AddColumn( ColumnNames::PluNo, "Plu No", LVCFMT_LEFT, 110, 110 );
	m_ColumnManager.AddColumn( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 110, 110 );
	m_ColumnManager.AddColumn( ColumnNames::Desc, "Description", LVCFMT_LEFT, 160, 160 );
	m_ColumnManager.AddColumn ( ColumnNames::Dept, "Dept", LVCFMT_LEFT, 50, 50 );					
	m_ColumnManager.AddColumn( ColumnNames::PluQty, "Qty", LVCFMT_RIGHT, 80, 80 );
	m_ColumnManager.AddColumn( ColumnNames::PluValue, "Value", LVCFMT_RIGHT, 80, 80 );
	m_ColumnManager.LoadSettings();
	
	m_bInitDialog = TRUE;
	m_ColumnManager.RedrawList( &m_listStock, this );
	CenterWindow();

	CMenuChanger MenuChanger;
	MenuChanger.EatMenuTopLevel( GetMenu() );
	MenuChanger.KillItem( IDM_IMPORT_WRAPPER );

	if ( StockOptions.GetImportSalesPluFlag() == FALSE )
		MenuChanger.KillParent( IDM_IMPORT_WRAPPER );

	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		MenuChanger.SetItemText( IDM_IMPORT, "By &Barcode Number" );
		
	MenuChanger.BuildMenu ( GetMenu() );
	DrawMenuBar();

	if ( m_strPreviousSession != "" )
	{
		CString strFilename = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_MANUALSALE_PLU );
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

	if ( NULL == m_pStocktake )
	{
		if ( ( StockOptions.GetImportSalesPluFlag() == TRUE ) && ( m_strAutoImportFile != "" ) )
			PostMessage( WM_APP + 2, 0, 0 );
		else
			PostMessage( WM_APP + 1, 0, 0 );
	}
	else
		PostMessage( WM_APP + 1, 0, 0 );

	return TRUE;
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listStock.MoveWindow ( 0,0,cx,cy );
	}
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::DisplayScreen ( int nIndex )
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

const char* CManualSalesDatabasePluDlg::GetDisplayLine( int nIndex )
{
	m_strDisplayLine = "";
	if ( nIndex >= 0 && nIndex < m_arraySales.GetSize() )
	{
		CCSVManualSales csvIn ( m_arraySales.GetAt( nIndex ) );
		
		CCSV csvOut ( '\t' );
		csvOut.Add ( ReportHelpers.GetDisplayPluNo( csvIn.GetPluNo() ) );
		csvOut.Add ( csvIn.GetStockCode() );
		csvOut.Add ( csvIn.GetDescription() );
		csvOut.Add ( csvIn.GetDeptNo() );
		csvOut.Add ( csvIn.GetTotalQty(), SysInfo.GetDPQty() );
		csvOut.Add ( csvIn.GetValue(), SysInfo.GetDPValue() );

		m_strDisplayLine = csvOut.GetLine();
	}
	return m_strDisplayLine;
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CManualSalesDatabasePluDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEdit();
	*pResult = 0;
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CManualSalesDatabasePluDlg::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	return 0L;
}

/**********************************************************************/

long CManualSalesDatabasePluDlg::OnProcessSalesItem( WPARAM wIndex, LPARAM lParam )
{

#ifdef STOCKMAN_SYSTEM
	CManualSalesEditPluDlg* pDlg = (CManualSalesEditPluDlg*) lParam;
#else
	CManualSalesEditDlgEcrman* pDlg = (CManualSalesEditDlgEcrman*) lParam;
#endif

	m_arraySales.Add( pDlg -> m_strOutputLine );
	m_listStock.SetItemCountEx( m_arraySales.GetSize() );
	m_listStock.SetCurSel( m_arraySales.GetSize() - 1 );
	m_listStock.Invalidate();
	return 0L;
}

/**********************************************************************/

long CManualSalesDatabasePluDlg::OnInitialAddItem( WPARAM wIndex, LPARAM lParam )
{
	OnAdd();
	return 0L;
}
	
/**********************************************************************/

void CManualSalesDatabasePluDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listStock, this );
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}


/**********************************************************************/

void CManualSalesDatabasePluDlg::OnAdd() 
{
	if ( m_pManualSalesEditDlg == NULL )
	{
		m_pManualSalesEditDlg = new CManualSalesEditPluDlg( m_nSpIdx, dbStockpoint.GetName(m_nSpIdx), &m_pManualSalesEditDlg, m_nStkTakeNo, this );
		m_pManualSalesEditDlg -> Create( IDD_MANUAL_SALES_EDIT_PLU, this );
	}
	
	m_pManualSalesEditDlg -> SetFocus();
}

/**********************************************************************/

long CManualSalesDatabasePluDlg::OnInitialImportPlu( WPARAM wIndex, LPARAM lParam )
{
	InitialImportPlu();
	return 0L;
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::InitialImportPlu()
{
	CString strMsg = "";
	strMsg += "Stock Manager has detected a pending file\n";
	strMsg += "of sales for this stockpoint.\n\n";
	strMsg += "Would you like to import this file now";

	if ( Prompter.YesNo( strMsg, FALSE ) != IDYES )
		return;

	int nLineType = DealerFlags.GetPluCSVImportType();
	CStockImportBasicSalesDlg dlgImport( m_nSpIdx, m_strAutoImportFile, nLineType, NULL, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No sales data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	int nAction = dlgImport.GetAction();

	StatusProgress.Lock( TRUE, "Importing sales" );
	ImportPluInternal ( m_strAutoImportFile, nAction, TRUE );
	StatusProgress.Unlock();

	WriteSessionFile( TRUE, TRUE );
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::OnImport() 
{
	CString strTitle;
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		strTitle = "Import Sales by Barcode Number";
	else
		strTitle = "Import Sales by Plu Number";
	
	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );

	CString strImportFile = StockOptions.GetImportSalesPluName();
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

	int nLineType = DealerFlags.GetPluCSVImportType();
	CString strFilename = dlgFile.GetFileName();
	strFilename.MakeUpper();
	
	CStockImportBasicSalesDlg dlgImport ( m_nSpIdx, strImportPath, nLineType, m_pStocktake, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No sales data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	int nAction = dlgImport.GetAction();

	StatusProgress.Lock( TRUE, "Importing sales" );	
	ImportPluInternal( strImportPath, nAction, FALSE );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::ImportPluInternal( CString& strImportPath, int nAction, bool bAutoImport )
{
	int nLineType = DealerFlags.GetPluCSVImportType();
	
	CSSFile fileImport;	
	if ( fileImport.Open ( strImportPath, "rb" ) == FALSE )
		return;
			
	CString strBuffer;
	int nLines = 0;
	
	while ( fileImport.ReadString ( strBuffer ) == TRUE )  
	{
		StatusProgress.SetPos ( fileImport );

		CCSV csv ( strBuffer );

		if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		{
			CString strPluNo;
			__int64 nBarcodeNo = _atoi64( csv.GetString(0) );
			__int64 nPluNo = DataManager.Barcode.GetPluFromBarcode( nBarcodeNo ); 
			::FormatPluNo( nPluNo, strPluNo );
			csv.SetAt( 0, strPluNo );
		}
		
		CReportPluInfoFinder PluInfoFinder( csv, nLineType );
		if ( PluInfoFinder.PluExists() == FALSE )
			continue;

		__int64 nPluNo = csv.GetInt64 ( 0 );
		int nLevel = PluInfoFinder.GetImportLineLevel();
		double dQty = PluInfoFinder.GetImportLineQty();
		double dValue = PluInfoFinder.GetImportLineVal();

		if ( nAction == SUBTRACT_STOCK_LEVELS )
		{
			dQty = -dQty;
			dValue = -dValue;
		}

		CExplodePluStockTake ExplodePlu( m_nSpNo, m_nStkTakeNo );
		
		CStringArray StockDetails;
		if ( ExplodePlu.ExplodePlu ( nPluNo, StockDetails, AUDIT_SALES, dQty, dValue ) == PLU_EXCEPTION_NONE )
		{
			PluInfoFinder.SetPluNo( nPluNo );
			CCSVExplodedPlu csvItem ( StockDetails.GetAt ( 0 ) );

			CCSVStockAudit csv;
			csv.Add ( SALES_TYPE_IMPORT );
			csv.Add ( nPluNo );
			csv.Add ( csvItem.GetDescription() );
			csv.Add ( nLevel );
			csv.AddStockAuditQty ( dQty );
			csv.AddStockAuditVal ( dValue );
			csv.Add ( PluInfoFinder.GetModifierDeptNo(0) );
			csv.Add ( ENTRY_MODE_PLUNO );

			if ( StockDetails.GetSize() == 1 )
				csv.Add ( csvItem.GetStockCode() );					
			else
				csv.Add ( "" );
			
			csv.Add ( 0 );					//MODIFIER
			csv.Add ( 1 );					//MULTIPLIER
			csv.AddStockAuditQty ( dQty );	//TOTAL QTY
		
			m_arraySales.Add ( csv.GetLine() );
		}
	}
	fileImport.Close();

	COleDateTime date;
	CString strPrevEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV_EX );
	CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
	::Append ( strPrevEx, strTempEx );
	
	CFileRemover FileRemover ( strPrevEx );

	if ( FALSE == bAutoImport )
		m_ImportFilesArray.Add ( strImportPath );
	else
		FileRemover.RemoveFile( strImportPath );

	m_listStock.SetItemCountEx( m_arraySales.GetSize() );
	m_listStock.SetCurSel( m_arraySales.GetSize() - 1 );
	m_listStock.Invalidate();
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::OnPreview() 
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

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}	
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::AuditSales( COleDateTime date, bool bUpdateStock )
{
	StatusProgress.Lock( TRUE, "Auditing Sales" ); 
	AuditSalesInternal ( date, bUpdateStock );
	StatusProgress.Unlock(); 
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::AuditSalesInternal( COleDateTime date, bool bUpdateStock )
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
	strTime = date.Format ( "%H%M%S" );
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
		CCSVManualSales csvSale ( strSale );
			
		int nSalesType = csvSale.GetSalesType();
		__int64 nPluNo = csvSale.GetPluNo();
		double dSaleQty = csvSale.GetTotalQty();
		double dSaleValue = csvSale.GetValue();
		int nLevel = csvSale.GetPriceLevel();
		
		CExplodePluStockTake ExplodePlu( m_nSpNo, m_nStkTakeNo );
		ExplodePlu.SetPriceLevel( csvSale.GetPriceLevel() );
		
		CStringArray StockDetails;
		if ( ExplodePlu.ExplodePlu ( nPluNo, StockDetails, AUDIT_SALES, dSaleQty, dSaleValue ) == PLU_EXCEPTION_NONE )
		{	
			for ( int nItemIdx = 0; nItemIdx < StockDetails.GetSize(); nItemIdx++ )
			{
				if ( bWrittenHeader == FALSE )
				{
					fileTemp.WriteLine( strHeader );
					bWrittenHeader = TRUE;
				}

				CCSVExplodedPlu csvItem ( StockDetails.GetAt ( nItemIdx ) );
	
				CCSVStockAudit csvOut;
				csvOut.Add ( "D" );
				csvOut.Add ( AUDIT_SALES );
				csvOut.Add ( 1 );
				csvOut.Add ( ( nItemIdx == 0 ) ? nSalesType : SALES_TYPE_RECIPEITEM);
				csvOut.Add ( csvItem.GetPluNo() );
				csvOut.Add ( "" );
				csvOut.Add ( csvItem.GetDescription() );
				csvOut.AddStockAuditQty ( csvItem.GetPluQty() );
				csvOut.AddStockAuditVal ( csvItem.GetPluValue() );
				csvOut.Add ( csvItem.GetStockCode() );
				csvOut.Add ( csvItem.GetSubUnits() );
				csvOut.AddStockAuditQty ( csvItem.GetStockQty() );
				csvOut.AddStockAuditVal ( csvItem.GetStockValue() );
				fileTemp.WriteLine ( csvOut.GetLine() );
				
				if ( bUpdateStock )
				{
					int nStockIdx = csvItem.GetStockIdx();
					
					csvOut.RemoveAll();
					csvOut.Add ( nMDay );
					csvOut.Add ( strTime );
					csvOut.Add ( csvItem.GetStockCode() );
					csvOut.Add ( nSalesType );
					csvOut.AddStockAuditQty ( csvItem.GetStockQty() );
					csvOut.AddStockAuditVal ( csvItem.GetStockValue() );
					csvOut.Add ( DataManager.Stock.GetCategory(nStockIdx) );
					csvOut.Add ( "" );
					csvOut.Add ( csvSale.GetPriceLevel() );
					fileHistory.WriteLine ( csvOut.GetLine() );
				
					double dStockQty = csvItem.GetStockQty();
					double dStockValue = csvItem.GetStockValue();

					CStockCSVRecord StockRecord;
					DataManager.Stock.GetAt ( nStockIdx, StockRecord );
						
					CStockLevelsCSVRecord StockLevels;
					DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, m_nSpNo, StockLevels );					
	
					if ( StockDetails.GetSize() >= 2 )
						StockLevels.AddRecipeSales ( dStockQty, dStockValue, ( m_pStocktake != NULL ) );
					else if ( nSalesType == SALES_TYPE_IMPORT )
						StockLevels.AddImportSales ( dStockQty, dStockValue, ( m_pStocktake != NULL ) );
					else
						StockLevels.AddManualSales ( dStockQty, dStockValue, ( m_pStocktake != NULL ) );

					DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, m_nSpNo, StockLevels );					
					m_bSaveChanges = TRUE;
				}
			}
		}
	}
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::OnEdit() 
{
	int nSel = m_listStock.GetCurSel();
	if ( nSel >= 0 && nSel < m_arraySales.GetSize() )
	{
		bool bFromAdd = FALSE;
		if ( m_pManualSalesEditDlg != NULL )
		{
			 delete m_pManualSalesEditDlg;
			 m_pManualSalesEditDlg = NULL;
			 bFromAdd = TRUE;
		}
		  
		CManualSalesEditPluDlg dlg ( m_nSpIdx, dbStockpoint.GetName( m_nSpIdx), m_nStkTakeNo, this );
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

void CManualSalesDatabasePluDlg::OnDelete() 
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

void CManualSalesDatabasePluDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
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

void CManualSalesDatabasePluDlg::OnCancel()
{
	if ( m_arraySales.GetSize() == 0 )
	{
		EndDialog ( IDOK );
		return;
	}

	CDeliveryCloseDlg dlg ( NODE_MANUALSALE_PLU, ( m_pStocktake != NULL ), this );
	dlg.DoModal();
		
	switch ( dlg.m_nAction )
	{
	case SESSION_FINALISE:
		{		
			DeleteSessionFile();
			DeleteImportFiles();

			COleDateTime timeNow;
	
			if ( NULL == m_pStocktake )
				timeNow = COleDateTime::GetCurrentTime();
			else
				timeNow = m_pStocktake -> GetOleDateTime();

			AuditSales ( timeNow, TRUE );
		
			CString strAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, timeNow, AUDIT_FAMILY_NORMAL );
			CString strTempAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, timeNow, AUDIT_FAMILY_TEMP );
			CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, timeNow, AUDIT_FAMILY_TEMP_EX );
			CString strPrevAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, timeNow, AUDIT_FAMILY_PREV );
			CString strPrevEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, timeNow, AUDIT_FAMILY_PREV_EX );
		
			::Append ( strTempAudit, strAudit );
		
			CSSFile fileAudit;
			if ( fileAudit.Open ( strAudit, "ab" ) == TRUE )
			{
				CStockAuditExceptionsSales Audit;
				Audit.SetReportType ( AUDIT_EX_SALES );
				
				CStockAuditHeaderInfo info;
				info.m_date = timeNow;
				fileAudit.WriteLine ( Audit.GetHeaderLine( info ) );

				fileAudit.Close();
				::Append ( strTempEx, strAudit );
			}
		
			CString strHistoryLogTime = timeNow.Format ( "#%Y%m%d,%H%M%S,%Y%m%d" );	
			CPluSalesSorterNonEcr PluSalesSorter ( DataManager.GetActiveDbIdx(), m_arraySales, strHistoryLogTime ); 
		
			EndDialog( IDOK );
		}
		break;

	case SESSION_DISCARD:
		{
			if ( "" == m_strPreviousSession )
				WriteSessionFile( FALSE, FALSE );
			
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
			WriteSessionFile( TRUE, FALSE );
			EndDialog( IDCANCEL );
		}
		break;

	case SESSION_CONTINUE:
	default:
		return;
	}
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::WriteSessionFile( bool bDetail, bool bAutoImport )
{
	::CreateSubdirectory( dbStockpoint.GetFolderPathSession( m_nSpIdx, -1 ) );
	::CreateSubdirectory( dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_MANUALSALE_PLU ) );
			
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

		if ( TRUE == bAutoImport )
			m_strPreviousSession = strFilename;
	}

	CString strFolder = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_MANUALSALE_PLU );
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

void CManualSalesDatabasePluDlg::DeleteImportFiles()
{
	for ( int nIndex = 0; nIndex < m_ImportFilesArray.GetSize(); nIndex++ )
	{
		CFileRemover FileRemover;
		CString strFile = m_ImportFilesArray.GetAt( nIndex );
		FileRemover.RemoveFile( strFile );
	}
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::DeleteSessionFile()
{		
	if (  "" != m_strPreviousSession ) 
	{
		CString strFolder = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_MANUALSALE_PLU );
		strFolder += "\\";
		CFileRemover FileRemover( strFolder + m_strPreviousSession );
	}
}

/**********************************************************************/

void CManualSalesDatabasePluDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/
