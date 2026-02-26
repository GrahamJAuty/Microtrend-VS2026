/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
#include "ExplodePlu.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "NodeTypes.h"
#include "RepCSV.h"
#include "StockAuditRetrySales.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ZSalesRedirectorStockman.h"
/**********************************************************************/

CZSalesRedirectorStockman::CZSalesRedirectorStockman()
{
	m_nDbIdx = 0;
	m_nSpIdx = 0;
}

/**********************************************************************/

void CZSalesRedirectorStockman::LookForStockpointSales( int nSpIdx, bool bPMS )
{
	m_nSpIdx = nSpIdx;

	CString strFilename = "";
	if ( FALSE == bPMS )
	{
		CFilenameUpdater FnUp( SysFiles::RedirectedSales, m_nSpIdx );
		strFilename = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::RedirectedSalesPMS, m_nSpIdx );
		strFilename = FnUp.GetFilenameToUse();
	}

	if ( ::FileExists( strFilename ) == FALSE )
		return;

	int nDbNo = dbStockpoint.GetDbNo( m_nSpIdx );
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, m_nDbIdx ) == FALSE )
		return;

	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_STOCK );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( m_nDbIdx, arrayReadWrite, info, FALSE ) == FALSE )
		return;
		
	if ( DataManager.OpenStockLevels( m_nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
	{	
		DataManager.CloseDatabase();
		return;
	}

	m_auditDate = COleDateTime::GetCurrentTime();

	m_strDateNow.Format ( "%2.2d%2.2d%4.4d",
		m_auditDate.GetDay(),
		m_auditDate.GetMonth(),
		m_auditDate.GetYear() );

	m_strTimeNow.Format ( "%2.2d%2.2d",
		m_auditDate.GetHour(),
		m_auditDate.GetMinute() );

	m_arrayFilesToImport.RemoveAll();
	m_arrayFilesToImport.Add( strFilename );
	ProcessStockpointPendingFiles( bPMS );	

	DataManager.CloseDatabase();
}

/**********************************************************************/

void CZSalesRedirectorStockman::ProcessStockpointPendingFiles( bool bPMS )
{
	for ( int n = 0; n < m_arrayFilesToImport.GetSize(); n++ )
	{
		CString strFilename = m_arrayFilesToImport.GetAt(n);
		if ( ProcessStockpointPendingFile( strFilename, bPMS ) == TRUE )
			CFileRemover FileRemover( strFilename );
	}
}

/**********************************************************************/

bool CZSalesRedirectorStockman::ProcessStockpointPendingFile( CString& strFilename, bool bPMS )
{	
	m_strDateOfSale = m_strDateNow;
	m_strTimeOfSale = m_strTimeNow;
	m_strReportDate = m_strDateNow;
	
	m_strSalesHistoryMonth.Format ( "%4.4d%2.2d", 
		m_auditDate.GetYear(), 
		m_auditDate.GetMonth() );
	
	m_strRetryDate.Format ( "%4.4d%2.2d%2.2d", 
		m_auditDate.GetYear(), 
		m_auditDate.GetMonth(), 
		m_auditDate.GetDay() );

	dbStockpoint.RemoveTemporaryAudits( m_nSpIdx );
	
	//MAKE SURE THAT SALES HISTORY FOLDER EXISTS
	CString strHistoryFolder = dbStockpoint.GetFolderPathHistory( m_nSpIdx );
	::CreateSubdirectory( strHistoryFolder );
	if ( ::ExistSubdirectory( strHistoryFolder ) == FALSE )
		return FALSE;
			
	//MAKE SURE THAT WE CAN READ THE SOURCE FILE
	CSSFile fileSource;
	if ( fileSource.Open( strFilename, "rb" ) == FALSE )
		return FALSE;

	m_arrayAuditBufferSales.RemoveAll();
	m_arrayExceptionsBufferSales.RemoveAll();
	m_arrayAuditBufferWastage.RemoveAll();
	m_arrayExceptionsBufferWastage.RemoveAll();
	m_arrayRetryBuffer.RemoveAll();
	m_arrayHistoryBuffer.RemoveAll();
	
	bool bWriteStockDatabase = FALSE;
				
	CString strBuffer;
	while ( fileSource.ReadString ( strBuffer ) == TRUE )  
	{		
		CCSV csvBuffer( strBuffer );
		if ( csvBuffer.GetSize() == 0 )
			continue;

		m_strEcrName = csvBuffer.GetString(0);
		m_strEcrName += " **";

		csvBuffer.RemoveAt(0);
		
		strBuffer = csvBuffer.GetLine();
		CCSVPendingStock csvSale ( strBuffer );

		m_strDateOfSale = csvSale.GetDate();
		m_strReportDate = csvSale.GetDate();
		m_strTimeOfSale = csvSale.GetTime();
		
		CSQLPluNoInfo infoPluNo;
		infoPluNo.m_nEposPluNo = csvSale.GetPluNo();
		::ProcessPluNo( infoPluNo );
		if ( FALSE == infoPluNo.m_bValid )
			continue;

		m_strLineShortPluNo.Format( "%I64d", infoPluNo.m_nEposPluNo );
		::FormatPluNo( infoPluNo.m_nBasePluNo, m_strLinePluNo );

		double dMultiplier = 1.0;
		if ( infoPluNo.m_nModifier != 0 )
			dMultiplier = DataManager.Modifier.GetMultiplierForQty( infoPluNo.m_nModifier );

		m_dLinePluQty = csvSale.GetQty() * dMultiplier;
		m_dLinePluValue = csvSale.GetValue();
		m_nLinePluLevel = csvSale.GetLevel();
		bool bWastage = csvSale.GetWastageFlag();

		CExplodePluStockLevel ExplodePlu( dbStockpoint.GetSpNo(m_nSpIdx) );
		ExplodePlu.SetPriceLevel ( m_nLinePluLevel );
		
		CStringArray StockDetails;
		int nResult = ExplodePlu.ExplodePlu ( _atoi64(m_strLinePluNo), StockDetails, AUDIT_SALES, m_dLinePluQty, m_dLinePluValue );
			
		switch( nResult )
		{
		case PLU_EXCEPTION_NONE:
			ProcessSale( StockDetails, bWastage, bPMS );
			bWriteStockDatabase = TRUE;
			break;

		case PLU_EXCEPTION_PLUNONSTOCK:
			break;

		case PLU_EXCEPTION_PLUNOTLINKED:
			if ( StockOptions.GetImportSalesExceptionFlag() )
				ProcessException( StockDetails, bWastage, bPMS );
			break;

		default:
			ProcessException( StockDetails, bWastage, bPMS );
			break;
		}
	}

	fileSource.Close();

	WriteHistoryBuffer();
	WriteAuditBuffers( FALSE );		//SALES
	WriteAuditBuffers( TRUE );		//WASTAGE
	WriteRetryBuffer( bPMS );
		
	CFileRemover FileRemover( strFilename );

	if ( bWriteStockDatabase == TRUE )
	{
		CDataManagerInfo info;
		DataManager.WriteStockLevels( m_nSpIdx, info );
	}
	
	return TRUE;
}

/**********************************************************************/
