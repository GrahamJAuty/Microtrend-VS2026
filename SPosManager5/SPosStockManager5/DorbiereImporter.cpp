 /**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DealerFlags.h"
//#include "DataManager.h"
//#include "EcrmanOptionsIni.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "RepCSV.h"
#include "ReportConsolidationArray.h"
#include "ReportHelpers.h"
#include "StockAuditDeliveryDetail.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "DorbiereImporter.h"
/**********************************************************************/

CDorbiereException::CDorbiereException()
{
	Reset();
}

/**********************************************************************/

void CDorbiereException::Reset()
{
	m_strStockCode = "";
	m_nReason = 0;
	m_strDescription = "";
}

/**********************************************************************/

void CDorbiereException::Add (CDorbiereException& source )
{
}

/**********************************************************************/

int CDorbiereException::Compare( CDorbiereException& source, int nHint )
{
	if ( m_strStockCode != source.m_strStockCode )
		return ( ( m_strStockCode > source.m_strStockCode ) ? 1 : -1 );

	if ( m_nReason != source.m_nReason )
		return ( ( m_nReason > source.m_nReason ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CDorbiereImporter::CDorbiereImporter(void)
{
}

/**********************************************************************/

void CDorbiereImporter::LogEvent( const char* szEvent1, const char* szEvent2 )
{
	CString strPath = "Dorbiere\\Import\\ImportLog.txt";
	GetDataProgramPath( strPath );

	CSSFile fileLog;
	if ( fileLog.Open( strPath, "ab" ) == TRUE )
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		CString strEvent;
		strEvent.Format( "%2.2d-%2.2d-%4.4d %2.2d:%2.2d:%2.2d   %s%s",
			timeNow.GetDay(),
			timeNow.GetMonth(),
			timeNow.GetYear(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond(),
			szEvent1,
			szEvent2 );

		fileLog.WriteLine( strEvent );
	}
}

/**********************************************************************/

void CDorbiereImporter::CheckForImport()
{
	CSSFile fileToken;
	CFilenameUpdater FnUp( SysFiles::DorbiereToken );
	if ( fileToken.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		CheckForImportInternal();
}

/**********************************************************************/

void CDorbiereImporter::CheckForImportInternal()
{
	CString strPath = "Dorbiere\\Import\\";
	GetDataProgramPath( strPath );
	
	{
		CFileFind finder;
		bool bWorking = ( finder.FindFile ( strPath + "*.CSV" ) != 0 );
		while (bWorking)   		
		{
			( bWorking = finder.FindNextFile() != 0 );

			if ( finder.IsDots() == TRUE )
				continue;
				
			if ( finder.IsDirectory() == TRUE )
				continue;

			ImportFileStage1( finder.GetFileName() );
			break;
		}
	}

	{
		CFileFind finder;
		bool bWorking = ( finder.FindFile ( strPath + "*.002" ) != 0 );
		while (bWorking)   		
		{
			( bWorking = finder.FindNextFile() != 0 );

			if ( finder.IsDots() == TRUE )
				continue;
				
			if ( finder.IsDirectory() == TRUE )
				continue;

			CString strFilename = finder.GetFileName();
			if ( strFilename.GetLength() == 7 )
			{
				CString strSiteNo = strFilename.Left(3);
				if ( ::TestNumeric( strSiteNo ) == TRUE )
				{
					int nSiteNo = atoi( strSiteNo );

					if ( nSiteNo != 0 )
					{
						for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
						{
							if ( dbStockpoint.GetImportSiteNo( nSpIdx ) == nSiteNo )
							{
								if ( ImportFileStage2( nSpIdx ) == TRUE )
									return;
								else
									break;
							}
						}
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CDorbiereImporter::ImportFileStage1( const char* szFilename )
{
	CString strPath = "Dorbiere\\Import\\";
	GetDataProgramPath( strPath );

	CString strFilename = szFilename;

	CString strOriginalPath = "";
	strOriginalPath += strPath;
	strOriginalPath += strFilename;

	LogEvent( "Detected File ", strFilename );

	CString strRenamedPath;
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		strRenamedPath.Format( "%s%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d_%s001",
			(const char*) strPath,
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond(),
			(const char*) strFilename.Left( strFilename.GetLength() - 3 ) );
	}

	if ( rename( strOriginalPath, strRenamedPath ) != 0 )
	{
		LogEvent( "Unable to rename file ", strFilename );
		return;
	}
	
	CSSFile fileIn;
	if ( fileIn.Open( strRenamedPath, "rb" ) == FALSE )
	{
		LogEvent( "Unable to open file ", strRenamedPath.Right( strFilename.GetLength() + 15 ) );
		return;
	}

	CSSFile fileOut;
	int nCurrentSiteNo = 0;
	int nCurrentSiteLines = 0;

	int nLineNo = 0;
	CString strBuffer = "";
	while( fileIn.ReadLine( strBuffer ) == TRUE )
	{
		CCSV csv( strBuffer, ',', '"', 1 );

		CString strSiteNo = csv.GetString(0);
		::TrimSpaces( strSiteNo, FALSE );

		int nImportSiteNo = atoi( strSiteNo );
		if ( ( nImportSiteNo >= 1 ) && ( nImportSiteNo <= 999 ) )
		{
			if ( nCurrentSiteNo != nImportSiteNo )
			{
				if ( nCurrentSiteNo != 0 )
				{
					fileOut.Close();

					CString strResult;
					strResult.Format( "Exported %d lines to %3.3d.002",
						nCurrentSiteLines,
						nCurrentSiteNo );

					LogEvent( strResult, "" );
					nCurrentSiteLines = 0;
				}

				CString strPathOut;
				strPathOut.Format( "%s%3.3d.002",
					(const char*) strPath,
					nImportSiteNo );

				if ( fileOut.Open( strPathOut, "ab" ) == TRUE )
					nCurrentSiteNo = nImportSiteNo;
				else
					nCurrentSiteNo = 0;
			}

			if ( nCurrentSiteNo != 0 )
			{
				fileOut.WriteLine( strBuffer );
				nCurrentSiteLines++;
			}
		}
	}

	if ( nCurrentSiteNo != 0 )
	{
		fileOut.Close();

		CString strResult;
		strResult.Format( "Exported %d lines to %3.3d.002",
			nCurrentSiteLines,
			nCurrentSiteNo );

		LogEvent( strResult, "" );
	}

	fileIn.Close();

	CFileRemover FileRemover;
	FileRemover.RemoveFile( strRenamedPath );
}

/**********************************************************************/

bool CDorbiereImporter::ImportFileStage2( int nSpIdx )
{
	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber( dbStockpoint.GetDbNo( nSpIdx ), nDbIdx ) == FALSE )
		return FALSE;

	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_PLU );
	arrayReadWrite.Add( NODE_STOCK );
	//arrayReadWrite.Add( NODE_BARCODE );

	if ( ( Sysset.IsBarmanSystem() == TRUE ) && ( EcrmanOptions.GetLabelsProductAutoBatchFlag() == TRUE ) )
		arrayReadWrite.Add( NODE_LABELS_PRODUCT );
		
	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info, FALSE ) == FALSE )
		return FALSE;
	
	if ( DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
	{	
		DataManager.CloseDatabase();
		return FALSE;
	}

	CString strOriginalPath;
	strOriginalPath.Format( "Dorbiere\\Import\\%3.3d.002",
		dbStockpoint.GetImportSiteNo( nSpIdx ) );

	GetDataProgramPath( strOriginalPath );

	CString strRenamedPath = "";
	strRenamedPath += strOriginalPath.Left( strOriginalPath.GetLength() - 3 );
	strRenamedPath += "003";

	if ( rename( strOriginalPath, strRenamedPath ) != 0 )
	{
		LogEvent( "Unable to rename file ", strOriginalPath.Right(7) );
		DataManager.CloseDatabase();
		return TRUE;
	}
	
	CSSFile fileIn;
	if ( fileIn.Open( strRenamedPath, "rb" ) == FALSE )
	{
		LogEvent( "Unable to open file ", strRenamedPath.Right(7) );
		DataManager.CloseDatabase();
		return TRUE;
	}

	CSSFile fileAudit;
	COleDateTime dateNow = COleDateTime::GetCurrentTime();
	CString strPath = dbStockpoint.GetFilePathAudit( nSpIdx, dateNow );
	if ( fileAudit.Open ( strPath, "ab" ) == FALSE )
	{
		LogEvent( "Unable to open audit ", strPath.Right(30) );
		DataManager.CloseDatabase();
		return TRUE;
	}

	CSSFile fileExceptions;
	{
		CString strPath = "Dorbiere\\Import\\Exceptions.txt";
		GetDataProgramPath( strPath );

		if ( fileExceptions.Open( strPath, "ab" ) == FALSE )
		{
			LogEvent( "Unable to open exceptions file", strPath.Right(30) );
			DataManager.CloseDatabase();
			return TRUE;
		}
	}

	CReportConsolidationArray<CDorbiereException> arrayExceptions;

	CStockAuditDeliveryDetail Audit( TRUE );
	Audit.SetReportType( AUDIT_DELIVERY );

	CString strCurrentDorbiereDate = "";
	CString strCurrentInvoice = "";

	int nLineCount = 0;
	int nUnknownCount = 0;
	int nNoSupplierCount = 0;

	CString strBuffer;
	while( fileIn.ReadString( strBuffer ) == TRUE )
	{
		CCSV csv( strBuffer );

		CString strStockCode = GetDorbiereString( csv, 5 );
		
		if ( strStockCode != "" )
		{
			CStockCSVRecord StockRecord;

			int nStockIdx;
			if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == FALSE )
			{
				CDorbiereException Exception;
				Exception.m_strStockCode = strStockCode;
				Exception.m_nReason = 1;
				Exception.m_strDescription = GetDorbiereString( csv, 6 );
				arrayExceptions.Consolidate( Exception );
				nUnknownCount++;
				continue;
			}

			//{
			//	StockRecord.SetStockCode( strStockCode );
			//	StockRecord.SetDescription( GetDorbiereString( csv, 6 ) );
			//	DataManager.Stock.InsertRecord( StockRecord, nStockIdx );
			//}
			//
				
			DataManager.Stock.GetAt( nStockIdx, StockRecord );

			if ( StockRecord.GetSupplierCount() == 0 )
			{
				CDorbiereException Exception;
				Exception.m_strStockCode = strStockCode;
				Exception.m_nReason = 2;
				Exception.m_strDescription = GetDorbiereString( csv, 6 );
				arrayExceptions.Consolidate( Exception );
				nNoSupplierCount++;
				continue;
			}

			double dDUYield = StockRecord.GetYield(0);
			double dDUQty = GetDorbiereInt( csv, 7 );
			//double dDUCost = StockRecord.GetCost(0);
			double dDUCost = GetDorbiereDouble( csv, 8 );
			double dSUQty = dDUQty * dDUYield;
			double dLineVal = dDUCost * dDUQty;
			
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, dbStockpoint.GetSpNo( nSpIdx ), StockLevels );
			StockLevels.AddDelivery( dSUQty, dLineVal, FALSE );
			DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, dbStockpoint.GetSpNo( nSpIdx ), StockLevels );

			CString strDorbiereDate = GetDorbiereString( csv, 3 );
			CString strInvoice = GetDorbiereString( csv, 4 );

			if ( ( strInvoice != strCurrentInvoice ) || ( strDorbiereDate != strCurrentDorbiereDate ) )
			{
				CStockAuditHeaderInfo infoHeader;
				infoHeader.m_date = dateNow;
				infoHeader.m_strReference1 = strDorbiereDate;
				infoHeader.m_strReference2 = strInvoice;
				infoHeader.m_strSuppName = "LWC";
				fileAudit.WriteLine( Audit.GetHeaderLine ( infoHeader ) );
				strCurrentInvoice = strInvoice;
				strCurrentDorbiereDate = strDorbiereDate;
			}

			double dCostTax = DataManager.Stock.CalculateTaxOnCost( StockRecord, dLineVal);
			double dCaseRetail = DataManager.Stock.CalculateRetailValue( StockRecord, dDUYield );
			double dLineRetail = DataManager.Stock.CalculateRetailValue( StockRecord, dSUQty );
			double dLineTax = DataManager.Stock.CalculateTaxContentFromValue( StockRecord, dLineRetail );
	
			double dLineWaste = 0.0;
			if ( StockOptions.GetStockSubUnitsFlag() )
			{
				double dNonWastePercent = 100.0 - StockRecord.GetWastePercent();
		
				if ( dNonWastePercent != 0.0 )
				{
					double dCostWithWaste = ( dLineVal * 100.0 ) / dNonWastePercent;  
					dLineWaste = dCostWithWaste - dLineVal;
				}
			}

			double dGPPercent = ReportHelpers.CalcGPPercent( dLineRetail - dLineTax, 0, dLineVal + dLineWaste );
			CString strCategory = DataManager.Category.GetText( StockRecord.GetCategory() );
			CString strSuppRef = StockRecord.GetSuppRefExternal(0,DealerFlags.GetSuppRefFlag() );
			
			CCSVStockAudit csvAudit;
			csvAudit.Add ( "D" );										//data line flag
			csvAudit.Add ( AUDIT_DELIVERY );							//report type	
			csvAudit.Add ( 4 );											//version no
			csvAudit.Add ( strStockCode );								//stock code
			csvAudit.Add ( StockRecord.GetDescription() );				//description	
			csvAudit.Add ( strCategory );								//category
			csvAudit.AddStockAuditQty ( dDUQty );						//case qty
			csvAudit.AddStockAuditQty ( dSUQty );						//stock unit qty
			csvAudit.AddStockAuditVal ( dDUCost );						//case cost
			csvAudit.AddStockAuditVal ( dLineVal );						//stock unit cost
			csvAudit.AddStockAuditVal ( dCaseRetail );					//case retail
			csvAudit.AddStockAuditVal ( dLineRetail );					//total retail
			csvAudit.AddStockAuditVal ( dLineTax );						//total tax content
			csvAudit.AddStockAuditVal ( dLineWaste );					//total wastage cost
			csvAudit.Add ( dGPPercent, 2 );								//GP Percent
			csvAudit.Add ( strSuppRef );								//supplier reference
			csvAudit.Add ( StockRecord.GetDUItems(0) );					//items per DU
			csvAudit.AddStockAuditQty( StockRecord.GetDUItemSize(0) );	//stock units per Item
			csvAudit.AddStockAuditVal( dCostTax );						//cost tax
			fileAudit.WriteLine( csvAudit.GetLine() );
			nLineCount++;

			if ( Sysset.IsBarmanSystem() && EcrmanOptions.GetLabelsProductAutoBatchFlag() )
				DataManager.ProductLabels.AutoBarcode ( StockRecord, int ( dSUQty ) );
		}
	}

	for ( int n = 0; n < arrayExceptions.GetSize(); n++ )
	{
		CDorbiereException Exception;
		arrayExceptions.GetAt(n, Exception);

		CCSV csv;
		csv.Add( Exception.m_strStockCode );
		csv.Add( Exception.m_strDescription );
		csv.Add( Exception.m_nReason );
		fileExceptions.WriteLine( csv.GetLine() );
	}

	fileIn.Close();
	fileAudit.Close();
	fileExceptions.Close();

	CFileRemover FileRemover;
	FileRemover.RemoveFile( strRenamedPath );

	DataManager.WriteStock( info, TRUE, TRUE );

	if ( ( Sysset.IsBarmanSystem() == TRUE ) && ( EcrmanOptions.GetLabelsProductAutoBatchFlag() == TRUE ) )
		DataManager.WriteProductLabels(info);

	DataManager.CloseDatabase();

	{
		CString strResult;
		strResult.Format( "Processed %d lines from %s",
			nLineCount,
			(const char*) strRenamedPath.Right(7) );
		LogEvent( strResult, "" );
	}

	if ( nUnknownCount != 0 )
	{
		CString strResult;
		strResult.Format( "Ignored %d unknown items from %s",
			nUnknownCount,
			(const char*) strRenamedPath.Right(7) );
		LogEvent( strResult, "" );
	}

	if ( nNoSupplierCount != 0 )
	{
		CString strResult;
		strResult.Format( "Ignored %d items with no supplier %s",
			nNoSupplierCount,
			(const char*) strRenamedPath.Right(7) );
		LogEvent( strResult, "" );
	}

	return TRUE;
}

/**********************************************************************/

const char* CDorbiereImporter::GetDorbiereString( CCSV& csv, int nField )
{
	m_strDorbiereString = csv.GetString(nField);
	::TrimSpaces( m_strDorbiereString, FALSE );
	return m_strDorbiereString;
}

/**********************************************************************/

int CDorbiereImporter::GetDorbiereInt( CCSV& csv, int nField )
{
	CString str = csv.GetString(nField);
	::TrimSpaces( str, FALSE );
	return atoi( str );
}

/**********************************************************************/

double CDorbiereImporter::GetDorbiereDouble( CCSV& csv, int nField )
{
	CString str = csv.GetString(nField);
	::TrimSpaces( str, FALSE );
	return atof( str );
}

/**********************************************************************/
