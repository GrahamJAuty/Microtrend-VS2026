/**********************************************************************/
#include "Append.h"
#include "CCSVReportLine.h"
#include "DeliveryCloseDlg.h"
#include "FileRemover.h"
#include "NodeTypes.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "Resource.h"
#include "StatusProgress.h"
#include "StockAuditAdjustmentStock.h"
#include "StockAuditExceptions.h"
#include "StockImportAdjustmentReasonDlg.h"
#include "StockImportBasicAdjustmentStockDlg.h"
#include "StockOptionsIni.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "AdjustmentsDatabaseStockDlg.h"
/**********************************************************************/

CAdjustmentsDatabaseStockDlg::CAdjustmentsDatabaseStockDlg( const char* szReference1, const char* szReference2, int nSpIdx, CStocktakeCSVRecord* pStocktake, CWnd* pParent )
	: CDialog("IDD_ADJUSTMENT_DATABASE_STOCK", pParent),
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CAdjustmentsDatabaseStockDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	

	m_pStocktake = pStocktake;
	m_nStkTakeNo = ( pStocktake != NULL ) ? pStocktake -> GetStocktakeNo() : -1;

	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_bInitDialog = FALSE;

	m_nSpIdx = nSpIdx;
	m_nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );
	m_nCacheIndex = -1;
	
	m_strPreviousSession = "";
	m_strAutoImportFile = "";

	m_strReference1 = szReference1;
	m_strReference2 = szReference2;

	CString strReference = "";
	::FormatAuditReference( m_strReference1, m_strReference2, strReference );

	m_strTitle.Format ( "Adjustments (%s, reference %s)",
		dbStockpoint.GetName( m_nSpIdx ),
		(const char*) strReference );

	m_pAdjustmentsStockEditDlg = NULL;
	m_strLastReason = "";

	CDataManagerInfo info;
	DataManagerNonDb.OpenAdjustText( DB_READONLY, info );
}	
	
/**********************************************************************/

CAdjustmentsDatabaseStockDlg::~CAdjustmentsDatabaseStockDlg()
{
	dbStockpoint.RemoveTemporaryAudits( m_nSpIdx );

	if ( m_pAdjustmentsStockEditDlg != NULL )
		delete m_pAdjustmentsStockEditDlg;

	CDataManagerInfo info;
	DataManagerNonDb.CloseAdjustText( info );
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdjustmentsDatabaseStockDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CAdjustmentsDatabaseStockDlg, CDialog)
	//{{AFX_MSG_MAP(CAdjustmentsDatabaseStockDlg)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC2_LIST, OnGetDispInfoList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC2_LIST, OnColumnClickList)
	ON_COMMAND(IDM2_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(IDM2_COLUMN_RESTORE, OnColumnRestore)
	ON_COMMAND(IDM2_COLUMN_DEFAULT, OnColumnDefault)
	ON_COMMAND(IDM2_ADD, OnAdd)
	ON_COMMAND(IDM2_PREVIEW, OnPreview)
	ON_COMMAND(IDM2_DELETE, OnDelete)
	ON_COMMAND(IDM2_EDIT, OnEdit)
	ON_NOTIFY(NM_DBLCLK, IDC2_LIST, OnDoubleClickList)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(LVN_ODCACHEHINT, IDC2_LIST, OnListCacheHint)
	ON_COMMAND(IDM_EXIT, OnCancel)
	ON_COMMAND(IDM2_FILE_EXIT, OnCancel)
	ON_COMMAND(IDM2_IMPORT, OnImportPlu)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_USER_BUTTON, OnUserButton )
	ON_MESSAGE ( WM_APP, OnProcessAdjustment)
	ON_MESSAGE ( WM_APP + 1, OnInitialAddItem)
	ON_MESSAGE ( WM_APP + 2, OnInitialImportPlu)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CAdjustmentsDatabaseStockDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );

	m_listStock.SubclassDlgItem ( IDC2_LIST, this );

	m_ColumnManager.Reset( TableNames::Adjustment );
	m_ColumnManager.AddColumn( ColumnNames::PluNo, "Plu No", LVCFMT_LEFT, 110, 110 );
	m_ColumnManager.AddColumn( ColumnNames::StockCode, "Stock Code", LVCFMT_LEFT, 115, 115 );
	m_ColumnManager.AddColumn( ColumnNames::Desc, "Description", LVCFMT_LEFT, 140, 180 );
	m_ColumnManager.AddColumn( ColumnNames::SUQty, "Quantity", LVCFMT_RIGHT, 70, 80 );
	m_ColumnManager.AddColumn( ColumnNames::SUValue, "Value", LVCFMT_RIGHT, 70, 80 );
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
		CString strFilename = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_ADJUSTMENT_STOCK );
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

	if ( NULL  == m_pStocktake )
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

void CAdjustmentsDatabaseStockDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_listStock.MoveWindow ( 0,0,cx,cy );
	}
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::DisplayScreen ( int nIndex )
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

const char* CAdjustmentsDatabaseStockDlg::GetDisplayLine( int nIndex )
{
	m_strDisplayLine = "";
	if ( nIndex >= 0 && nIndex < m_arrayAdjustments.GetSize() )
	{
		CCSVAdjustmentInfo csvIn ( m_arrayAdjustments.GetAt( nIndex ) );
		
		CStockReportLine line;

		CCSV csvOut ( '\t' );
		csvOut.Add( csvIn.GetPluNo() );	
		csvOut.Add( csvIn.GetStockCode() );
		csvOut.Add( csvIn.GetDescription() );

		double dQuantity = csvIn.GetQuantity();
		int nSubUnits = csvIn.GetSubUnits();

		if ( nSubUnits < 2 )
			csvOut.Add( dQuantity, SysInfo.GetDPQty() );
		else
			csvOut.Add( line.GetStockLevelString( dQuantity, nSubUnits ) );

		csvOut.Add( csvIn.GetValue(), SysInfo.GetDPValue() );
		csvOut.Add( csvIn.GetReason() );

		m_strDisplayLine = csvOut.GetLine();
	}
	return m_strDisplayLine;
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CAdjustmentsDatabaseStockDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEdit();
	*pResult = 0;
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iSubItem;
	PostMessage ( WM_USER_BUTTON, (WPARAM) nIndex, 0 );
	*pResult = 0;
}

/**********************************************************************/

long CAdjustmentsDatabaseStockDlg::OnUserButton( WPARAM wIndex, LPARAM lParam )
{
	return 0L;
}

/**********************************************************************/

long CAdjustmentsDatabaseStockDlg::OnProcessAdjustment( WPARAM wIndex, LPARAM lParam )
{
	CAdjustmentsEditStockDlg* pDlg = (CAdjustmentsEditStockDlg*) lParam;
	m_arrayAdjustments.Add( pDlg -> m_strOutputLine );
	m_listStock.SetItemCountEx( m_arrayAdjustments.GetSize() );
	m_listStock.SetCurSel( m_arrayAdjustments.GetSize() - 1 );
	m_listStock.Invalidate();
	return 0L;
}
	
/**********************************************************************/

long CAdjustmentsDatabaseStockDlg::OnInitialAddItem( WPARAM wIndex, LPARAM lParam )
{
	OnAdd();
	return 0L;
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::OnColumnSave() 
{
	m_ColumnManager.SaveCurrentLine( &m_listStock, this );
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::OnColumnRestore() 
{
	m_ColumnManager.RestoreStartOfSessionLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::OnColumnDefault() 
{
	m_ColumnManager.RestoreDefaultLine();
	m_ColumnManager.RedrawList( &m_listStock, this );
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::OnAdd() 
{
	if ( m_pAdjustmentsStockEditDlg == NULL )
	{
		m_pAdjustmentsStockEditDlg = new CAdjustmentsEditStockDlg( m_nSpIdx, m_strLastReason, &m_pAdjustmentsStockEditDlg, m_nStkTakeNo, this );
		m_pAdjustmentsStockEditDlg -> Create( "IDD_ADJUSTMENT_EDIT_STOCK", this);
	}
	
	m_pAdjustmentsStockEditDlg -> SetFocus();
}

/**********************************************************************/

long CAdjustmentsDatabaseStockDlg::OnInitialImportPlu( WPARAM wIndex, LPARAM lParam )
{
	InitialImportPlu();
	return 0L;
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::InitialImportPlu()
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

	CStockImportBasicAdjustmentStockDlg dlgImport ( m_nSpIdx, m_strAutoImportFile, m_strReference1, m_strReference2, m_strLastReason, NULL, this );
	
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

void CAdjustmentsDatabaseStockDlg::OnImportPlu() 
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
	
	CStockImportBasicAdjustmentStockDlg dlgImport ( m_nSpIdx, strImportPath, m_strReference1, m_strReference2, m_strLastReason, m_pStocktake, this );
	
	if ( dlgImport.GotData() == FALSE )
	{
		Prompter.Error ( "No adjustment data was found" );
		return;
	}

	if ( dlgImport.DoModal() == IDCANCEL )
		return;

	StatusProgress.Lock( TRUE, "Importing adjustments" );
	ImportPluInternal ( strImportPath, dlgImport.GetAction(), FALSE );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::ImportPluInternal( CString& strImportPath, int nAction, bool bAutoImport )
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
		if ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, m_nSpNo, m_nStkTakeNo ) != STOCKLEVEL_IMPORT_ACCEPT )
			continue;

		double dStockQtyPerPlu;
		DataManager.Plu.GetPluStockQty ( nPluNo, dStockQtyPerPlu );
		double dStockQty = dQty * dStockQtyPerPlu;

		//IMPORT ADJUSTMENT VALUE FROM FILE OR CALCULATE IT FROM STOCK RECORD
		double dValue = 0.0;
		if ( StockOptions.GetImportAdjustValueFlag() == TRUE )
			dValue = csvIn.GetDouble(2);
		else
			dValue = DataManager.Stock.CalculateRetailValue( StockRecord, dStockQty );
		
		if ( nAction == SUBTRACT_STOCK_LEVELS )
		{
			dStockQty = -dStockQty;
			dValue = - dValue;
		}

		CCSVStockAudit csvOut;
		csvOut.Add ( ReportHelpers.GetDisplayPluNo( nPluNo ) );
		csvOut.Add ( strStockCode );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.AddStockAuditQty ( dStockQty );
		csvOut.AddStockAuditVal ( dValue );
		csvOut.Add ( ENTRY_MODE_PLUNO );
		csvOut.Add ( 1 );		
		csvOut.Add ( m_strLastReason );	
		
		m_arrayAdjustments.Add( csvOut.GetLine() );
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

	m_listStock.SetItemCountEx( m_arrayAdjustments.GetSize() );
	m_listStock.SetCurSel( m_arrayAdjustments.GetSize() - 1 );
	m_listStock.Invalidate();
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::OnPreview() 
{
	COleDateTime date;
	
	if ( NULL == m_pStocktake )
		date = COleDateTime::GetCurrentTime();
	else
		date = m_pStocktake -> GetOleDateTime();

	AuditAdjustments( date, FALSE );

	CStockAuditAdjustmentStock Audit;
	Audit.LockDatabase();
	Audit.SetReportType( AUDIT_ADJUSTMENT_STOCK );
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

void CAdjustmentsDatabaseStockDlg::AuditAdjustments( COleDateTime date, bool bUpdateStock )
{
	StatusProgress.Lock( TRUE, "Auditing adjustments" ); 
	AuditAdjustmentsInternal ( date, bUpdateStock );
	StatusProgress.Unlock(); 
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::AuditAdjustmentsInternal( COleDateTime date, bool bUpdateStock )
{
	CSSFile fileTemp;
	CString strPath = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, 1 );
	if ( fileTemp.Open ( strPath, "wb" ) == FALSE )
		return;

	bool bWrittenHeader = FALSE;
	CStockAuditAdjustmentStock Audit;
	Audit.SetReportType( AUDIT_ADJUSTMENT_STOCK );

	CStockAuditHeaderInfo info;
	info.m_date = date;
	info.m_strReference1 = m_strReference1;
	info.m_strReference2 = m_strReference2;
	CString strHeader = Audit.GetHeaderLine ( info );
	
	int nCount = m_arrayAdjustments.GetSize();
	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, nCount );
		CCSVAdjustmentInfo csvIn ( m_arrayAdjustments.GetAt( nIndex ) );

		int nStockIdx;
		CString strStockCode = csvIn.GetStockCode();
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
			continue;

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );
		CString strCategory = DataManager.Category.GetText ( StockRecord.GetCategory() );

		CString strDescription = csvIn.GetDescription();
		int nSubUnits = csvIn.GetSubUnits();
		double dQuantity = csvIn.GetQuantity();
		double dValue = csvIn.GetValue();
		CString strReason = csvIn.GetReason();

		CCSVStockAudit csvOut;
				
		if ( bWrittenHeader == FALSE )
		{
			fileTemp.WriteLine( strHeader );
			bWrittenHeader = TRUE;
		}

		csvOut.RemoveAll();
		csvOut.Add ( "D" );									//data line flag
		csvOut.Add ( AUDIT_ADJUSTMENT_STOCK );					//report type	
		csvOut.Add ( 1 );									//version no
		csvOut.Add ( csvIn.GetStockCode() );					//stock code
		csvOut.Add ( strDescription );						//description	
		csvOut.Add ( strCategory );							//category
		csvOut.Add ( nSubUnits );							//sub units per SU
		csvOut.AddStockAuditQty ( dQuantity );					//stock unit qty
		csvOut.AddStockAuditVal ( dValue );					//stock unit cost
		csvOut.Add ( strReason );							//reason for adjustment
		fileTemp.WriteLine ( csvOut.GetLine() );
		
		if ( bUpdateStock == TRUE )
		{
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, m_nSpNo, StockLevels );
			StockLevels.AddAdjustment ( dQuantity, dValue, ( m_pStocktake != NULL ) );	
			DataManager.Stock.SetStockpointTotals ( nStockIdx, StockRecord, m_nSpNo, StockLevels );		
		}
	}
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::OnEdit() 
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
		  
		CString strDummy;
		CAdjustmentsEditStockDlg dlg ( m_nSpIdx, strDummy, m_nStkTakeNo, this );
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

void CAdjustmentsDatabaseStockDlg::OnDelete() 
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

void CAdjustmentsDatabaseStockDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
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

void CAdjustmentsDatabaseStockDlg::OnCancel()
{
	if ( m_arrayAdjustments.GetSize() == 0 )
	{
		EndDialog ( IDOK );
		return;
	}

	CDeliveryCloseDlg dlg ( NODE_ADJUSTMENT_STOCK, ( m_pStocktake != NULL ), this );
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

			AuditAdjustments ( date, TRUE );

			CString strAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_NORMAL );
			CString strTempAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP );
			CString strTempEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_TEMP_EX );
			CString strPrevAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV );
			CString strPrevEx = dbStockpoint.GetFilePathAudit( m_nSpIdx, date, AUDIT_FAMILY_PREV_EX );
		
			::Append ( strTempAudit, strAudit );
		
			CSSFile fileAudit;
			if ( fileAudit.Open ( strAudit, "ab" ) == TRUE )
			{
				CStockAuditExceptions Audit( AUDIT_EX_ADJUSTMENT_STOCK );
				
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

void CAdjustmentsDatabaseStockDlg::WriteSessionFile( bool bDetail, bool bAutoImport )
{
	::CreateSubdirectory( dbStockpoint.GetFolderPathSession( m_nSpIdx, -1 ) );
	::CreateSubdirectory( dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_ADJUSTMENT_STOCK ) );
			
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

	CString strFolder = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_ADJUSTMENT_STOCK );
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

void CAdjustmentsDatabaseStockDlg::UpdateAutoReferenceNo()
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

void CAdjustmentsDatabaseStockDlg::DeleteImportFiles()
{
	for ( int nIndex = 0; nIndex < m_ImportFilesArray.GetSize(); nIndex++ )
	{
		CFileRemover FileRemover;
		CString strFile = m_ImportFilesArray.GetAt( nIndex );
		FileRemover.RemoveFile( strFile );
	}
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::DeleteSessionFile()
{		
	if (  "" != m_strPreviousSession ) 
	{
		CString strFolder = dbStockpoint.GetFolderPathSession( m_nSpIdx, NODE_ADJUSTMENT_STOCK );
		strFolder += "\\";
		CFileRemover FileRemover( strFolder + m_strPreviousSession );
	}
}

/**********************************************************************/

void CAdjustmentsDatabaseStockDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/
