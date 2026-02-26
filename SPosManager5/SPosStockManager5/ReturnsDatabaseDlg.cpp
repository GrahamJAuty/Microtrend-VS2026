/**********************************************************************/
#include "Append.h"
#include "DeliveryCloseDlg.h"
#include "FileRemover.h"
#include "NodeTypes.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "StockAuditExceptions.h"
#include "StockAuditReturns.h"
#include "StockImportBasicReturnsDlg.h"
#include "StockImportBasicWrapperReturnsDlg.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "ReturnsDatabaseDlg.h"
/**********************************************************************/

CReturnsDatabaseDlg::CReturnsDatabaseDlg( const char* szReference1, const char* szReference2, int nSuppIdx, int nSpIdx, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
	: CDialog(CReturnsDatabaseDlg::IDD, pParent),
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CReturnsDatabaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	

	m_pStocktake = pStocktake;
	m_nStkTakeNo = ( m_pStocktake != NULL ) ? m_pStocktake -> GetStocktakeNo() : -1;

	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_bInitDialog = FALSE;

	m_strPreviousSession = "";
	m_strAutoImportFile = "";
	m_strReference1 = szReference1;
	m_strReference2 = szReference2;
	m_nSuppIdx = nSuppIdx;
	m_nSpIdx = nSpIdx;
	m_nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );
	m_nCacheIndex = -1;
	
	if ( m_nSuppIdx >= 0 && m_nSuppIdx < DataManager.Supplier.GetSize() )
	{
		m_strSupplier = DataManager.Supplier.GetName ( m_nSuppIdx );
		m_nSuppNo = DataManager.Supplier.GetSuppNo ( m_nSuppIdx );
	}
	else
	{
		m_strSupplier = "Unspecified Supplier";
		m_nSuppIdx = -1;
		m_nSuppNo = -1;
	}

	CString strReference = "";
	::FormatAuditReference( m_strReference1, m_strReference2, strReference );

	m_strTitle.Format ( "Returns (%s, %s, reference %s)",
		dbStockpoint.GetName( m_nSpIdx ),
		(const char*) m_strSupplier,
		(const char*) strReference );

	m_pReturnsEditDlg = NULL;
}	
	
/**********************************************************************/

CReturnsDatabaseDlg::~CReturnsDatabaseDlg()
{
	dbStockpoint.RemoveTemporaryAudits( m_nSpIdx );

	if ( m_pReturnsEditDlg != NULL )
		delete m_pReturnsEditDlg;
}

/**********************************************************************/

void CReturnsDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReturnsDatabaseDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CReturnsDatabaseDlg, CDialog)
	//{{AFX_MSG_MAP(CReturnsDatabaseDlg)
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
	ON_COMMAND(IDM_IMPORT, OnImportPlu)
	ON_COMMAND(IDM_IMPORT_WRAPPER, OnImportWrapper)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_USER_BUTTON, OnUserButton )
	ON_MESSAGE ( WM_APP, OnProcessReturn)
	ON_MESSAGE ( WM_APP + 1, OnInitialAddItem)
	ON_MESSAGE ( WM_APP + 2, OnInitialImportPlu)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CReturnsDatabaseDlg::OnInitDialog() 
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
		
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		MenuChanger.SetItemText( IDM_IMPORT, "By &Barcode Number" );
	
	bool bKeepImportMenu = FALSE;

	if ( StockOptions.GetImportReturnPluFlag() == FALSE )
		MenuChanger.KillItem( IDM_IMPORT );
	else
		bKeepImportMenu = TRUE;

	if ( StockOptions.GetImportReturnWrapperFlag() == FALSE )
		MenuChanger.KillItem( IDM_IMPORT_WRAPPER );
	else
		bKeepImportMenu = TRUE;

	if ( FALSE == bKeepImportMenu )
		MenuChanger.KillParent( IDM_IMPORT );

	MenuChanger.BuildMenu( GetMenu() );
	DrawMenuBar();

	if ( m_strPreviousSession != "" )
	{
		CString strFilename = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_RETURN );
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
					m_arrayReturns.Add( strBuffer );
				else if ( csv.GetInt(0) != 1 )
					break;

				bFirstLine = FALSE;
			}
		}

		m_listStock.SetItemCountEx( m_arrayReturns.GetSize() );
	}

	DisplayScreen ( 0 );

	if ( NULL == m_pStocktake )
	{
		if ( ( StockOptions.GetImportReturnPluFlag() == TRUE ) && ( m_strAutoImportFile != "" ) )
			PostMessage ( WM_APP + 2, 0, 0 );
		else
			PostMessage ( WM_APP + 1, 0, 0 );
	}
	else
		PostMessage ( WM_APP + 1, 0, 0 );

	return TRUE;
}

/**********************************************************************/

void CReturnsDatabaseDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listStock.MoveWindow ( 0,0,cx,cy );
	}
}

/**********************************************************************/

void CReturnsDatabaseDlg::DisplayScreen ( int nIndex )
{
	int nSize = m_arrayReturns.GetSize();
	
	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listStock.SetCurSel ( nIndex );
	else
		m_listStock.SetCurSel ( nSize - 1 );

	SetWindowText( m_strTitle );
	m_listStock.Invalidate();
}

/**********************************************************************/

const char* CReturnsDatabaseDlg::GetDisplayLine( int nIndex )
{
	m_strDisplayLine = "";
	if ( nIndex >= 0 && nIndex < m_arrayReturns.GetSize() )
	{
		CCSVMiscStockLevels csvIn ( m_arrayReturns.GetAt( nIndex ) );
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

void CReturnsDatabaseDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CReturnsDatabaseDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEdit();
	*pResult = 0;
}

/**********************************************************************/

void CReturnsDatabaseDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CReturnsDatabaseDlg::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	return 0L;
}

/**********************************************************************/

long CReturnsDatabaseDlg::OnProcessReturn( WPARAM wIndex, LPARAM lParam )
{
	CReturnsEditDlg* pDlg = (CReturnsEditDlg*) lParam;
	m_arrayReturns.Add( pDlg -> m_strOutputLine );
	m_listStock.SetItemCountEx( m_arrayReturns.GetSize() );
	m_listStock.SetCurSel( m_arrayReturns.GetSize() - 1 );
	m_listStock.Invalidate();
	return 0L;
}
	
/**********************************************************************/

long CReturnsDatabaseDlg::OnInitialAddItem( WPARAM wIndex, LPARAM lParam )
{
	OnAdd();
	return 0L;
}

/**********************************************************************/

void CReturnsDatabaseDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listStock, this );
}

/**********************************************************************/

void CReturnsDatabaseDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CReturnsDatabaseDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CReturnsDatabaseDlg::OnAdd() 
{
	if ( m_pReturnsEditDlg == NULL )
	{
		m_pReturnsEditDlg = new CReturnsEditDlg( m_nSuppIdx, m_nSpIdx, m_SuppRefFinder, &m_pReturnsEditDlg, m_nStkTakeNo, this );
		m_pReturnsEditDlg -> Create( IDD_RETURNS_EDIT, this );
	}
	
	m_pReturnsEditDlg -> SetFocus();
}

/**********************************************************************/

long CReturnsDatabaseDlg::OnInitialImportPlu( WPARAM wIndex, LPARAM lParam )
{
	InitialImportPlu();
	return 0L;
}

/**********************************************************************/

void CReturnsDatabaseDlg::InitialImportPlu()
{
	CString strMsg = "";
	strMsg += "Stock Manager has detected a pending file\n";
	strMsg += "of returns for this stockpoint.\n\n";
	strMsg += "Would you like to import this file now";

	if ( Prompter.YesNo( strMsg, FALSE ) != IDYES )
		return;

	CStockImportBasicReturnsDlg dlgImport ( m_nSpIdx, m_nSuppIdx, m_strAutoImportFile, m_strReference1, m_strReference2, NULL, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No returns data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing returns" );
	ImportPluInternal ( m_strAutoImportFile, dlgImport.GetAction(), TRUE );
	StatusProgress.Unlock();

	WriteSessionFile( TRUE, TRUE );
}

/**********************************************************************/

void CReturnsDatabaseDlg::OnImportPlu() 
{
	CString strTitle;
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		strTitle = "Import Returns by Barcode Number";
	else
		strTitle = "Import Returns by Plu Number";
	
	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );

	CString strImportFile = StockOptions.GetImportReturnPluName();
	if ( ( strImportFile.GetLength() == 0 ) || ( strImportFile.GetLength() > 25 ) )
		strImportFile = "returns";

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
	
	CStockImportBasicReturnsDlg dlgImport ( m_nSpIdx, m_nSuppIdx, strImportPath, m_strReference1, m_strReference2, m_pStocktake, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No returns data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing returns" );
	ImportPluInternal ( strImportPath, dlgImport.GetAction(), FALSE );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CReturnsDatabaseDlg::ImportPluInternal( CString& strImportPath, int nAction, bool bAutoImport )
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
		
		__int64 nPluNo;
		if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
			nPluNo = DataManager.Barcode.GetPluFromBarcode( csvIn.GetInt64(0) );
		else
			nPluNo = csvIn.GetInt64(0);
		
		double dQty = csvIn.GetDouble( 1 );

		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == FALSE )
			continue;

		CString strStockCode;
		DataManager.Plu.GetPluStockCode ( nPluNo, strStockCode, FALSE );
		
		if ( strStockCode == "" )
			continue;

		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
			continue;

		CStockCSVRecord StockRecord;
		CStockLevelsCSVRecord StockLevels;
		if ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, dbStockpoint.GetSpNo( m_nSpIdx ), m_nStkTakeNo ) != STOCKLEVEL_IMPORT_ACCEPT )
			continue;
		
		double dStockQtyPerPlu;
		DataManager.Plu.GetPluStockQty ( nPluNo, dStockQtyPerPlu );
		double dStockQty = dQty * dStockQtyPerPlu;

		//IMPORT DELIVERY VALUE FROM FILE OR CALCULATE IT FROM STOCK RECORD
		double dValue = ( StockOptions.GetImportReturnValueFlag() ) ? csvIn.GetDouble(2) : StockRecord.GetUnitCost() * dStockQty;
		
		//GET SUPPLIER REFERENCE AND DELIVERY VALUE FOR LINKED SUPPLIER
		int nStockSuppIdxToUse = -1;
		CString strSuppRef = "";

		if ( m_nSuppIdx >= 0 && m_nSuppIdx < DataManager.Supplier.GetSize() )
		{
			for ( int nStockSuppIdx = 0; nStockSuppIdx < StockRecord.GetSupplierCount(); nStockSuppIdx++ )
			{
				if ( StockRecord.GetSuppNo ( nStockSuppIdx ) == m_nSuppNo )
				{
					nStockSuppIdxToUse = nStockSuppIdx;
					break;
				}
			}

			if ( nStockSuppIdxToUse >= 0 && nStockSuppIdxToUse < StockRecord.GetSupplierCount() )
			{
				strSuppRef = StockRecord.GetSuppRefExternal ( nStockSuppIdxToUse, DealerFlags.GetSuppRefFlag() );

				if ( StockOptions.GetImportReturnValueFlag() == FALSE )
					dValue = StockRecord.GetUnitCost( nStockSuppIdxToUse ) * dStockQty;		 
			}
		}

		if ( nAction == SUBTRACT_STOCK_LEVELS )
		{
			dStockQty = -dStockQty;
			dValue = - dValue;
		}

		CCSVStockAudit csvOut;
		csvOut.Add ( ReportHelpers.GetDisplayPluNo( nPluNo ) );
		csvOut.Add ( strSuppRef );
		csvOut.Add ( strStockCode );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( FALSE );			//FREE STOCK
		csvOut.Add ( FALSE );			//USE DU FLAG
		csvOut.AddStockAuditQty ( 0.0 );	//DU QTY
		csvOut.Add ( 0 );				//ITEMS PER DU
		csvOut.AddStockAuditQty ( 0.0 );	//DU ITEM SIZE
		csvOut.AddStockAuditQty ( dStockQty );
		csvOut.AddStockAuditVal ( 0.0 );	//COST PER DU
		csvOut.AddStockAuditVal ( dValue );
		csvOut.Add ( ENTRY_MODE_PLUNO );
		csvOut.Add ( 0 );				//NO SUPPLIER
		
		m_arrayReturns.Add( csvOut.GetLine() );
	}
	fileImport.Close();

	COleDateTime date;
	CString strPrevEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV_EX );
	CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
	::Append ( strPrevEx, strTempEx );
	
	CFileRemover FileRemover( strPrevEx );
	
	if ( FALSE == bAutoImport )
		m_ImportFilesArray.Add ( strImportPath );
	else
		FileRemover.RemoveFile( strImportPath );

	m_listStock.SetItemCountEx( m_arrayReturns.GetSize() );
	m_listStock.SetCurSel( m_arrayReturns.GetSize() - 1 );
	m_listStock.Invalidate();
}

/**********************************************************************/

void CReturnsDatabaseDlg::OnImportWrapper() 
{
	CString strTitle = "Import Returns by Wrapper Code";
	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );

	CString strImportFile = StockOptions.GetImportReturnWrpName();
	if ( ( strImportFile.GetLength() == 0 ) || ( strImportFile.GetLength() > 25 ) )
		strImportFile = "returns";

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
	
	m_SuppRefFinder.Rebuild( m_nSuppNo );

	CStockImportBasicWrapperReturnsDlg dlgImport ( m_nSpIdx, m_nSuppIdx, strImportPath, m_strReference1, m_strReference2, m_SuppRefFinder, m_pStocktake, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No returns data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing returns" );
	ImportWrapperInternal ( strImportPath, dlgImport.GetAction() );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CReturnsDatabaseDlg::ImportWrapperInternal( CString& strImportPath, int nAction )
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
		double dTotalCost = csvIn.GetDouble( 2 );

		int nStockIdx, nStockSuppIdx;
		if ( m_SuppRefFinder.FindItemByWrapper( strWrapper, nStockIdx, nStockSuppIdx ) == FALSE )
			continue;

		CStockCSVRecord StockRecord;
		CStockLevelsCSVRecord StockLevels;
		if ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, dbStockpoint.GetSpNo( m_nSpIdx ), m_nStkTakeNo ) != STOCKLEVEL_IMPORT_ACCEPT )
			continue;
		
		CString strStockCode = StockRecord.GetStockCode();
		CString strSuppRef = StockRecord.GetSuppRefExternal ( nStockSuppIdx, DealerFlags.GetSuppRefFlag() );

		double dCostPerDU = 0.0;
		if ( StockOptions.GetImportReturnValueFlag() == TRUE )
			dCostPerDU = ( dDUQty != 0.0 ) ? dTotalCost / dDUQty : 0.0;
		else
		{
			dCostPerDU = StockRecord.GetCost( nStockSuppIdx );
			dTotalCost = dCostPerDU * dDUQty;
		}

		double dSUQty = dDUQty * StockRecord.GetYield( nStockSuppIdx );
		int nDUItems = StockRecord.GetDUItems( nStockSuppIdx );
		double dDUItemSize = StockRecord.GetDUItemSize( nStockSuppIdx );

		if ( nAction == SUBTRACT_STOCK_LEVELS )
		{
			dDUQty = -dDUQty;
			dSUQty = -dSUQty;
			dTotalCost = -dTotalCost;
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
		csvOut.AddStockAuditVal ( dCostPerDU );	//COST PER DU
		csvOut.AddStockAuditVal ( dTotalCost );
		csvOut.Add ( ENTRY_MODE_WRAPPER );
		csvOut.Add ( StockRecord.GetSuppNo( nStockSuppIdx ) );
		csvOut.Add ( strWrapper );			//WRAPPER CODE
		
		m_arrayReturns.Add( csvOut.GetLine() );
	}
	fileImport.Close();

	COleDateTime date;
	CString strPrevEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV_EX );
	CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
	::Append ( strPrevEx, strTempEx );
	
	CFileRemover FileRemover;
	FileRemover.RemoveFile ( strPrevEx );

	m_ImportFilesArray.Add ( strImportPath );

	m_listStock.SetItemCountEx( m_arrayReturns.GetSize() );
	m_listStock.SetCurSel( m_arrayReturns.GetSize() - 1 );
	m_listStock.Invalidate();
}

/**********************************************************************/

void CReturnsDatabaseDlg::OnPreview() 
{
	COleDateTime date;
	
	if ( NULL == m_pStocktake )
		date = COleDateTime::GetCurrentTime();
	else
		date = m_pStocktake -> GetOleDateTime();
	
	AuditReturns( date, FALSE );

	CStockAuditReturns Audit;
	Audit.SetReportType( AUDIT_RETURNS );
	Audit.SetConLevel( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_TEMP );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CReturnsDatabaseDlg::AuditReturns( COleDateTime date, bool bUpdateStock )
{
	StatusProgress.Lock( TRUE, "Auditing returns" ); 
	AuditReturnsInternal ( date, bUpdateStock );
	StatusProgress.Unlock(); 
}

/**********************************************************************/

void CReturnsDatabaseDlg::AuditReturnsInternal( COleDateTime date, bool bUpdateStock )
{
	CSSFile fileTemp;
	CString strPath = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, 1 );
	if ( fileTemp.Open ( strPath, "wb" ) == FALSE )
		return;

	bool bWrittenHeader = FALSE;
	CStockAuditReturns Audit;
	Audit.SetReportType( AUDIT_RETURNS );
	
	CStockAuditHeaderInfo info;
	info.m_date = date;
	info.m_strReference1 = m_strReference1;
	info.m_strReference2 = m_strReference2;
	info.m_strSuppName = m_strSupplier;
	CString strHeader = Audit.GetHeaderLine ( info );
	
	int nCount = m_arrayReturns.GetSize();
	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, nCount );
		CCSVMiscStockLevels csvIn ( m_arrayReturns.GetAt( nIndex ) );

		int nStockIdx;
		CString strStockCode = csvIn.GetStockCode();
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
			continue;

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );
		CString strCategory = DataManager.Category.GetText ( StockRecord.GetCategory() );

		CString strDescription = csvIn.GetDescription();
		double dDUQty = csvIn.GetDUQty();
		double dDUCost = csvIn.GetDUCost();
		double dSUQty = csvIn.GetSUQty();
		double dTotalCost = csvIn.GetTotalCost();
		bool bFreeStock = csvIn.GetFreeStockFlag();
		CString strSuppRef = csvIn.GetSuppRef();

		CCSVStockAudit csvOut;
				
		if ( bWrittenHeader == FALSE )
		{
			fileTemp.WriteLine( strHeader );
			bWrittenHeader = TRUE;
		}

		csvOut.RemoveAll();
		csvOut.Add ( "D" );									//data line flag
		csvOut.Add ( AUDIT_RETURNS );						//report type	
		csvOut.Add ( 5 );									//version no
		csvOut.Add ( csvIn.GetStockCode() );				//stock code
		csvOut.Add ( strDescription );						//description	
		csvOut.Add ( strCategory );							//category
		csvOut.AddStockAuditQty ( dDUQty );					//case qty
		csvOut.AddStockAuditQty ( dSUQty );					//stock unit qty
		csvOut.AddStockAuditVal ( dDUCost );				//case cost
		csvOut.AddStockAuditVal ( dTotalCost );				//stock unit cost
		csvOut.Add ( strSuppRef );							//supplier reference
		csvOut.Add ( csvIn.GetDUItems() );					//items per DU
		csvOut.AddStockAuditQty( csvIn.GetDUItemSize() );	//stock Units per Item
		fileTemp.WriteLine ( csvOut.GetLine() );
		
		if ( bUpdateStock == TRUE )
		{
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, m_nSpNo, StockLevels );
			StockLevels.AddReturns ( dSUQty, dTotalCost, ( m_pStocktake != NULL ) );	
			DataManager.Stock.SetStockpointTotals ( nStockIdx, StockRecord, m_nSpNo, StockLevels );		
		}
	}
}

/**********************************************************************/

void CReturnsDatabaseDlg::OnEdit() 
{
	int nSel = m_listStock.GetCurSel();
	if ( nSel >= 0 && nSel < m_arrayReturns.GetSize() )
	{
		bool bFromAdd = FALSE;
		if ( m_pReturnsEditDlg != NULL )
		{
			 delete m_pReturnsEditDlg;
			 m_pReturnsEditDlg = NULL;
			 bFromAdd = TRUE;
		}
		  
		CReturnsEditDlg dlg ( m_nSuppIdx, m_nSpIdx, m_SuppRefFinder, m_nStkTakeNo, this );
		dlg.m_strInputLine = m_arrayReturns.GetAt( nSel );

		if ( dlg.DoModal() == IDOK )
		{
			m_arrayReturns.SetAt( nSel, dlg.m_strOutputLine );
			m_listStock.Invalidate();
		}

		if ( bFromAdd == TRUE )
			OnAdd();
	}
}

/**********************************************************************/

void CReturnsDatabaseDlg::OnDelete() 
{
	int nSel = m_listStock.GetCurSel();
	if ( nSel >= 0 && nSel < m_arrayReturns.GetSize() )
	{
		if ( Prompter.YesNo ( "Are you sure you wish to delete this return" ) == IDYES )
		{
			m_arrayReturns.RemoveAt( nSel );
			m_listStock.SetItemCountEx( m_arrayReturns.GetSize() );

			if ( nSel < m_listStock.GetItemCount() )
				m_listStock.SetCurSel ( nSel );
			else
				m_listStock.SetCurSel ( nSel - 1 );

			m_listStock.Invalidate();
		}
	}
}

/**********************************************************************/

void CReturnsDatabaseDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//NOTHING TO DO IF NO LINES IN LIST
	if ( m_arrayReturns.GetSize() == 0 )
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

void CReturnsDatabaseDlg::OnCancel()
{
	if ( m_arrayReturns.GetSize() == 0 )
	{
		EndDialog ( IDOK );
		return;
	}

	CDeliveryCloseDlg dlg ( NODE_RETURN, ( m_pStocktake != NULL ), this );
	dlg.DoModal();
	
	switch ( dlg.m_nAction )
	{
	case SESSION_FINALISE:	
		{
			DeleteSessionFile();
			DeleteImportFiles();
			UpdateAutoReferenceNo();

			COleDateTime date;
			
			if ( NULL == m_pStocktake )
				date = COleDateTime::GetCurrentTime();
			else
				date = m_pStocktake -> GetOleDateTime();

			AuditReturns ( date, TRUE );

			CString strAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_NORMAL );
			CString strTempAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
			CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
			CString strPrevAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV );
			CString strPrevEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV_EX );
		
			::Append ( strTempAudit, strAudit );
		
			CSSFile fileAudit;
			if ( fileAudit.Open ( strAudit, "ab" ) == TRUE )
			{
				CStockAuditExceptions Audit( AUDIT_EX_RETURNS );
			
				CStockAuditHeaderInfo info;
				info.m_date = date;
				info.m_strReference1 = m_strReference1;
				info.m_strReference2 = m_strReference2;
				fileAudit.WriteLine ( Audit.GetHeaderLine( info ) );

				fileAudit.Close();
				::Append ( strTempEx, strAudit );
			}
		
			CWaitCursor wait;

			CDataManagerInfo info;
			if ( DataManager.WriteStockLevels( m_nSpIdx, info ) == FALSE )
				Prompter.WriteError( info );
			
			EndDialog( IDOK );
		}
		break;

	case SESSION_DISCARD:
		{
			if ( "" == m_strPreviousSession )
			{
				WriteSessionFile( FALSE, FALSE );
				UpdateAutoReferenceNo();
			}

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
			
			if ( "" == m_strPreviousSession )
				UpdateAutoReferenceNo();

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

void CReturnsDatabaseDlg::WriteSessionFile( bool bDetail, bool bAutoImport )
{
	::CreateSubdirectory( dbStockpoint.GetFolderPathSession( m_nSpIdx, -1 ) );
	::CreateSubdirectory( dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_RETURN ) );
			
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

	CString strFolder = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_RETURN );
	strFolder += "\\";

	CSSFile fileSession;
	if ( fileSession.Open( strFolder + strFilename, "wb" ) == TRUE )
	{
		CCSV csv;
		csv.Add( 1 );
		csv.Add( DataManager.Supplier.GetSuppNo( m_nSuppIdx ) );
		csv.Add( m_strReference1 );
		csv.Add( m_strReference2 );
		fileSession.WriteLine( csv.GetLine() );

		if ( TRUE == bDetail )
		{
			for ( int n = 0; n < m_arrayReturns.GetSize(); n++ )
				fileSession.WriteLine( m_arrayReturns.GetAt(n) );
		}
	}
}

/**********************************************************************/

void CReturnsDatabaseDlg::UpdateAutoReferenceNo()
{
	if ( StockOptions.GetAutoNumReturnFlag() == TRUE )
	{
		int nNum = StockOptions.GetAutoNumReturn() + 1;

		m_strReference1.Format( "%s%6.6d",
			StockOptions.GetAutoNumReturnPrefix(),
			nNum );

		if ( nNum >= 999999 ) nNum = 0;
			StockOptions.SetAutoNumReturn( nNum );
	}
}

/**********************************************************************/

void CReturnsDatabaseDlg::DeleteImportFiles()
{
	for ( int nIndex = 0; nIndex < m_ImportFilesArray.GetSize(); nIndex++ )
	{
		CFileRemover FileRemover;
		CString strFile = m_ImportFilesArray.GetAt( nIndex );
		FileRemover.RemoveFile( strFile );
	}
}

/**********************************************************************/

void CReturnsDatabaseDlg::DeleteSessionFile()
{		
	if (  "" != m_strPreviousSession ) 
	{
		CString strFolder = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_RETURN );
		strFolder += "\\";
		CFileRemover FileRemover( strFolder + m_strPreviousSession );
	}
}

/**********************************************************************/

void CReturnsDatabaseDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/

