/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ExplodePlu.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockAuditBaseReport.h"
#include "StockOptionsIni.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockInfoMinimumDlg.h"
/**********************************************************************/

CStockInfoMinimumDlg::CStockInfoMinimumDlg( int nSpIdx, CReorderImportHelper& ImportHelper, CSuppRefFinder& SuppRefFinder, CWnd* pParent )
	: CDialog(CStockInfoMinimumDlg::IDD, pParent), m_ImportHelper( ImportHelper), m_SuppRefFinder( SuppRefFinder )
{
	//{{AFX_DATA_INIT(CStockInfoMinimumDlg)
	//}}AFX_DATA_INIT
	m_nSpIdx = nSpIdx;
	m_strImportPath = "";
	m_bGotData = FALSE;
	m_nLinesFound = 0;
	m_nExceptionLines = 0;
	m_bAllowReorder = ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES );
}

/**********************************************************************/

void CStockInfoMinimumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockInfoMinimumDlg)
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_buttonClose);
	DDX_Control(pDX, IDC_STATIC_IMPORT, m_staticImport);
	DDX_Control(pDX, IDC_STATIC_LINES, m_staticLines);
	DDX_Control(pDX, IDC_STATIC_EXCEPTIONS, m_staticExceptions);
	DDX_Control(pDX, IDC_COMBO_REPORT, m_comboReport);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockInfoMinimumDlg, CDialog)
	//{{AFX_MSG_MAP(CStockInfoMinimumDlg)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockInfoMinimumDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText ( GetTitle() );

	m_comboReport.AddString( "Stock changes" );
	m_comboReport.AddString( "Exceptions" );
	
	if ( m_nLinesFound > m_nExceptionLines )
	{
		m_comboReport.SetCurSel(0);
		m_buttonClose.ShowWindow( SW_HIDE );
	}
	else
	{
		m_comboReport.SetCurSel(1);
		m_comboReport.EnableWindow( FALSE );
		m_buttonOK.ShowWindow( SW_HIDE );
		m_buttonCancel.ShowWindow( SW_HIDE );
		m_staticImport.ShowWindow( SW_HIDE );
	}

	m_staticLines.SetWindowText( m_strLinesFound );
	m_staticExceptions.SetWindowText( m_strExceptions );

	return TRUE;  
}

/**********************************************************************/

void CStockInfoMinimumDlg::ProcessImportData( const char* szPath )
{
	m_strImportPath = szPath;
	
	m_nLinesFound = 0;
	m_nExceptionLines = 0;
	
	StatusProgress.Lock( TRUE, "Processing import data" );
	
	if ( m_ImportHelper.IsWrapperImport() == FALSE )
		ProcessImportDataPlu();
	else
		ProcessImportDataWrapper();

	StatusProgress.Unlock();

	m_strLinesFound.Format ( "%d", m_nLinesFound );
	m_strExceptions.Format ( "%d", m_nExceptionLines );
}

/**********************************************************************/

void CStockInfoMinimumDlg::ProcessImportDataPlu()
{	
	int nDbIdx;
	dbDatabase.FindDatabaseByNumber ( dbStockpoint.GetDbNo( m_nSpIdx ), nDbIdx );

	CSSFile fileImportSource;	
	if ( fileImportSource.Open ( m_strImportPath, "rb" ) == FALSE )
		return;

	//SKIP HEADER
	CString strBuffer;
	fileImportSource.ReadString( strBuffer );

	while ( fileImportSource.ReadString ( strBuffer ) == TRUE )  
	{				
		m_nLinesFound++;

		CCSV csv ( strBuffer );

		__int64 nPluNo;
		if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
			nPluNo = DataManager.Barcode.GetPluFromBarcode( _atoi64 ( csv.GetString(0) ) ); 
		else
			nPluNo = csv.GetInt64 ( 0 );

		CExplodePluStockLevel ExplodePlu( dbStockpoint.GetSpNo(m_nSpIdx) );
		
		CStringArray StockDetails;
		if ( ExplodePlu.ExplodePlu( nPluNo, StockDetails, AUDIT_TRANSFER, 1.0, 1.0 ) != PLU_EXCEPTION_NONE )
			m_nExceptionLines++;

		StatusProgress.SetPos ( fileImportSource );
	}
}

/**********************************************************************/

void CStockInfoMinimumDlg::ProcessImportDataWrapper()
{
	int nDbIdx;
	dbDatabase.FindDatabaseByNumber ( dbStockpoint.GetDbNo( m_nSpIdx ), nDbIdx );

	CSSFile fileImportSource;	
	if ( fileImportSource.Open ( m_strImportPath, "rb" ) == FALSE )
		return;

	//SKIP HEADER
	CString strBuffer;
	fileImportSource.ReadString( strBuffer );

	while ( fileImportSource.ReadString ( strBuffer ) == TRUE )  
	{
		m_nLinesFound++;

		CCSV csvIn ( strBuffer );
		CString strWrapper = csvIn.GetString ( 0 );
		
		int nStockIdx, nStockSuppIdx;
		if ( m_SuppRefFinder.FindItemByWrapper( strWrapper, nStockIdx, nStockSuppIdx ) == FALSE )
			m_nExceptionLines++;
		else
		{
			CStockCSVRecord StockRecord;
			CStockLevelsCSVRecord StockLevels;
			if ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, dbStockpoint.GetSpNo(m_nSpIdx), -1 ) != STOCKLEVEL_IMPORT_ACCEPT )
				m_nExceptionLines++;
		}

		StatusProgress.SetPos ( fileImportSource );
	}
}

/**********************************************************************/

void CStockInfoMinimumDlg::OnButtonDisplay() 
{
	if ( m_ImportHelper.IsWrapperImport() == FALSE )
	{
		if ( m_comboReport.GetCurSel() == 0 )	DisplayPluReport();
		if ( m_comboReport.GetCurSel() == 1 )	DisplayPluExceptions();
	}
	else
	{
		if ( m_comboReport.GetCurSel() == 0 )	DisplayWrapperReport();
		if ( m_comboReport.GetCurSel() == 1 )	DisplayWrapperExceptions();
	}
}

/**********************************************************************/

void CStockInfoMinimumDlg::DisplayPluReport()
{
	bool bShowMinimum = m_ImportHelper.GotMinField();
	bool bShowReorder = m_ImportHelper.GotReorderField();
	bool bShowMaximum = m_ImportHelper.GotMaxField();

	int nDbIdx;
	dbDatabase.FindDatabaseByNumber ( dbStockpoint.GetDbNo( m_nSpIdx ), nDbIdx );

	CSSFile fileImportSource;	
	if ( fileImportSource.Open ( m_strImportPath, "rb" ) == FALSE )
		return;

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		ReportFile.SetStyle1 ( GetTitle() );
		ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
		ReportFile.AddColumn ( "Description", TA_LEFT, 300 );
		ReportFile.AddColumn ( "Category", TA_LEFT, 250 );
		if ( bShowMinimum == TRUE )	ReportFile.AddColumn ( "Minimum", TA_RIGHT, 200 );
		if ( bShowReorder == TRUE )	ReportFile.AddColumn ( "Reorder", TA_RIGHT, 200 );
		if ( bShowMaximum == TRUE )	ReportFile.AddColumn ( "Maximum", TA_RIGHT, 200 );

		StatusProgress.Lock( TRUE, "Creating report" );

		//SKIP HEADER
		CString strBuffer;
		fileImportSource.ReadString( strBuffer );

		while ( fileImportSource.ReadString ( strBuffer ) == TRUE )  
		{
			StatusProgress.SetPos ( fileImportSource );

			CCSV csv ( strBuffer );
			
			__int64 nPluNo;
			if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
				nPluNo = DataManager.Barcode.GetPluFromBarcode( _atoi64 ( csv.GetString(0) ) );
			else
				nPluNo = csv.GetInt64( 0 );

			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == FALSE )
				continue;

			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt ( nPluIdx, PluRecord );
			if ( DataManager.Plu.IsRecipe ( PluRecord.GetPluNoInt() ) )
				continue;
		
			CString strStockCode;
			DataManager.Plu.GetPluStockCode ( PluRecord.GetPluNoInt(), strStockCode, FALSE );
			if ( strStockCode == "" )
				continue;
		
			int nStockIdx;
			if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
				continue;

			CStockCSVRecord StockRecord;
			CStockLevelsCSVRecord StockLevels;
			if ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, dbStockpoint.GetSpNo(m_nSpIdx), -1 ) != STOCKLEVEL_IMPORT_ACCEPT )  
				continue;

			double dStockQtyPerPlu;
			DataManager.Plu.GetPluStockQty ( PluRecord.GetPluNoInt(), dStockQtyPerPlu );

			CCSV csvOut ( '\t' );
			csvOut.Add( StockRecord.GetStockCode() );
			csvOut.Add( StockRecord.GetDescription() );
			csvOut.Add( DataManager.Category.GetText( StockRecord.GetCategory() ) );

			if ( TRUE == bShowMinimum )
			{
				double dMin = m_ImportHelper.GetMinQty( csv );
				csvOut.Add( DataManager.Stock.GetStockLevelString( StockRecord, dMin * dStockQtyPerPlu ) );
			}

			if ( TRUE == bShowReorder )
			{
				double dReorder = m_ImportHelper.GetReorderQty( csv );
				csvOut.Add( DataManager.Stock.GetStockLevelString( StockRecord, dReorder * dStockQtyPerPlu ) );
			}

			if ( TRUE == bShowMaximum )
			{
				double dMax = m_ImportHelper.GetMaxQty( csv );
				csvOut.Add( DataManager.Stock.GetStockLevelString( StockRecord, dMax * dStockQtyPerPlu ) );
			}
			
			ReportFile.WriteLine( csvOut.GetLine() );
		}

		ReportFile.WriteLine( "" );
		StatusProgress.Unlock();
		ReportFile.Close();
	}

	CString strKey = "1";
	strKey += ( bShowMinimum ) ? "1" : "0";
	strKey += ( bShowReorder ) ? "1" : "0";
	strKey += ( bShowMaximum ) ? "1" : "0";
	
	CReportManager ReportManager;
	CFilenameUpdater FnUp( SysFiles::MinLevelImport );
	ReportManager.DisplayReport( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, FALSE );
}

/**********************************************************************/

void CStockInfoMinimumDlg::DisplayPluExceptions()
{
	bool bShowMinimum = m_ImportHelper.GotMinField();
	bool bShowReorder = m_ImportHelper.GotReorderField();
	bool bShowMaximum = m_ImportHelper.GotMaxField();
	
	int nDbIdx;
	dbDatabase.FindDatabaseByNumber ( dbStockpoint.GetDbNo( m_nSpIdx ), nDbIdx );

	CSSFile fileImportSource;	
	if ( fileImportSource.Open ( m_strImportPath, "rb" ) == FALSE )
		return;

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		ReportFile.SetStyle1 ( GetTitle() );

		if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
			ReportFile.AddColumn ( "Barcode No", TA_LEFT, 350 );

		ReportFile.AddColumn ( "Plu No", TA_LEFT, 350 );
		if ( bShowMinimum == TRUE )	ReportFile.AddColumn ( "Minimum", TA_RIGHT, 200 );
		if ( bShowReorder == TRUE )	ReportFile.AddColumn ( "Reorder", TA_RIGHT, 200 );
		if ( bShowMaximum == TRUE )	ReportFile.AddColumn ( "Maximum", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
		ReportFile.AddColumn ( "Problem", TA_LEFT, 600 );

		StatusProgress.Lock( TRUE, "Creating report" );

		//SKIP HEADER
		CString strBuffer;
		fileImportSource.ReadString( strBuffer );

		while ( fileImportSource.ReadString ( strBuffer ) == TRUE )  
		{
			StatusProgress.SetPos ( fileImportSource );

			CCSV csv ( strBuffer );
			
			__int64 nBarcodeNo, nPluNo;
			if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
			{
				nBarcodeNo = csv.GetInt64(0);
				nPluNo = DataManager.Barcode.GetPluFromBarcode( nBarcodeNo );
			}
			else
			{
				nBarcodeNo = 0;
				nPluNo = csv.GetInt64( 0 );
			}

			CExplodePluStockLevel ExplodePlu( dbStockpoint.GetSpNo( m_nSpIdx ) );
			
			CStringArray StockDetails;
			if ( ExplodePlu.ExplodePlu( nPluNo, StockDetails, AUDIT_TRANSFER, 1.0, 0.0 ) == PLU_EXCEPTION_NONE )
				continue;

			if ( StockDetails.GetSize() == 0 )
				continue;

			CCSVExplodedPlu csvInfo( StockDetails.GetAt(0) );

			CString strProblem = csvInfo.GetProblem();
			if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
			{
				int nBarcodeIdx;
				if ( DataManager.Barcode.FindBarcodeByNumber( nBarcodeNo, nBarcodeIdx ) == FALSE )
					strProblem = "Barcode not found";
			}
			
			CCSV csvOut('\t');

			if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
				csvOut.Add( nBarcodeNo );

			csvOut.Add( csvInfo.GetPluNo() );
			if ( TRUE == bShowMinimum )	csvOut.Add( m_ImportHelper.GetMinQty( csv ),  SysInfo.GetDPQty() );
			if ( TRUE == bShowReorder )	csvOut.Add( m_ImportHelper.GetReorderQty( csv ), SysInfo.GetDPQty() );
			if ( TRUE == bShowMaximum )	csvOut.Add( m_ImportHelper.GetMaxQty( csv ), SysInfo.GetDPQty() );
			csvOut.Add( csvInfo.GetStockCode() );
			csvOut.Add( strProblem );
			ReportFile.WriteLine( csvOut.GetLine() );
		}

		ReportFile.WriteLine( "" );
		StatusProgress.Unlock();
		ReportFile.Close();
	}
	
	CString strKey = "2";
	strKey += ( bShowMinimum ) ? "1" : "0";
	strKey += ( bShowReorder ) ? "1" : "0";
	strKey += ( bShowMaximum ) ? "1" : "0";
	
	CReportManager ReportManager;
	CFilenameUpdater FnUp( SysFiles::MinLevelImport );
	ReportManager.DisplayReport( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, FALSE );
}

/**********************************************************************/

void CStockInfoMinimumDlg::DisplayWrapperReport()
{
	bool bShowMinimum = m_ImportHelper.GotMinField();
	bool bShowReorder = m_ImportHelper.GotReorderField();
	bool bShowMaximum = m_ImportHelper.GotMaxField();

	int nDbIdx;
	dbDatabase.FindDatabaseByNumber ( dbStockpoint.GetDbNo( m_nSpIdx ), nDbIdx );

	CSSFile fileImportSource;	
	if ( fileImportSource.Open ( m_strImportPath, "rb" ) == FALSE )
		return;

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		ReportFile.SetStyle1 ( GetTitle() );
		ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
		ReportFile.AddColumn ( "Description", TA_LEFT, 300 );
		ReportFile.AddColumn ( "Category", TA_LEFT, 250 );
		if ( bShowMinimum == TRUE )	ReportFile.AddColumn ( "DU Minimum", TA_RIGHT, 250 );
		if ( bShowMinimum == TRUE )	ReportFile.AddColumn ( "SU Minimum", TA_RIGHT, 250 );
		if ( bShowReorder == TRUE )	ReportFile.AddColumn ( "DU Reorder", TA_RIGHT, 250 );
		if ( bShowReorder == TRUE )	ReportFile.AddColumn ( "SU Reorder", TA_RIGHT, 250 );
		if ( bShowMaximum == TRUE )	ReportFile.AddColumn ( "DU Maximum", TA_RIGHT, 250 );
		if ( bShowMaximum == TRUE )	ReportFile.AddColumn ( "SU Maximum", TA_RIGHT, 250 );

		StatusProgress.Lock( TRUE, "Creating report" );

		//SKIP HEADER
		CString strBuffer;
		fileImportSource.ReadString( strBuffer );

		while ( fileImportSource.ReadString ( strBuffer ) == TRUE )  
		{
			StatusProgress.SetPos ( fileImportSource );

			CCSV csv ( strBuffer );
			CString strWrapper = csv.GetString ( 0 );
			
			int nStockIdx, nStockSuppIdx;
			if ( m_SuppRefFinder.FindItemByWrapper( strWrapper, nStockIdx, nStockSuppIdx ) == FALSE )
				continue;
		
			CStockCSVRecord StockRecord;
			CStockLevelsCSVRecord StockLevels;
			if ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, dbStockpoint.GetSpNo(m_nSpIdx), -1 ) != STOCKLEVEL_IMPORT_ACCEPT )
				continue;
	
			double dYield = StockRecord.GetYield( nStockSuppIdx );

			CCSV csvOut ( '\t' );
			csvOut.Add( StockRecord.GetStockCode() );
			csvOut.Add( StockRecord.GetDescription() );
			csvOut.Add( DataManager.Category.GetText( StockRecord.GetCategory() ) );
			
			if ( TRUE == bShowMinimum )
			{
				double dDUQty = m_ImportHelper.GetMinQty( csv );
				csvOut.Add( dDUQty, SysInfo.GetDPQty() );
				csvOut.Add( DataManager.Stock.GetStockLevelString( StockRecord, dDUQty * dYield ) );
			}

			if ( TRUE == bShowReorder )
			{
				double dDUQty = m_ImportHelper.GetReorderQty( csv );
				csvOut.Add( dDUQty, SysInfo.GetDPQty() );
				csvOut.Add( DataManager.Stock.GetStockLevelString( StockRecord, dDUQty * dYield ) );
			}
			
			if ( TRUE == bShowMaximum )
			{
				double dDUQty = m_ImportHelper.GetMaxQty( csv );
				csvOut.Add( dDUQty, SysInfo.GetDPQty() );
				csvOut.Add( DataManager.Stock.GetStockLevelString( StockRecord, dDUQty * dYield ) );
			}
			
			ReportFile.WriteLine( csvOut.GetLine() );
		}

		ReportFile.WriteLine( "" );
		StatusProgress.Unlock();
		ReportFile.Close();
	}

	CString strKey = "3";
	strKey += ( bShowMinimum ) ? "1" : "0";
	strKey += ( bShowReorder ) ? "1" : "0";
	strKey += ( bShowMaximum ) ? "1" : "0";
	
	CReportManager ReportManager;
	CFilenameUpdater FnUp( SysFiles::MinLevelImport );
	ReportManager.DisplayReport( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, FALSE );
}

/**********************************************************************/

void CStockInfoMinimumDlg::DisplayWrapperExceptions()
{
	bool bShowMinimum = m_ImportHelper.GotMinField();
	bool bShowReorder = m_ImportHelper.GotReorderField();
	bool bShowMaximum = m_ImportHelper.GotMaxField();

	int nDbIdx;
	dbDatabase.FindDatabaseByNumber ( dbStockpoint.GetDbNo( m_nSpIdx ), nDbIdx );

	CSSFile fileImportSource;	
	if ( fileImportSource.Open ( m_strImportPath, "rb" ) == FALSE )
		return;

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		ReportFile.SetStyle1 ( GetTitle() );
		ReportFile.AddColumn ( "Code", TA_LEFT, 380 );
		if ( bShowMinimum == TRUE )	ReportFile.AddColumn ( "DU Minimum", TA_RIGHT, 250 );
		if ( bShowReorder == TRUE )	ReportFile.AddColumn ( "DU Reorder", TA_RIGHT, 250 );
		if ( bShowMaximum == TRUE )	ReportFile.AddColumn ( "DU Maximum", TA_RIGHT, 250 );
		ReportFile.AddColumn ( "Problem", TA_LEFT, 600 );

		StatusProgress.Lock( TRUE, "Creating report" );

		//SKIP HEADER
		CString strBuffer;
		fileImportSource.ReadString( strBuffer );

		while ( fileImportSource.ReadString ( strBuffer ) == TRUE )  
		{
			StatusProgress.SetPos ( fileImportSource );

			CCSV csv ( strBuffer );
			CString strWrapper = csv.GetString ( 0 );
		
			CString strProblem = "";

			int nStockIdx, nStockSuppIdx;
			if ( m_SuppRefFinder.FindItemByWrapper( strWrapper, nStockIdx, nStockSuppIdx ) == FALSE )
				strProblem = "Unknown wrapper code";
			else
			{
				CStockCSVRecord StockRecord;
				CStockLevelsCSVRecord StockLevels;
				if ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, dbStockpoint.GetSpNo(m_nSpIdx), -1 ) != STOCKLEVEL_IMPORT_ACCEPT )  
					strProblem = "Stock item not active at this stockpoint";
			}

			if ( strProblem != "" )
			{
				CCSV csvOut ( '\t' );
				csvOut.Add( strWrapper );
				if ( TRUE == bShowMinimum )	csvOut.Add( m_ImportHelper.GetMinQty( csv ),  SysInfo.GetDPQty() );
				if ( TRUE == bShowReorder )	csvOut.Add( m_ImportHelper.GetReorderQty( csv ), SysInfo.GetDPQty() );
				if ( TRUE == bShowMaximum )	csvOut.Add( m_ImportHelper.GetMaxQty( csv ), SysInfo.GetDPQty() );
				csvOut.Add( strProblem );

				ReportFile.WriteLine( csvOut.GetLine() );
			}
		}

		ReportFile.WriteLine( "" );
		StatusProgress.Unlock();
		ReportFile.Close();
	}
	
	CString strKey = "4";
	strKey += ( bShowMinimum ) ? "1" : "0";
	strKey += ( bShowReorder ) ? "1" : "0";
	strKey += ( bShowMaximum ) ? "1" : "0";
	
	CReportManager ReportManager;
	CFilenameUpdater FnUp( SysFiles::MinLevelImport );
	ReportManager.DisplayReport( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, FALSE );
}

/**********************************************************************/

const char* CStockInfoMinimumDlg::GetTitle()
{
	GetTitle( m_bAllowReorder, m_strTitle );
	return m_strTitle;
}

/**********************************************************************/

void CStockInfoMinimumDlg::GetTitle( bool bAllowReorder, CString& strTitle )
{
	strTitle = "Import ";

	if ( TRUE == bAllowReorder )
		strTitle + "Min / Reorder / Max";
	else
		strTitle + "Min / Max";

	strTitle += "Levels";
}

/**********************************************************************/

void CStockInfoMinimumDlg::OnButtonClose() 
{
	EndDialog( IDCANCEL );	
}

/**********************************************************************/

void CStockInfoMinimumDlg::OnOK() 
{
	if ( m_nLinesFound > m_nExceptionLines )	
		EndDialog( IDOK );
	else
		EndDialog( IDCANCEL );
}

/**********************************************************************/
