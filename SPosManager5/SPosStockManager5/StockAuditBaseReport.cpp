/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
//#include "DealerFlags.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "NodeTypes.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockAuditBaseReport.h"
/**********************************************************************/

CStockAuditHeaderInfo::CStockAuditHeaderInfo()
{
	m_date = COleDateTime::GetCurrentTime();
	m_strReference1 = "";
	m_strReference2 = "";
	m_strSuppName = "";
	m_strSpName1 = "";
	m_strSpName2 = "";
	m_strImportPath = "";
}

/**********************************************************************/

CStockAuditBaseReport::CStockAuditBaseReport()
{
	m_nReportType = AUDIT_DELIVERY;
	m_nConLevel = NODE_STOCKPOINT;
	m_nAuditMode = AUDIT_MODE_NORMAL;
	m_nTableNo = 1;
	m_strConLevel = "";
	m_nDbIdx = 0;
	m_strHeaderReferenceFilter = "";
	m_strLineReferenceFilter = "";
	m_strPreviewHeader = "";
	m_strStockCodeFrom = "";
	m_strStockCodeTo = "";
	m_bWaitForData = FALSE;
	m_bTransferIn = FALSE;
	m_bLockedDatabase = FALSE;
}

/**********************************************************************/

const char* CStockAuditBaseReport::GetReportName()
{
	switch ( m_nReportType )
	{
	case AUDIT_DELIVERY:					m_strReportName =  "Delivery Detail";						break;
	case AUDIT_DELIVERY_SUMMARY:			m_strReportName =  "Delivery Summary";						break;
	case AUDIT_ADJUSTMENT_STOCK:			
	case AUDIT_ADJUSTMENT_PLU:				m_strReportName =  "Adjustment";							break;
	case AUDIT_OPENING:						m_strReportName =  "Opening Stock";							break;
	case AUDIT_CLOSING:						m_strReportName =  "Closing Stock";							break;
	case AUDIT_STOCKTAKE:					m_strReportName =  "Stocktaking";							break;
	case AUDIT_RETURNS:						m_strReportName =  "Returns";								break;
	case AUDIT_RETURNS_SUMMARY:				m_strReportName =  "Returns Summary";						break;
	case AUDIT_SALES:						m_strReportName =  "Sales";									break;
	case AUDIT_TRANSFER:					m_strReportName =  "Transfers";								break;
	case AUDIT_ITEM_ACTIVITY:				m_strReportName =  "Stock Item Activity";					break;
	case AUDIT_EX_SALES:					m_strReportName =  "Sales Exceptions";						break;
	case AUDIT_EX_DELIVERY:					m_strReportName =  "Delivery Exceptions";					break;
	case AUDIT_EX_RETURNS:					m_strReportName =  "Returns Exceptions";					break;
	case AUDIT_EX_OPENING:					m_strReportName =  "Opening Stock Exceptions";				break;
	case AUDIT_EX_CLOSING:					m_strReportName =  "Closing Stock Exceptions";				break;
	case AUDIT_EX_ADJUSTMENT_STOCK:			
	case AUDIT_EX_ADJUSTMENT_PLU:			m_strReportName =  "Adjustment Exceptions";					break;
	case AUDIT_EX_TRANSFER:					m_strReportName =  "Transfer Exceptions";					break;
	case AUDIT_EX_IMPORT:					m_strReportName =  "Stock Exceptions";						break;
	case AUDIT_IMPORT_PLU_ADJUSTMENT_STOCK:
	case AUDIT_IMPORT_PLU_ADJUSTMENT_PLU:	m_strReportName =  "Import Adjustments by Plu Number";		break;
	case AUDIT_IMPORT_PLU_DELIVERY:			m_strReportName =  "Import Deliveries by Plu Number";		break;
	case AUDIT_IMPORT_PLU_RETURNS:			m_strReportName =  "Import Returns by Plu Number";			break;
	case AUDIT_IMPORT_PLU_SALES:			m_strReportName =  "Import Sales by Plu Number";			break;
	case AUDIT_IMPORT_PLU_OPENING:			m_strReportName =  "Import Opening Stock by Plu Number";	break;
	case AUDIT_IMPORT_PLU_CLOSING:			m_strReportName =  "Import Closing Stock by Plu Number";	break;
	case AUDIT_IMPORT_PLU_TRANSFER:			m_strReportName =  "Import Stock Transfers by Plu Number";	break;
	case AUDIT_IMPORT_WRAPPER_DELIVERY:		m_strReportName =  "Import Deliveries by Wrapper Code";		break;
	case AUDIT_IMPORT_WRAPPER_RETURNS:		m_strReportName =  "Import Returns by Wrapper Code";		break;
	case AUDIT_IMPORT_WRAPPER_SALES:		m_strReportName =  "Import Sales by Wrapper Code";			break;
	case AUDIT_IMPORT_WRAPPER_OPENING:		m_strReportName =  "Import Opening Stock by Wrapper Code";		break;
	case AUDIT_IMPORT_WRAPPER_CLOSING:		m_strReportName =  "Import Closing Stock by Wrapper Code";		break;
	case AUDIT_IMPORT_WRAPPER_TRANSFER:		m_strReportName =  "Import Stock Transfers by Wrapper Code";	break;
	case AUDIT_RETRY_SALES:					m_strReportName =  "Exceptions Retry";						break;
	default:								m_strReportName =  "Unknown";								break;
	}

	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
	{
		switch( m_nReportType )
		{
		case AUDIT_IMPORT_PLU_ADJUSTMENT_STOCK:	
		case AUDIT_IMPORT_PLU_ADJUSTMENT_PLU:	m_strReportName =  "Import Adjustments by Barcode Number";		break;		
		case AUDIT_IMPORT_PLU_DELIVERY:			m_strReportName =  "Import Deliveries by Barcode Number";		break;
		case AUDIT_IMPORT_PLU_RETURNS:			m_strReportName =  "Import Returns by Barcode Number";			break;
		case AUDIT_IMPORT_PLU_SALES:			m_strReportName =  "Import Sales by Barcode Number";			break;
		case AUDIT_IMPORT_PLU_OPENING:			m_strReportName =  "Import Opening Stock by Barcode Number";	break;
		case AUDIT_IMPORT_PLU_CLOSING:			m_strReportName =  "Import Closing Stock by Barcode Number";	break;
		case AUDIT_IMPORT_PLU_TRANSFER:			m_strReportName =  "Import Stock Transfers by Barcode Number";	break;
		}
	}

	return m_strReportName;
}

/**********************************************************************/

const char* CStockAuditBaseReport::GetHeaderLine( CStockAuditHeaderInfo& info )
{
	CString strDate;
	strDate.Format ( "%2.2d/%2.2d/%4.4d", 
		info.m_date.GetDay(), 
		info.m_date.GetMonth(), 
		info.m_date.GetYear() );

	CString strTime;
	strTime.Format( "%2.2d:%2.2d", 
		info.m_date.GetHour(), 
		info.m_date.GetMinute() );

	int nExtraFields = 0;
	switch( GetAuditDataType() )
	{
	case AUDIT_STOCKTAKE:
	case AUDIT_ADJUSTMENT_STOCK:
	case AUDIT_ADJUSTMENT_PLU:
	case AUDIT_EX_ADJUSTMENT_STOCK:
	case AUDIT_EX_ADJUSTMENT_PLU:
	case AUDIT_EX_CLOSING:
		nExtraFields = 2;
		break;

	case AUDIT_DELIVERY:
	case AUDIT_DELIVERY_SUMMARY:
	case AUDIT_RETURNS:
	case AUDIT_RETURNS_SUMMARY:
	case AUDIT_EX_DELIVERY:
	case AUDIT_EX_RETURNS:
		nExtraFields = 3;
		break;

	case AUDIT_EX_TRANSFER:
		nExtraFields = 5;
		break;

	case AUDIT_IMPORT_PLU_ADJUSTMENT_STOCK:
	case AUDIT_IMPORT_PLU_ADJUSTMENT_PLU:
	case AUDIT_IMPORT_PLU_DELIVERY:
	case AUDIT_IMPORT_PLU_RETURNS:
	case AUDIT_IMPORT_PLU_SALES:
	case AUDIT_IMPORT_PLU_OPENING:
	case AUDIT_IMPORT_PLU_CLOSING:
	case AUDIT_IMPORT_PLU_TRANSFER:
	case AUDIT_IMPORT_WRAPPER_DELIVERY:
	case AUDIT_IMPORT_WRAPPER_RETURNS:
	case AUDIT_IMPORT_WRAPPER_SALES:
	case AUDIT_IMPORT_WRAPPER_OPENING:
	case AUDIT_IMPORT_WRAPPER_CLOSING:
	case AUDIT_IMPORT_WRAPPER_TRANSFER:
	case AUDIT_TRANSFER:
		nExtraFields = 6;
		break;
	}

	CCSV csv;
	csv.Add ( "H" );				//HEADER LINE
	csv.Add ( m_nReportType );		//DATA TYPE
	csv.Add ( 2 );					//VERSION NO
	csv.Add ( strDate );
	csv.Add ( strTime );
	if ( nExtraFields >= 1 )	csv.Add ( info.m_strReference1 );
	if ( nExtraFields >= 2 )	csv.Add ( info.m_strReference2 );
	if ( nExtraFields >= 3 )	csv.Add ( info.m_strSuppName );
	if ( nExtraFields >= 4 )	csv.Add ( info.m_strSpName1 );
	if ( nExtraFields >= 5 )	csv.Add ( info.m_strSpName2 );
	if ( nExtraFields >= 6 )	csv.Add ( info.m_strImportPath );
	
	m_strHeaderLine = csv.GetLine();
	return m_strHeaderLine;
}

/**********************************************************************/

bool CStockAuditBaseReport::IsExceptionReport()
{
	switch( m_nReportType )
	{
	case AUDIT_EX_DELIVERY:
	case AUDIT_EX_RETURNS:
	case AUDIT_EX_ADJUSTMENT_STOCK:
	case AUDIT_EX_ADJUSTMENT_PLU:
	case AUDIT_EX_OPENING:
	case AUDIT_EX_CLOSING:
	case AUDIT_EX_SALES:
	case AUDIT_EX_TRANSFER:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CStockAuditBaseReport::IsImportReport()
{
	switch( m_nReportType )
	{
	case AUDIT_IMPORT_PLU_DELIVERY:
	case AUDIT_IMPORT_PLU_RETURNS:
	case AUDIT_IMPORT_PLU_ADJUSTMENT_STOCK:
	case AUDIT_IMPORT_PLU_ADJUSTMENT_PLU:
	case AUDIT_IMPORT_PLU_SALES:
	case AUDIT_IMPORT_PLU_OPENING:
	case AUDIT_IMPORT_PLU_CLOSING:
	case AUDIT_IMPORT_PLU_TRANSFER:
	case AUDIT_IMPORT_WRAPPER_DELIVERY:
	case AUDIT_IMPORT_WRAPPER_RETURNS:
	case AUDIT_IMPORT_WRAPPER_SALES:
	case AUDIT_IMPORT_WRAPPER_OPENING:
	case AUDIT_IMPORT_WRAPPER_CLOSING:
	case AUDIT_IMPORT_WRAPPER_TRANSFER:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

int CStockAuditBaseReport::GetAuditDataType()
{
	switch( m_nReportType )
	{
	case AUDIT_DELIVERY_SUMMARY:	return AUDIT_DELIVERY;
	case AUDIT_RETURNS_SUMMARY:		return AUDIT_RETURNS;
	default:						return m_nReportType;
	}
}

/**********************************************************************/

const char* CStockAuditBaseReport::GetReportParamsFilename()
{
	CFilenameUpdater FnUp ( SysFiles::StockAuditPrm, 0, m_nReportType );
	m_strReportParamsFilename = FnUp.GetFilenameToUse();
	return m_strReportParamsFilename;
}

/**********************************************************************/

bool CStockAuditBaseReport::CreateReport( int nDbIdx )
{
	m_nDbIdx = nDbIdx;
	bool bSuccess = FALSE;

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		bSuccess = TRUE;
		AddColumns( ReportFile );
		SeparateStockpoints ( ReportFile );
		AfterReport( ReportFile );
		ShowStockpoints ( ReportFile );
		ReportFile.Close();
	}
	return bSuccess;
}

/**********************************************************************/

bool CStockAuditBaseReport::ProcessHeader( const char* szHeader )
{
	CCSV csv ( szHeader );

	bool bAccept = FALSE;

	int nAuditType = GetAuditDataType();
	int nLineType = csv.GetInt(1);

	switch( nAuditType )
	{
	case AUDIT_ADJUSTMENT_PLU:
	case AUDIT_ADJUSTMENT_STOCK:
		bAccept = ( AUDIT_ADJUSTMENT_PLU == nLineType ) || ( AUDIT_ADJUSTMENT_STOCK == nLineType );
		break;

	case AUDIT_ITEM_ACTIVITY:
		switch( nLineType )
		{
		case AUDIT_DELIVERY:
		case AUDIT_ADJUSTMENT_STOCK:
		case AUDIT_OPENING:
		case AUDIT_STOCKTAKE:
		case AUDIT_RETURNS:
		case AUDIT_SALES:
		case AUDIT_TRANSFER:
		case AUDIT_ADJUSTMENT_PLU:
			bAccept = TRUE;
			break;
		}
		break;

	default:
		bAccept = ( nAuditType == nLineType );
		break;
	}

	if ( FALSE == bAccept )
		return FALSE;

	switch ( csv.GetInt(2) )
	{
	case 1:
		m_strHeaderDate = csv.GetString(3);
		m_strHeaderTime = csv.GetString(4);
		m_strHeaderReference1 = csv.GetString(5);
		m_strHeaderReference2 = "";
		m_strHeaderSuppName = csv.GetString(6);
		m_strHeaderSpName1 = csv.GetString(7);
		m_strHeaderSpName2 = csv.GetString(8);
		m_strHeaderImportPath = csv.GetString(9);
		break;

	case 2:
		m_strHeaderDate = csv.GetString(3);
		m_strHeaderTime = csv.GetString(4);
		m_strHeaderReference1 = csv.GetString(5);
		m_strHeaderReference2 = csv.GetString(6);
		m_strHeaderSuppName = csv.GetString(7);
		m_strHeaderSpName1 = csv.GetString(8);
		m_strHeaderSpName2 = csv.GetString(9);
		m_strHeaderImportPath = csv.GetString(10);
		break;

	default:
		m_strHeaderDate = "";
		m_strHeaderTime = "";
		m_strHeaderReference1 = "";
		m_strHeaderReference2 = "";
		m_strHeaderSuppName = "";
		m_strHeaderSpName1 = "";
		m_strHeaderSpName2 = "";
		m_strHeaderImportPath = "";
		break;
	}

	CString strTempRef1 = m_strHeaderReferenceFilter;
	
	if ( strTempRef1 == "" )
		return TRUE;
	
	CString strTempRef2 = m_strHeaderReference1;
	CString strTempRef3 = m_strHeaderReference2;
	
	strTempRef1.MakeUpper();
	strTempRef2.MakeUpper();
	strTempRef3.MakeUpper();

	if ( strTempRef2.Find( strTempRef1 ) != -1 )
		return TRUE;

	if ( strTempRef3.Find( strTempRef1 ) != -1 )
		return TRUE;

	switch( m_nReportType )
	{
	case AUDIT_DELIVERY:
	case AUDIT_DELIVERY_SUMMARY:
	case AUDIT_RETURNS:
	case AUDIT_RETURNS_SUMMARY:
		strTempRef2 = m_strHeaderSuppName;
		strTempRef2.MakeUpper();
		return ( strTempRef2.Find( strTempRef1 ) != -1 );

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CStockAuditBaseReport::ProcessLine( const char* szLine )
{
	return FALSE;
}

/**********************************************************************/

void CStockAuditBaseReport::SeparateStockpoints( CReportFile& ReportFile )
{	
	/*
	if ( EcrmanOptions.GetSortLocNameFlag() == FALSE )
	{
		for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
			SeparateStockpoints( ReportFile, nSpIdx );
	}
	else
	*/
	{
		CWordArray arraySpIdx;
		dbStockpoint.SortByName( arraySpIdx );

		for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
		{
			int nSpIdx = arraySpIdx.GetAt(n);
			SeparateStockpoints( ReportFile, nSpIdx );
		}
	}

	AppendGrandTotals( ReportFile );
	ReportFile.WriteLine ( "" );
}

/**********************************************************************/

void CStockAuditBaseReport::SeparateStockpoints( CReportFile& ReportFile, int nSpIdx )
{
	if ( MatchStockpoint ( nSpIdx ) == FALSE )
			return;;
		
	if ( FALSE == m_bLockedDatabase )
	{
		switch( m_nReportType )
		{
		case AUDIT_RETRY_SALES:
		case AUDIT_ADJUSTMENT_PLU:
		case AUDIT_ADJUSTMENT_STOCK:
			{
				int nDbIdx;
				int nDbNo = dbStockpoint.GetDbNo( nSpIdx );
				if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
					return;

				CDataManagerInfo info;
				DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
			}
			break;
		}
	}

	if ( IsImportReport() == FALSE )
	{
		CString strLine = "<..>";
		strLine += dbStockpoint.GetName( nSpIdx );
		ReportFile.WriteLine ( strLine );
		ReportFile.WriteLine ( "<LI>" );
	}

	__int64 nTotalFileSize = 0;
	CReportConsolidationArray<CSortedStringItem> arrayFiles;
	GetAuditFileList( nSpIdx, arrayFiles, nTotalFileSize );

	bool bPreviewHeader = FALSE;
	if ( m_strPreviewHeader != "" )
	{
		if ( ProcessHeader( m_strPreviewHeader ) == TRUE )
		{
			WriteHeader( ReportFile );
			bPreviewHeader = TRUE;
		}
	}

	bool bGotStockpointLine = TRUE;

	__int64 nProgressSoFar = 0;
	StatusProgress.SetDialogText( dbStockpoint.GetName( nSpIdx ) );
	StatusProgress.SetPos( 0, nTotalFileSize );

	for ( int nIndex = 0; nIndex < arrayFiles.GetSize(); nIndex++ )
	{
		CString strFilename;
			
		if ( m_nAuditMode == AUDIT_MODE_RETRY )
			strFilename = dbStockpoint.GetFolderPathSalesXXX( nSpIdx );
		else
			strFilename = dbStockpoint.GetFolderPathAudit( nSpIdx );

		CSortedStringItem item;
		arrayFiles.GetAt( nIndex, item );

		strFilename += "\\";
		strFilename += item.m_strItem;

		m_bTransferIn = ( strFilename.Right(1) == "b" );

		CSSFile fileAudit;
		if ( fileAudit.Open ( strFilename, "rb" ) == TRUE )
		{	
			CString strBuffer;
			m_bProcessBlock = bPreviewHeader;
			bool bWrittenHeader = bPreviewHeader;		
				
			while ( fileAudit.ReadString ( strBuffer ) == TRUE )
			{
				StatusProgress.SetPos( nProgressSoFar + fileAudit.Tell(), nTotalFileSize );
			
				if ( strBuffer.Left(1) == "H" )
				{
					if ( ( m_bProcessBlock == TRUE ) && ( bWrittenHeader == TRUE ) )
						AppendTotals ( ReportFile );

					m_bProcessBlock = ProcessHeader( strBuffer );
								
					if ( ( m_bProcessBlock == TRUE ) && ( m_bWaitForData == FALSE ) )
					{
						WriteHeader( ReportFile );
						bWrittenHeader = TRUE;
					}
					else
						bWrittenHeader = FALSE;
				}
				else if ( m_bProcessBlock == TRUE )
				{
					if ( ProcessLine( strBuffer ) == TRUE )
					{
						if ( bWrittenHeader == FALSE )
						{
							WriteHeader( ReportFile );
							bWrittenHeader = TRUE;
						}

						WriteLine( ReportFile, nSpIdx );
						bGotStockpointLine = TRUE;
					}
				}
			}
			if ( ( m_bProcessBlock == TRUE ) && ( bWrittenHeader == TRUE ) )
				AppendTotals ( ReportFile );

			nProgressSoFar += fileAudit.GetLength();
			fileAudit.Close();
		}
	}
	AppendStockpointTotals( ReportFile );

	if ( bGotStockpointLine == TRUE )
		ReportFile.WriteLine ( "" );
}

/**********************************************************************/

int CStockAuditBaseReport::CountStockpoints()
{
	int nCount = 0;

	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
		if ( MatchStockpoint ( nSpIdx ) == TRUE )
			nCount++;

	return nCount;
}

/**********************************************************************/

void CStockAuditBaseReport::ShowStockpoints( CReportFile& ReportFile )
{
	ReportFile.WriteLine ( "<..>Report includes stockpoints" );
	ReportFile.WriteLine ( "<LI>" );

	CArray<int,int> arraySpIdx;
	
	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
		if ( MatchStockpoint ( nSpIdx ) == TRUE )
			arraySpIdx.Add( nSpIdx );

	//if ( EcrmanOptions.GetSortLocNameFlag() == TRUE )
		dbStockpoint.SortSpListByName( arraySpIdx );

	for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
	{
		int nSpIdx = arraySpIdx.GetAt(n);

		CString strLine = "<..>";
		strLine += dbStockpoint.GetName( nSpIdx );
		ReportFile.WriteLine ( strLine );
	}
}

/**********************************************************************/

bool CStockAuditBaseReport::MatchStockpoint ( int nSpIdx )
{
	if ( m_nConLevel == NODE_SYSTEM )
		return TRUE;

	if ( m_nDbIdx < 0 || m_nDbIdx >= dbDatabase.GetSize() )
		return FALSE;

	int nDbNo = dbDatabase.GetDbNo( m_nDbIdx );
	if ( dbStockpoint.GetDbNo( nSpIdx ) != nDbNo )
		return FALSE;

	bool bMatch = FALSE;
	switch ( m_nConLevel )
	{
	case NODE_STOCKPOINT:	bMatch = ( dbStockpoint.GetSpNo( nSpIdx ) == m_nTableNo );		break;
	case NODE_SITE:		bMatch = ( dbStockpoint.GetSiteNo( nSpIdx ) == m_nTableNo );	break;
	case NODE_SYSTEM:		
	case NODE_DATABASE:		bMatch = TRUE;		break;
	default:				bMatch = FALSE;	break;
	}
	return bMatch;
}

/**********************************************************************/

const char* CStockAuditBaseReport::GetReportTitle()
{
	CString strDateFrom, strDateTo;
	strDateFrom.Format ( "%2.2d/%2.2d/%4.4d", m_AuditDateFrom.GetDay(), m_AuditDateFrom.GetMonth(), m_AuditDateFrom.GetYear() );
	strDateTo.Format ( "%2.2d/%2.2d/%4.4d", m_AuditDateTo.GetDay(), m_AuditDateTo.GetMonth(), m_AuditDateTo.GetYear() );

	CString strFamily = "";
	switch( m_nAuditMode )
	{
	case AUDIT_MODE_NORMAL:
		strFamily = "audit";
		break;

	default:
		strFamily = "preview";
		break;
	}

	m_strReportTitle.Format ( "%s %s for %s", 
			GetReportName(),
			( const char* ) strFamily,
			( const char* ) m_strConLevel );
	
	switch( m_nAuditMode )
	{
	case AUDIT_MODE_NORMAL:
	case AUDIT_MODE_RETRY:
		{
			CString strDateRange;
			strDateRange.Format ( " (%s - %s)",
				(const char*) strDateFrom,
				(const char*) strDateTo );
			m_strReportTitle += strDateRange;
		}
		break;
	}

	return m_strReportTitle;
}

/**********************************************************************/

const char* CStockAuditBaseReport::GetDisplayStockCode ( const char* szCode )
{
	m_strDisplayStockCode = szCode;

	if ( StockOptions.GetReportsTrimStockZerosFlag() == TRUE )
		::TrimSpaces ( m_strDisplayStockCode, TRUE );
		
	return m_strDisplayStockCode;
}

/*********************************************************************/

void CStockAuditBaseReport::GetAuditFileList( int nSpIdx, CReportConsolidationArray<CSortedStringItem>& arrayFiles, __int64& nTotalFileSize )
{
	nTotalFileSize = 0;
	arrayFiles.RemoveAll();

	CString strDateFrom, strDateTo;
	strDateFrom.Format ( "%4.4d%2.2d%2.2d", m_AuditDateFrom.GetYear(), m_AuditDateFrom.GetMonth(), m_AuditDateFrom.GetDay() );
	strDateTo.Format ( "%4.4d%2.2d%2.2d", m_AuditDateTo.GetYear(), m_AuditDateTo.GetMonth(), m_AuditDateTo.GetDay() );

	CStringArray arrayTemp;

	switch ( m_nAuditMode )
	{
	case AUDIT_MODE_TEMP:
		if ( IsExceptionReport() )
			arrayTemp.Add ( dbStockpoint.GetFileNameAudit( nSpIdx, m_AuditDateFrom, AUDIT_FAMILY_TEMP_EX ) );
		else
			arrayTemp.Add( dbStockpoint.GetFileNameAudit( nSpIdx, m_AuditDateFrom, AUDIT_FAMILY_TEMP ) );
		break;

	case AUDIT_MODE_TEMP2:
		if ( IsExceptionReport() )
			arrayTemp.Add( dbStockpoint.GetFileNameAudit( nSpIdx, m_AuditDateFrom, AUDIT_FAMILY_TEMP2_EX  ) );
		else
			arrayTemp.Add( dbStockpoint.GetFileNameAudit( nSpIdx, m_AuditDateFrom, AUDIT_FAMILY_TEMP2 ) );
		break;

	case AUDIT_MODE_PREVIEW:
		if ( IsExceptionReport() )
			arrayTemp.Add( dbStockpoint.GetFileNameAudit( nSpIdx, m_AuditDateFrom, AUDIT_FAMILY_PREV_EX ) );
		else
			arrayTemp.Add( dbStockpoint.GetFileNameAudit( nSpIdx, m_AuditDateFrom, AUDIT_FAMILY_PREV ) );
		break;

	case AUDIT_MODE_IMPORT:
		arrayTemp.Add( dbStockpoint.GetFileNameAudit( nSpIdx, m_AuditDateFrom, AUDIT_FAMILY_IMPORT ) );
		break;

	case AUDIT_MODE_NORMAL:
		{	
			CString strPath = dbStockpoint.GetFolderPathAudit( nSpIdx );
	
			CFileFind FileFinder;	
			bool bWorking = ( FileFinder.FindFile ( strPath + "\\*.*" ) != 0 );

			while (bWorking)   
			{
				( bWorking = FileFinder.FindNextFile() != 0 );

				//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
				if ( FileFinder.IsDots() == TRUE )
					continue;

				if ( FileFinder.IsDirectory() == TRUE )
					continue;

				CString strFilename = FileFinder.GetFileName();

				bool bAcceptExtension = FALSE;

				if ( ( strFilename.GetLength() == 12 ) && ( strFilename.Right(4) == ".001" ) )
					bAcceptExtension = TRUE;
				else if ( ( AUDIT_TRANSFER == m_nReportType ) || ( AUDIT_ITEM_ACTIVITY == m_nReportType ) )
				{
					if ( ( strFilename.GetLength() == 13 ) && ( strFilename.Right(5) == ".001b" ) )
						bAcceptExtension = TRUE;
				}

				if ( FALSE == bAcceptExtension )
					continue;

				CString strDate = strFilename.Left(8);
				if ( strDate < strDateFrom || strDate > strDateTo || ( ::TestNumeric( strDate ) == FALSE ) )
					continue;

				arrayTemp.Add( strFilename );
			}
		}
		break;

	case AUDIT_MODE_RETRY:
		{	
			CString strPath = dbStockpoint.GetFolderPathSalesXXX( nSpIdx );
	
			CFileFind FileFinder;	
			bool bWorking = ( FileFinder.FindFile ( strPath + "\\*.*" ) != 0 );

			while (bWorking)   
			{
				( bWorking = FileFinder.FindNextFile() != 0 );

				//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
				if ( FileFinder.IsDots() == TRUE )
					continue;

				if ( FileFinder.IsDirectory() == TRUE )
					continue;

				CString strFilename = FileFinder.GetFileName();
				strFilename.MakeUpper();

				CString strDate = "";

				if ( strFilename.GetLength() == 21 )
					if ( strFilename.Left(9) == "SALESXXX_" )
						if ( strFilename.Right(4) == ".001" )
							strDate = strFilename.Mid(9,8);

				if ( strFilename.GetLength() == 25 )
					if ( strFilename.Left(13) == "SALESXXX_PMS_" )
						if ( strFilename.Right(4) == ".001" )
							strDate = strFilename.Mid(13,8);
			
				if ( strDate == "" )
					continue;
				
				if ( strDate < strDateFrom || strDate > strDateTo || ( ::TestNumeric( strDate ) == FALSE ) )
					continue;

				arrayTemp.Add( strFilename );
			}
		}
		break;
	}

	CString strPath = "";
	if ( m_nAuditMode == AUDIT_MODE_RETRY )
		strPath = dbStockpoint.GetFolderPathSalesXXX( nSpIdx );
	else
		strPath = dbStockpoint.GetFolderPathAudit( nSpIdx );

	strPath += "\\";

	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CString strFilename = arrayTemp.GetAt(n);

		CSortedStringItem item;
		item.m_strItem = strFilename;
		arrayFiles.Consolidate( item );

		CFileStatus FileStatus;
		CFile::GetStatus( strPath + strFilename, FileStatus );
		nTotalFileSize += FileStatus.m_size;
	}
}

/**********************************************************************/

