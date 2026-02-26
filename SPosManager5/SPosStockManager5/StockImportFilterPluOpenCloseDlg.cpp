/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ExplodePlu.h"
#include "FileRemover.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockAuditImportPlu.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockImportFilterPluOpenCloseDlg.h"
/**********************************************************************/

CStockImportFilterPluOpenCloseDlg::CStockImportFilterPluOpenCloseDlg( int nSpIdx, bool bClosing, const char* szPath, CStockImportFilterArray& arrayImport, CWnd* pParent )
: CStockImportFilterPluDlg( nSpIdx, bClosing, szPath, arrayImport, pParent )
{
	if (DealerFlags.GetImportBarcodeFlag() == TRUE)
	{
		m_strTitle += " by Barcode Number";
	}
	else
	{
		m_strTitle += " by Plu Number";
	}

	m_bClosing = bClosing;
}

/**********************************************************************/

void CStockImportFilterPluOpenCloseDlg::ProcessImportDataInternal()
{
	int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );

	m_nLinesFound = 0;
	m_nExceptionLines = 0;
	m_nTotalExceptions = 0;
			
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
				
	CString strBuffer;
	while ( fileImportSource.ReadString ( strBuffer ) == TRUE )  
	{				
		m_nLinesFound++;

		CStringArray StockDetails;
		CCSV csvImport ( strBuffer );

		CString strPluNo = ""; 
		CCSV csvOriginalData;	
		double dPluQty = 0.0;
		double dPluValue = 0.0;

		if ( TRUE == m_bClosing )
			GetImportLineInfoQtyValue( csvImport, strPluNo, csvOriginalData, dPluQty, dPluValue );
		else
			GetImportLineInfoQtyValue( csvImport, strPluNo, csvOriginalData, dPluQty, dPluValue );
		
		CExplodePluStockTake ExplodePlu( nSpNo, m_nStocktakeNo );
		
		bool bExplodeImportData = TRUE;

		switch ( ExplodePlu.ExplodePlu ( _atoi64(strPluNo), StockDetails, m_nPluExplodeType, dPluQty, dPluValue ) )
		{
		case PLU_EXCEPTION_NONE:
			for ( int n = 0; n < StockDetails.GetSize(); n++ )
			{
				CCSVExplodedPlu csvDetails ( StockDetails.GetAt ( n ) );
				m_arrayImport.AddItem( csvDetails.GetStockIdx(), 0, 0, csvDetails.GetStockQty() );
			}
			bExplodeImportData = FALSE;
			break;

		case PLU_EXCEPTION_PLUNONSTOCK:
			break;

		case PLU_EXCEPTION_PLUNOTEXIST:
		case PLU_EXCEPTION_PLUISRECIPE:
		case PLU_EXCEPTION_STOCKNOTEXIST:
		case PLU_EXCEPTION_STOCKTAKE:
		case PLU_EXCEPTION_STOCKINACTIVE:
			AuditExceptions ( fileExceptions, StockDetails, csvOriginalData );
			m_nTotalExceptions += StockDetails.GetSize();
			m_nExceptionLines++;
			break;

		case PLU_EXCEPTION_PLUNOTLINKED:
			if ( ( m_nPluExplodeType != AUDIT_SALES ) || ( StockOptions.GetImportSalesExceptionFlag() ) )
			{
				AuditExceptions ( fileExceptions, StockDetails, csvOriginalData );
				m_nTotalExceptions += StockDetails.GetSize();
				m_nExceptionLines++;
			}
			break;
		}

		AuditImportData ( fileImportReport, StockDetails, csvOriginalData, bExplodeImportData );

		StatusProgress.SetPos ( fileImportSource );
	}
}

/**********************************************************************/

void CStockImportFilterPluOpenCloseDlg::GetImportLineInfoQtyValue( CCSV& csvImport, CString& strPluNo, CCSV& csvOriginalData, double& dPluQty, double& dPluValue )
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

void CStockImportFilterPluOpenCloseDlg::AuditExceptions( CSSFile& fileExceptions, CStringArray& StockDetails, CCSV& csvOriginalData )
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
		fileExceptions.WriteLine( csvOut.GetLine() );
	}
}

/**********************************************************************/

void CStockImportFilterPluOpenCloseDlg::AuditImportData ( CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData, bool bExplode )
{	
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvExplode ( StockDetails.GetAt ( nIndex ) );

		CString strProblem = csvExplode.GetProblem();

		if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		{
			int nBarcodeIdx;
			__int64 nBarcodeNo = csvOriginalData.GetInt64(0);
			if ( DataManager.Barcode.FindBarcodeByNumber( nBarcodeNo, nBarcodeIdx ) == FALSE )
				strProblem = "Barcode not found";
		}
		
		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( m_nImportFileReportType );

		if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		{
			csvOut.Add ( 2 );
			csvOut.Add ( csvOriginalData.GetString(0) );
			csvOut.Add ( csvOriginalData.GetString(1) );
			csvOut.Add ( csvExplode.GetDescription() );
			csvOut.Add ( csvOriginalData.GetString(3) );
			csvOut.Add ( csvOriginalData.GetString(4) );		
			csvOut.Add ( strProblem );
		}
		else
		{
			csvOut.Add ( 1 );
			csvOut.Add ( csvOriginalData.GetString(0) );
			csvOut.Add ( csvExplode.GetDescription() );
			csvOut.Add ( csvOriginalData.GetString(2) );
			csvOut.Add ( csvOriginalData.GetString(3) );		
			csvOut.Add ( strProblem );
		}

		fileTempReport.WriteLine ( csvOut.GetLine() );

		if ( FALSE == bExplode )
			break;
	}
}

/**********************************************************************/

void CStockImportFilterPluOpenCloseDlg::PreviewImportedData()
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

