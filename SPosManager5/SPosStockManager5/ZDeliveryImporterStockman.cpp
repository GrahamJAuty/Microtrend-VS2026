/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
//#include "DealerFlags.h"
#include "ExplodePlu.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PMSOptions.h"
#include "RepCSV.h"
#include "SalesTypes.h"
#include "StockAuditRetrySales.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ZDeliveryImporterStockman.h"
/**********************************************************************/

CZDeliveryImporterStockman::CZDeliveryImporterStockman()
{
	m_nLocIdx = 0;
	m_nEcrIdx = 0;
	m_nDbIdx = 0;
	m_nSpIdx = 0;
	m_nNetworkIdx = 0;
}

/**********************************************************************/

void CZDeliveryImporterStockman::LookForDeliveries( int nLocIdx, int nEcrIdx )
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
	case CONNECTION_TYPE_SPTBOOK_WEB:
	case CONNECTION_TYPE_SMARTENT_NONE:
	case CONNECTION_TYPE_SMARTENT_WEB:
		return;
	}

	CString strPath = dbNetwork.GetFolderPathSyssetStock( m_nNetworkIdx );
	strPath += "\\";

	CString strFilename;
	strFilename.Format( "stock_delivery.%3.3d",
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
		ProcessPendingFiles();	

		DataManager.CloseDatabase();
		fileLock.Close();
	}
}

/**********************************************************************/

void CZDeliveryImporterStockman::ProcessPendingFiles()
{
	CString strPath = dbNetwork.GetFolderPathSyssetStock( m_nNetworkIdx );
	strPath += "\\";

	for ( int n = 0; n < m_arrayFilesToImport.GetSize(); n++ )
	{
		CString strFilename = m_arrayFilesToImport.GetAt(n);
		if ( ProcessPendingFile( strPath, strFilename ) == TRUE )
			CFileRemover FileRemover( strPath + strFilename );
	}
}

/**********************************************************************/

bool CZDeliveryImporterStockman::ProcessPendingFile( CString& strPath, CString& strFilename )
{	
	m_strDateOfSale = m_strDateNow;
	m_strTimeOfSale = m_strTimeNow;
	m_strReportDate = m_strDateNow;

	m_strRetryDate.Format ( "%4.4d%2.2d%2.2d", 
		m_auditDate.GetYear(), 
		m_auditDate.GetMonth(), 
		m_auditDate.GetDay() );

	m_strEcrName.Format ( "%s, %s",
		dbLocation.GetName( m_nLocIdx ),
		dbLocation.GetTerminalName( m_nLocIdx, m_nEcrIdx ) );
	
	dbStockpoint.RemoveTemporaryAudits( m_nSpIdx );
		
	//MAKE SURE THAT WE CAN READ THE SOURCE FILE
	CSSFile fileSource;
	if ( fileSource.Open( strPath + strFilename, "rb" ) == FALSE )
		return FALSE;

	m_arrayAuditBuffer.RemoveAll();
	m_arrayExceptionsBuffer.RemoveAll();
	m_arrayRetryBuffer.RemoveAll();
	
	bool bWriteStockDatabase = FALSE;
				
	CString strBuffer;
	while ( fileSource.ReadString ( strBuffer ) == TRUE )  
	{		
		if ( strBuffer.Left(1) != "/" )
		{
			CCSVPendingStockDelivery csvDelivery ( strBuffer );

			m_strDateOfSale = csvDelivery.GetDate();
			::MakeYearFirstDate( m_strDateOfSale );
			m_strReportDate = m_strDateOfSale;
			m_strTimeOfSale = csvDelivery.GetTime();
			
			CSQLPluNoInfo infoPluNo;
			infoPluNo.m_nEposPluNo = csvDelivery.GetPluNo();
			::ProcessPluNo( infoPluNo );
			if ( FALSE == infoPluNo.m_bValid )
				continue;

			m_strLineShortPluNo.Format( "%I64d", infoPluNo.m_nEposPluNo );
			::FormatPluNo( infoPluNo.m_nBasePluNo, m_strLinePluNo );

			double dMultiplier = 1.0;
			if ( infoPluNo.m_nModifier != 0 )
				dMultiplier = DataManager.Modifier.GetMultiplierForQty( infoPluNo.m_nModifier );

			m_dLinePluQty = csvDelivery.GetQty() * dMultiplier;
			m_dLinePluValue = 0.0;
			
			CExplodePluStockLevel ExplodePlu( dbStockpoint.GetSpNo( m_nSpIdx ) );

			__int64 nPluNo = _atoi64(m_strLinePluNo);

			if ( StockOptions.GetPluRedirectToStockpointFlag() == TRUE )
			{
				int nRedirSpNo = ExplodePlu.GetRedirectionSpNo( nPluNo );

				if ( ( nRedirSpNo != 0 ) || ( nRedirSpNo != dbStockpoint.GetSpNo( m_nSpIdx ) ) )
				{
					int nRedirSpIdx;
					int nDbNo = dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() );
					if ( dbStockpoint.FindStockpointByNumber( nDbNo, nRedirSpNo, nRedirSpIdx ) == TRUE )
					{
						CFilenameUpdater FnUp( SysFiles::RedirectedDeliveries, nRedirSpIdx );
						CString strRedirFilename = FnUp.GetFilenameToUse();
						
						CSSFile fileRedir;
						if ( fileRedir.Open( strRedirFilename, "ab" ) == TRUE )
						{
							csvDelivery.InsertAt( 0, m_strEcrName );
							fileRedir.WriteLine( csvDelivery.GetLine() );
							continue;
						}
					}
				}
			}
			
			CStringArray StockDetails;
			int nResult = ExplodePlu.ExplodePlu ( nPluNo, StockDetails, AUDIT_DELIVERY, m_dLinePluQty, m_dLinePluValue );
				
			switch( nResult )
			{
			case PLU_EXCEPTION_NONE:
				ProcessDelivery( StockDetails );
				bWriteStockDatabase = TRUE;
				break;

			case PLU_EXCEPTION_PLUNONSTOCK:
				break;

			case PLU_EXCEPTION_PLUNOTLINKED:
				if ( StockOptions.GetImportSalesExceptionFlag() )
					ProcessException( StockDetails );
				break;

			default:
				ProcessException( StockDetails );
				break;
			}
		}
	}

	fileSource.Close();

	WriteAuditBuffers();	
	WriteRetryBuffer();
		
	CFileRemover FileRemover( strPath + strFilename );

	if ( bWriteStockDatabase == TRUE )
	{
		CDataManagerInfo info;
		DataManager.WriteStockLevels( m_nSpIdx, info );
	}
	
	return TRUE;
}

/**********************************************************************/

void CZDeliveryImporterStockman::ProcessDelivery( CStringArray& StockDetails )
{
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvDetails ( StockDetails.GetAt ( nIndex ) );
							
		int nStockIdx = csvDetails.GetStockIdx();
		CString strStockCode = csvDetails.GetStockCode();
		double dStockQty = csvDetails.GetStockQty();
		int nSpNo = dbStockpoint.GetSpNo( m_nSpIdx );
					
		//ADJUST APPARENT STOCK LEVELS
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );
		double dStockValue = dStockQty * StockRecord.GetUnitCost(0);
					
		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
		StockLevels.AddDelivery( dStockQty, dStockValue, FALSE );	
		DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_DELIVERY );
		csvOut.Add ( 1 );
		csvOut.Add ( strStockCode );
		csvOut.Add ( StockRecord.GetDescription() );
		csvOut.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csvOut.Add ( 0 );
		csvOut.Add ( dStockQty, 5 );
		csvOut.Add ( 0.0, 1 );
		csvOut.Add ( dStockValue, 3 );
		csvOut.Add ( StockRecord.GetSuppRefExternal(0, DealerFlags.GetSuppRefFlag() ) );
		m_arrayAuditBuffer.Add( csvOut.GetLine() );
	}
}

/**********************************************************************/

void CZDeliveryImporterStockman::ProcessException( CStringArray& StockDetails )
{
	if ( m_strReportDate.GetLength() == 8 )
	{
		if ( m_strReportDate != m_strRetryDate )
		{
			WriteRetryBuffer();
			m_strRetryDate = m_strReportDate;
			m_arrayRetryBuffer.RemoveAll();
		}
	}
			
	for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
	{
		CCSVExplodedPlu csvProblem ( StockDetails.GetAt ( nIndex ) );

		CCSVStockAudit csvOut;
		csvOut.Add ( "D" );
		csvOut.Add ( AUDIT_EX_DELIVERY );
		csvOut.Add ( 1 );
		csvOut.Add ( m_strLineShortPluNo );
		csvOut.AddStockAuditQty ( m_dLinePluQty );
		csvOut.Add ( csvProblem.GetStockCode() );
		csvOut.Add ( csvProblem.GetProblem() );
		csvOut.Add ( m_strDateOfSale );
		csvOut.Add ( m_strTimeOfSale );
		m_arrayExceptionsBuffer.Add( csvOut.GetLine() );
			
		{
			CCSVStockAudit csvOut;
			csvOut.Add ( "D" );
			csvOut.Add ( 1 );
			csvOut.Add ( STOCK_XXX_DELIVERY );
			csvOut.Add ( m_strLineShortPluNo );
			csvOut.Add (0 ); //DUMMY PLU LEVEL
			csvOut.AddStockAuditQty ( m_dLinePluQty );
			csvOut.AddStockAuditVal ( m_dLinePluValue );
				
			m_arrayRetryBuffer.Add( csvOut.GetLine() );
		}
	}
}

/**********************************************************************/

void CZDeliveryImporterStockman::WriteAuditBuffers()
{
	CString strPeriodAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, m_auditDate, AUDIT_FAMILY_NORMAL );
	
	CSSFile fileAudit;
	if ( fileAudit.Open( strPeriodAudit, "ab" ) == TRUE )
	{
		if ( m_arrayAuditBuffer.GetSize() > 0 )
		{
			CStockAuditBaseReport Audit;
			Audit.SetReportType( AUDIT_DELIVERY );

			CStockAuditHeaderInfo info;
			info.m_date = m_auditDate;
			info.m_strReference1 = "EPOS";
			info.m_strSuppName = "Mixed Suppliers";
			fileAudit.WriteLine( Audit.GetHeaderLine( info ) );

			for ( int n = 0; n < m_arrayAuditBuffer.GetSize(); n++ )
				fileAudit.WriteLine( m_arrayAuditBuffer.GetAt( n ) );
		}

		if ( m_arrayExceptionsBuffer.GetSize() > 0 )
		{
			CStockAuditBaseReport Exceptions;
			Exceptions.SetReportType( AUDIT_EX_DELIVERY );
		
			CStockAuditHeaderInfo info;
			info.m_date = m_auditDate;
			info.m_strReference1 = "EPOS";
			info.m_strSuppName = "Mixed Suppliers";
			fileAudit.WriteLine( Exceptions.GetHeaderLine( info ) );
			
			for ( int n = 0; n < m_arrayExceptionsBuffer.GetSize(); n++ )
				fileAudit.WriteLine( m_arrayExceptionsBuffer.GetAt( n ) );
		}

		fileAudit.Close();
	}
}

/**********************************************************************/

void CZDeliveryImporterStockman::WriteRetryBuffer()
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

	CString strPeriodAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, retryDate, AUDIT_FAMILY_RETRY );
			
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
