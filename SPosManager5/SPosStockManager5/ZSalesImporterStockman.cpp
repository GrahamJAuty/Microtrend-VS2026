/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ExplodePlu.h"
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PMSOptions.h"
#include "RepCSV.h"
#include "SalesTypes.h"
#include "StockAuditRetrySales.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ZSalesImporterStockman.h"
/**********************************************************************/

CZSalesImporterStockman::CZSalesImporterStockman()
{
	m_nLocIdx = 0;
	m_nEcrIdx = 0;
	m_nDbIdx = 0;
	m_nSpIdx = 0;
	m_nNetworkIdx = 0;
}

/**********************************************************************/

void CZSalesImporterStockman::LookForSales( int nLocIdx, int nEcrIdx )
{
	m_nLocIdx = nLocIdx;
	m_nEcrIdx = nEcrIdx;

	int nNetworkNo = dbLocation.GetNetworkNo( m_nLocIdx );
	if ( dbNetwork.FindNetworkByNumber( nNetworkNo, m_nNetworkIdx ) == FALSE )
		return;

	switch( dbNetwork.GetConnectionType( m_nNetworkIdx ) )
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SMARTENT_NONE:
		return;
	}

	CString strPath = dbNetwork.GetFolderPathSyssetStock( m_nNetworkIdx );
	strPath += "\\";

	CString strFilename;
	strFilename.Format( "stock.%3.3d",
		dbLocation.GetTNo( m_nLocIdx, m_nEcrIdx ) );

	if ( ::FileExists( strPath + strFilename ) == FALSE )
		return;

	int nDbNo = dbLocation.GetDbNo( m_nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, m_nDbIdx ) == FALSE )
		return;

	int nSpNo = dbLocation.GetSpNo( m_nLocIdx, m_nEcrIdx );
	if ( dbStockpoint.FindStockpointByNumber( nDbNo, nSpNo, m_nSpIdx ) == FALSE )
		return;
	
	CSSFile fileLock;
	CFilenameUpdater FnUp( SysFiles::NetworkCommsLock, m_nNetworkIdx );
	if ( fileLock.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_STOCK );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( m_nDbIdx, arrayReadWrite, info, FALSE ) == FALSE )
		{
			fileLock.Close();
			return;
		}

		if ( DataManager.OpenStockLevels( m_nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
		{
			fileLock.Close();
			DataManager.CloseDatabase();
			return;
		}

		m_auditDate = COleDateTime::GetCurrentTime();

		m_strDateNow.Format ( "%4.4d%2.2d%2.2d",
			m_auditDate.GetYear(),
			m_auditDate.GetMonth(),
			m_auditDate.GetDay() );

		m_strTimeNow.Format ( "%2.2d%2.2d",
			m_auditDate.GetHour(),
			m_auditDate.GetMinute() );

		m_arrayFilesToImport.RemoveAll();
		m_arrayFilesToImport.Add( strFilename );
		ProcessPendingFiles( FALSE );	

		DataManager.CloseDatabase();
		fileLock.Close();
	}
}

/**********************************************************************/

void CZSalesImporterStockman::LookForPMSSales()
{
#ifdef COMPILE_PMS

	m_nNetworkIdx = -1;
	m_nLocIdx = 0;
	m_nEcrIdx = 0;

	CString strPath = dbNetwork.GetFolderPathSyssetStock( m_nNetworkIdx );
	strPath += "\\";

	CString strFilename = "stock.001";
	if ( ::FileExists( strPath + strFilename ) == FALSE )
		return;

	if ( dbDatabase.GetSize() == 0 )
		return;

	int nDbNo = dbDatabase.GetDbNo(0);
	int nSpNo = PMSOptions.GetMiscManualSalesStockpoint();

	if ( 0 == nSpNo )
		return;

	if ( dbStockpoint.FindStockpointByNumber( nDbNo, nSpNo, m_nSpIdx ) == FALSE )
		return;
	
	CSSFile fileLock;
	CFilenameUpdater FnUp( SysFiles::NetworkCommsLock, m_nNetworkIdx );
	if ( fileLock.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_STOCK );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( m_nDbIdx, arrayReadWrite, info, FALSE ) == FALSE )
		{
			fileLock.Close();
			return;
		}

		if ( DataManager.OpenStockLevels( m_nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
		{
			fileLock.Close();
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
		ProcessPendingFiles( TRUE );	

		DataManager.CloseDatabase();
		fileLock.Close();
	}
#endif
}

/**********************************************************************/

void CZSalesImporterStockman::ProcessPendingFiles( bool bPMS )
{
	CString strPath = dbNetwork.GetFolderPathSyssetStock( m_nNetworkIdx );
	strPath += "\\";

	for ( int n = 0; n < m_arrayFilesToImport.GetSize(); n++ )
	{
		CString strFilename = m_arrayFilesToImport.GetAt(n);
		if ( ProcessPendingFile( strPath, strFilename, bPMS ) == TRUE )
			CFileRemover FileRemover( strPath + strFilename );
	}
}

/**********************************************************************/

bool CZSalesImporterStockman::ProcessPendingFile( CString& strPath, CString& strFilename, bool bPMS )
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

	if ( TRUE == bPMS )
		m_strEcrName = "PMS";
	else
	{
		m_strEcrName.Format ( "%s, %s",
			dbLocation.GetName( m_nLocIdx ),
			dbLocation.GetTerminalName( m_nLocIdx, m_nEcrIdx ) );
	}

	dbStockpoint.RemoveTemporaryAudits( m_nSpIdx );
	
	//MAKE SURE THAT SALES HISTORY FOLDER EXISTS
	CString strHistoryFolder = dbStockpoint.GetFolderPathHistory( m_nSpIdx );
	::CreateSubdirectory( strHistoryFolder );
	if ( ::ExistSubdirectory( strHistoryFolder ) == FALSE )
		return FALSE;
		
	//MAKE SURE THAT WE CAN READ THE SOURCE FILE
	CSSFile fileSource;
	if ( fileSource.Open( strPath + strFilename, "rb" ) == FALSE )
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
		if ( strBuffer.Left(1) != "/" )
		{
			CCSVPendingStock csvSale ( strBuffer );

			m_strDateOfSale = csvSale.GetDate();
			::MakeYearFirstDate( m_strDateOfSale );
			m_strReportDate = m_strDateOfSale;
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
			ExplodePlu.SetPriceLevel( m_nLinePluLevel );

			__int64 nPluNo = _atoi64(m_strLinePluNo);

			if ( StockOptions.GetPluRedirectToStockpointFlag() == TRUE )
			{
				int nRedirSpNo = ExplodePlu.GetRedirectionSpNo(nPluNo);
				if ( ( nRedirSpNo != 0 ) || ( nRedirSpNo != dbStockpoint.GetSpNo( m_nSpIdx ) ) )
				{
					int nRedirSpIdx;
					int nDbNo = dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() );
					if ( dbStockpoint.FindStockpointByNumber( nDbNo, nRedirSpNo, nRedirSpIdx ) == TRUE )
					{
						CString strRedirFilename = "";
						if ( FALSE == bPMS )
						{
							CFilenameUpdater FnUp( SysFiles::RedirectedSales, nRedirSpIdx );
							strRedirFilename = FnUp.GetFilenameToUse();
						}
						else
						{
							CFilenameUpdater FnUp( SysFiles::RedirectedSalesPMS, nRedirSpIdx );
							strRedirFilename = FnUp.GetFilenameToUse();
						}

						CSSFile fileRedir;
						if ( fileRedir.Open( strRedirFilename, "ab" ) == TRUE )
						{
							csvSale.InsertAt( 0, m_strEcrName );
							fileRedir.WriteLine( csvSale.GetLine() );
							continue;
						}
					}
				}
			}
			
			CStringArray StockDetails;
			int nResult = ExplodePlu.ExplodePlu ( nPluNo, StockDetails, AUDIT_SALES, m_dLinePluQty, m_dLinePluValue );
				
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
	}

	fileSource.Close();

	WriteHistoryBuffer();
	WriteAuditBuffers( FALSE );		//SALES
	WriteAuditBuffers( TRUE );		//WASTAGE
	WriteRetryBuffer( bPMS );
		
	CFileRemover FileRemover( strPath + strFilename );

	if ( bWriteStockDatabase == TRUE )
	{
		CDataManagerInfo info;
		DataManager.WriteStockLevels( m_nSpIdx, info );
	}
	
	return TRUE;
}

/**********************************************************************/

void CZSalesImporterStockman::ProcessSale( CStringArray& StockDetails, bool bWastage, bool bPMS )
{
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvDetails ( StockDetails.GetAt ( nIndex ) );
							
		int nStockIdx = csvDetails.GetStockIdx();
		CString strStockCode = csvDetails.GetStockCode();
		double dStockQty = csvDetails.GetStockQty();
		double dStockValue = csvDetails.GetStockValue();
		int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );
					
		//ADJUST APPARENT STOCK LEVELS
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );
					
		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
					
		if ( FALSE == bWastage )
		{
			if ( TRUE == bPMS )
				StockLevels.AddManualSales( dStockQty, dStockValue, FALSE );
			else if ( StockDetails.GetSize() >= 2 )
				StockLevels.AddRecipeSales ( dStockQty, dStockValue, FALSE );
			else 		
				StockLevels.AddEposSales( m_nLinePluLevel, dStockQty, dStockValue );
		}
		else
			StockLevels.AddEcrWastage( dStockQty, dStockValue );
		
		DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );

		if ( FALSE == bWastage )
		{
			CCSVStockAudit csvOut;
			csvOut.Add ( "D" );
			csvOut.Add ( AUDIT_SALES );
			csvOut.Add ( 1 );

			if ( TRUE == bPMS )
				csvOut.Add ( SALES_TYPE_MANUAL );
			else if ( 0 == nIndex )
				csvOut.Add ( SALES_TYPE_E1 );
			else
				csvOut.Add ( SALES_TYPE_RECIPEITEM );

			csvOut.Add ( m_strLineShortPluNo );
			csvOut.Add ( m_strEcrName );
			csvOut.Add ( csvDetails.GetDescription() );
			csvOut.AddStockAuditQty ( m_dLinePluQty );
			csvOut.AddStockAuditVal ( m_dLinePluValue );
			csvOut.Add ( strStockCode );
			csvOut.Add ( csvDetails.GetSubUnits() );
			csvOut.AddStockAuditQty ( dStockQty );
			csvOut.AddStockAuditVal ( dStockValue );
			csvOut.Add ( m_strDateOfSale );
			csvOut.Add ( m_strTimeOfSale );
			m_arrayAuditBufferSales.Add ( csvOut.GetLine() );	
		}
		else
		{
			CString strStockDesc = StockRecord.GetDescription();
			if ( strStockDesc == csvDetails.GetDescription() )
				strStockDesc = "";
			
			CCSVStockAudit csvOut;
			csvOut.Add ( "D" );
			csvOut.Add ( AUDIT_ADJUSTMENT_PLU );
			csvOut.Add ( 1 );
			csvOut.Add ( ( nIndex == 0 ) ? 0 : 1 );
			csvOut.Add ( m_strLineShortPluNo );
			csvOut.Add ( csvDetails.GetDescription() );
			csvOut.AddStockAuditQty ( m_dLinePluQty );
			csvOut.AddStockAuditVal ( m_dLinePluValue );
			csvOut.Add ( strStockCode );
			csvOut.Add ( csvDetails.GetSubUnits() );
			csvOut.AddStockAuditQty ( dStockQty );
			csvOut.AddStockAuditVal ( dStockValue );
			csvOut.Add( "Epos Wastage" );
			csvOut.Add( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
			csvOut.Add( strStockDesc );
			csvOut.Add ( m_strDateOfSale );
			csvOut.Add ( m_strTimeOfSale );	
			m_arrayAuditBufferWastage.Add ( csvOut.GetLine() );
		}

		int nHistorySalesType = 0;

		if ( TRUE == bPMS )
			nHistorySalesType = SALES_TYPE_MANUAL;
		else if ( TRUE == bWastage )
			nHistorySalesType = SALES_TYPE_ECRWASTE;
		else
			nHistorySalesType = SALES_TYPE_E1;
			
		if ( m_strReportDate.GetLength() == 8 )
		{
			CString strRepMonth = m_strReportDate.Left(6);
			if ( strRepMonth != m_strSalesHistoryMonth )
			{
				WriteHistoryBuffer();
				m_strSalesHistoryMonth = strRepMonth;
				m_arrayHistoryBuffer.RemoveAll();
			}
		}
		
		CCSV csvOut;
		csvOut.Add ( atoi( m_strReportDate.Right(2) ) );
		csvOut.Add ( m_strTimeOfSale );
		csvOut.Add ( strStockCode );
		csvOut.Add ( nHistorySalesType );
		csvOut.Add ( dStockQty, 5 );
		csvOut.Add ( dStockValue, 5 );
		csvOut.Add ( StockRecord.GetCategory() );
		csvOut.Add ( "" );
		csvOut.Add ( m_nLinePluLevel );
		m_arrayHistoryBuffer.Add ( csvOut.GetLine() );
	}
}

/**********************************************************************/

void CZSalesImporterStockman::ProcessException( CStringArray& StockDetails, bool bWastage, bool bPMS )
{
	if ( m_strReportDate.GetLength() == 8 )
	{
		if ( m_strReportDate != m_strRetryDate )
		{
			WriteRetryBuffer( bPMS );
			m_strRetryDate = m_strReportDate;
			m_arrayRetryBuffer.RemoveAll();
		}
	}
					
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvProblem ( StockDetails.GetAt ( nIndex ) );

		if ( FALSE == bWastage )
		{
			CCSVStockAudit csvOut;
			csvOut.Add ( "D" );
			csvOut.Add ( AUDIT_EX_SALES );
			csvOut.Add ( 2 );

			if ( TRUE == bPMS )
				csvOut.Add( SALES_TYPE_MANUAL );
			else
				csvOut.Add( SALES_TYPE_E1 );

			csvOut.Add ( m_strLineShortPluNo );
			csvOut.Add ( m_strEcrName );
			csvOut.AddStockAuditQty ( m_dLinePluQty );
			csvOut.AddStockAuditVal ( m_dLinePluValue );
			csvOut.Add ( csvProblem.GetStockCode() );
			csvOut.Add ( csvProblem.GetProblem() );
			csvOut.Add ( m_strDateOfSale );
			csvOut.Add ( m_strTimeOfSale );
			csvOut.Add ( FALSE );	//NOT A WRAPPER
			m_arrayExceptionsBufferSales.Add( csvOut.GetLine() );
		}
		else
		{
			CCSVStockAudit csvOut;
			csvOut.Add ( "D" );
			csvOut.Add ( AUDIT_EX_ADJUSTMENT_PLU );
			csvOut.Add ( 1 );
			csvOut.Add ( m_strLineShortPluNo );
			csvOut.AddStockAuditQty ( m_dLinePluQty );
			csvOut.AddStockAuditVal ( m_dLinePluValue );		
			csvOut.Add ( "Epos Wastage" );
			csvOut.Add ( csvProblem.GetStockCode() );
			csvOut.Add ( csvProblem.GetProblem() );
			csvOut.Add ( m_strDateOfSale );
			csvOut.Add ( m_strTimeOfSale );
			m_arrayExceptionsBufferWastage.Add( csvOut.GetLine() );
		}
			
		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( 1 );
		csvOut.Add ( bWastage ? STOCK_XXX_WASTAGE : STOCK_XXX_SALES );
		csvOut.Add ( m_strLineShortPluNo );
		csvOut.Add ( m_nLinePluLevel );
		csvOut.AddStockAuditQty ( m_dLinePluQty );
		csvOut.AddStockAuditVal ( m_dLinePluValue );
			
		m_arrayRetryBuffer.Add( csvOut.GetLine() );
	}
}

/**********************************************************************/

void CZSalesImporterStockman::WriteHistoryBuffer()
{
	if ( m_arrayHistoryBuffer.GetSize() == 0 )
		return;

	CString strHistoryFilename = dbStockpoint.GetFolderPathHistory( m_nSpIdx );
	strHistoryFilename += "\\St";
	strHistoryFilename += m_strSalesHistoryMonth;
	strHistoryFilename += ".002";

	CSSFile fileHistory;
	if ( fileHistory.Open( strHistoryFilename, "ab" ) == TRUE )
	{
		for ( int n = 0; n < m_arrayHistoryBuffer.GetSize(); n++ )
			fileHistory.WriteLine( m_arrayHistoryBuffer.GetAt(n) );
	}
}

/**********************************************************************/

void CZSalesImporterStockman::WriteAuditBuffers( bool bWastage )
{
	CString strPeriodAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, m_auditDate, AUDIT_FAMILY_NORMAL );
			
	int nAuditType, nExceptionType;
	if ( FALSE == bWastage )
	{
		nAuditType = AUDIT_SALES;
		nExceptionType = AUDIT_EX_SALES;
	}
	else
	{
		nAuditType = AUDIT_ADJUSTMENT_PLU;
		nExceptionType = AUDIT_EX_ADJUSTMENT_PLU;
	}
	
	CSSFile fileAudit;
	if ( fileAudit.Open( strPeriodAudit, "ab" ) == TRUE )
	{
		CStockAuditBaseReport Audit;
		Audit.SetReportType( nAuditType );

		CStockAuditHeaderInfo info;
		info.m_date = m_auditDate;
		fileAudit.WriteLine( Audit.GetHeaderLine( info ) );

		if ( FALSE == bWastage )
		{
			for ( int n = 0; n < m_arrayAuditBufferSales.GetSize(); n++ )
				fileAudit.WriteLine( m_arrayAuditBufferSales.GetAt( n ) );
		}
		else
		{
			for ( int n = 0; n < m_arrayAuditBufferWastage.GetSize(); n++ )
				fileAudit.WriteLine( m_arrayAuditBufferWastage.GetAt( n ) );
		}

		CStockAuditBaseReport Exceptions;
		Exceptions.SetReportType( nExceptionType );
		
		CStockAuditHeaderInfo infoHeader;
		infoHeader.m_date = m_auditDate;
		fileAudit.WriteLine( Exceptions.GetHeaderLine( infoHeader ) );
			
		if ( FALSE == bWastage )
		{
			for ( int n = 0; n < m_arrayExceptionsBufferSales.GetSize(); n++ )
				fileAudit.WriteLine( m_arrayExceptionsBufferSales.GetAt( n ) );
		}
		else
		{
			for ( int n = 0; n < m_arrayExceptionsBufferWastage.GetSize(); n++ )
				fileAudit.WriteLine( m_arrayExceptionsBufferWastage.GetAt( n ) );
		}

		fileAudit.Close();
	}
}

/**********************************************************************/

void CZSalesImporterStockman::WriteRetryBuffer( bool bPMS )
{
	if ( m_arrayRetryBuffer.GetSize() == 0 )
		return;

	if ( m_strRetryDate.GetLength() != 8 )
		return;

	int nYear = atoi( m_strRetryDate.Left(4) );
	int nMonth = atoi ( m_strRetryDate.Mid(4,2) );
	int nDay = atoi( m_strRetryDate.Right(2) );

	COleDateTime retryDate ( nYear, nMonth, nDay, 0, 0, 0 );
	
	if ( retryDate.GetStatus() != COleDateTime::valid )
		return;

	CString strPeriodAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, retryDate, AUDIT_FAMILY_RETRY, bPMS );
			
	CSSFile fileAudit;
	if ( fileAudit.Open( strPeriodAudit, "ab" ) == TRUE )
	{
		CStockAuditRetrySales Audit;
		Audit.SetReportType( AUDIT_RETRY_SALES );
		fileAudit.WriteLine( Audit.GetHeaderLine( retryDate, m_strDateOfSale, m_strTimeOfSale, m_strEcrName ) );

		for ( int n = 0; n < m_arrayRetryBuffer.GetSize(); n++ )
			fileAudit.WriteLine( m_arrayRetryBuffer.GetAt( n ) );

		fileAudit.Close();
	}
}

/**********************************************************************/
