/**********************************************************************/
 
/**********************************************************************/
#include "CashRSPImporter.h"
#include "Consolidation.h"
#include "DatabaseCSVArray.h"
#include "Direct.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "ReportConsolidationArray.h"
#include "NodeTypes.h"
/**********************************************************************/
#include "ClearHistoryEposSales.h"
/**********************************************************************/

CClearHistoryEposSales::CClearHistoryEposSales( COleDateTime& dateTo, CLocationSelectorEntity& LocSelEntity, CEposSelectArray& SelectArray, bool bDeletePlu, bool bDeleteSummary, bool bDeleteDetail, bool bIncludeToDate ) 
{
	COleDateTime dateFrom = COleDateTime( 1800, 1, 1, 0, 0, 0 );
	ClearEposSales( dateFrom, dateTo, LocSelEntity, SelectArray, bDeletePlu, bDeleteSummary, bDeleteDetail, bIncludeToDate );
}

/**********************************************************************/

CClearHistoryEposSales::CClearHistoryEposSales( COleDateTime& dateFrom, COleDateTime& dateTo, CLocationSelectorEntity& LocSelEntity, CEposSelectArray& SelectArray, bool bDeletePlu, bool bDeleteSummary, bool bDeleteDetail, bool bIncludeToDate ) 
{
	ClearEposSales( dateFrom, dateTo, LocSelEntity, SelectArray, bDeletePlu, bDeleteSummary, bDeleteDetail, bIncludeToDate );
}

/**********************************************************************/

void CClearHistoryEposSales::ClearEposSales( COleDateTime& dateFrom, COleDateTime& dateTo, CLocationSelectorEntity& LocSelEntity, CEposSelectArray& SelectArray, bool bDeletePlu, bool bDeleteSummary, bool bDeleteDetail, bool bIncludeToDate ) 
{
	m_bDeletePlu = bDeletePlu;
	m_bDeleteSummary = bDeleteSummary;
	m_bDeleteDetail = bDeleteDetail;

	CString strDateFrom;
	strDateFrom.Format ( "%4.4d%2.2d%2.2d", dateFrom.GetYear(), dateFrom.GetMonth(), dateFrom.GetDay() );

	CString strDateTo;
	strDateTo.Format ( "%4.4d%2.2d%2.2d", dateTo.GetYear(), dateTo.GetMonth(), dateTo.GetDay() );

	SelectArray.MakeList();
	ClearEposSales( SelectArray, strDateFrom, strDateTo, bIncludeToDate );	

#ifdef STOCKMAN_SYSTEM
	if ( TRUE == m_bDeletePlu )
	{
		switch( LocSelEntity.GetConType() )
		{
		case NODE_DATABASE:
			ClearDatabaseSales( LocSelEntity.GetDbIdx(), strDateFrom, strDateTo );
			break;

		case NODE_SYSTEM:
			{
				for ( int n = 0; n < dbDatabase.GetSize(); n++ )
					ClearDatabaseSales( n, strDateFrom, strDateTo );
			}
		}
	}
#endif
}

/**********************************************************************/

void CClearHistoryEposSales::ClearEposSales( CEposSelectArray& SelectArray, CString& strDateFrom, CString& strDateTo, bool bIncludeToDate )
{
	for ( int n = 0; n < SelectArray.GetListSize(); n++ )
	{
		CEposSelectArrayListItem ListItem;
		SelectArray.GetListItem( n, ListItem );

		int nNetworkIdx;
		int nNetworkNo = dbLocation.GetNetworkNo( ListItem.m_nLocIdx );
		if ( dbNetwork.FindNetworkByNumber(nNetworkNo, nNetworkIdx ) == FALSE )
			continue;

		CString strPath = dbNetwork.GetFolderPathData( nNetworkIdx );
		strPath += "\\";
		
		CFileFind FileFinder;	
		bool bWorking = ( FileFinder.FindFile ( strPath + "*.*" ) != 0 );
		
		CReportConsolidationArray<CSortedStringItem> arrayFolders;

		while (bWorking)   
		{
			bWorking = ( FileFinder.FindNextFile() != 0 );
			
			//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
			if ( FileFinder.IsDots() == TRUE )
				continue;

			//WE ONLY NEED TO CHECK FOLDERS
			if ( FileFinder.IsDirectory() == FALSE )
				continue;

			//FOLDER NAME MUST BE 8 CHARS OR 17 CHARS
			CString strFilename = FileFinder.GetFileName();
			int nFilenameLength = strFilename.GetLength();
			if ( ( nFilenameLength != 8 ) && ( nFilenameLength != 17 ) )
				continue;

			//FOLDER NAME MUST BE NUMERIC
			CString strTranDate = strFilename.Left(8);
			if ( ::TestNumeric( strTranDate ) == FALSE )
				continue;

			if ( nFilenameLength == 17 )
			{
				if ( strFilename.Mid(8,1) != "_" )
					continue;

				if ( ::TestNumeric( strFilename.Right(8) ) == FALSE )
					continue;
			}

			//FOLDER NAME MUST COME BEFORE SEARCH DATE
			if ( FALSE == bIncludeToDate )
			{
				if ( ( strTranDate < strDateFrom ) || ( strTranDate >= strDateTo ) )
					continue;
			}
			else
			{
				if ( ( strTranDate < strDateFrom ) || ( strTranDate > strDateTo ) )
					continue;
			}

			{
				CSortedStringItem item;
				item.m_strItem = strFilename;
				arrayFolders.Consolidate( item );
			}

			//NOW REMOVE THE RELEVANT FILES
			CString strRemoveFile = "";
			CString strDatePath = strPath + strFilename + "\\";
			{
				int nTNo = dbLocation.GetTNo( ListItem.m_nLocIdx, ListItem.m_nTermIdx );

				CFileRemover FileRemover;
				
				if ( TRUE == m_bDeletePlu )
				{
					strRemoveFile.Format ( "Sales.%3.3d", nTNo );
					FileRemover.RemoveFile ( strDatePath + strRemoveFile );

					strRemoveFile.Format ( "Discnt.%3.3d", nTNo );
					FileRemover.RemoveFile ( strDatePath + strRemoveFile );
				}

				if ( TRUE == m_bDeleteSummary )
				{
					strRemoveFile.Format ( "Trans.%3.3d", nTNo );
					FileRemover.RemoveFile ( strDatePath + strRemoveFile );
				}

				if ( TRUE == m_bDeleteDetail )
				{
					CString strTradingFilename;
					::GetTermSaleFilename( CASHRSP_SALES_TRADING, nTNo, strTradingFilename );
					FileRemover.RemoveFile ( strDatePath + strTradingFilename );

					CString strTrainingFilename;
					::GetTermSaleFilename( CASHRSP_OBSOLETE_TRMSAVE2, nTNo, strTrainingFilename );
					FileRemover.RemoveFile ( strDatePath + strTrainingFilename );

					CString strLegacyFilename;
					::GetTermSaleFilename( CASHRSP_OBSOLETE_TERMSAVE, nTNo, strLegacyFilename );
					FileRemover.RemoveFile ( strDatePath + strLegacyFilename );

					strRemoveFile.Format ( "Accnt.%3.3d", nTNo );
					FileRemover.RemoveFile ( strDatePath + strRemoveFile );

					strRemoveFile.Format ( "RfndVoid.%3.3d", nTNo );
					FileRemover.RemoveFile ( strDatePath + strRemoveFile );

					strRemoveFile.Format ( "Reasons.%3.3d", nTNo );
					FileRemover.RemoveFile ( strDatePath + strRemoveFile );

					strRemoveFile.Format ( "ItemPay.%3.3d", nTNo );
					FileRemover.RemoveFile ( strDatePath + strRemoveFile );
				}
			}
		}

		if ( ( TRUE == m_bDeletePlu ) && ( TRUE == m_bDeleteSummary ) && ( TRUE == m_bDeleteDetail ) )
		{
			for ( int n = 0; n < arrayFolders.GetSize(); n++ )
			{
				CSortedStringItem item;
				arrayFolders.GetAt(n,item);
				_rmdir( strPath + item.m_strItem );
			}
		}
	}
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CClearHistoryEposSales::ClearDatabaseSales( int nDbIdx, CString& strDateFrom, CString& strDateTo )
{
	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	CString strDataPath = dbDatabase.GetFolderPathDataHistory( nDbIdx );
	if ( ::ExistSubdirectory( strDataPath ) == FALSE )
		return;

	strDataPath += "\\";

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strDataPath + "*.*" ) != 0 );
		
	while (bWorking)   
	{
		bWorking = ( FileFinder.FindNextFile() != 0 );
			
		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
				continue;

		//WE ONLY NEED TO CHECK FILES
		if ( FileFinder.IsDirectory() )
			continue;

		//FILE NAME MUST BE 8 CHARS + EXTENSION
		CString strFilename = FileFinder.GetFileName();
		if ( strFilename.GetLength() != 12 )
			continue;

		if ( strFilename.Mid(8,1) != "." )
			continue;

		//FILE NAME MUST BE NUMERIC
		CString strTest = strFilename.Left(8);
		if ( ::TestNumeric( strTest ) == FALSE )
			continue;
			
		//FILE NAME MUST COME BEFORE SEARCH DATE
		if ( ( strFilename < strDateFrom ) || ( strFilename >= strDateTo ) )
			continue;
		
		CString strExt= strFilename.Right(3);
		if ( strExt == "002" || strExt == "003" )
			CFileRemover FileRemover ( strDataPath + strFilename );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
