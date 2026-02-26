/**********************************************************************/
#include "Append.h"
#include "ColumnManager.h"
#include "..\CommonEcrStock\CustomExportManager.h"
#include "DeliveryCloseDlg.h"
#include "FileRemover.h"
#include "..\CommonEcrStock\KeyboardHelpers.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "OpeningStockAuditer.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "Resource.h"
#include "StatusProgress.h"
#include "StockAuditDeliveryDetail.h"
#include "StockAuditExceptions.h"
#include "StockImportBasicDeliveryDlg.h"
#include "StockImportBasicWrapperDeliveryDlg.h"
#include "StockOptionsIni.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "AdhocDeliveryDatabaseDlg.h"
/**********************************************************************/

CAdhocDeliveryDatabaseDlg::CAdhocDeliveryDatabaseDlg( const char* szReference1, const char* szReference2, int nSuppIdx, int nSpIdx, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
	: CDialog("IDD_ADHOC_DELIVERY_DATABASE", pParent),
	m_CacheCSV( '\t' )
{
	//{{AFX_DATA_INIT(CAdhocDeliveryDatabaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	

	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();

	m_pStocktake = pStocktake;
	m_nStkTakeNo = ( pStocktake != NULL ) ? pStocktake -> GetStocktakeNo() : -1;

	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_bInitDialog = FALSE;

	m_strPreviousSession = "";
	m_strAutoImportPluFile = "";
	m_strAutoImportWrapperFile = "";
	m_strReference1 = szReference1;
	m_strReference2 = szReference2;
	m_nSuppIdx = nSuppIdx;
	m_nSpIdx = nSpIdx;
	m_nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );

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

	m_strTitle.Format ( "Adhoc Delivery (%s, %s, reference %s)",
		dbStockpoint.GetName( m_nSpIdx ),
		(const char*) m_strSupplier,
		(const char*) strReference );

	m_nNewPluDeptNo = 0;

	m_pDeliveryEditDlg = NULL;
	m_nCacheIndex = -1;
}	
	
/**********************************************************************/

CAdhocDeliveryDatabaseDlg::~CAdhocDeliveryDatabaseDlg()
{
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();

	dbStockpoint.RemoveTemporaryAudits(m_nSpIdx);

	if (m_pDeliveryEditDlg != NULL)
	{
		delete m_pDeliveryEditDlg;
	}
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdhocDeliveryDatabaseDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CAdhocDeliveryDatabaseDlg, CDialog)
	//{{AFX_MSG_MAP(CAdhocDeliveryDatabaseDlg)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC2_LIST, OnGetDispInfoList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC2_LIST, OnColumnClickList)
	ON_COMMAND(IDM2_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM2_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM2_COLUMN_DEFAULT, OnColumnDefault)
	ON_COMMAND(IDM2_IMPORT, OnImportPlu)
	ON_COMMAND(IDM2_ADD, OnAdd)
	ON_COMMAND(IDM2_PREVIEW, OnPreview)
	ON_COMMAND(IDM2_DELETE, OnDelete)
	ON_COMMAND(IDM2_EDIT, OnEdit)
	ON_NOTIFY(NM_DBLCLK, IDC2_LIST, OnDoubleClickList)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(LVN_ODCACHEHINT, IDC2_LIST, OnListCacheHint)
	ON_COMMAND(IDM_EXIT, OnCancel)
	ON_COMMAND(IDM2_FILE_EXIT, OnCancel)
	ON_COMMAND(IDM2_IMPORT_WRAPPER, OnImportWrapper)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_USER_BUTTON, OnUserButton )
	ON_MESSAGE ( WM_APP, OnProcessDeliveryItem)
	ON_MESSAGE ( WM_APP + 1, OnInitialAddItem)
	ON_MESSAGE ( WM_APP + 2, OnInitialImportPlu)
	ON_MESSAGE ( WM_APP + 3, OnInitialImportWrapper)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CAdhocDeliveryDatabaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );

	m_listStock.SubclassDlgItem ( IDC2_LIST, this );

	m_ColumnManager.Reset( TableNames::AdhocDelivery );
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

	CFileRemover FileRemover;
	CFilenameUpdater FnUp( SysFiles::OpeningTemp, DataManager.GetActiveDbIdx() );
	FileRemover.RemoveFile( FnUp.GetFilenameToUse() );
	
	CMenuChanger MenuChanger;
	MenuChanger.EatMenuTopLevel( GetMenu() );
		
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		MenuChanger.SetItemText( IDM2_IMPORT, "By &Barcode Number" );
	
	bool bKeepImportMenu = FALSE;

	if ( StockOptions.GetImportAdhocPluFlag() == FALSE )
		MenuChanger.KillItem( IDM2_IMPORT );
	else
		bKeepImportMenu = TRUE;

	if ( StockOptions.GetImportAdhocWrapperFlag() == FALSE )
		MenuChanger.KillItem( IDM2_IMPORT_WRAPPER );
	else
		bKeepImportMenu = TRUE;

	if ( FALSE == bKeepImportMenu )
		MenuChanger.KillParent( IDM2_IMPORT );

	MenuChanger.BuildMenu( GetMenu() );
	DrawMenuBar();

	if ( m_strPreviousSession != "" )
	{
		CString strFilename = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_DELIVERY );
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
					m_arrayDeliveries.Add( strBuffer );
				else if ( csv.GetInt(0) != 1 )
					break;

				bFirstLine = FALSE;
			}
		}

		m_listStock.SetItemCountEx( m_arrayDeliveries.GetSize() );
	}

	DisplayScreen ( 0 );

	MessageLogger.LogSystem ( 1, 1, TRUE );

	if ( NULL == m_pStocktake )
	{
		if ( ( StockOptions.GetImportAdhocPluFlag() == TRUE ) && ( m_strAutoImportPluFile != "" ) )
			PostMessage( WM_APP + 2, 0, 0 );
		else if ( ( StockOptions.GetImportAdhocWrapperFlag() == TRUE ) && ( m_strAutoImportWrapperFile !="" ) )
			PostMessage( WM_APP + 3, 0, 0 );
		else
			PostMessage( WM_APP + 1, 0, 0 );
	}
	else
		PostMessage( WM_APP + 1, 0, 0 );

	return TRUE;
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listStock.MoveWindow ( 0,0,cx,cy );
	}
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::DisplayScreen ( int nIndex )
{
	int nSize = m_arrayDeliveries.GetSize();
	
	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listStock.SetCurSel ( nIndex );
	else
		m_listStock.SetCurSel ( nSize - 1 );

	SetWindowText( m_strTitle );
	m_listStock.Invalidate();
}

/**********************************************************************/

const char* CAdhocDeliveryDatabaseDlg::GetDisplayLine( int nIndex )
{
	m_strDisplayLine = "";
	if ( nIndex >= 0 && nIndex < m_arrayDeliveries.GetSize() )
	{
		CCSVMiscStockLevels csvIn ( m_arrayDeliveries.GetAt( nIndex ) );
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

void CAdhocDeliveryDatabaseDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CAdhocDeliveryDatabaseDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEdit();
	*pResult = 0;
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CAdhocDeliveryDatabaseDlg::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	return 0L;
}

/**********************************************************************/

long CAdhocDeliveryDatabaseDlg::OnProcessDeliveryItem( WPARAM wIndex, LPARAM lParam )
{
	CAdhocDeliveryEditDlg* pDlg = (CAdhocDeliveryEditDlg*) lParam;
	m_arrayDeliveries.Add( pDlg -> m_strOutputLine );

	m_listStock.SetItemCountEx( m_arrayDeliveries.GetSize() );
	m_listStock.SetCurSel( m_arrayDeliveries.GetSize() - 1 );
	m_listStock.Invalidate();

	return 0L;
}

/**********************************************************************/

long CAdhocDeliveryDatabaseDlg::OnInitialAddItem( WPARAM wIndex, LPARAM lParam )
{
	OnAdd();
	return 0L;
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listStock, this );
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnAdd() 
{
	if ( m_pDeliveryEditDlg == NULL )
	{
		m_pDeliveryEditDlg = new CAdhocDeliveryEditDlg( m_nSuppIdx, m_nSpIdx, m_SuppRefFinder, m_nNewPluDeptNo, &m_pDeliveryEditDlg, m_nStkTakeNo, this );
		m_pDeliveryEditDlg -> Create( "IDD_ADHOC_DELIVERY_EDIT", this);
	}
	
	m_pDeliveryEditDlg -> SetFocus();
}

/**********************************************************************/

long CAdhocDeliveryDatabaseDlg::OnInitialImportPlu( WPARAM wIndex, LPARAM lParam )
{
	InitialImportPlu();
	return 0L;
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::InitialImportPlu()
{
	CString strMsg = "";
	strMsg += "Stock Manager has detected a pending file of\n";
	strMsg += "deliveries by Plu for this stockpoint.\n\n";
	strMsg += "Would you like to import this file now";

	if ( Prompter.YesNo( strMsg, FALSE ) != IDYES )
		return;

	CStockImportBasicDeliveryDlg dlgImport ( m_nSpIdx, m_nSuppIdx, m_strAutoImportPluFile, m_strReference1, m_strReference2, NULL, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No delivery data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing deliveries" );
	ImportPluInternal ( m_strAutoImportPluFile, dlgImport.GetAction(), TRUE );
	StatusProgress.Unlock();

	WriteSessionFile( TRUE, TRUE );
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnImportPlu() 
{
	CString strTitle;
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		strTitle = "Import Deliveries by Barcode Number";
	else
		strTitle = "Import Deliveries by Plu Number";

	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );

	CString strImportFile = StockOptions.GetImportAdhocPluName();
	if ( ( strImportFile.GetLength() == 0 ) || ( strImportFile.GetLength() > 25 ) )
		strImportFile = "delivery";

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

	CStockImportBasicDeliveryDlg dlgImport ( m_nSpIdx, m_nSuppIdx, strImportPath, m_strReference1, m_strReference2, m_pStocktake, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No delivery data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing deliveries" );
	ImportPluInternal ( strImportPath, dlgImport.GetAction(), FALSE );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::ImportPluInternal( CString& strImportPath, int nAction, bool bAutoImport )
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
		
		CString strPluNo;
		if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		{
			__int64 nBarcodeNo = _atoi64( csvIn.GetString(0) );
			__int64 nPluNo = DataManager.Barcode.GetPluFromBarcode( nBarcodeNo ); 
			::FormatPluNo( nPluNo, strPluNo );
		}
		else
			strPluNo = csvIn.GetString(0);
		
		double dQty = csvIn.GetDouble( 1 );

		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber ( _atoi64(strPluNo), nPluIdx ) == FALSE )
			continue;

		CString strStockCode;
		__int64 nPluNo = DataManager.Plu.GetPluNoInt( nPluIdx );
		DataManager.Plu.GetPluStockCode ( nPluNo, strStockCode, FALSE );
		
		if ( strStockCode == "" )
			continue;

		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
			continue;

		CStockCSVRecord StockRecord;
		CStockLevelsCSVRecord StockLevels;
		if ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, m_nSpNo, m_nStkTakeNo ) != STOCKLEVEL_IMPORT_ACCEPT )
			continue;

		double dStockQtyPerPlu;
		DataManager.Plu.GetPluStockQty ( nPluNo, dStockQtyPerPlu );
		double dStockQty = dQty * dStockQtyPerPlu;

		//IMPORT DELIVERY VALUE FROM FILE OR CALCULATE IT FROM STOCK RECORD
		double dValue = ( StockOptions.GetImportAdhocValueFlag() ) ? csvIn.GetDouble(2) : StockRecord.GetUnitCost() * dStockQty;
		
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

				if ( StockOptions.GetImportAdhocValueFlag() == FALSE )
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
		
		m_arrayDeliveries.Add( csvOut.GetLine() );
		
		if ( StockOptions.GetImportAdhocCostFlag() == TRUE )
		{
			if ( nStockSuppIdxToUse >= 0 && nStockSuppIdxToUse < StockRecord.GetSupplierCount() )
			{
				double dYield = ( double ) StockRecord.GetYield ( nStockSuppIdxToUse );
				
				if ( dStockQty != 0.0 )
				{
					StockRecord.SetCost ( nStockSuppIdxToUse, ( dValue * dYield ) / dStockQty );
					StockRecord.MakePreferred ( nStockSuppIdxToUse );
					DataManager.Stock.SetAt ( nStockIdx, StockRecord );
					m_SuppRefFinder.HandlePreferredSupplierChange( nStockIdx );
				}
			}
		}
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

	m_listStock.SetItemCountEx( m_arrayDeliveries.GetSize() );
	m_listStock.SetCurSel( m_arrayDeliveries.GetSize() - 1 );
	m_listStock.Invalidate();
}

/**********************************************************************/

long CAdhocDeliveryDatabaseDlg::OnInitialImportWrapper( WPARAM wIndex, LPARAM lParam )
{
	InitialImportWrapper();
	return 0L;
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::InitialImportWrapper()
{
	CString strMsg = "";
	strMsg += "Stock Manager has detected a pending file of\n";
	strMsg += "deliveries by wrapper for this stockpoint.\n\n";
	strMsg += "Would you like to import this file now";

	if ( Prompter.YesNo( strMsg, FALSE ) != IDYES )
		return;

	CStockImportBasicWrapperDeliveryDlg dlgImport ( m_nSpIdx, m_nSuppIdx, m_strAutoImportWrapperFile, m_strReference1, m_strReference2, m_SuppRefFinder, NULL, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No delivery data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing deliveries" );
	ImportWrapperInternal ( m_strAutoImportWrapperFile, dlgImport.GetAction(), TRUE );
	StatusProgress.Unlock();

	WriteSessionFile( TRUE, TRUE );
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnImportWrapper() 
{
	CString strTitle = "Import Deliveries by Wrapper Code";
	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );

	CString strImportFile = StockOptions.GetImportAdhocWrpName();
	if ( ( strImportFile.GetLength() == 0 ) || ( strImportFile.GetLength() > 25 ) )
		strImportFile = "delivery";

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

	CStockImportBasicWrapperDeliveryDlg dlgImport ( m_nSpIdx, m_nSuppIdx, strImportPath, m_strReference1, m_strReference2, m_SuppRefFinder, m_pStocktake, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No delivery data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing deliveries" );
	ImportWrapperInternal ( strImportPath, dlgImport.GetAction(), FALSE );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::ImportWrapperInternal( CString& strImportPath, int nAction, bool bAutoImport )
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
		if ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, m_nSpNo, m_nStkTakeNo ) != STOCKLEVEL_IMPORT_ACCEPT )
			continue;
		
		CString strStockCode = StockRecord.GetStockCode();
		CString strSuppRef = StockRecord.GetSuppRefExternal ( nStockSuppIdx, DealerFlags.GetSuppRefFlag() );

		double dCostPerDU = 0.0;
		if ( StockOptions.GetImportAdhocValueFlag() == TRUE )
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
		
		m_arrayDeliveries.Add( csvOut.GetLine() );
		
		if ( StockOptions.GetImportAdhocCostFlag() == TRUE )
		{
			StockRecord.SetCost ( nStockSuppIdx, dCostPerDU );
			StockRecord.MakePreferred ( nStockSuppIdx );
			DataManager.Stock.SetAt ( nStockIdx, StockRecord );
			m_SuppRefFinder.HandlePreferredSupplierChange( nStockIdx );
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

	m_listStock.SetItemCountEx( m_arrayDeliveries.GetSize() );
	m_listStock.SetCurSel( m_arrayDeliveries.GetSize() - 1 );
	m_listStock.Invalidate();
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnPreview() 
{
	COleDateTime date;
	
	if ( NULL == m_pStocktake )
		date = COleDateTime::GetCurrentTime();
	else
		date = m_pStocktake -> GetOleDateTime();
	
	AuditDeliveries( date, FALSE );

	CStockAuditDeliveryDetail Audit( DealerFlags.GetDeliveryDetailFlag() );
	Audit.SetReportType( AUDIT_DELIVERY );
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

void CAdhocDeliveryDatabaseDlg::AuditDeliveries( COleDateTime date, bool bUpdateStock )
{
	StatusProgress.Lock( TRUE, "Auditing deliveries" ); 
	AuditDeliveriesInternal ( date, bUpdateStock );
	StatusProgress.Unlock(); 
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::AuditDeliveriesInternal( COleDateTime date, bool bUpdateStock )
{
	CSSFile fileTemp;
	CString strPath = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, 1 );
	if ( fileTemp.Open ( strPath, "wb" ) == FALSE )
		return;

	bool bWrittenHeader = FALSE;
	CStockAuditDeliveryDetail Audit( TRUE );
	Audit.SetReportType( AUDIT_DELIVERY );

	CStockAuditHeaderInfo info;
	info.m_date = date;
	info.m_strReference1 = m_strReference1;
	info.m_strReference2 = m_strReference2;
	info.m_strSuppName = m_strSupplier;
	CString strHeader = Audit.GetHeaderLine ( info );
	
	int nCount = m_arrayDeliveries.GetSize();
	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, nCount );
		CCSVMiscStockLevels csvIn ( m_arrayDeliveries.GetAt( nIndex ) );

		int nStockIdx;
		CString strStockCode = csvIn.GetStockCode();
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
			continue;

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );
		CString strCategory = DataManager.Category.GetText ( StockRecord.GetCategory() );
		
		double dCostTax = DataManager.Stock.CalculateTaxOnCost( StockRecord, csvIn.GetTotalCost() );
		double dSUPerCase = csvIn.GetDUItemSize() * csvIn.GetDUItems();
		double dCaseRetail = DataManager.Stock.CalculateRetailValue( StockRecord, dSUPerCase );
		double dLineRetail = DataManager.Stock.CalculateRetailValue( StockRecord, csvIn.GetSUQty() );
		double dLineTax = DataManager.Stock.CalculateTaxContentFromValue( StockRecord, dLineRetail );
	
		double dLineWaste = 0.0;
		if ( StockOptions.GetStockSubUnitsFlag() )
		{
			double dNonWastePercent = 100.0 - StockRecord.GetWastePercent();
		
			if ( dNonWastePercent != 0.0 )
			{
				double dCostWithWaste = ( csvIn.GetTotalCost() * 100.0 ) / dNonWastePercent;  
				dLineWaste = dCostWithWaste - csvIn.GetTotalCost();
			}
		}

		double dGPPercent = ReportHelpers.CalcGPPercent( dLineRetail - dLineTax, 0, csvIn.GetTotalCost() + dLineWaste );

		CCSVStockAudit csvOut;
				
		if ( bWrittenHeader == FALSE )
		{
			fileTemp.WriteLine( strHeader );
			bWrittenHeader = TRUE;
		}

		csvOut.RemoveAll();
		csvOut.Add ( "D" );									//data line flag
		csvOut.Add ( AUDIT_DELIVERY );						//report type	
		csvOut.Add ( 4 );									//version no
		csvOut.Add ( csvIn.GetStockCode() );				//stock code
		csvOut.Add ( csvIn.GetDescription() );				//description	
		csvOut.Add ( strCategory );							//category
		csvOut.AddStockAuditQty ( csvIn.GetDUQty() );		//case qty
		csvOut.AddStockAuditQty ( csvIn.GetSUQty() );		//stock unit qty
		csvOut.AddStockAuditVal ( csvIn.GetDUCost() );		//case cost
		csvOut.AddStockAuditVal ( csvIn.GetTotalCost() );	//stock unit cost
		csvOut.AddStockAuditVal ( dCaseRetail );			//case retail
		csvOut.AddStockAuditVal ( dLineRetail );			//total retail
		csvOut.AddStockAuditVal ( dLineTax );				//total tax content
		csvOut.AddStockAuditVal ( dLineWaste );				//total wastage cost
		csvOut.Add ( dGPPercent, 2 );						//GP Percent
		csvOut.Add ( csvIn.GetSuppRef() );					//supplier reference
		csvOut.Add ( csvIn.GetDUItems() );					//items per DU
		csvOut.AddStockAuditQty( csvIn.GetDUItemSize() );	//stock Units per Item
		csvOut.AddStockAuditVal( dCostTax );

		fileTemp.WriteLine ( csvOut.GetLine() );
		
		if ( bUpdateStock == TRUE )
		{
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, m_nSpNo, StockLevels );

			bool bStocktake = ( m_pStocktake != NULL );

			if ( csvIn.GetFreeStockFlag() == FALSE )
				StockLevels.AddDelivery ( csvIn.GetSUQty(), csvIn.GetTotalCost(), bStocktake );
			else
				StockLevels.AddFreeStock ( csvIn.GetSUQty(), bStocktake );
				
			DataManager.Stock.SetStockpointTotals ( nStockIdx, StockRecord, m_nSpNo, StockLevels );

			if ( Sysset.IsBarmanSystem() && EcrmanOptions.GetLabelsProductAutoBatchFlag() )
				DataManager.ProductLabels.AutoBarcode ( StockRecord, int ( csvIn.GetSUQty() ) );			
		}
	}
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnEdit() 
{
	int nSel = m_listStock.GetCurSel();
	if ( nSel >= 0 && nSel < m_arrayDeliveries.GetSize() )
	{
		bool bFromAdd = FALSE;
		if ( m_pDeliveryEditDlg != NULL )
		{
			 delete m_pDeliveryEditDlg;
			 m_pDeliveryEditDlg = NULL;
			 bFromAdd = TRUE;
		}
		  
		CAdhocDeliveryEditDlg dlg ( m_nSuppIdx, m_nSpIdx, m_SuppRefFinder, m_nNewPluDeptNo, m_nStkTakeNo, this );
		dlg.m_strInputLine = m_arrayDeliveries.GetAt( nSel );

		if ( dlg.DoModal() == IDOK )
		{
			m_arrayDeliveries.SetAt( nSel, dlg.m_strOutputLine );
			m_listStock.Invalidate();
		}

		if ( bFromAdd == TRUE )
			OnAdd();
	}
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnDelete() 
{
	int nSel = m_listStock.GetCurSel();
	if ( nSel >= 0 && nSel < m_arrayDeliveries.GetSize() )
	{
		if ( Prompter.YesNo ( "Are you sure you wish to delete this delivery" ) == IDYES )
		{
			m_arrayDeliveries.RemoveAt( nSel );
			m_listStock.SetItemCountEx( m_arrayDeliveries.GetSize() );

			if ( nSel < m_listStock.GetItemCount() )
				m_listStock.SetCurSel ( nSel );
			else
				m_listStock.SetCurSel ( nSel - 1 );

			m_listStock.Invalidate();
		}
	}
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//NOTHING TO DO IF NO LINES IN LIST
	if ( m_arrayDeliveries.GetSize() == 0 )
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
	VERIFY(menu.LoadMenu(IDR2_POPUP_STOCK_LIST));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnCancel()
{
	if ( m_arrayDeliveries.GetSize() == 0 )
	{
		MessageLogger.LogSystem ( 1, 2, FALSE );
		EndDialog ( IDOK );
		return;
	}

	CDeliveryCloseDlg dlg ( NODE_DELIVERY, ( m_pStocktake != NULL ), this );
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
			
			AuditDeliveries ( date, TRUE );

			CString strAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_NORMAL );
			CString strTempAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
			CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
			CString strPrevAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV );
			CString strPrevEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV_EX );
		
			::Append ( strTempAudit, strAudit );
		
			CSSFile fileAudit;
			if ( fileAudit.Open ( strAudit, "ab" ) == TRUE )
			{
				CStockAuditExceptions Audit( AUDIT_EX_DELIVERY );
				
				CStockAuditHeaderInfo info;
				info.m_date = date;
				info.m_strReference1 = m_strReference1;
				info.m_strReference2 = m_strReference2;
				fileAudit.WriteLine ( Audit.GetHeaderLine( info ) );

				fileAudit.Close();
				::Append ( strTempEx, strAudit );
			}
		
			SaveStockChanges( TRUE );

			if ( Sysset.IsBarmanSystem() == TRUE )
			{
				if ( EcrmanOptions.GetLabelsProductAutoBatchFlag() == TRUE )
				{
					CDataManagerInfo info;
					if ( DataManager.WriteProductLabels( info ) == FALSE )
						Prompter.WriteError( info );
				}
			}
			
			MessageLogger.LogSystem ( 1, 3, FALSE, m_arrayDeliveries.GetSize() );
			EndDialog ( IDOK );
		}
		break;

	case SESSION_DISCARD:
		{
			if ( "" == m_strPreviousSession )
			{
				WriteSessionFile( FALSE, FALSE );
				UpdateAutoReferenceNo();
			}

			MessageLogger.LogSystem ( 1, 4, FALSE, m_arrayDeliveries.GetSize() );	
			DataManager.ResourceWrapperPlu.UndoDatabaseChanges();
			DataManager.ListKeyImageWrapperPlu.UndoDatabaseChanges();
			EndDialog ( IDCANCEL );
		}
		break;

	case SESSION_ABANDON:
		{
			DeleteSessionFile();
			MessageLogger.LogSystem ( 1, 4, FALSE, m_arrayDeliveries.GetSize() );		
			DataManager.ResourceWrapperPlu.UndoDatabaseChanges();
			DataManager.ListKeyImageWrapperPlu.UndoDatabaseChanges();
			EndDialog ( IDCANCEL );
		}
		break;

	case SESSION_FILE:
		{
			DeleteImportFiles();
			
			if ( "" == m_strPreviousSession )
				UpdateAutoReferenceNo();

			WriteSessionFile( TRUE, FALSE );
			SaveStockChanges( FALSE );

			EndDialog( IDCANCEL );
		}
		break;

	case SESSION_CONTINUE:
	default:
		return;
	}
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::WriteSessionFile( bool bDetail, bool bAutoImport )
{
	::CreateSubdirectory( dbStockpoint.GetFolderPathSession( m_nSpIdx, -1 ) );
	::CreateSubdirectory( dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_DELIVERY ) );
			
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

	CString strFolder = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_DELIVERY );
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
			for ( int n = 0; n < m_arrayDeliveries.GetSize(); n++ )
				fileSession.WriteLine( m_arrayDeliveries.GetAt(n) );
		}
	}
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::UpdateAutoReferenceNo()
{
	if ( StockOptions.GetAutoNumAdhocFlag() == TRUE )
	{
		int nNum = StockOptions.GetAutoNumAdhoc() + 1;

		m_strReference1.Format( "%s%6.6d",
			StockOptions.GetAutoNumAdhocPrefix(),
			nNum );

		if ( nNum >= 999999 ) nNum = 0;
			StockOptions.SetAutoNumAdhoc ( nNum );
	}
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::DeleteImportFiles()
{
	for ( int nIndex = 0; nIndex < m_ImportFilesArray.GetSize(); nIndex++ )
	{
		CFileRemover FileRemover;
		CString strFile = m_ImportFilesArray.GetAt( nIndex );
		FileRemover.RemoveFile( strFile );
	}
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::DeleteSessionFile()
{		
	if (  "" != m_strPreviousSession ) 
	{
		CString strFolder = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_DELIVERY );
		strFolder += "\\";
		CFileRemover FileRemover( strFolder + m_strPreviousSession );
	}
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::SaveStockChanges( bool bLevels )
{
	CWaitCursor wait;
	DataManager.Plu.UpdatePrices();
		
	CDataManagerInfo info;
	if ( DataManager.WriteStock( info, TRUE, bLevels ) == FALSE )
		Prompter.WriteError( info );
	else
	{
		DataManager.ResourceWrapperPlu.SaveDatabaseChanges();
		DataManager.ListKeyImageWrapperPlu.SaveDatabaseChanges();

		DataManager.Plu.UpdateSalesManCostPrice();
		if ( DataManager.WritePlu( info ) == FALSE )
			Prompter.WriteError( info );
		else if ( DataManager.WriteBarcode( info ) == FALSE )
			Prompter.WriteError( info );
	}

	CKeyboardHelpers::RemovePluFile( DataManager.GetActiveDbIdx() );
	CCustomExportManagerPlu CustomExport;			
	COpeningStockAuditer OpeningStockAuditer;
}

/**********************************************************************/

void CAdhocDeliveryDatabaseDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/

