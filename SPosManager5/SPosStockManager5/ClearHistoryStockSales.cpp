/**********************************************************************/
 
/**********************************************************************/
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "RepCSV.h"
#include "StockAuditBaseReport.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "ClearHistoryStockSales.h"
/**********************************************************************/

CClearHistoryStockSales::CClearHistoryStockSales( int nYear, int nMonth, int nDay, CLocationSelectorEntity& LocSelEntity )
{		
	CString strSearchDate;
	strSearchDate.Format ( "%4.4d%2.2d%2.2d", nYear, nMonth, nDay );

	CStockAuditBaseReport Audit;
	Audit.SetDbIdx( LocSelEntity.GetDbIdx() );
	Audit.SetConLevel( LocSelEntity.GetConType() );
	Audit.SetTableNo( LocSelEntity.GetEntityNo() );

	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
	{
		if ( Audit.MatchStockpoint( nSpIdx ) == FALSE )
			continue;

		ClearStockSales( nSpIdx, strSearchDate );
	}
}

/**********************************************************************/

void CClearHistoryStockSales::ClearStockSales ( int nSpIdx, const char* szSearchDate )
{
	CString strSearchDate = szSearchDate;
	if ( strSearchDate.GetLength() != 8 )
		return;

	CString strSearchMonth = strSearchDate.Left(6);

	CString strPath = dbStockpoint.GetFolderPathHistory( nSpIdx );
	strPath += "\\";

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strPath + "*.*" ) != 0 );

	CStringArray arrayFilesToFilter;

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		//WE ONLY NEED TO CHECK FILES
		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		//MUST BE 8 CHARS PLUS DOT AND EXTENSION
		CString strFilename = FileFinder.GetFileName();
		if ( strFilename.GetLength() != 12 )
			continue;

		//MUST BEGIN WITH ST
		strFilename.MakeUpper();
		if ( strFilename.Left(2) != "ST" )
			continue;

		//MUST BE A STOCK SALES HISTORY FILE
		if ( strFilename.Right(4) != ".001" && strFilename.Right(4) != ".002" )
			continue;

		//MUST CONTAIN A VALID MONTH
		CString strMonth = strFilename.Mid(2,6);
		if ( ::TestNumeric( strMonth ) == FALSE )
			continue;

		//MONTH MUST BE IN RANGE
		if ( strMonth < strSearchMonth )
			CFileRemover FileRemover ( strPath + strFilename );
		else if ( strMonth == strSearchMonth )
			arrayFilesToFilter.Add ( strPath + strFilename );
	}
	
	int nMDay = atoi ( strSearchDate.Right(2) );
	FilterStockSales( arrayFilesToFilter, nMDay );
}

/**********************************************************************/

void CClearHistoryStockSales::FilterStockSales( CStringArray& FileArray, int nMDay )
{
	for ( int nIndex = 0; nIndex < FileArray.GetSize(); nIndex++ )
	{
		CString strCurrentFile = FileArray.GetAt( nIndex );
		
		int nLen = strCurrentFile.GetLength();
		CString strTempFile = ( strCurrentFile.Left( nLen - 3 ) + "tmp" );

		CSSFile fileCurrent, fileTemp;
		if ( fileCurrent.Open ( strCurrentFile, "rb" ) == FALSE )
			continue;

		if ( fileTemp.Open ( strTempFile, "wb" ) == FALSE )
			continue;

		CString strBuffer;
		while ( fileCurrent.ReadString( strBuffer ) )
		{
			CCSVSalesHistory csv ( strBuffer );
			if ( csv.GetMDay() >= nMDay )
				fileTemp.WriteLine( strBuffer );
		}

		fileTemp.Close();
		fileCurrent.Close();
		CFileRemover FileRemover ( strCurrentFile );
		rename ( strTempFile, strCurrentFile );
	}
}

/**********************************************************************/


