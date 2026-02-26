/**********************************************************************/
 
/**********************************************************************/
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "StockAuditBaseReport.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "ClearHistoryStockAudits.h"
/**********************************************************************/

CClearHistoryStockAudits::CClearHistoryStockAudits( int nYear, int nMonth, int nDay, CLocationSelectorEntity& LocSelEntity )
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

		CString strPath = dbStockpoint.GetFolderPathAudit( nSpIdx );
		strPath += "\\";

		CFileFind FileFinder;	
		bool bWorking = ( FileFinder.FindFile ( strPath + "*.*" ) != 0 );

		CStringArray arrayToDelete;

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
			
			bool bAcceptExtension = FALSE;

			if ( ( strFilename.GetLength() == 12 ) && ( strFilename.Right(4) == ".001" ) )
				bAcceptExtension = TRUE;
			else if ( ( strFilename.GetLength() == 13 ) && ( strFilename.Right(5) == ".001b" ) )
				bAcceptExtension = TRUE;
				
			if ( FALSE == bAcceptExtension )
				continue;

			//DATE MUST BE NUMERIC
			CString strDate = strFilename.Left(8);
			if ( ::TestNumeric( strDate ) == FALSE )
				continue;

			//DATE MUST BE BEFORE SEARCH DATE
			if ( strDate >= strSearchDate )
				continue;
		
			arrayToDelete.Add ( strFilename );
		}
		for ( int nIndex = 0; nIndex < arrayToDelete.GetSize(); nIndex++ )
		{
			CString strFile = strPath + arrayToDelete.GetAt( nIndex );
			CFileRemover FileRemover ( strFile );
		}
	}
}

/**********************************************************************/


