/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ExplodePlu.h"
#include "FileRemover.h"
#include "NodeTypes.h"
#include "RepCSV.h"
#include "SalesHistoryFields.h"
#include "SalesTypes.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "SalesXXXImporterStockman.h"
/**********************************************************************/

CSalesXXXImporterStockman::CSalesXXXImporterStockman( bool bDiscard )
{
	m_nFilesFound = 0;
	m_nFilesOpened = 0;
	m_nDataLines = 0;
	m_nSalesLines = 0;
	m_nWasteLines = 0;
	m_nDeliveryLines = 0;
	m_nFiledLines = 0;
	m_bDiscard = bDiscard;
}

/**********************************************************************/

void CSalesXXXImporterStockman::ProcessSales( CWordArray& arraySpIdx, COleDateTime& dateFrom, COleDateTime& dateTo )
{
	m_RetryAudit.SetReportType( AUDIT_RETRY_SALES );
	m_RetryAudit.SetAuditDateFrom( dateFrom );
	m_RetryAudit.SetAuditDateTo( dateTo );
	m_RetryAudit.SetAuditMode( AUDIT_MODE_RETRY );

	m_nTotalFileSize = 0;
	m_nProgress = 0;

	for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
	{
		int nSpIdx = arraySpIdx.GetAt(n);
		if ( nSpIdx < 0 || nSpIdx >= dbStockpoint.GetSize() )
			continue;
	
		CString strPath = dbStockpoint.GetFolderPathSalesXXX( nSpIdx );
		strPath += "\\";

		__int64 nSpFileSize = 0;
		CReportConsolidationArray<CSortedStringItem> arrayFiles;
		m_RetryAudit.GetAuditFileList( nSpIdx, arrayFiles, nSpFileSize );
		m_nTotalFileSize += nSpFileSize;

		for ( int x = 0; x < arrayFiles.GetSize(); x++ )
		{
			CSortedStringItem item;
			arrayFiles.GetAt( x, item );
			CString strFilename = item.m_strItem;

			CSalesXXXFileInfo info;
			info.m_nSpIdx = nSpIdx;
			info.m_strFilename = strFilename;
			m_arrayFileInfo.Add( info );
		}
	}

	m_nFilesFound = m_arrayFileInfo.GetSize();
	
	if ( m_nFilesFound > 0 )
	{
		if ( m_bDiscard == FALSE )
		{
			StatusProgress.Lock( TRUE, "Processing exceptions" );
			ProcessSalesInternal();
			StatusProgress.Unlock();
		}
		else
		{
			StatusProgress.Lock( TRUE, "Discarding exceptions" );
			DiscardSalesInternal();
			StatusProgress.Unlock();
		}
	}
}

/**********************************************************************/
	
void CSalesXXXImporterStockman::ProcessSalesInternal()
{
	int nCurrentDbIdx = -1;
	
	for ( int x = 0; x < m_arrayFileInfo.GetSize(); x++ )
	{
		m_nSpIdx = m_arrayFileInfo[x].m_nSpIdx;

		int nDbIdx;
		int nDbNo = dbStockpoint.GetDbNo( m_nSpIdx );
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
			continue;

		if ( nDbIdx != nCurrentDbIdx )
		{
			if ( nCurrentDbIdx != -1 )
			{
				DataManager.CloseDatabase();
				nCurrentDbIdx = -1;
			}

			CWordArray arrayReadWrite;
			arrayReadWrite.Add( NODE_STOCK );

			CDataManagerInfo info;			
			if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info, FALSE ) == TRUE )
				nCurrentDbIdx = nDbIdx;
		}

		if ( nCurrentDbIdx == -1 )
			continue;

		CString strPath = dbStockpoint.GetFolderPathSalesXXX( m_nSpIdx );
		strPath += "\\";

		bool bPMS = FALSE;
		CString strFilenameIn = m_arrayFileInfo[x].m_strFilename;
		CString strFilenameOut = "";

		if ( strFilenameIn.GetLength() == 25 )
		{
			bPMS = TRUE;
			m_strReportDate = strFilenameIn.Mid(13,8);
			strFilenameOut = strFilenameIn.Left(21) + ".002";
		}
		else
		{
			bPMS = FALSE;
			m_strReportDate = strFilenameIn.Mid(9,8);
			strFilenameOut = strFilenameIn.Left(17) + ".002";
		}

		CSSFile fileIn;
		if ( fileIn.Open( strPath + strFilenameIn, "rb" ) == FALSE )
			continue;
		
		CSSFile fileOut;
		if ( fileOut.Open( strPath + strFilenameOut, "wb" ) == FALSE )
			continue;

		CDataManagerInfo info;
		if ( DataManager.OpenStockLevels( m_nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
			continue;

		m_nFilesOpened++;

		bool bWriteStockDatabase = FALSE;

		m_arraySalesAuditBuffer.RemoveAll();
		m_arrayWasteAuditBuffer.RemoveAll();
		m_arrayDeliveryAuditBuffer.RemoveAll();
		m_arraySalesHistoryBuffer.RemoveAll();

		CString strHeader = "";
		bool bFiledData = FALSE;
		
		CString strBuffer;
		while ( fileIn.ReadString( strBuffer ) )
		{
			StatusProgress.SetPos( m_nProgress + fileIn.Tell(), m_nTotalFileSize );

			CCSV csv( strBuffer );
			CString strLineType = csv.GetString(0);
			strLineType.MakeUpper();

			//DELAY WRITING HEADER UNTIL WE HAVE DATA
			if ( strLineType == "H" )
			{
				m_RetryAudit.ProcessHeader( strBuffer );
				strHeader = strBuffer;
				continue;
			}

			//DISCARD LINES THAT ARE NOT HEADER OR DATA
			if ( strLineType != "D" )
				continue;
			
			//KEEP DATA LINES THAT CANNOT BE PROCESSED
			if ( m_RetryAudit.ProcessLine( strBuffer ) == FALSE )
			{
				if ( strHeader != "" )
				{
					fileOut.WriteLine( strHeader );
					strHeader = "";
				}

				fileOut.WriteLine( strBuffer );
				bFiledData = TRUE;
				continue;
			}

			CSQLPluNoInfo infoPluNo;
			infoPluNo.m_nEposPluNo = _atoi64( m_RetryAudit.GetLinePluNo() );
			::ProcessPluNo( infoPluNo );

			bool bFileLine = FALSE;
				
			if ( FALSE == infoPluNo.m_bValid )
				bFileLine = TRUE;
			else
			{
				CExplodePluStockLevel ExplodePlu( dbStockpoint.GetSpNo( m_nSpIdx ) );
				ExplodePlu.SetPriceLevel ( m_RetryAudit.GetLinePluLevel() );
				
				int nExplodeType = AUDIT_SALES;
				if ( m_RetryAudit.GetLineActionType() == STOCK_XXX_DELIVERY )
					nExplodeType = AUDIT_DELIVERY;
			
				CStringArray StockDetails;
				int nResult = ExplodePlu.ExplodePlu ( infoPluNo.m_nBasePluNo, StockDetails, nExplodeType, m_RetryAudit.GetLinePluQty(), m_RetryAudit.GetLinePluValue() );
			
				switch( nResult )
				{
				case PLU_EXCEPTION_NONE:
					if ( nExplodeType != AUDIT_DELIVERY )
						ProcessSale( StockDetails, bPMS );
					else
						ProcessDelivery( StockDetails );
					bWriteStockDatabase = TRUE;
					break;

				case PLU_EXCEPTION_PLUNOTLINKED:
					bFileLine = ( StockOptions.GetImportSalesExceptionFlag() );
					break;

				case PLU_EXCEPTION_PLUNONSTOCK:
					bFileLine = FALSE;
					break;

				default:
					bFileLine = TRUE;
					break;
				}
			}

			if ( bFileLine == TRUE )	
			{
				m_nDataLines++;
				m_nFiledLines++;

				if ( strHeader != "" )
				{
					fileOut.WriteLine( strHeader );
					strHeader = "";
				}

				fileOut.WriteLine( strBuffer );
				bFiledData = TRUE;
			}
		}

		m_nProgress += fileIn.GetLength();

		fileIn.Close();
		fileOut.Close();

		WriteHistoryBuffer();
		WriteAuditBuffers();
		
		CFileRemover FileRemover;
		FileRemover.RemoveFile( strPath + strFilenameIn );
		
		if ( bFiledData == TRUE )
			rename( strPath + strFilenameOut, strPath + strFilenameIn );
		else
			FileRemover.RemoveFile( strPath + strFilenameOut );
		
		if ( bWriteStockDatabase == TRUE )
		{
			CDataManagerInfo info;
			DataManager.WriteStockLevels( m_nSpIdx, info );
		}

		DataManager.CloseStockLevels( m_nSpIdx, info );
	}

	if ( nCurrentDbIdx != -1 )
		DataManager.CloseDatabase();
}

/**********************************************************************/

void CSalesXXXImporterStockman::DiscardSalesInternal()
{
	int nSize = m_arrayFileInfo.GetSize();
	CFileRemover FileRemover;

	for ( int x = 0; x < nSize; x++ )
	{
		m_nSpIdx = m_arrayFileInfo[x].m_nSpIdx;

		CString strPath = dbStockpoint.GetFolderPathSalesXXX( m_nSpIdx );
		strPath += "\\";
		strPath += m_arrayFileInfo[x].m_strFilename;

		if ( FileRemover.RemoveFile( strPath ) == TRUE )
			m_nFilesOpened++;

		StatusProgress.SetPos( x, nSize );
	}

	m_nFilesFound = nSize;
}

/**********************************************************************/

void CSalesXXXImporterStockman::ProcessSale( CStringArray& StockDetails, bool bPMS )
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
					
		if ( TRUE == bPMS )
			StockLevels.AddManualSales ( dStockQty, dStockValue, FALSE );
		else if ( m_RetryAudit.GetLineActionType() != STOCK_XXX_WASTAGE )
		{
			if ( StockDetails.GetSize() >= 2 )
				StockLevels.AddRecipeSales ( dStockQty, dStockValue, FALSE );
			else 
				StockLevels.AddEposSales( m_RetryAudit.GetLinePluLevel(), dStockQty, dStockValue );
		}
		else
			StockLevels.AddEcrWastage( dStockQty, dStockValue );
							
		DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );

		//AUDIT SALES
		if ( m_RetryAudit.GetLineActionType() != STOCK_XXX_WASTAGE )
		{
			CCSVStockAudit csvOut;
			csvOut.Add ( "D" );
			csvOut.Add ( AUDIT_SALES );
			csvOut.Add ( 1 );

			if ( TRUE == bPMS )
				csvOut.Add( SALES_TYPE_MANUAL );
			else if ( 0 == nIndex )
				csvOut.Add( SALES_TYPE_E1 );
			else
				csvOut.Add( SALES_TYPE_RECIPEITEM );

			csvOut.Add ( m_RetryAudit.GetLinePluNo() );
			csvOut.Add ( m_RetryAudit.GetHeaderEcrName() );
			csvOut.Add ( csvDetails.GetDescription() );
			csvOut.AddStockAuditQty ( m_RetryAudit.GetLinePluQty() );
			csvOut.AddStockAuditVal ( m_RetryAudit.GetLinePluValue() );
			csvOut.Add ( strStockCode );
			csvOut.Add ( csvDetails.GetSubUnits() );
			csvOut.AddStockAuditQty ( dStockQty );
			csvOut.AddStockAuditVal ( dStockValue );
			csvOut.Add ( m_RetryAudit.GetHeaderDateOfSale() );	
			csvOut.Add ( m_RetryAudit.GetHeaderTimeOfSale() );	
			m_arraySalesAuditBuffer.Add ( csvOut.GetLine() );		
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
			csvOut.Add ( m_RetryAudit.GetLinePluNo() );
			csvOut.Add ( csvDetails.GetDescription() );
			csvOut.AddStockAuditQty ( m_RetryAudit.GetLinePluQty() );
			csvOut.AddStockAuditVal ( m_RetryAudit.GetLinePluValue() );
			csvOut.Add ( strStockCode );
			csvOut.Add ( csvDetails.GetSubUnits() );
			csvOut.AddStockAuditQty ( dStockQty );
			csvOut.AddStockAuditVal ( dStockValue );
			csvOut.Add( "Epos Wastage" );
			csvOut.Add( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
			csvOut.Add( strStockDesc );
			csvOut.Add ( m_RetryAudit.GetHeaderDateOfSale() );
			csvOut.Add ( m_RetryAudit.GetHeaderTimeOfSale() );	
			m_arrayWasteAuditBuffer.Add ( csvOut.GetLine() );
		}

		int nHistorySalesType = 0;

		if ( TRUE == bPMS )
			nHistorySalesType = SALES_TYPE_MANUAL;
		else if ( m_RetryAudit.GetLineActionType() == STOCK_XXX_WASTAGE )
			nHistorySalesType = SALES_TYPE_ECRWASTE; 
		else
			nHistorySalesType = SALES_TYPE_E1;
		
		CCSV csvOut;
		csvOut.Add ( atoi ( m_strReportDate.Right(2) ) );
		csvOut.Add ( m_RetryAudit.GetHeaderTimeOfSale() );			
		csvOut.Add ( strStockCode );
		csvOut.Add ( nHistorySalesType );
		csvOut.Add ( dStockQty, 5 );
		csvOut.Add ( dStockValue, 5 );
		csvOut.Add ( StockRecord.GetCategory() );
		m_arraySalesHistoryBuffer.Add ( csvOut.GetLine() );
	}

	m_nDataLines++;
	
	if ( m_RetryAudit.GetLineActionType() == STOCK_XXX_WASTAGE )
		m_nWasteLines++;
	else
		m_nSalesLines++;
}

/**********************************************************************/

void CSalesXXXImporterStockman::ProcessDelivery( CStringArray& StockDetails )
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
		csvOut.Add( StockRecord.GetDescription() );
		csvOut.Add( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csvOut.Add( 0 );
		csvOut.Add( dStockQty, 5 );
		csvOut.Add( 0.0, 1 );
		csvOut.Add( dStockValue, 3 );
		csvOut.Add( StockRecord.GetSuppRefExternal(0, DealerFlags.GetSuppRefFlag() ) );
		
		m_arrayDeliveryAuditBuffer.Add ( csvOut.GetLine() );
	}
	
	m_nDataLines++;
	m_nDeliveryLines++;
}

/**********************************************************************/

void CSalesXXXImporterStockman::WriteAuditBuffers()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strPeriodAudit = dbStockpoint.GetFilePathAudit( m_nSpIdx, timeNow, AUDIT_FAMILY_NORMAL );
			
	CSSFile fileAudit;
	if ( fileAudit.Open( strPeriodAudit, "ab" ) == TRUE )
	{
		if ( m_arraySalesAuditBuffer.GetSize() != 0 )
		{
			CStockAuditBaseReport Audit;
			Audit.SetReportType( AUDIT_SALES );

			CStockAuditHeaderInfo info;
			info.m_date = timeNow;
			fileAudit.WriteLine( Audit.GetHeaderLine( info ) );

			for ( int n = 0; n < m_arraySalesAuditBuffer.GetSize(); n++ )
				fileAudit.WriteLine( m_arraySalesAuditBuffer.GetAt( n ) );
		}

		if ( m_arrayWasteAuditBuffer.GetSize() != 0 )
		{
			CStockAuditBaseReport Audit;
			Audit.SetReportType( AUDIT_ADJUSTMENT_PLU );

			CStockAuditHeaderInfo info;
			info.m_date = timeNow;
			fileAudit.WriteLine( Audit.GetHeaderLine( info ) );

			for ( int n = 0; n < m_arrayWasteAuditBuffer.GetSize(); n++ )
				fileAudit.WriteLine( m_arrayWasteAuditBuffer.GetAt( n ) );
		}

		if ( m_arrayDeliveryAuditBuffer.GetSize() != 0 )
		{
			CStockAuditBaseReport Audit;
			Audit.SetReportType( AUDIT_DELIVERY );

			CStockAuditHeaderInfo info;
			info.m_date = timeNow;
			info.m_strReference1 = "Epos";
			info.m_strSuppName = "Mixed Suppliers";
			fileAudit.WriteLine( Audit.GetHeaderLine( info ) );

			for ( int n = 0; n < m_arrayDeliveryAuditBuffer.GetSize(); n++ )
				fileAudit.WriteLine( m_arrayDeliveryAuditBuffer.GetAt( n ) );
		}

		fileAudit.Close();
	}
}

/**********************************************************************/

void CSalesXXXImporterStockman::WriteHistoryBuffer()
{
	if ( m_arraySalesHistoryBuffer.GetSize() == 0 )
		return;

	CString strHistoryFilename = dbStockpoint.GetFolderPathHistory( m_nSpIdx );
	strHistoryFilename += "\\St";
	strHistoryFilename += m_strReportDate.Left(6);
	strHistoryFilename += ".002";

	CSSFile fileHistory;
	if ( fileHistory.Open( strHistoryFilename, "ab" ) == TRUE )
	{
		for ( int n = 0; n < m_arraySalesHistoryBuffer.GetSize(); n++ )
			fileHistory.WriteLine( m_arraySalesHistoryBuffer.GetAt(n) );
	}
}

/**********************************************************************/
