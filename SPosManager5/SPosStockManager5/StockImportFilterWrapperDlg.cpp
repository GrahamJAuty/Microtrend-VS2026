/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "NodeTypes.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockAuditBaseReport.h"
#include "StockAuditExceptions.h"
#include "StockAuditImportWrapper.h"
#include "StockAuditOpeningClosing.h"
#include "StockImportFilterListDlg.h"
#include "StockpointCSVArray.h"
#include "WrapperErrors.h"
/**********************************************************************/
#include "StockImportFilterWrapperDlg.h"
/**********************************************************************/

CStockImportFilterWrapperDlg::CStockImportFilterWrapperDlg( int nSpIdx, bool bClosing, const char* szPath, CSuppRefFinder& SuppRefFinder, CStockImportFilterArray& arrayImport, CWnd* pParent)
	: CDialog(CStockImportFilterWrapperDlg::IDD, pParent), m_arrayImport( arrayImport ), m_SuppRefFinder( SuppRefFinder )
{
	//{{AFX_DATA_INIT(CStockImportFilterWrapperDlg)
	//}}AFX_DATA_INIT

	m_nSuppNo = 0;

	m_nSpIdx = nSpIdx;
	m_bClosing = bClosing;
	m_nStocktakeNo = -1;
	m_nLinesFound = 0;
	m_nExceptionLines = 0;
	m_nAction = 0;

	m_strHeaderImportPath = szPath;
	m_strHeaderReference1 = "";
	m_strHeaderReference2 = "";
	m_strHeaderSuppName = "";
	m_strHeaderSpName1 = dbStockpoint.GetName( m_nSpIdx );
	m_strHeaderSpName2 = "";

	m_ImportDate = COleDateTime::GetCurrentTime();

	if ( TRUE == m_bClosing )
	{
		m_strTitle = "Import Closing Stock";
		m_strRadio1 = "Add imported levels to closing stock";
		m_strRadio2 = "Subtract imported levels from closing stock";
		m_strRadio3 = "Overwrite closing stock with imported levels";
		m_nImportFileReportType = AUDIT_IMPORT_WRAPPER_CLOSING;
		m_nExceptionReportType = AUDIT_EX_CLOSING;
		m_nAuditReportType = AUDIT_CLOSING;
	}
	else
	{
		m_strTitle = "Import Opening Stock";
		m_strRadio1 = "Add imported levels to opening stock";
		m_strRadio2 = "Subtract imported levels from opening stock";
		m_strRadio3 = "Overwrite opening stock with imported levels";
		m_nImportFileReportType = AUDIT_IMPORT_WRAPPER_OPENING;
		m_nExceptionReportType = AUDIT_EX_OPENING;
		m_nAuditReportType = AUDIT_OPENING;
	}
	
	m_strTitle += " by Wrapper Code";
}

/**********************************************************************/

CStockImportFilterWrapperDlg::~CStockImportFilterWrapperDlg()
{
}

/**********************************************************************/

void CStockImportFilterWrapperDlg::ProcessImportData()
{
	m_arrayImport.RemoveAll();

	StatusProgress.Lock( TRUE, "Processing import data" );
	ProcessImportDataInternal();
	StatusProgress.Unlock();
}

/**********************************************************************/

void CStockImportFilterWrapperDlg::ProcessImportDataInternal()
{
	int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );

	m_nLinesFound = 0;
	m_nExceptionLines = 0;
			
	int nDbIdx;
	dbDatabase.FindDatabaseByNumber ( dbStockpoint.GetDbNo( m_nSpIdx ), nDbIdx );

	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );
	strPath += "\\";		
	
	CString strImportReportFilename = dbStockpoint.GetFilePathAudit( m_nSpIdx, m_ImportDate, AUDIT_FAMILY_IMPORT );
	CString strExceptionsFilename = dbStockpoint.GetFilePathAudit( m_nSpIdx, m_ImportDate, AUDIT_FAMILY_PREV_EX );
	
	CFileRemover FileRemover;
	FileRemover.RemoveFile ( strImportReportFilename );
	FileRemover.RemoveFile ( strExceptionsFilename );

	CSSFile fileImportSource;
	CSSFile fileImportReport;
	CSSFile fileExceptions;
	
	if ( fileImportSource.Open ( m_strHeaderImportPath, "rb" ) == FALSE )
		return;

	if ( fileImportReport.Open ( strImportReportFilename, "wb" ) == FALSE )
		return;

	if ( fileExceptions.Open ( strExceptionsFilename, "wb" ) == FALSE )
		return;

	/*
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
	*/

	m_SuppRefFinder.Rebuild( m_nSuppNo );
			
	CString strBuffer;
	while ( fileImportSource.ReadString ( strBuffer ) == TRUE )  
	{				
		m_nLinesFound++;

		CCSV csvImport ( strBuffer );

		CString strWrapperCode = csvImport.GetString(0);
		double dDUQty = csvImport.GetDouble( 1 );
		double dTotalCost = csvImport.GetDouble( 2 );

		int nError = WRAPPER_ERROR_NONE;

		int nStockIdx, nStockSuppIdx;
		if ( m_SuppRefFinder.FindItemByWrapper( strWrapperCode, nStockIdx, nStockSuppIdx ) == TRUE )
		{
			if ( TRUE == m_bClosing )
				nError = CheckStockLevelStatusClosing( nStockIdx );
			else
				nError = CheckStockLevelStatusOpening( nStockIdx );
			
			if ( WRAPPER_ERROR_NONE == nError )
			{
				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt ( nStockIdx, StockRecord );
				double dSUQty = dDUQty * StockRecord.GetYield( nStockSuppIdx );
				m_arrayImport.AddItem( nStockIdx, nStockSuppIdx, dDUQty, dSUQty );
			}
			else
			{
				CString strStockCode = DataManager.Stock.GetStockCode( nStockIdx );
				AuditExceptions ( fileExceptions, csvImport, nError, strStockCode );
				m_nExceptionLines++;
			}
		}
		else
		{
			nError = WRAPPER_ERROR_NOT_FOUND;
			AuditExceptions ( fileExceptions, csvImport, nError, "" );
			m_nExceptionLines++;
		}

		AuditImportData( fileImportReport, csvImport, nError );
		
		StatusProgress.SetPos ( fileImportSource );
	}
}

/**********************************************************************/

void CStockImportFilterWrapperDlg::AuditExceptions( CSSFile& fileExceptions, CCSV& csvOriginalData, int nError, const char* szStockCode )
{
	CCSVStockAudit csvOut;
	csvOut.Add ( "D" );
	csvOut.Add ( m_nExceptionReportType );
	csvOut.Add ( 2 );
	csvOut.Add ( csvOriginalData.GetString(0) );
	csvOut.Add ( csvOriginalData.GetString(1) );
	csvOut.Add ( szStockCode );
	csvOut.Add ( GetErrorText( nError ) );
	csvOut.Add ( "" );		//IGNORE SALE DATE
	csvOut.Add ( "" );		//IGNORE SALE TIME
	csvOut.Add ( TRUE );	//LINE IS WRAPPER
	fileExceptions.WriteLine( csvOut.GetLine() );
}

/**********************************************************************/

void CStockImportFilterWrapperDlg::AuditImportData ( CSSFile& fileImportReport, CCSV& csvOriginalData, int nError )
{	
	CCSVStockAudit csvOut;
	csvOut.Add ( "D" );
	csvOut.Add ( m_nImportFileReportType );
	csvOut.Add ( 1 );
	csvOut.Add ( csvOriginalData.GetString(0) );
	csvOut.Add ( csvOriginalData.GetString(1) );
	csvOut.Add ( csvOriginalData.GetString(2) );
	csvOut.Add ( GetErrorText( nError ) );

	fileImportReport.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/

const char* CStockImportFilterWrapperDlg::GetErrorText( int nError )
{
	m_strError = "";
	switch( nError )
	{
	case WRAPPER_ERROR_NONE:		
		m_strError = "";								
		break;

	case WRAPPER_ERROR_IN_STOCKTAKE:
		m_strError = "Stockcode in pending stock take";	
		break;

	case WRAPPER_ERROR_WRONG_STOCKTAKE:		
		m_strError = "Stockcode not in this stock take";	
		break;

	case WRAPPER_ERROR_STOCK_INACTIVE:		
		m_strError = "Stockitem not active at this stockpoint";	
		break;

	case WRAPPER_ERROR_NOT_FOUND:
		m_strError = "Unknown wrapper code";				
		break;

	default:	
		m_strError = "Unknown error";						
		break;
	}
	return m_strError;
}

/**********************************************************************/

void CStockImportFilterWrapperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockImportFilterWrapperDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_STATIC_LINES, m_staticLines);
	DDX_Control( pDX, IDC_STATIC_EXCEPTIONS, m_staticExceptions);
	DDX_Control( pDX, IDC_STATIC_SELECTED, m_staticSelected);
	DDX_Control( pDX, IDC_BUTTON_EXCEPTIONS, m_buttonExceptions);
	DDX_Control( pDX, IDC_BUTTON_PREVIEW, m_buttonPreview);
	DDX_Control( pDX, IDC_BUTTON_SELECT, m_buttonSelect);
	DDX_Control( pDX, IDC_RADIO1, m_radio1 );
	DDX_Control( pDX, IDC_RADIO2, m_radio2 );
	DDX_Control( pDX, IDC_RADIO3, m_radio3 );
	DDX_Control( pDX, IDOK, m_buttonOK );
	DDX_Control( pDX, IDCANCEL, m_buttonCancel );
	DDX_Radio( pDX, IDC_RADIO1, m_nAction );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockImportFilterWrapperDlg, CDialog)
	//{{AFX_MSG_MAP(CStockImportFilterWrapperDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON_IMPORT, OnButtonImport )
	ON_BN_CLICKED( IDC_BUTTON_EXCEPTIONS, OnButtonExceptions )
	ON_BN_CLICKED( IDC_BUTTON_PREVIEW, OnButtonPreview )
	ON_BN_CLICKED( IDC_BUTTON_SELECT, OnButtonSelect )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockImportFilterWrapperDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText( m_strTitle );

	CString strLines;
	strLines.Format( "%d", m_nLinesFound );
	m_staticLines.SetWindowText( strLines );

	CString strExceptions;
	strExceptions.Format( "%d", m_nExceptionLines );
	m_staticExceptions.SetWindowText( strExceptions );

	m_radio1.SetWindowText( m_strRadio1 );
	m_radio2.SetWindowText( m_strRadio2 );
	m_radio3.SetWindowText( m_strRadio3 );

	if ( m_arrayImport.GetSize() == 0 )
	{
		m_buttonOK.ShowWindow( SW_HIDE );
		m_buttonOK.EnableWindow( FALSE );
		m_buttonPreview.ShowWindow( SW_HIDE );
		m_buttonPreview.EnableWindow( FALSE );
		m_buttonSelect.ShowWindow( SW_HIDE );
		m_buttonSelect.EnableWindow( FALSE );
		m_buttonCancel.SetWindowText( "Close" );
		m_radio1.EnableWindow( FALSE );
		m_radio2.EnableWindow( FALSE );
		m_radio3.EnableWindow( FALSE );
	}

	if ( 0 == m_nExceptionLines )
	{
		m_buttonExceptions.ShowWindow( SW_HIDE );
		m_buttonExceptions.EnableWindow( FALSE );
	}

	UpdateSelectionCount();

	return TRUE;  
}

/**********************************************************************/

void CStockImportFilterWrapperDlg::UpdateSelectionCount()
{
	CString str;
	str.Format( "%d of %d",
		m_arrayImport.GetSelectionCount(),
		m_arrayImport.GetSize() );

	m_staticSelected.SetWindowText( str );
}

/**********************************************************************/

void CStockImportFilterWrapperDlg::OnButtonImport()
{
	CStockAuditImportWrapper Audit;
	
	Audit.SetReportType ( m_nImportFileReportType );
	Audit.SetConLevel ( NODE_STOCKPOINT );			
	Audit.SetTableNo ( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode ( AUDIT_MODE_IMPORT );
	
	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
	info.m_strSpName1 = m_strHeaderSpName1;
	info.m_strSpName2 = m_strHeaderSpName2;
	info.m_strImportPath = m_strHeaderImportPath;	
	Audit.SetPreviewHeader ( Audit.GetHeaderLine( info ) );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CStockImportFilterWrapperDlg::OnButtonExceptions()
{
	CStockAuditExceptions Audit( m_nExceptionReportType );
	Audit.SetConLevel ( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_PREVIEW );
	
	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
	info.m_strReference1 = m_strHeaderReference1;
	info.m_strReference2 = m_strHeaderReference2;
	Audit.SetPreviewHeader( Audit.GetHeaderLine( info ) );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CStockImportFilterWrapperDlg::UpdateImportArray()
{
	int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );

	StatusProgress.Lock( TRUE, "Checking Current Stock Levels" );

	int nLastStockIdx = -1;
	double dLastStockQty = 0.0;

	for ( int n = 0; n < m_arrayImport.GetSize(); n++ )
	{
		StatusProgress.SetPos( n, m_arrayImport.GetSize() );

		CStockImportFilterItem item;
		m_arrayImport.GetItem( n, item );

		item.m_bSelectBuffer = item.m_bSelected;

		if ( TRUE == item.m_bSelected )
		{
			int nStockIdx = item.m_nStockIdx;
			if ( nStockIdx == nLastStockIdx )
			{
				item.m_dCurrentSUQty = dLastStockQty;
			}
			else
			{
				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt( nStockIdx, StockRecord );

				CStockLevelsCSVRecord StockLevels;
				DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );

				if ( TRUE == m_bClosing )
					item.m_dCurrentSUQty = StockLevels.GetStocktakeClosingQty();
				else
					item.m_dCurrentSUQty = StockLevels.GetOpeningQty();
				
				nLastStockIdx = nStockIdx;
			}

			switch( m_nAction )
			{
			case OVERWRITE_STOCK_LEVELS:
				dLastStockQty = item.m_dImportSUQty;
				break;

			case SUBTRACT_STOCK_LEVELS:
				dLastStockQty = item.m_dCurrentSUQty - item.m_dImportSUQty;
				break;

			case ADD_STOCK_LEVELS:
			default:
				dLastStockQty = item.m_dCurrentSUQty + item.m_dImportSUQty;
				break;
			}
		}
		
		m_arrayImport.SetItem( n, item );
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

void CStockImportFilterWrapperDlg::OnButtonPreview()
{
	if ( UpdateData( TRUE ) == FALSE )
		return;
	
	CSSFile fileAudit;
	CString strAuditFilename = dbStockpoint.GetFilePathAudit( m_nSpIdx, m_ImportDate, AUDIT_FAMILY_PREV );
	if ( fileAudit.Open( strAuditFilename, "wb" ) == FALSE )
	{
		Prompter.Error( "Unable to create audit file" );
		return;
	}

	UpdateImportArray();
	
	for ( int nIndex = 0; nIndex < m_arrayImport.GetSize(); nIndex++ )
	{
		CStockImportFilterItem item;
		m_arrayImport.GetItem( nIndex, item );

		if ( FALSE == item.m_bSelected )
			continue;

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( item.m_nStockIdx, StockRecord );

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( m_nAuditReportType );
		csvOut.Add ( 2 );
		csvOut.Add ( StockRecord.GetStockCode() );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csvOut.Add ( DataManager.Stock.GetSubUnits( StockRecord ) );
		csvOut.Add ( m_nAction );
		csvOut.AddStockAuditQty ( 0.0 );
		csvOut.AddStockAuditQty ( item.m_dImportSUQty );
		csvOut.AddStockAuditQty ( item.GetNewStockLevel( m_nAction ) );
		fileAudit.WriteLine ( csvOut.GetLine() );
	}

	fileAudit.Close();

	CStockAuditOpeningClosing Audit;
	Audit.SetReportType ( m_nAuditReportType );
	Audit.SetConLevel ( NODE_STOCKPOINT );
	Audit.SetConLevelText( dbStockpoint.GetName( m_nSpIdx ) );
	Audit.SetTableNo( dbStockpoint.GetSpNo( m_nSpIdx ) );
	Audit.SetAuditMode( AUDIT_MODE_PREVIEW );

	CStockAuditHeaderInfo info;
	info.m_date = m_ImportDate;
	Audit.SetPreviewHeader( Audit.GetHeaderLine( info ) );

	if ( Audit.CreateReport( DataManager.GetActiveDbIdx() ) )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), Audit.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CStockImportFilterWrapperDlg::OnButtonSelect() 
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		UpdateImportArray();
		
		m_arrayImport.ClearSelectionModifiedFlag();
		CStockImportFilterListDlg dlgList( m_nSpIdx, m_bClosing ? NODE_CLOSING : NODE_OPENING, m_nAction, m_arrayImport, this );
		
		if ( dlgList.DoModal() != IDOK )
		{
			for ( int n = 0; n < m_arrayImport.GetSize(); n++ )
			{
				CStockImportFilterItem item;
				m_arrayImport.GetItem( n, item );
				item.m_bSelected = item.m_bSelectBuffer;
				m_arrayImport.SetItem( n, item );
			}
		}

		UpdateSelectionCount();
	}
}

/**********************************************************************/

int CStockImportFilterWrapperDlg::CheckStockLevelStatusOpening( int nStockIdx )
{
	CStockCSVRecord StockRecord;
	CStockLevelsCSVRecord StockLevels;
	
	int nError = WRAPPER_ERROR_NONE;

	switch( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, dbStockpoint.GetSpNo( m_nSpIdx ), 0 ) )
	{
	case STOCKLEVEL_IMPORT_REJECT_STOCKTAKE:
		nError = WRAPPER_ERROR_IN_STOCKTAKE;
		break;

	case STOCKLEVEL_IMPORT_REJECT_INACTIVE:
		nError = WRAPPER_ERROR_STOCK_INACTIVE;
		break;
	}

	return nError;
}

/**********************************************************************/

int CStockImportFilterWrapperDlg::CheckStockLevelStatusClosing( int nStockIdx )
{
	CStockCSVRecord StockRecord;
	CStockLevelsCSVRecord StockLevels;
	
	int nError = WRAPPER_ERROR_NONE;

	switch( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, dbStockpoint.GetSpNo( m_nSpIdx ), m_nStocktakeNo ) )
	{
	case STOCKLEVEL_IMPORT_REJECT_STOCKTAKE:
		nError = WRAPPER_ERROR_WRONG_STOCKTAKE;
		break;

	case STOCKLEVEL_IMPORT_REJECT_INACTIVE:
		nError = WRAPPER_ERROR_STOCK_INACTIVE;
		break;
	}

	return nError;
}

/**********************************************************************/

void CStockImportFilterWrapperDlg::OnOK() 
{
	if ( UpdateData( TRUE ) == TRUE )
		EndDialog( IDOK );
}

/**********************************************************************/

void CStockImportFilterWrapperDlg::OnCancel() 
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/

