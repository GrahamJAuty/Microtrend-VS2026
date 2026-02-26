/**********************************************************************/
#include "Consolidation.h"
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "PMSCheckoutDateArray.h"
#include "PMSReportFileFinder.h"
#include "StringDateChecker.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluSalesHistoryFileArray.h"
/**********************************************************************/

CPluSalesHistoryFileArray::CPluSalesHistoryFileArray( CEposSelectArray& SelectArray ) : m_SelectArray( SelectArray )
{
	m_bEposSales = TRUE;
	m_bEODMode = FALSE;
	m_bGuestToTerminal = FALSE;
	m_bManualSales = FALSE;
	m_bImportSales = FALSE;
	m_bSaleTime = FALSE;
	m_nTotalFileSize = 0;
}

/**********************************************************************/

void CPluSalesHistoryFileArray::BuildFileList()
{
	CWaitCursor wait;

	m_nTotalFileSize = 0;
	m_arrayFileInfo.RemoveAll();
	
	if ( m_bEposSales )
	{
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

		bool bWantPMSSales = FALSE;
		int nPMSLocIdx = 0;
		int nPMSSelArrayPos = 0;

		//WORK THROUGH THE CLUMPS OF LOCATIONS TO CREATE THE FILE LIST
		for ( int nIndex = 0; nIndex < arrayLocGroupStart.GetSize() - 1; nIndex++ )
		{
			int nStartPos = arrayLocGroupStart.GetAt( nIndex );
			int nEndPos =	arrayLocGroupStart.GetAt( nIndex + 1 ) - 1;
			
			CEposSelectArrayListItem ListItemLoc;
			m_SelectArray.GetListItem( nStartPos, ListItemLoc );
			
			if ( SysInfo.IsPMSSystem() == TRUE )
			{
				if ( dbLocation.IsPMSPseudoLocation( ListItemLoc.m_nLocIdx ) == TRUE )
				{
					if ( FALSE == m_bEODMode )
					{	
						if ( FALSE == m_bGuestToTerminal )
						{
							bWantPMSSales = TRUE;
							nPMSLocIdx = ListItemLoc.m_nLocIdx;
							nPMSSelArrayPos = nStartPos;
						}
					}
					continue;
				}
			}

			if ( ListItemLoc.m_nLineType != EPOSSELECT_TERMINAL )
				continue;

			int nNetworkIdx;
			int nNetworkNo = dbLocation.GetNetworkNo( ListItemLoc.m_nLocIdx );
			if ( dbNetwork.FindNetworkByNumber ( nNetworkNo, nNetworkIdx ) == FALSE )
				continue;
		
			CString strDataPath = dbNetwork.GetFolderPathData ( nNetworkIdx );
			strDataPath += "\\";
		
			CFileFind FileFinder;	
			bool bWorking = ( FileFinder.FindFile ( strDataPath + "*.*" ) != 0 );

			CSortedDateTimeItem CheckDateFrom;
			CSortedDateTimeItem CheckDateTo;
			
			if ( FALSE == m_bEODMode )
			{
				CheckDateFrom = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateFrom();
				CheckDateTo = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateTo();
			}
			else
			{
				CSortedDateTimeItem DateFrom = DataManagerNonDb.SessionDateTimeFilter.GetDateFrom();
				CSortedDateTimeItem DateTo = DataManagerNonDb.SessionDateTimeFilter.GetDateTo();
				
				DataManagerNonDb.EODDateTimeFilterArray.CheckFile( ListItemLoc.m_nLocIdx );
				if ( DataManagerNonDb.EODDateTimeFilterArray.GetFilterTimes( ListItemLoc.m_nLocIdx, DateFrom.m_strDateTime, DateTo.m_strDateTime ) == TRUE )
				{
					CheckDateFrom = DataManagerNonDb.EODDateTimeFilterArray.GetCheckDateFrom( ListItemLoc.m_nLocIdx );
					CheckDateTo  = DataManagerNonDb.EODDateTimeFilterArray.GetCheckDateTo( ListItemLoc.m_nLocIdx );
				}
				else
				{
					bWorking = FALSE;
				}
			}

			CStringDateChecker StringDateChecker( CheckDateFrom.m_strDateTime, CheckDateTo.m_strDateTime );

			while (bWorking)   
			{
				( bWorking = FileFinder.FindNextFile() != 0 );

				//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
				if ( FileFinder.IsDots() == TRUE )
					continue;

				//WE ONLY NEED TO CHECK DIRECTORIES
				if ( FileFinder.IsDirectory() == FALSE )
					continue;

				COleDateTime oleDummy;
				CString strDateFolder = FileFinder.GetFileName();

				CString strDateTran = "";
				CString strDateSale = "";
				bool bIncludeFolder = FALSE;

				if ( TRUE == m_bSaleTime )
				{
					if ( strDateFolder.GetLength() == 17 )
					{
						strDateTran = strDateFolder.Left(8);
						strDateSale = strDateFolder.Right(8);
						bIncludeFolder = StringDateChecker.CheckDate( strDateSale, oleDummy );
					}
				}
				else
				{
					switch( strDateFolder.GetLength() )
					{
					case 17:
						strDateTran = strDateFolder.Left(8);
						strDateSale = strDateFolder.Right(8);
						bIncludeFolder = StringDateChecker.CheckDate( strDateTran, oleDummy );
						break;

					case 8:
						strDateTran = strDateFolder;
						bIncludeFolder = StringDateChecker.CheckDate( strDateTran, oleDummy );
						break;
					}
				}

				if ( FALSE == bIncludeFolder )
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
					m_arrayFileInfo.Consolidate( infoFile );
				
					m_nTotalFileSize += fileTest.GetLength();
				}
			}
		}

#ifdef COMPILE_PMS
		if ( TRUE == bWantPMSSales )
		{
			CSortedDateTimeItem CheckDateFrom = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateFrom();	
			CSortedDateTimeItem CheckDateTo = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateTo();
			
			PMSCheckoutDateArray.BuildCheckoutDateArray();
			CReportConsolidationArray<CSortedIntItem> arrayChecked;

			CPMSReportFileFinder PMSReportFileFinder;
			PMSReportFileFinder.SetIncludePendingFlag( FALSE );
			PMSReportFileFinder.SetIncludeActiveFlag( TRUE );
			PMSReportFileFinder.SetIncludeCancelledFlag( FALSE );

			PMSReportFileFinder.SetPMSDateMode( PMS_DATEMODE_ACTUAL, CheckDateFrom.m_strDateTime, CheckDateTo.m_strDateTime, FALSE );

			PMSReportFileFinder.GetPMSFileListComplete( m_SelectArray, arrayChecked, nPMSLocIdx, nPMSSelArrayPos, m_arrayFileInfo, m_nTotalFileSize );
			PMSReportFileFinder.GetPMSFileListIncomplete( m_SelectArray, arrayChecked, nPMSLocIdx, nPMSSelArrayPos, m_arrayFileInfo, m_nTotalFileSize );
		}
#endif
	}
			
#ifdef STOCKMAN_SYSTEM
	if ( ( FALSE == m_bEODMode ) &&( m_bManualSales == TRUE || m_bImportSales == TRUE ) )
	{
		CSortedDateTimeItem CheckDateFrom = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateFrom();	
		CSortedDateTimeItem CheckDateTo = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateTo();

		for ( int nIndex = 0; nIndex < m_SelectArray.GetListSize(); nIndex++ )
		{
			CEposSelectArrayListItem ListItem;
			m_SelectArray.GetListItem( nIndex, ListItem );

			if ( ( ListItem.m_nLineType != EPOSSELECT_DB_MANUAL ) && ( ListItem.m_nLineType != EPOSSELECT_DB_IMPORT ) )
				continue;

			CString strDataPath = dbDatabase.GetFolderPathDataHistory( ListItem.m_nDbIdx );
			if ( ::ExistSubdirectory( strDataPath ) == FALSE )
				continue;

			CFileFind FileFinder;	
			strDataPath += "\\";
			bool bWorking = ( FileFinder.FindFile ( strDataPath + "*.*" ) != 0 );
	
			while (bWorking)   	
			{
				( bWorking = FileFinder.FindNextFile() != 0 );

				//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
				if ( FileFinder.IsDots() == TRUE )
					continue;

				//WE ONLY NEED TO CHECK FILENAMES
				if ( FileFinder.IsDirectory() )
					continue;

				CString strSalesFilename = FileFinder.GetFileName();
				if ( strSalesFilename.GetLength() != 12 )
					continue;

				CString strDate = strSalesFilename.Left(8);
				if ( ::TestNumeric( strDate ) == FALSE )
					continue;

				if ( strDate < CheckDateFrom.m_strDateTime || strDate > CheckDateTo.m_strDateTime )
					continue;

				int nLocIdx = 0;
				CString strExt = strSalesFilename.Right(3);
				if ( ( EPOSSELECT_DB_MANUAL == ListItem.m_nLineType ) && ( strExt == "002" ) ) nLocIdx = LOCIDX_SPECIAL_MANUAL;
				if ( ( EPOSSELECT_DB_IMPORT == ListItem.m_nLineType ) && ( strExt == "003" ) ) nLocIdx = LOCIDX_SPECIAL_IMPORT;
				if ( nLocIdx == 0 )
					continue;

				CString strFullSalesPath;
				strFullSalesPath.Format ( "%s%s",
					(const char*) strDataPath,
					(const char*) strSalesFilename );

				CSSFile fileTest;
				if ( fileTest.Open ( strFullSalesPath, "rb", _SH_DENYNO ) == FALSE )
					continue;

				CTermFileListInfo infoFile;
				infoFile.m_strFilename = strFullSalesPath;
				infoFile.m_strDateFolder = strDate;
				infoFile.m_strDateTran = strDate;
				infoFile.m_strDateSale = strDate;
				infoFile.m_nDbIdx = ListItem.m_nDbIdx;
				infoFile.m_nLocIdx = nLocIdx;
				infoFile.m_nTermIdx = 0;
				infoFile.m_nSelectArrayIdx = nIndex;
				m_arrayFileInfo.Consolidate( infoFile );
				
				m_nTotalFileSize += fileTest.GetLength();
			}
		}
	}
#endif
}

/**********************************************************************/

void CPluSalesHistoryFileArray::GetFileInfo( int nIdx, CTermFileListInfo& info )
{
	info.Reset();

	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayFileInfo.GetSize() ) )
		m_arrayFileInfo.GetAt( nIdx, info );
}

/**********************************************************************/
