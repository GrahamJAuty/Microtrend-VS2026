/**********************************************************************/
#include "Append.h"
#include "ColumnManager.h"
#include "DeliveryCloseDlg.h"
#include "ExplodePlu.h"
#include "FileRemover.h"
#include "NodeTypes.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "ReportHelpers.h"
#include "Resource.h"
#include "StatusProgress.h"
#include "StockAuditExceptionsSales.h"
#include "StockAuditAdjustmentPlu.h"
#include "StockImportAdjustmentReasonDlg.h"
#include "StockImportBasicAdjustmentPluDlg.h"
#include "StockOptionsIni.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "AdjustmentsDatabasePluDlg.h"
/**********************************************************************/

CAdjustmentsDatabasePluDlg::CAdjustmentsDatabasePluDlg( const char* szReference1, const char* szReference2, int nSpIdx, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
	: CSSDialog("IDD_ADJUSTMENT_DATABASE_PLU", pParent),
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CAdjustmentsDatabasePluDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pStocktake = pStocktake;
	m_nStkTakeNo = ( pStocktake != NULL ) ? pStocktake -> GetStocktakeNo() : -1;

	m_nCacheIndex = -1;

	m_strPreviousSession = "";
	m_strAutoImportFile = "";
	m_strReference1 = szReference1;
	m_strReference2 = szReference2;

	CString strReference = "";
	::FormatAuditReference( m_strReference1, m_strReference2, strReference );

	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_bInitDialog = FALSE;

	m_nSpIdx = nSpIdx;
	m_nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );

	m_strTitle.Format ( "Adjustments by Plu (%s, reference %s)",
		dbStockpoint.GetName( m_nSpIdx ),
		(const char*) strReference );
		
	m_strLastReason = "";
	
	m_pAdjustmentsStockEditDlg = NULL;

	CDataManagerInfo info;
	DataManagerNonDb.OpenAdjustText( DB_READONLY, info );
}	
	
/**********************************************************************/

CAdjustmentsDatabasePluDlg::~CAdjustmentsDatabasePluDlg()
{
	dbStockpoint.RemoveTemporaryAudits( m_nSpIdx );

	if ( m_pAdjustmentsStockEditDlg != NULL )
		delete m_pAdjustmentsStockEditDlg;
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdjustmentsDatabasePluDlg)	
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CAdjustmentsDatabasePluDlg, CDialog)
	//{{AFX_MSG_MAP(CAdjustmentsDatabasePluDlg)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC2_LIST, OnGetDispInfoList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC2_LIST, OnColumnClickList)
	ON_COMMAND(IDM2_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM2_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM2_COLUMN_DEFAULT, OnColumnDefault)
	ON_COMMAND(IDM2_IMPORT, OnImport)
	ON_COMMAND(IDM2_ADD, OnAdd)
	ON_COMMAND(IDM2_PREVIEW, OnPreview)
	ON_COMMAND(IDM2_DELETE, OnDelete)
	ON_COMMAND(IDM2_EDIT, OnEdit)
	ON_NOTIFY(NM_DBLCLK, IDC2_LIST, OnDoubleClickList)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_EXIT, OnCancel)
	ON_COMMAND(IDM2_FILE_EXIT, OnCancel)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC2_LIST, OnListCacheHint)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_USER_BUTTON, OnUserButton )
	ON_MESSAGE ( WM_APP, OnProcessAdjustment)
	ON_MESSAGE ( WM_APP + 1, OnInitialAddItem)
	ON_MESSAGE ( WM_APP + 2, OnInitialImportPlu)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CAdjustmentsDatabasePluDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );

	m_listStock.SubclassDlgItem ( IDC2_LIST, this );

	m_ColumnManager.Reset( TableNames::Adjustment );
	m_ColumnManager.AddColumn( ColumnNames::PluNo, "Plu No", LVCFMT_LEFT, 110, 110 );
	m_ColumnManager.AddColumn( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 110, 110 );
	m_ColumnManager.AddColumn( ColumnNames::Desc, "Description", LVCFMT_LEFT, 160, 160 );
	m_ColumnManager.AddColumn ( ColumnNames::Dept, "Dept", LVCFMT_LEFT, 50, 50 );					
	m_ColumnManager.AddColumn( ColumnNames::PluQty, "Qty", LVCFMT_RIGHT, 80, 80 );
	m_ColumnManager.AddColumn( ColumnNames::PluValue, "Value", LVCFMT_RIGHT, 80, 80 );
	m_ColumnManager.AddColumn( ColumnNames::Reason, "Reason", LVCFMT_LEFT, 100, 160 );
	m_ColumnManager.LoadSettings();
	
	m_bInitDialog = TRUE;
	m_ColumnManager.RedrawList( &m_listStock, this );
	CenterWindow();

	CMenuChanger MenuChanger;
	MenuChanger.EatMenuTopLevel( GetMenu() );
	MenuChanger.KillItem( IDM2_IMPORT_WRAPPER );

	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		MenuChanger.SetItemText( IDM2_IMPORT, "By &Barcode Number" );

	if ( StockOptions.GetImportAdjustPluFlag() == FALSE )
		MenuChanger.KillParent( IDM2_IMPORT );
		
	MenuChanger.BuildMenu ( GetMenu() );
	DrawMenuBar();

	if ( m_strPreviousSession != "" )
	{
		CString strFilename = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_ADJUSTMENT_PLU );
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
					m_arrayAdjustments.Add( strBuffer );
				else if ( csv.GetInt(0) != 1 )
					break;

				bFirstLine = FALSE;
			}
		}

		m_listStock.SetItemCountEx( m_arrayAdjustments.GetSize() );
	}

	DisplayScreen ( 0 );

	if ( NULL == m_pStocktake )
	{
		if ( m_strAutoImportFile != "" )
			PostMessage( WM_APP + 2, 0, 0 );
		else
			PostMessage( WM_APP + 1, 0, 0 );
	}
	else
		PostMessage( WM_APP + 1, 0, 0 );

	return TRUE;
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listStock.MoveWindow ( 0,0,cx,cy );
	}
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::DisplayScreen ( int nIndex )
{
	int nSize = m_arrayAdjustments.GetSize();
	
	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listStock.SetCurSel ( nIndex );
	else
		m_listStock.SetCurSel ( nSize - 1 );

	SetWindowText( m_strTitle );
	m_listStock.Invalidate();
}

/**********************************************************************/

const char* CAdjustmentsDatabasePluDlg::GetDisplayLine( int nIndex )
{
	m_strDisplayLine = "";
	if ( nIndex >= 0 && nIndex < m_arrayAdjustments.GetSize() )
	{
		CCSVAdjustmentPlu csvIn ( m_arrayAdjustments.GetAt( nIndex ) );
		
		CCSV csvOut ( '\t' );
		csvOut.Add ( ReportHelpers.GetDisplayPluNo( csvIn.GetPluNo() ) );
		csvOut.Add ( csvIn.GetStockCode() );
		csvOut.Add ( csvIn.GetDescription() );
		csvOut.Add ( csvIn.GetDeptNo() );
		csvOut.Add ( csvIn.GetTotalQty(), SysInfo.GetDPQty() );
		csvOut.Add ( csvIn.GetValue(), SysInfo.GetDPValue() );
		csvOut.Add ( csvIn.GetReason() );

		m_strDisplayLine = csvOut.GetLine();
	}

	return m_strDisplayLine;
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CAdjustmentsDatabasePluDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEdit();
	*pResult = 0;
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CAdjustmentsDatabasePluDlg::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	return 0L;
}

/**********************************************************************/

long CAdjustmentsDatabasePluDlg::OnProcessAdjustment( WPARAM wIndex, LPARAM lParam )
{
	CAdjustmentsEditPluDlg* pDlg = (CAdjustmentsEditPluDlg*) lParam;

	m_arrayAdjustments.Add( pDlg -> m_strOutputLine );
	m_listStock.SetItemCountEx( m_arrayAdjustments.GetSize() );
	m_listStock.SetCurSel( m_arrayAdjustments.GetSize() - 1 );
	m_listStock.Invalidate();

	return 0L;
}

/**********************************************************************/

long CAdjustmentsDatabasePluDlg::OnInitialAddItem( WPARAM wIndex, LPARAM lParam )
{
	OnAdd();
	return 0L;
}
	
/**********************************************************************/

void CAdjustmentsDatabasePluDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listStock, this );
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::OnAdd() 
{
	if ( m_pAdjustmentsStockEditDlg == NULL )
	{
		m_pAdjustmentsStockEditDlg = new CAdjustmentsEditPluDlg( m_nSpIdx, dbStockpoint.GetName(m_nSpIdx), m_strLastReason, &m_pAdjustmentsStockEditDlg, m_nStkTakeNo, this );
		m_pAdjustmentsStockEditDlg -> Create( "IDD_ADJUSTMENT_EDIT_PLU", this);
	}
	
	m_pAdjustmentsStockEditDlg -> SetFocus();
}

/**********************************************************************/

long CAdjustmentsDatabasePluDlg::OnInitialImportPlu( WPARAM wIndex, LPARAM lParam )
{
	InitialImportPlu();
	return 0L;
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::InitialImportPlu()
{
	CString strMsg = "";
	strMsg += "Stock Manager has detected a pending file\n";
	strMsg += "of adjustments for this stockpoint.\n\n";
	strMsg += "Would you like to import this file now";

	if ( Prompter.YesNo( strMsg, FALSE ) != IDYES )
		return;

	CStockImportAdjustmentReasonDlg dlg ( m_strLastReason, this );
	if ( dlg.DoModal() == IDCANCEL )
		return;

	int nLineType = DealerFlags.GetPluCSVImportType();
	CStockImportBasicAdjustmentPluDlg dlgImport ( m_nSpIdx, m_strAutoImportFile, nLineType, m_strLastReason, m_strReference1, m_strReference2, NULL, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No adjustment data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing adjustments" );
	ImportPluInternal ( m_strAutoImportFile, dlgImport.GetAction(), TRUE );
	StatusProgress.Unlock();

	WriteSessionFile( TRUE, TRUE );
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::OnImport() 
{
	CStockImportAdjustmentReasonDlg dlg ( m_strLastReason, this );
	if ( dlg.DoModal() == IDCANCEL )
		return;

	CString strTitle;
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		strTitle.Format ( "Import Adjustments by Barcode Number (%s)", (const char*) m_strLastReason );
	else
		strTitle.Format ( "Import Adjustments by Plu Number (%s)", (const char*) m_strLastReason );
	
	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );

	CString strImportFile = StockOptions.GetImportAdjustPluName();
	if ( ( strImportFile.GetLength() == 0 ) || ( strImportFile.GetLength() > 25 ) )
		strImportFile = "adjust";

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
	
	CStockImportBasicAdjustmentPluDlg dlgImport ( m_nSpIdx, strImportPath, nLineType, m_strLastReason, m_strReference1, m_strReference2, m_pStocktake, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No adjustment data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	int nAction = dlgImport.GetAction();

	StatusProgress.Lock( TRUE, "Importing adjustments" );	
	ImportPluInternal( strImportPath, nAction, FALSE );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::ImportPluInternal( CString& strImportPath, int nAction, bool bAutoImport )
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

		__int64 nPluNo = csv.GetInt64( 0 );
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
		if ( ExplodePlu.ExplodePlu ( nPluNo, StockDetails, AUDIT_ADJUSTMENT_PLU, dQty, dValue ) == PLU_EXCEPTION_NONE )
		{
			PluInfoFinder.SetPluNo( nPluNo );
			CCSVExplodedPlu csvItem ( StockDetails.GetAt ( 0 ) );

			CCSVStockAudit csv;
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
			csv.Add ( m_strLastReason );

			m_arrayAdjustments.Add ( csv.GetLine() );
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

	m_listStock.SetItemCountEx( m_arrayAdjustments.GetSize() );
	m_listStock.SetCurSel( m_arrayAdjustments.GetSize() - 1 );
	m_listStock.Invalidate();
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::OnPreview() 
{
	COleDateTime date;
	
	if ( NULL == m_pStocktake )
		date = COleDateTime::GetCurrentTime();
	else
		date = m_pStocktake -> GetOleDateTime();

	AuditAdjustments( date, FALSE );

	CStockAuditAdjustmentPlu Audit;
	Audit.LockDatabase();
	Audit.SetReportType( AUDIT_ADJUSTMENT_PLU );
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

void CAdjustmentsDatabasePluDlg::AuditAdjustments( COleDateTime date, bool bUpdateStock )
{
	StatusProgress.Lock( TRUE, "Auditing Adjustments" ); 
	AuditAdjustmentsInternal ( date, bUpdateStock );
	StatusProgress.Unlock(); 
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::AuditAdjustmentsInternal( COleDateTime date, bool bUpdateStock )
{
	CSSFile fileTemp;
	CString strPath = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
	if ( fileTemp.Open ( strPath, "wb" ) == FALSE )
		return;

	bool bWrittenHeader = FALSE;
	CStockAuditAdjustmentPlu Audit;
	Audit.SetReportType( AUDIT_ADJUSTMENT_PLU );
	
	CStockAuditHeaderInfo info;
	info.m_date = date;
	info.m_strReference1 = m_strReference1;
	info.m_strReference2 = m_strReference2;
	CString strHeader = Audit.GetHeaderLine ( info );
	
	int nCount = m_arrayAdjustments.GetSize();
	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, nCount );
		CString strAdjustment = m_arrayAdjustments.GetAt ( nIndex );
		CCSVAdjustmentPlu csvAdjustment ( strAdjustment );
			
		__int64 nPluNo = csvAdjustment.GetPluNo();
		double dSaleQty = csvAdjustment.GetTotalQty();
		double dSaleValue = csvAdjustment.GetValue();
		int nLevel = csvAdjustment.GetPriceLevel();
		
		CExplodePluStockTake ExplodePlu( m_nSpNo, m_nStkTakeNo );
		ExplodePlu.SetPriceLevel( csvAdjustment.GetPriceLevel() );
		
		CStringArray StockDetails;
		if ( ExplodePlu.ExplodePlu ( nPluNo, StockDetails, AUDIT_ADJUSTMENT_PLU, dSaleQty, dSaleValue ) == PLU_EXCEPTION_NONE )
		{	
			for ( int nItemIdx = 0; nItemIdx < StockDetails.GetSize(); nItemIdx++ )
			{
				if ( bWrittenHeader == FALSE )
				{
					fileTemp.WriteLine( strHeader );
					bWrittenHeader = TRUE;
				}

				CCSVExplodedPlu csvItem ( StockDetails.GetAt ( nItemIdx ) );
	
				int nStockIdx = csvItem.GetStockIdx();
				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt ( nStockIdx, StockRecord );
					
				CString strPluDesc = csvItem.GetDescription();
				CString strStkDesc = StockRecord.GetDescription();
				if ( strPluDesc == strStkDesc )
					strStkDesc = "";

				CCSVStockAudit csvOut;
				csvOut.Add ( "D" );
				csvOut.Add ( AUDIT_ADJUSTMENT_PLU );
				csvOut.Add ( 1 );
				csvOut.Add ( ( nItemIdx == 0 ) ? 0 : 1 );
				csvOut.Add ( csvItem.GetPluNo() );
				csvOut.Add ( strPluDesc );
				csvOut.AddStockAuditQty ( csvItem.GetPluQty() );
				csvOut.AddStockAuditVal ( csvItem.GetPluValue() );
				csvOut.Add ( csvItem.GetStockCode() );
				csvOut.Add ( csvItem.GetSubUnits() );
				csvOut.AddStockAuditQty ( csvItem.GetStockQty() );
				csvOut.AddStockAuditVal ( csvItem.GetStockValue() );
				csvOut.Add( csvAdjustment.GetReason() );
				csvOut.Add( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
				csvOut.Add( strStkDesc );
				fileTemp.WriteLine ( csvOut.GetLine() );
			
				if ( TRUE == bUpdateStock )
				{
					double dStockQty = csvItem.GetStockQty();
					double dStockValue = csvItem.GetStockValue();		
					CStockLevelsCSVRecord StockLevels;
					DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, m_nSpNo, StockLevels );					
					StockLevels.AddAdjustment ( dStockQty, dStockValue, ( m_pStocktake != NULL ) );
					DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, m_nSpNo, StockLevels );					
				}
			}
		}
	}
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::OnEdit() 
{
	int nSel = m_listStock.GetCurSel();
	if ( nSel >= 0 && nSel < m_arrayAdjustments.GetSize() )
	{
		bool bFromAdd = FALSE;
		if ( m_pAdjustmentsStockEditDlg != NULL )
		{
			 delete m_pAdjustmentsStockEditDlg;
			 m_pAdjustmentsStockEditDlg = NULL;
			 bFromAdd = TRUE;
		}
		  
		CAdjustmentsEditPluDlg dlg ( m_nSpIdx, dbStockpoint.GetName( m_nSpIdx), m_strLastReason, m_nStkTakeNo, this );
		dlg.m_strInputLine = m_arrayAdjustments.GetAt( nSel );

		if ( dlg.DoModal() == IDOK )
		{
			m_arrayAdjustments.SetAt( nSel, dlg.m_strOutputLine );
			m_listStock.Invalidate();
		}

		if ( bFromAdd == TRUE )
			OnAdd();
	}
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::OnDelete() 
{
	int nSel = m_listStock.GetCurSel();
	if ( nSel >= 0 && nSel < m_arrayAdjustments.GetSize() )
	{
		if ( Prompter.YesNo ( "Are you sure you wish to delete this adjustment" ) == IDYES )
		{
			m_arrayAdjustments.RemoveAt( nSel );
			m_listStock.SetItemCountEx( m_arrayAdjustments.GetSize() );

			if ( nSel < m_listStock.GetItemCount() )
				m_listStock.SetCurSel ( nSel );
			else
				m_listStock.SetCurSel ( nSel - 1 );

			m_listStock.Invalidate();
		}
	}
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//NOTHING TO DO IF NO LINES IN LIST
	
	if ( m_arrayAdjustments.GetSize() == 0 )
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

void CAdjustmentsDatabasePluDlg::OnCancel()
{
	if ( m_arrayAdjustments.GetSize() == 0 )
	{
		EndDialog ( IDOK );
		return;
	}

	CDeliveryCloseDlg dlg ( NODE_ADJUSTMENT_PLU, ( m_pStocktake != NULL ), this );
	dlg.DoModal();
		
	switch ( dlg.m_nAction )
	{
	case SESSION_FINALISE:
		{		
			DeleteSessionFile();
			DeleteImportFiles();
			UpdateAutoReferenceNo();

			COleDateTime timeNow;
	
			if ( NULL == m_pStocktake )
				timeNow = COleDateTime::GetCurrentTime();
			else
				timeNow = m_pStocktake -> GetOleDateTime();

			AuditAdjustments ( timeNow, TRUE );
		
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
				Audit.SetReportType ( AUDIT_EX_ADJUSTMENT_PLU );
				
				CStockAuditHeaderInfo info;
				info.m_date = timeNow;
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

void CAdjustmentsDatabasePluDlg::WriteSessionFile( bool bDetail, bool bAutoImport )
{
	::CreateSubdirectory( dbStockpoint.GetFolderPathSession( m_nSpIdx, -1 ) );
	::CreateSubdirectory( dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_ADJUSTMENT_PLU ) );
			
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

	CString strFolder = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_ADJUSTMENT_PLU );
	strFolder += "\\";

	CSSFile fileSession;
	if ( fileSession.Open( strFolder + strFilename, "wb" ) == TRUE )
	{
		CCSV csv;
		csv.Add( 1 );
		csv.Add( 0 );
		csv.Add( m_strReference1 );
		csv.Add( m_strReference2 );
		fileSession.WriteLine( csv.GetLine() );

		if ( TRUE == bDetail )
		{
			for ( int n = 0; n < m_arrayAdjustments.GetSize(); n++ )
				fileSession.WriteLine( m_arrayAdjustments.GetAt(n) );
		}
	}
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::UpdateAutoReferenceNo()
{
	if ( StockOptions.GetAutoNumAdjustFlag() == TRUE )
	{
		int nNum = StockOptions.GetAutoNumAdjust() + 1;

		m_strReference1.Format( "%s%6.6d",
			StockOptions.GetAutoNumAdjustPrefix(),
			nNum );

		if ( nNum >= 999999 ) nNum = 0;
		StockOptions.SetAutoNumAdjust( nNum );
	}
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::DeleteImportFiles()
{
	for ( int nIndex = 0; nIndex < m_ImportFilesArray.GetSize(); nIndex++ )
	{
		CFileRemover FileRemover;
		CString strFile = m_ImportFilesArray.GetAt( nIndex );
		FileRemover.RemoveFile( strFile );
	}
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::DeleteSessionFile()
{		
	if (  "" != m_strPreviousSession ) 
	{
		CString strFolder = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_ADJUSTMENT_PLU );
		strFolder += "\\";
		CFileRemover FileRemover( strFolder + m_strPreviousSession );
	}
}

/**********************************************************************/

void CAdjustmentsDatabasePluDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/
