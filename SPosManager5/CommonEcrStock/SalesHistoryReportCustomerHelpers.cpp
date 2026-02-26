/**********************************************************************/
#include <share.h>
/**********************************************************************/
//#include "DataManager.h"
//#include "DataManagerNonDb.h"
#include "DatabaseCSVArray.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "SalesHistoryReportPlu.h"
#include "SessionCSVArray.h"
#include "StatusProgress.h"
#include "StringDateChecker.h"
/**********************************************************************/
#include "SalesHistoryReportCustomer.h"
/**********************************************************************/

const char* CSalesHistoryReportCustomer::GetRepMonth( COleDateTime date, bool bShortName, bool bFullLine )
{
	m_strRepMonth = ( bFullLine ) ? "<..>" : "";

	if ( bShortName == FALSE )
		m_strRepMonth += date.Format ( "%B %Y" );
	else
		m_strRepMonth += date.Format ( "%b %y" );

	return m_strRepMonth;
}

/**********************************************************************/

void CSalesHistoryReportCustomer::WriteTerminalList()
{
	CSSFile fileReport;
	if ( fileReport.Open ( Super.ReportFilename(), "ab" ) == FALSE )
		return;

	if ( m_nConLevel == NODE_SYSTEM )
		fileReport.WriteLine ( "<..>EPOS Sales for System" );
	else
	{
		for ( int nIndex = 0; nIndex < m_SelectArray.GetListSize(); nIndex++ )
		{
			CEposSelectArrayListItem ListItem;
			m_SelectArray.GetListItem( nIndex, ListItem );

			{
				int nSPOSVersion;
				int nConnectionType;
				dbLocation.GetLocSPOSVersion( ListItem.m_nLocIdx, nSPOSVersion, nConnectionType );
				if ( CONNECTION_TYPE_STANDARD_NONE != nConnectionType )
					continue;
			}

			if ( EPOSSELECT_TERMINAL == ListItem.m_nLineType )
			{	
				CString strName;
				bool bHideNoData = FALSE;
				strName.Format ( "<..>EPOS Sales for %s %s %s", 
					dbLocation.GetName( ListItem.m_nLocIdx ), 
					dbLocation.GetTerminalName ( ListItem.m_nLocIdx, ListItem.m_nTermIdx ),
					m_SelectArray.GetDateInfo( ListItem, bHideNoData ) );
				
				if ( FALSE == bHideNoData )
					fileReport.WriteLine ( strName );
			}
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportCustomer::SetDates ( COleDateTime dateFrom, COleDateTime dateTo, CSessionCSVRecord& ReportSession, CTimeSliceMapCSVRecord& TimeSlice )
{
	DataManagerNonDb.SessionDateTimeFilter.SetDates( dateFrom, dateTo, ReportSession );
	
	m_TimeSliceMap.SetReportType( TimeSlice.GetReportType() );
	m_TimeSliceMap.SetTimeSliceFileNo( TimeSlice.GetTimeSliceFileNo() );

	if ( m_TimeSliceMap.GetReportType() == SH_CUSTOM )
		m_TimeMap.BuildMap( m_TimeSliceMap.GetTimeSliceFileNo() );
}

/**********************************************************************/

void CSalesHistoryReportCustomer::GetFileListPluSales ( CArray<CTermFileListInfo,CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize )
{
	CWaitCursor wait;

	arrayFileInfo.RemoveAll();

	nTotalFileSize = 0;

	CString strCheckDateFrom = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateFrom().m_strDateTime;
	CString strCheckDateTo = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateTo().m_strDateTime;
		
	//BUILD AN ARRAY OF THE CLUMPS OF LOCATIONS IN THE SELECT ARRAY
	int nSelIdx;
	int nCurrentLocIdx = -1;
	CWordArray arrayLocGroupStart;
	for ( nSelIdx = 0; nSelIdx < m_SelectArray.GetListSize(); nSelIdx++ )
	{
		CEposSelectArrayListItem ListItem;
		m_SelectArray.GetListItem( nSelIdx, ListItem );

		if ( ListItem.m_nLineType != EPOSSELECT_TERMINAL )
			break;

		if ( ListItem.m_nLocIdx != nCurrentLocIdx )
		{
			nCurrentLocIdx = ListItem.m_nLocIdx;
			arrayLocGroupStart.Add ( nSelIdx );
		}
	}
	arrayLocGroupStart.Add ( nSelIdx );

	//WORK THROUGH THE CLUMPS OF LOCATIONS TO CREATE THE FILE LIST
	for ( int nIndex = 0; nIndex < arrayLocGroupStart.GetSize() - 1; nIndex++ )
	{
		int nStartPos = arrayLocGroupStart.GetAt( nIndex );
		int nEndPos =	arrayLocGroupStart.GetAt( nIndex + 1 ) - 1;

		CEposSelectArrayListItem ListItemLoc;
		m_SelectArray.GetListItem( nStartPos, ListItemLoc );

		{
			int nSPOSVersion;
			int nConnectionType;
			dbLocation.GetLocSPOSVersion( ListItemLoc.m_nLocIdx, nSPOSVersion, nConnectionType );
			if ( CONNECTION_TYPE_STANDARD_NONE != nConnectionType )
				continue;
		}

		if ( EPOSSELECT_TERMINAL == ListItemLoc.m_nLineType )
		{
			int nNetworkIdx;
			int nNetworkNo = dbLocation.GetNetworkNo( ListItemLoc.m_nLocIdx );
			if ( dbNetwork.FindNetworkByNumber ( nNetworkNo, nNetworkIdx ) == TRUE )
			{
				CString strDataPath = dbNetwork.GetFolderPathData ( nNetworkIdx );
				strDataPath += "\\";
			
				CStringDateChecker StringDateChecker( strCheckDateFrom, strCheckDateTo );

				CFileFind FileFinder;	
				bool bWorking = ( FileFinder.FindFile ( strDataPath + "*.*" ) != 0 );

				while (bWorking)   
				{
					( bWorking = FileFinder.FindNextFile() != 0 );

					//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
					if ( FileFinder.IsDots() == TRUE )
						continue;

					//WE ONLY NEED TO CHECK DIRECTORIES
					if ( FileFinder.IsDirectory() == FALSE )
						continue;

					CString strDateFolder = FileFinder.GetFileName();
					
					CString strDateTran = "";
					CString strDateSale = "";
					if ( StringDateChecker.CheckDate( FALSE, HISTORYFILE_TYPE_SALES, strDateFolder, strDateTran, strDateSale ) == FALSE )
						continue;
					
					CString strFolder;
					strFolder.Format ( "%s%s",
						(const char*) strDataPath,
						(const char*) strDateFolder );

					for ( int nPos = nStartPos; nPos <= nEndPos; nPos++ )
					{
						CEposSelectArrayListItem ListItemTerm;
						m_SelectArray.GetListItem( nPos, ListItemTerm );
						int nTerminalNo = dbLocation.GetTNo ( ListItemLoc.m_nLocIdx, ListItemTerm.m_nTermIdx );
				
						CString strFilename;
						strFilename.Format ( "%s\\sales.%3.3d", 
							(const char*) strFolder, nTerminalNo );

						CSSFile fileTest;
						if ( fileTest.Open ( strFilename, "rb", _SH_DENYNO ) == FALSE )
							continue;

						CTermFileListInfo infoFile;
						infoFile.m_strFilename = strFilename;
						infoFile.m_strDateFolder = strDateFolder;
						infoFile.m_strDateTran = strDateTran;
						infoFile.m_strDateSale = strDateSale;
						infoFile.m_nDbIdx = ListItemLoc.m_nDbIdx;
						infoFile.m_nLocIdx = ListItemLoc.m_nLocIdx;
						infoFile.m_nTermIdx = ListItemTerm.m_nTermIdx + 1;
						infoFile.m_nSelectArrayIdx = nPos;
						arrayFileInfo.Add( infoFile );
					
						nTotalFileSize += fileTest.GetLength();
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CSalesHistoryReportCustomer::GetFileListCustomer ( CArray<CTermFileListInfo,CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize  )
{
	CWaitCursor wait;

	CString strCheckDateFrom = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateFrom().m_strDateTime;
	CString strCheckDateTo = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateTo().m_strDateTime;

	arrayFileInfo.RemoveAll();
	nTotalFileSize = 0;

	CString strCASHRSPFileType = "Accnt";
		
	//BUILD AN ARRAY OF THE CLUMPS OF LOCATIONS IN THE SELECT ARRAY
	int nCurrentLocIdx = -1;
	CWordArray arrayLocGroupStart;
	for ( int nIndex = 0; nIndex < m_SelectArray.GetListSize(); nIndex++ )
	{
		CEposSelectArrayListItem ListItem;
		m_SelectArray.GetListItem( nIndex, ListItem );

		if ( ListItem.m_nLocIdx != nCurrentLocIdx )
		{
			nCurrentLocIdx = ListItem.m_nLocIdx;
			arrayLocGroupStart.Add ( nIndex );
		}
	}
	arrayLocGroupStart.Add ( m_SelectArray.GetListSize() );

	//WORK THROUGH THE CLUMPS OF LOCATIONS TO CREATE THE FILE LIST
	for ( int nIndex = 0; nIndex < arrayLocGroupStart.GetSize() - 1; nIndex++ )
	{
		int nStartPos = arrayLocGroupStart.GetAt( nIndex );
		int nEndPos =	arrayLocGroupStart.GetAt( nIndex + 1 ) - 1;

		CEposSelectArrayListItem ListItemLoc;
		m_SelectArray.GetListItem( nStartPos, ListItemLoc );

		{
			int nSPOSVersion;
			int nConnectionType;
			dbLocation.GetLocSPOSVersion( ListItemLoc.m_nLocIdx, nSPOSVersion, nConnectionType );
			if ( CONNECTION_TYPE_STANDARD_NONE != nConnectionType )
				continue;
		}

		int nNetworkIdx;
		int nNetworkNo = dbLocation.GetNetworkNo( ListItemLoc.m_nLocIdx );
		if ( dbNetwork.FindNetworkByNumber ( nNetworkNo, nNetworkIdx ) == FALSE )
			continue;

		CString strDataPath = dbNetwork.GetFolderPathData ( nNetworkIdx );
		strDataPath += "\\";

		CStringDateChecker StringDateChecker( strCheckDateFrom, strCheckDateTo );
		
		CFileFind FileFinder;	
		bool bWorking = ( FileFinder.FindFile ( strDataPath + "*.*" ) != 0 );

		while (bWorking)   
		{
			( bWorking = FileFinder.FindNextFile() != 0 );

			//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
			if ( FileFinder.IsDots() == TRUE )
				continue;

			//WE ONLY NEED TO CHECK DIRECTORIES
			if ( FileFinder.IsDirectory() == FALSE )
				continue;

			CString strDateFolder = FileFinder.GetFileName();

			CString strDateTran = "";
			CString strDateSale = "";
			if ( StringDateChecker.CheckDate( FALSE, HISTORYFILE_TYPE_ACCOUNT, strDateFolder, strDateTran, strDateSale ) == FALSE )
				continue;
		
			CString strFolder;
			strFolder.Format ( "%s%s",
				(const char*) strDataPath,
				(const char*) strDateFolder );

			for ( int nPos = nStartPos; nPos <= nEndPos; nPos++ )
			{
				CEposSelectArrayListItem ListItemTerm;
				m_SelectArray.GetListItem( nPos, ListItemTerm );
				int nTerminalNo = dbLocation.GetTNo( ListItemLoc.m_nLocIdx, ListItemTerm.m_nTermIdx );
				
				CString strFilename;
				strFilename.Format ( "%s\\%s.%3.3d", 
					(const char*) strFolder, (const char*) strCASHRSPFileType, nTerminalNo );

				CSSFile fileTest;
				if ( fileTest.Open ( strFilename, "rb", _SH_DENYNO ) == FALSE )
					continue;

				CTermFileListInfo infoFile;
				infoFile.m_nSelectArrayIdx = nPos;
				infoFile.m_strFilename = strFilename;
				infoFile.m_strDateFolder = strDateFolder;
				infoFile.m_strDateTran = strDateTran;
				infoFile.m_strDateSale = strDateSale;
				infoFile.m_nDbIdx = ListItemLoc.m_nDbIdx;
				infoFile.m_nLocIdx = ListItemLoc.m_nLocIdx;
				infoFile.m_nTermIdx = ListItemTerm.m_nTermIdx;
				arrayFileInfo.Add( infoFile );
				
				nTotalFileSize += fileTest.GetLength();

				COleDateTime oleDate = COleDateTime (
					atoi(strDateFolder.Left(4)),
					atoi(strDateFolder.Mid(4,2)),
					atoi(strDateFolder.Right(2)),
					0, 0, 0 );

				m_SelectArray.ProcessStartDate( nPos, oleDate );
				m_SelectArray.ProcessEndDate( nPos, oleDate );
			}
		}
	}
}

/**********************************************************************/

const char* CSalesHistoryReportCustomer::GetReportTitle( const char* szReportName )
{
	CSortedDateTimeItem DateFrom = DataManagerNonDb.SessionDateTimeFilter.GetDateFrom();
	CSortedDateTimeItem DateTo = DataManagerNonDb.SessionDateTimeFilter.GetDateTo();

	m_strTitle.Format ( "%s (%2.2d/%2.2d/%4.4d - %2.2d/%2.2d/%4.4d, %s%s)",
		 szReportName,
		 DateFrom.m_oleDateTime.GetDay(),
		 DateFrom.m_oleDateTime.GetMonth(),
		 DateFrom.m_oleDateTime.GetYear(),	
		 DateTo.m_oleDateTime.GetDay(),
		 DateTo.m_oleDateTime.GetMonth(),
		 DateTo.m_oleDateTime.GetYear(),
		 (const char*) m_strSessionTitle,
		 m_ReportFilters.DeptFilter.GetFilterNameForReport() );

	return m_strTitle;
}

/**********************************************************************/