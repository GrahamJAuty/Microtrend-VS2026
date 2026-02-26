/**********************************************************************/
#include "CashRSPImporterDefines.h"
#include "EODFileMover.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "PMSCheckoutDateArray.h"
#include "StringDateChecker.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportFileListMaker.h"
/**********************************************************************/

CEposReportFileListMaker::CEposReportFileListMaker( bool bEODMode, bool bSaleTime, CEposReportPMSModes& PMSModes ) : m_PMSModes( PMSModes )
{
	m_bEODMode = bEODMode;
	m_bSaleTime = bSaleTime;
	m_strCheckDateTimeFrom = "";
	m_strCheckDateTimeTo = "";
}

/**********************************************************************/

CEposReportFileListMaker::~CEposReportFileListMaker(void)
{
}

/**********************************************************************/

void CEposReportFileListMaker::GetFileList ( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int nFileType, CEposSelectArray& SelectArray, bool bNoMakeList, int& nTotalFileSize  )
{
	CWaitCursor wait;

	arrayFileInfo.RemoveAll();
	nTotalFileSize = 0;

	if ( FALSE == bNoMakeList )
		SelectArray.MakeList( TRUE );

	CStringArray arrayCashRSPFileType;

	switch( nFileType )
	{
	case HISTORYFILE_TYPE_SALES:
		arrayCashRSPFileType.Add( "Sales" );
		break;

	case HISTORYFILE_TYPE_DISCOUNT:
		arrayCashRSPFileType.Add( "Discnt" );
		break;

	case HISTORYFILE_TYPE_ACCOUNT:
		arrayCashRSPFileType.Add( "Accnt" );
		break;

	case HISTORYFILE_TYPE_REASONS_ALL:
		arrayCashRSPFileType.Add( "RfndVoid" );
		arrayCashRSPFileType.Add( "Reasons" );
		break;

	case HISTORYFILE_TYPE_REASONS_NEW:
		arrayCashRSPFileType.Add( "Reasons" );
		break;

	case HISTORYFILE_TYPE_ITEMPAY:
		arrayCashRSPFileType.Add( "ItemPay" );
		break;
	}

	//BUILD AN ARRAY OF THE CLUMPS OF LOCATIONS IN THE SELECT ARRAY
	int nCurrentLocIdx = -1;
	CWordArray arrayLocGroupStart;
	for ( int nIndex = 0; nIndex < SelectArray.GetListSize(); nIndex++ )
	{
		CEposSelectArrayListItem ListItem;
		SelectArray.GetListItem( nIndex, ListItem );

		if ( ListItem.m_nLocIdx != nCurrentLocIdx )
		{
			nCurrentLocIdx = ListItem.m_nLocIdx;
			arrayLocGroupStart.Add ( nIndex );
		}
	}
	arrayLocGroupStart.Add ( SelectArray.GetListSize() );

	bool bWantPMSSales = FALSE;
	int nPMSLocIdx = 0;
	int nPMSSelArrayPos = 0;

	//WORK THROUGH THE CLUMPS OF LOCATIONS TO CREATE THE FILE LIST
	for ( int nIndex = 0; nIndex < arrayLocGroupStart.GetSize() - 1; nIndex++ )
	{
		int nStartPos = arrayLocGroupStart.GetAt( nIndex );
		int nEndPos =	arrayLocGroupStart.GetAt( nIndex + 1 ) - 1;
		
		CEposSelectArrayListItem ListItemLoc;
		SelectArray.GetListItem( nStartPos, ListItemLoc );

#ifdef COMPILE_PMS
		if ( SysInfo.IsPMSSystem() == TRUE )
		{
			if ( dbLocation.IsPMSPseudoLocation( ListItemLoc.m_nLocIdx ) == TRUE )
			{
				if ( ( m_PMSModes.GetEposFlag() == FALSE ) && ( FALSE == m_bEODMode ) )
				{
					bWantPMSSales = TRUE;
					nPMSLocIdx = ListItemLoc.m_nLocIdx;
					nPMSSelArrayPos = nStartPos;
				}

				switch( m_PMSModes.GetAllowMode() )
				{
				case PMS_ALLOWMODE_NONE:
					bWantPMSSales = FALSE;
					break;

				case PMS_ALLOWMODE_ACCOUNT:
					if ( m_PMSModes.GetDateMode() == PMS_DATEMODE_ACTUAL )
						bWantPMSSales = FALSE;
					break;
				}

				continue;
			}
		}
#endif

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
				CheckDateTo = DataManagerNonDb.EODDateTimeFilterArray.GetCheckDateTo( ListItemLoc.m_nLocIdx );
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

			CString strDateFolder = FileFinder.GetFileName();
			
			CString strDateTran = "";
			CString strDateSale = "";
			if ( StringDateChecker.CheckDate( m_bSaleTime, nFileType, strDateFolder, strDateTran, strDateSale ) == FALSE )
				continue;
	
			CString strFolder;
			strFolder.Format ( "%s%s",
				(const char*) strDataPath,
				(const char*) strDateFolder );

			for ( int nPos = nStartPos; nPos <= nEndPos; nPos++ )
			{
				CEposSelectArrayListItem ListItemTerm;
				SelectArray.GetListItem( nPos, ListItemTerm );

				int nTerminalNo = dbLocation.GetTNo( ListItemLoc.m_nLocIdx, ListItemTerm.m_nTermIdx );
				
				for ( int nFileIdx = 0; nFileIdx < arrayCashRSPFileType.GetSize(); nFileIdx++ )
				{
					CString strFilename;
					strFilename.Format ( "%s\\%s.%3.3d", 
						(const char*) strFolder, 
						(const char*) arrayCashRSPFileType.GetAt( nFileIdx), 
						nTerminalNo );

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
					arrayFileInfo.Consolidate( infoFile );
					
					nTotalFileSize += fileTest.GetLength();

					COleDateTime oleDate;
					if ( FALSE == m_bSaleTime )
					{
						oleDate = COleDateTime (
							atoi(strDateTran.Left(4)),
							atoi(strDateTran.Mid(4,2)),
							atoi(strDateTran.Right(2)),
							0, 0, 0 );
					}
					else
					{
						oleDate = COleDateTime (
							atoi(strDateSale.Left(4)),
							atoi(strDateSale.Mid(4,2)),
							atoi(strDateSale.Right(2)),
							0, 0, 0 );
					}

					SelectArray.ProcessStartDate( nPos, oleDate );
					SelectArray.ProcessEndDate( nPos, oleDate );
				}
			}
		}
	}

#ifdef COMPILE_PMS

	if ( TRUE == bWantPMSSales )
	{
		CSortedDateTimeItem CheckDateFrom = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateFrom();	
		CSortedDateTimeItem CheckDateTo = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateTo();

		CPMSReportFileFinder PMSReportFileFinder;
		PMSReportFileFinder.SetPMSDateMode( m_PMSModes.GetDateMode(), CheckDateFrom.m_strDateTime, CheckDateTo.m_strDateTime, m_PMSModes.GetWantDepositsFlag() );

		if ( ( DataManagerNonDb.SessionDateTimeFilter.IncludesPMS() == TRUE ) || ( m_PMSModes.GetDateMode() == PMS_DATEMODE_ACTUAL ) )
		{
			PMSCheckoutDateArray.BuildCheckoutDateArray();
			CReportConsolidationArray<CSortedIntItem> arrayChecked;

			if ( m_PMSModes.GetWantCompleteFlag() == TRUE )
				PMSReportFileFinder.GetPMSFileListComplete( SelectArray, arrayChecked, nPMSLocIdx, nPMSSelArrayPos, arrayFileInfo, nTotalFileSize );
			
			PMSReportFileFinder.SetIncludePendingFlag( m_PMSModes.GetWantPendingFlag() );
			PMSReportFileFinder.SetIncludeActiveFlag( m_PMSModes.GetWantActiveFlag() );
			PMSReportFileFinder.SetIncludeCancelledFlag( m_PMSModes.GetWantCancelledFlag() );
			PMSReportFileFinder.GetPMSFileListIncomplete( SelectArray, arrayChecked, nPMSLocIdx, nPMSSelArrayPos, arrayFileInfo, nTotalFileSize );
		}
		else
		{
			PMSCheckoutDateArray.BuildCheckoutDateArray();
			CReportConsolidationArray<CSortedIntItem> arrayChecked;

			PMSReportFileFinder.SetIncludePendingFlag( m_PMSModes.GetWantPendingFlag() );
			PMSReportFileFinder.SetIncludeActiveFlag( m_PMSModes.GetWantActiveFlag() );
			PMSReportFileFinder.SetIncludeCancelledFlag( m_PMSModes.GetWantCancelledFlag() );
			PMSReportFileFinder.GetPMSFileListIncomplete( SelectArray, arrayChecked, nPMSLocIdx, nPMSSelArrayPos, arrayFileInfo, nTotalFileSize );
		}
	}

#endif
}

/**********************************************************************/

void CEposReportFileListMaker::GetTermFileList ( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, CEposSelectArray& SelectArray, bool bNoMakeList, int& nTotalFileSize )
{
	CWaitCursor wait;

	arrayFileInfo.RemoveAll();
	nTotalFileSize = 0;

	if ( FALSE == bNoMakeList )
		SelectArray.MakeList( TRUE );
	
	//BUILD AN ARRAY OF THE CLUMPS OF LOCATIONS IN THE SELECT ARRAY
	int nCurrentLocIdx = -1;
	CWordArray arrayLocGroupStart;
	for ( int nIndex = 0; nIndex < SelectArray.GetListSize(); nIndex++ )
	{
		CEposSelectArrayListItem ListItem;
		SelectArray.GetListItem( nIndex, ListItem );

		if ( ListItem.m_nLocIdx != nCurrentLocIdx )
		{
			nCurrentLocIdx = ListItem.m_nLocIdx;
			arrayLocGroupStart.Add ( nIndex );
		}
	}
	arrayLocGroupStart.Add ( SelectArray.GetListSize() );

	bool bWantPMSSales = FALSE;
	int nPMSLocIdx = 0;
	int nPMSSelArrayPos = 0;

	//WORK THROUGH THE CLUMPS OF LOCATIONS TO CREATE THE FILE LIST
	for ( int nIndex = 0; nIndex < arrayLocGroupStart.GetSize() - 1; nIndex++ )
	{
		int nStartPos = arrayLocGroupStart.GetAt( nIndex );
		int nEndPos =	arrayLocGroupStart.GetAt( nIndex + 1 ) - 1;
		
		CEposSelectArrayListItem ListItemLoc;
		SelectArray.GetListItem( nStartPos, ListItemLoc );
		
		if ( SysInfo.IsPMSSystem() == TRUE )
		{
			if ( dbLocation.IsPMSPseudoLocation( ListItemLoc.m_nLocIdx ) == TRUE )
			{
				if ( ( m_PMSModes.GetEposFlag() == FALSE ) && ( FALSE == m_bEODMode ) )
				{
					bWantPMSSales = TRUE;
					nPMSLocIdx = ListItemLoc.m_nLocIdx;
					nPMSSelArrayPos = nStartPos;
				}

				switch( m_PMSModes.GetAllowMode() )
				{
				case PMS_ALLOWMODE_NONE:
					bWantPMSSales = FALSE;
					break;

				case PMS_ALLOWMODE_ACCOUNT:
					if ( m_PMSModes.GetDateMode() == PMS_DATEMODE_ACTUAL )
						bWantPMSSales = FALSE;
					break;
				}

				continue;
			}
		}

		int nNetworkIdx;
		int nNetworkNo = dbLocation.GetNetworkNo( ListItemLoc.m_nLocIdx );
		if ( dbNetwork.FindNetworkByNumber ( nNetworkNo, nNetworkIdx ) == FALSE )
			continue;

		CString strDataPath = dbNetwork.GetFolderPathData ( nNetworkIdx );
		strDataPath += "\\";

		CReportConsolidationArray<CEposReportConsolTermFile> arrayFiles;
		
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
				CheckDateTo = DataManagerNonDb.EODDateTimeFilterArray.GetCheckDateTo( ListItemLoc.m_nLocIdx );
			}
			else
			{
				bWorking = FALSE;
			}
		}

		m_strCheckDateTimeFrom = CheckDateFrom.m_strDateTime;
		m_strCheckDateTimeTo = CheckDateTo.m_strDateTime;
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

			CString strDateFolder = FileFinder.GetFileName();

			CString strDateTran = "";
			CString strDateSale = "";
			if ( StringDateChecker.CheckDate( FALSE, HISTORYFILE_TYPE_TERM, strDateFolder, strDateTran, strDateSale ) == FALSE )
				continue;

			CString strPath;
			strPath.Format ( "%s%s\\",
				(const char*) strDataPath,
				(const char*) strDateFolder );

			for ( int nPos = nStartPos; nPos <= nEndPos; nPos++ )
			{
				CEposSelectArrayListItem ListItemTerm;
				SelectArray.GetListItem( nPos, ListItemTerm );
				
				CEposReportConsolTermFile infoTermFile;
				infoTermFile.m_nSelArrayIdx = nPos;
				infoTermFile.m_nTNo = dbLocation.GetTNo ( ListItemLoc.m_nLocIdx, ListItemTerm.m_nTermIdx );
				infoTermFile.m_nFileType = 0;
				infoTermFile.m_strDateFolder = strDateFolder;
				
				{
					CString strLegacyFilename;
					::GetTermSaleFilename( CASHRSP_OBSOLETE_TERMSAVE, infoTermFile.m_nTNo, strLegacyFilename );
				
					CSSFile fileTest;
					CString strFilenameTest = strPath + strLegacyFilename;
					if ( fileTest.Open ( strFilenameTest, "rb", _SH_DENYNO ) == TRUE )
					{
						infoTermFile.m_nFileType = 1;
						arrayFiles.Consolidate( infoTermFile );
						nTotalFileSize += fileTest.GetLength();
						fileTest.Close();
					}
				}

				{
					CString strTradingFilename;
					::GetTermSaleFilename( CASHRSP_SALES_TRADING, infoTermFile.m_nTNo, strTradingFilename );
			
					CSSFile fileTest;
					CString strFilenameTest = strPath + strTradingFilename;
					if ( fileTest.Open ( strFilenameTest, "rb", _SH_DENYNO ) == TRUE )
					{
						infoTermFile.m_nFileType = 2;
						arrayFiles.Consolidate( infoTermFile );
						nTotalFileSize += fileTest.GetLength();
						fileTest.Close();
					}
				}
			}
		}

		for ( int n = 0; n < arrayFiles.GetSize(); n++ )
		{
			CEposReportConsolTermFile infoTermFile;
			arrayFiles.GetAt( n, infoTermFile );
			
			int nMode = 0;
			CString strFilename = "";
		
			switch( infoTermFile.m_nFileType )
			{
			case 1:
				nMode = CASHRSP_OBSOLETE_TERMSAVE;
				::GetTermSaleFilename( CASHRSP_OBSOLETE_TERMSAVE, infoTermFile.m_nTNo, strFilename );
				break;

			case 2:
				nMode = CASHRSP_SALES_TRADING;
				::GetTermSaleFilename( CASHRSP_SALES_TRADING, infoTermFile.m_nTNo, strFilename );
				break;

			default:
				break;
			}

			if ( strFilename != "" )
			{
				CString strFilePath;
				strFilePath.Format( "%s\\%s\\%s",
					dbNetwork.GetFolderPathData( nNetworkIdx ),
					(const char*) infoTermFile.m_strDateFolder,
					(const char*) strFilename );

				CTermFileListInfo info;
				info.m_nSelectArrayIdx = infoTermFile.m_nSelArrayIdx;
				info.m_strFilename = strFilePath;
				info.m_nCashRSPMode = nMode;
				info.m_nDbIdx = ListItemLoc.m_nDbIdx;
				info.m_nLocIdx = ListItemLoc.m_nLocIdx;
				info.m_strDateFolder = infoTermFile.m_strDateFolder;
				info.m_strDateTran = info.m_strDateFolder.Left(8);
				info.m_strDateSale = info.m_strDateTran;

				if ( dbLocation.FindTerminalByNumber( ListItemLoc.m_nLocIdx, infoTermFile.m_nTNo, info.m_nTermIdx ) == FALSE )
					info.m_nTermIdx = 0;

				arrayFileInfo.Consolidate( info );
			}
		}
	}

#ifdef COMPILE_PMS

	if ( TRUE == bWantPMSSales )
	{
		CSortedDateTimeItem CheckDateFrom = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateFrom();	
		CSortedDateTimeItem CheckDateTo = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateTo();

		CPMSReportFileFinder PMSReportFileFinder;
		PMSReportFileFinder.SetPMSDateMode( m_PMSModes.GetDateMode(), CheckDateFrom.m_strDateTime, CheckDateTo.m_strDateTime, m_PMSModes.GetWantDepositsFlag() );

		if ( ( DataManagerNonDb.SessionDateTimeFilter.IncludesPMS() == TRUE ) || ( m_PMSModes.GetDateMode() == PMS_DATEMODE_ACTUAL ) )
		{
			PMSCheckoutDateArray.BuildCheckoutDateArray();
			CReportConsolidationArray<CSortedIntItem> arrayChecked;

			if ( TRUE == m_PMSModes.GetWantCompleteFlag() )
				PMSReportFileFinder.GetPMSFileListComplete( SelectArray, arrayChecked, nPMSLocIdx, nPMSSelArrayPos, arrayFileInfo, nTotalFileSize );
				
			PMSReportFileFinder.SetIncludePendingFlag( m_PMSModes.GetWantPendingFlag() );
			PMSReportFileFinder.SetIncludeActiveFlag( m_PMSModes.GetWantActiveFlag() );
			PMSReportFileFinder.SetIncludeCancelledFlag( m_PMSModes.GetWantCancelledFlag() );
			PMSReportFileFinder.GetPMSFileListIncomplete( SelectArray, arrayChecked, nPMSLocIdx, nPMSSelArrayPos, arrayFileInfo, nTotalFileSize );
		}
		else
		{
			if ( TRUE == m_PMSModes.GetWantActiveFlag() )
			{
				PMSCheckoutDateArray.BuildCheckoutDateArray();
				CReportConsolidationArray<CSortedIntItem> arrayChecked;

				PMSReportFileFinder.SetIncludePendingFlag( m_PMSModes.GetWantPendingFlag() );
				PMSReportFileFinder.SetIncludeActiveFlag( m_PMSModes.GetWantActiveFlag() );
				PMSReportFileFinder.SetIncludeCancelledFlag( m_PMSModes.GetWantCancelledFlag() );
				PMSReportFileFinder.GetPMSFileListIncomplete( SelectArray, arrayChecked, nPMSLocIdx, nPMSSelArrayPos, arrayFileInfo, nTotalFileSize );
			}
		}
	}

#endif
}

/**********************************************************************/

void CEposReportFileListMaker::GetTermFileListRebuild( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, CEposSelectArray& SelectArray, int& nTotalFileSize )
{
	CWaitCursor wait;

	arrayFileInfo.RemoveAll();
	nTotalFileSize = 0;

	SelectArray.MakeList( TRUE );
	
	//BUILD AN ARRAY OF THE CLUMPS OF LOCATIONS IN THE SELECT ARRAY
	int nCurrentLocIdx = -1;
	CWordArray arrayLocGroupStart;
	for ( int nIndex = 0; nIndex < SelectArray.GetListSize(); nIndex++ )
	{
		CEposSelectArrayListItem ListItem;
		SelectArray.GetListItem( nIndex, ListItem );

		if ( ListItem.m_nLocIdx != nCurrentLocIdx )
		{
			nCurrentLocIdx = ListItem.m_nLocIdx;
			arrayLocGroupStart.Add ( nIndex );
		}
	}
	arrayLocGroupStart.Add ( SelectArray.GetListSize() );

	//WORK THROUGH THE CLUMPS OF LOCATIONS TO CREATE THE FILE LIST
	for ( int nIndex = 0; nIndex < arrayLocGroupStart.GetSize() - 1; nIndex++ )
	{
		int nStartPos = arrayLocGroupStart.GetAt( nIndex );
		int nEndPos =	arrayLocGroupStart.GetAt( nIndex + 1 ) - 1;
		
		CEposSelectArrayListItem ListItemLoc;
		SelectArray.GetListItem( nStartPos, ListItemLoc );
		
		int nNetworkIdx;
		int nNetworkNo = dbLocation.GetNetworkNo( ListItemLoc.m_nLocIdx );
		if ( dbNetwork.FindNetworkByNumber ( nNetworkNo, nNetworkIdx ) == FALSE )
			continue;
		
		CString strDataPath = dbNetwork.GetFolderPathData ( nNetworkIdx );
		strDataPath += "\\";
		
		for ( int nPos = nStartPos; nPos <= nEndPos; nPos++ )
		{
			CEposSelectArrayListItem ListItemTerm;
			SelectArray.GetListItem( nPos, ListItemTerm );
			int nTerminalNo = dbLocation.GetTNo ( ListItemLoc.m_nLocIdx, ListItemTerm.m_nTermIdx );
			
			CString strLegacyFilename;
			::GetTermSaleFilename( CASHRSP_OBSOLETE_TERMSAVE, nTerminalNo, strLegacyFilename );

			CSSFile fileTest1;
			CString strFilename1 = strDataPath + strLegacyFilename;
			if ( fileTest1.Open ( strFilename1, "rb", _SH_DENYNO ) == TRUE )
			{
				CTermFileListInfo info;
				info.m_nSelectArrayIdx = nPos;
				info.m_strFilename = strFilename1;
				info.m_nCashRSPMode = CASHRSP_OBSOLETE_TERMSAVE;
				info.m_nDbIdx = ListItemLoc.m_nDbIdx;
				info.m_nLocIdx = ListItemLoc.m_nLocIdx;

				if ( dbLocation.FindTerminalByNumber( ListItemLoc.m_nLocIdx, nTerminalNo, info.m_nTermIdx ) == FALSE )
					info.m_nTermIdx = 0;

				arrayFileInfo.Consolidate( info );

				nTotalFileSize += fileTest1.GetLength();
				fileTest1.Close();
			}
			
			CString strTradingFilename;
			::GetTermSaleFilename( CASHRSP_SALES_TRADING, nTerminalNo, strTradingFilename );
			
			CSSFile fileTest2;
			CString strFilename2 = strDataPath + strTradingFilename;
			if ( fileTest2.Open ( strFilename2, "rb", _SH_DENYNO ) == TRUE )
			{
				CTermFileListInfo info;
				info.m_nSelectArrayIdx = nPos;
				info.m_strFilename = strFilename2;
				info.m_nCashRSPMode = CASHRSP_SALES_TRADING;
				info.m_nDbIdx = ListItemLoc.m_nDbIdx;
				info.m_nLocIdx = ListItemLoc.m_nLocIdx;
				
				if ( dbLocation.FindTerminalByNumber( ListItemLoc.m_nLocIdx, nTerminalNo, info.m_nTermIdx ) == FALSE )
					info.m_nTermIdx = 0;

				arrayFileInfo.Consolidate( info );

				nTotalFileSize += fileTest2.GetLength();
				fileTest2.Close();
			}
		}
	}
}

/**********************************************************************/

void CEposReportFileListMaker::GetReasonFileList ( CArray<CReasonFileListInfo,CReasonFileListInfo>& array, CEposSelectArray& SelectArray, int& nTotalFileSize )
{
	CWaitCursor wait;

	array.RemoveAll();
	nTotalFileSize = 0;

	SelectArray.MakeList( TRUE );
	
	for ( int nSelIdx = 0; nSelIdx < SelectArray.GetListSize(); nSelIdx++ )
	{
		CEposSelectArrayListItem ListItem;
		SelectArray.GetListItem( nSelIdx, ListItem );

		int nTNo = dbLocation.GetTNo( ListItem.m_nLocIdx, ListItem.m_nTermIdx );

		int nNetworkIdx;
		if ( dbNetwork.FindNetworkByNumber( dbLocation.GetNetworkNo( ListItem.m_nLocIdx ), nNetworkIdx ) == FALSE )
			continue;

		if ( dbLocation.IsPMSPseudoLocation( ListItem.m_nLocIdx ) == TRUE )
			continue;

		if ( dbNetwork.CheckReasonTokenFile( nNetworkIdx ) == TRUE )
		{
			CSSFile fileLock;
			CFilenameUpdater FnUp( SysFiles::NetworkCommsLock, nNetworkIdx );
			if ( fileLock.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
			{
				CEODFileMover EODFileMover;
				EODFileMover.TidyReasonFiles(nNetworkIdx);
				dbNetwork.DeleteReasonTokenFile(nNetworkIdx);
				fileLock.Close();
			}
		}

		CString strPath = dbNetwork.GetFolderPathDataEODFiles( nNetworkIdx, "Reasons" );
		strPath += "\\";

		CString strFilter;
		strFilter.Format( "*.%3.3d",
			nTNo );

		CFileFind finder;
		BOOL bWorking = finder.FindFile( strPath + strFilter );

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

			DataManagerNonDb.EODDateTimeFilterArray.CheckFile( ListItem.m_nLocIdx );
			if ( DataManagerNonDb.EODDateTimeFilterArray.GetFilterTimes( ListItem.m_nLocIdx, DateFrom.m_strDateTime, DateTo.m_strDateTime ) == TRUE )
			{
				CheckDateFrom = DataManagerNonDb.EODDateTimeFilterArray.GetCheckDateFrom( ListItem.m_nLocIdx );
				CheckDateTo = DataManagerNonDb.EODDateTimeFilterArray.GetCheckDateTo( ListItem.m_nLocIdx );
			}
			else
			{
				bWorking = FALSE;
			}
		}

		CStringDateChecker StringDateChecker( CheckDateFrom.m_strDateTime, CheckDateTo.m_strDateTime );

		while ( TRUE == bWorking )
		{
			bWorking = finder.FindNextFile();

			if ( finder.IsDots() == TRUE )
				continue;

			if ( finder.IsDirectory() == TRUE )
				continue;

			CString strFilename = finder.GetFileName();

			int nLength = strFilename.GetLength();
			if ( ( nLength != 12 ) && ( nLength != 16 ) )
				continue;

			COleDateTime oleDummy;
			CString strDate = strFilename.Left(8);
			if ( StringDateChecker.CheckDate( strDate, oleDummy ) == FALSE )
				continue;

			CSSFile fileCheck;
			if ( fileCheck.Open( strPath + strFilename, "rb" ) == TRUE )
			{
				CReasonFileListInfo info;
				info.m_nSelIdx = nSelIdx;
				info.m_nNetworkIdx = nNetworkIdx;
				info.m_strFilename = strFilename;
				array.Add( info );

				nTotalFileSize += fileCheck.GetLength();
			}
		}
	}
}
		
/**********************************************************************/
