/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ExplodePlu.h"
#include "FileRemover.h"
#include "NodeTypes.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockAuditBaseReport.h"
#include "StockAuditExceptions.h"
#include "StockAuditImportPlu.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockImportBasicDlg.h"
/**********************************************************************/

CStockImportBasicDlg::CStockImportBasicDlg(int nSpIdx, const char* szPath, CWnd* pParent )
	: CDialog(CStockImportBasicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockImportBasicDlg)
	m_strExceptions = _T("");
	//}}AFX_DATA_INIT
	m_nSpIdx = nSpIdx;
	m_strHeaderImportPath = szPath;
	m_strHeaderReference1 = "";
	m_strHeaderReference2 = "";
	m_strHeaderSuppName = "";
	m_strHeaderSpName1 = dbStockpoint.GetName( m_nSpIdx );
	m_strHeaderSpName2 = "";
	m_ImportDate = COleDateTime::GetCurrentTime();
	m_nStocktakeNo = -1;
	m_nAction = ADD_STOCK_LEVELS;
}

/**********************************************************************/

CStockImportBasicDlg::~CStockImportBasicDlg()
{
}

/**********************************************************************/

void CStockImportBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockImportBasicDlg)
	DDX_Control(pDX, IDC_COMBO_ACTION, m_comboAction);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_buttonClose);
	DDX_Control(pDX, IDC_COMBO_REPORT, m_comboReport);
	DDX_Text(pDX, IDC_STATIC_LINES, m_strLinesFound);
	DDX_Text(pDX, IDC_STATIC_IMPORT, m_strImport);
	DDX_Text(pDX, IDC_STATIC_EXCEPTIONS, m_strExceptions);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockImportBasicDlg, CDialog)
	//{{AFX_MSG_MAP(CStockImportBasicDlg)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockImportBasicDlg::OnInitDialog() 
{
	if ( m_nLinesFound == m_nExceptionLines )
		m_strImport = "";

	CDialog::OnInitDialog();

	if ( m_nLinesFound != m_nExceptionLines )
	{
		m_comboReport.AddString ( "Imported Data" );
		m_comboReport.AddString ( "Stock Changes" );
		m_comboReport.AddString ( "Exceptions" );
	}
	else
	{
		m_comboReport.AddString ( "Imported Data" );
		m_comboReport.AddString ( "Exceptions" );
	}
	
	m_comboReport.SetCurSel(0);
	
	SetWindowText ( m_strTitle );

	if ( m_nLinesFound != m_nExceptionLines )
		m_buttonClose.ShowWindow( SW_HIDE );
	else
	{
		m_buttonOK.ShowWindow( SW_HIDE );
		m_buttonCancel.ShowWindow( SW_HIDE );
	}

	FillActionCombo();

	return TRUE;  
}

/**********************************************************************/

void CStockImportBasicDlg::OnButtonDisplay() 
{
	if ( m_nLinesFound != m_nExceptionLines )
	{
		switch ( m_comboReport.GetCurSel() )
		{
		case 0:	
			ShowImportFile();		
			break;
		
		case 1:	
			if ( UpdateAction() == TRUE ) 
				ProcessImportData( m_nAction );

			ShowStockChanges();		
			break;
		
		case 2:	
			ShowExceptions();		
			break;
		}
	}
	else
		switch ( m_comboReport.GetCurSel() )
		{
		case 0:	
			ShowImportFile();		
			break;
		
		case 1:	
			ShowExceptions();		
			break;
		}
}

/**********************************************************************/

bool CStockImportBasicDlg::UpdateAction() 
{
	int nSel = m_comboAction.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboAction.GetCount() )
	{
		int nAction = m_comboAction.GetItemData( nSel );
		if ( nAction != m_nAction )
		{
			m_nAction = nAction;
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/

void CStockImportBasicDlg::ShowImportFile()
{
	CStockAuditImportPlu Audit;
	
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

void CStockImportBasicDlg::ShowExceptions()
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

void CStockImportBasicDlg::AuditExceptions( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData )
{
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvProblem ( StockDetails.GetAt ( nIndex ) );

		CString strProblem = csvProblem.GetProblem();
		__int64 nPluOrBarcodeNo = csvProblem.GetPluNo();

		if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		{
			int nBarcodeIdx;
			__int64 nBarcodeNo = csvOriginalData.GetInt64(0);
			if ( DataManager.Barcode.FindBarcodeByNumber( nBarcodeNo, nBarcodeIdx ) == FALSE )
			{
				strProblem = "Barcode not found";
				nPluOrBarcodeNo = nBarcodeNo;
			}
		}
		
		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( m_nExceptionReportType );
		csvOut.Add ( 2 );
		csvOut.Add ( nPluOrBarcodeNo );
		csvOut.AddStockAuditQty ( csvProblem.GetPluQty() );
		csvOut.Add ( csvProblem.GetStockCode() );
		csvOut.Add ( strProblem );
		csvOut.Add ( "" );		//IGNORE SALE DATE
		csvOut.Add ( "" );		//IGNORE SALE TIME
		csvOut.Add ( FALSE );	//NOT A WRAPPER
		file.WriteLine( csvOut.GetLine() );

		csvOut.RemoveAll();
		csvOut.Add ( "D" );
		csvOut.Add ( m_nImportFileReportType );

		if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		{
			csvOut.Add ( 2 );
			csvOut.Add ( csvOriginalData.GetString(0) );
			csvOut.Add ( csvOriginalData.GetString(1) );
			csvOut.Add ( csvProblem.GetDescription() );
			csvOut.Add ( csvOriginalData.GetString(3) );
			csvOut.Add ( csvOriginalData.GetString(4) );		
			csvOut.Add ( strProblem );
		}
		else
		{
			csvOut.Add ( 1 );
			csvOut.Add ( csvOriginalData.GetString(0) );
			csvOut.Add ( csvProblem.GetDescription() );
			csvOut.Add ( csvOriginalData.GetString(2) );
			csvOut.Add ( csvOriginalData.GetString(3) );		
			csvOut.Add ( strProblem );
		}

		fileTempReport.WriteLine ( csvOut.GetLine() );
	}
}

/**********************************************************************/

void CStockImportBasicDlg::ProcessImportData( int nAction )
{
	StatusProgress.Lock( TRUE, "Processing import data" );
	ProcessImportDataInternal( nAction );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CStockImportBasicDlg::ProcessImportDataInternal( int nAction )
{
	int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );
	int nSpNo2 = GetSpNo2();

	m_nLinesFound = 0;
	m_nExceptionLines = 0;
	m_nTotalExceptions = 0;
			
	int nDbIdx;
	dbDatabase.FindDatabaseByNumber ( dbStockpoint.GetDbNo( m_nSpIdx ), nDbIdx );

	CString strPath = dbStockpoint.GetFolderPath( m_nSpIdx );
	strPath += "\\";		
	
	CString strImportReportFilename = dbStockpoint.GetFilePathAudit( m_nSpIdx, m_ImportDate, AUDIT_FAMILY_IMPORT );
	CString strStockChangesFilename = dbStockpoint.GetFilePathAudit( m_nSpIdx, m_ImportDate, AUDIT_FAMILY_PREV );
	CString strExceptionsFilename = dbStockpoint.GetFilePathAudit( m_nSpIdx, m_ImportDate, AUDIT_FAMILY_PREV_EX );
	
	CFileRemover FileRemover;
	FileRemover.RemoveFile ( strImportReportFilename );
	FileRemover.RemoveFile ( strStockChangesFilename );
	FileRemover.RemoveFile ( strExceptionsFilename );

	CSSFile fileImportSource;
	CSSFile fileImportReport;
	CSSFile fileStockChanges;
	CSSFile fileExceptions;
	
	if ( fileImportSource.Open ( m_strHeaderImportPath, "rb" ) == FALSE )
		return;

	if ( fileImportReport.Open ( strImportReportFilename, "wb" ) == FALSE )
		return;

	if ( fileStockChanges.Open ( strStockChangesFilename, "wb" ) == FALSE )
		return;

	if ( fileExceptions.Open ( strExceptionsFilename, "wb" ) == FALSE )
		return;

	/*
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
	}
	*/
				
	CString strBuffer;
	while ( fileImportSource.ReadString ( strBuffer ) == TRUE )  
	{			
		m_nLinesFound++;

		CStringArray StockDetails;
		CCSV csvImport ( strBuffer );

		CString strPluNo; 
		CCSV csvOriginalData;	
		double dPluQty, dPluValue;

		GetImportLineInfo( csvImport, strPluNo, csvOriginalData, dPluQty, dPluValue );
		
		int nExplodeResult;
		if ( -1 == m_nStocktakeNo )
		{
			CExplodePluStockLevel ExplodePlu( nSpNo, nSpNo2 );
			nExplodeResult = ExplodePlu.ExplodePlu ( _atoi64(strPluNo), StockDetails, m_nStockChangesReportType, dPluQty, dPluValue ); 		
		}
		else
		{
			CExplodePluStockTake ExplodePlu( nSpNo, m_nStocktakeNo );
			nExplodeResult = ExplodePlu.ExplodePlu ( _atoi64(strPluNo), StockDetails, m_nStockChangesReportType, dPluQty, dPluValue );
		}

		switch ( nExplodeResult )
		{
		case PLU_EXCEPTION_NONE:
			AuditStockChanges ( fileStockChanges, fileImportReport, StockDetails, csvOriginalData, nAction );
			break;

		case PLU_EXCEPTION_PLUNONSTOCK:
			AuditIgnored ( fileImportReport, StockDetails, csvOriginalData );
			break;

		case PLU_EXCEPTION_PLUNOTEXIST:
		case PLU_EXCEPTION_PLUISRECIPE:
		case PLU_EXCEPTION_STOCKNOTEXIST:
		case PLU_EXCEPTION_STOCKTAKE:
		case PLU_EXCEPTION_STOCKINACTIVE:
			AuditExceptions ( fileExceptions, fileImportReport, StockDetails, csvOriginalData );
			m_nTotalExceptions += StockDetails.GetSize();
			m_nExceptionLines++;
			break;

		case PLU_EXCEPTION_PLUNOTLINKED:
			if ( ( m_nStockChangesReportType != AUDIT_SALES ) || ( StockOptions.GetImportSalesExceptionFlag() ) )
			{
				AuditExceptions ( fileExceptions, fileImportReport, StockDetails, csvOriginalData );
				m_nTotalExceptions += StockDetails.GetSize();
				m_nExceptionLines++;
			}
			else
				AuditIgnored ( fileImportReport, StockDetails, csvOriginalData );

			break;
		}

		StatusProgress.SetPos ( fileImportSource );
	}
	m_strLinesFound.Format ( "%d", m_nLinesFound );
	m_strExceptions.Format ( "%d", m_nTotalExceptions );
}

/**********************************************************************/

void CStockImportBasicDlg::GetImportLineInfo( CCSV& csvImport, CString& strPluNo, CCSV& csvOriginalData, double& dPluQty, double& dPluValue )
{
	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
	{
		CString strBarcodeNo = csvImport.GetString(0);
		
		__int64 nPluNo = DataManager.Barcode.GetPluFromBarcode( _atoi64 ( strBarcodeNo ) );
		::FormatPluNo( nPluNo, strPluNo );
		
		dPluQty = csvImport.GetDouble( 1 );
		dPluValue = csvImport.GetDouble( 2 );

		CString strOriginalPluNo = strPluNo;
		::TrimSpaces( strOriginalPluNo, TRUE );

		csvOriginalData.RemoveAll();
		csvOriginalData.Add( strBarcodeNo );
		csvOriginalData.Add( strOriginalPluNo );
		csvOriginalData.Add( "" );
		csvOriginalData.Add( csvImport.GetString(1) );
		csvOriginalData.Add( csvImport.GetString(2) );
	}
	else
	{
		strPluNo = csvImport.GetString ( 0 );
		::AddLeading ( strPluNo, Super.MaxPluLen(), '0' );
		dPluQty = csvImport.GetDouble( 1 );
		dPluValue = csvImport.GetDouble( 2 );

		csvOriginalData.RemoveAll();
		csvOriginalData.Add( csvImport.GetString(0) );
		csvOriginalData.Add( "" );
		csvOriginalData.Add( csvImport.GetString(1) );
		csvOriginalData.Add( csvImport.GetString(2) );
	}
}

/**********************************************************************/

void CStockImportBasicDlg::OnOK() 
{
	if ( m_nLinesFound != m_nExceptionLines )
	{
		UpdateAction();
		EndDialog ( IDOK );
	}
	else
		EndDialog ( IDCANCEL );
}

/**********************************************************************/

void CStockImportBasicDlg::OnButtonClose() 
{
	EndDialog ( IDCANCEL );
}

/**********************************************************************/

