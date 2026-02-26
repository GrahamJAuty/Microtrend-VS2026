/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "NodeTypes.h"
#include "RepCSV.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockAuditImportWrapper.h"
#include "StockpointCSVArray.h"
#include "WrapperErrors.h"
/**********************************************************************/
#include "StockImportBasicWrapperDlg.h"
/**********************************************************************/

CStockImportBasicWrapperDlg::CStockImportBasicWrapperDlg ( int nSpIdx, const char* szPath, CSuppRefFinder& SuppRefFinder, CWnd* pParent )
	: CStockImportBasicDlg ( nSpIdx, szPath, pParent ), m_SuppRefFinder( SuppRefFinder )
{
	m_nSuppNo = 0;
	m_nStocktakeNo = -1;
}

/**********************************************************************/

void CStockImportBasicWrapperDlg::ProcessImportDataInternal( int nAction )
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

		int nStockIdx, nStockSuppIdx;
		if ( m_SuppRefFinder.FindItemByWrapper( strWrapperCode, nStockIdx, nStockSuppIdx ) == TRUE )
		{
			int nTest = CheckStockLevelStatus( nStockIdx );
			
			if ( nTest == 0 )
				AuditStockChanges ( fileStockChanges, fileImportReport, csvImport, nStockIdx, nStockSuppIdx, dDUQty, dTotalCost, nAction );
			else
			{
				CString strStockCode = DataManager.Stock.GetStockCode( nStockIdx );
				AuditExceptions ( fileExceptions, fileImportReport, csvImport, nTest, strStockCode );
				m_nTotalExceptions++;
			}
		}
		else
		{
			AuditExceptions ( fileExceptions, fileImportReport, csvImport, 3, "" );
			m_nTotalExceptions++;
		}
		
		StatusProgress.SetPos ( fileImportSource );
	}
	m_strLinesFound.Format ( "%d", m_nLinesFound );
	m_strExceptions.Format ( "%d", m_nTotalExceptions );
}

/**********************************************************************/

void CStockImportBasicWrapperDlg::AuditExceptions ( CSSFile& file, CSSFile& fileTempReport, CCSV& csvOriginalData, int nReason, const char* szStockCode )
{
	CString strError;
	switch( nReason )
	{
	case WRAPPER_ERROR_IN_STOCKTAKE:		strError = "Stockcode in pending stock take";				break;
	case WRAPPER_ERROR_WRONG_STOCKTAKE:		strError = "Stockcode not in this stock take";				break;
	case WRAPPER_ERROR_NOT_FOUND:			strError = "Unknown wrapper code";							break;
	case WRAPPER_ERROR_STOCK_INACTIVE:		strError = "Stock item not active at this stockpoint";		break;
	case WRAPPER_ERROR_STOCK_INACTIVE2:		strError = "Stock item not active at source stockpoint";	break;
	case WRAPPER_ERROR_STOCK_INACTIVE3:		strError = "Stock item not active at target stockpoint";	break;
	default:								strError = "Unknown error";									break;
	}

	CCSVStockAudit csvOut;
	csvOut.Add ( "D" );
	csvOut.Add ( m_nExceptionReportType );
	csvOut.Add ( 2 );
	csvOut.Add ( csvOriginalData.GetString(0) );
	csvOut.Add ( csvOriginalData.GetString(1) );
	csvOut.Add ( szStockCode );
	csvOut.Add ( strError );
	csvOut.Add ( "" );		//IGNORE SALE DATE
	csvOut.Add ( "" );		//IGNORE SALE TIME
	csvOut.Add ( TRUE );	//LINE IS WRAPPER
	file.WriteLine( csvOut.GetLine() );

	csvOut.RemoveAll();
	csvOut.Add ( "D" );
	csvOut.Add ( m_nImportFileReportType );
	csvOut.Add ( 1 );
	csvOut.Add ( csvOriginalData.GetString(0) );
	csvOut.Add ( csvOriginalData.GetString(1) );
	csvOut.Add ( csvOriginalData.GetString(2) );
	csvOut.Add ( strError );

	fileTempReport.WriteLine ( csvOut.GetLine() );
}

/**********************************************************************/
	
void CStockImportBasicWrapperDlg::ShowImportFile()
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

int CStockImportBasicWrapperDlg::CheckStockLevelStatus( int nStockIdx )
{
	int nError = WRAPPER_ERROR_NONE;

	CStockCSVRecord StockRecord;
	CStockLevelsCSVRecord StockLevels;
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
