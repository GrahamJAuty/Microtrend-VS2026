/**********************************************************************/
#include "CashRSPFlusher.h"
#include "Consolidation.h"
#include "FileRemover.h"
#include "RealTimeCommsThread.h"
/**********************************************************************/
#include "5000Server.h"
/**********************************************************************/

CRealTimeFileBufferInfo::CRealTimeFileBufferInfo()
{
	m_strFileTitle = "";
	m_strFileExt = "";
	m_nRenameType = REALTIME_FILEBUFFER_SALE;
	m_nFileType = 0;
}

/**********************************************************************/

const char* CRealTimeFileBufferInfo::GetOriginalFilename()
{
	m_strOriginalFilename = "";
	
	switch ( m_nRenameType )
	{
	case 1:
		m_strOriginalFilename = "SSTEMP01_";
		break;

	case 99:
		m_strOriginalFilename = "SSTEMP99_";
		break;
	}

	switch ( m_nRenameType )
	{
	case 0:
	case 1:
	case 99:
		m_strOriginalFilename += GetDestinationFilename();
		break;
	}

	return m_strOriginalFilename;
}

/**********************************************************************/

const char* CRealTimeFileBufferInfo::GetRenamedFilename()
{
	m_strRenamedFilename = "";
	
	switch ( m_nRenameType )
	{
	case 0:
		m_strRenamedFilename = "SSTEMP01_";
		break;

	case 1:
		m_strRenamedFilename = "SSTEMP99_";
		break;
	}

	switch ( m_nRenameType )
	{
	case 0:
	case 1:
		m_strRenamedFilename += GetDestinationFilename();
		break;
	}

	return m_strRenamedFilename;
}

/**********************************************************************/

const char* CRealTimeFileBufferInfo::GetDestinationFilename()
{
	m_strDestinationFilename = "";
	m_strDestinationFilename += m_strFileTitle;
	m_strDestinationFilename += ".";
	m_strDestinationFilename += m_strFileExt;
	return m_strDestinationFilename;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CRealTimeCommsThreadInfo::CRealTimeCommsThreadInfo()
{
	m_nSpeed = 1;

	m_bCASHRSP = FALSE;
	m_bLogAllCASHRSPFiles = FALSE;
	m_bEnableLocal = FALSE;
	m_nTransactionCount = 0;
	m_nZeroFileCount = 0;
	m_bForceQuit = FALSE;

	m_bProducts = FALSE;
	m_bCustomer = FALSE;

	m_bPMS = FALSE;
	m_bAllPMS = FALSE;
	m_strPMSDownloadFolder = "";
	m_strPMSRoomFolder = "";
	m_strPMSLockFile = "";
	m_strPMSPostPath = "";
	m_strPMSFolderPending = "";
	m_strPMSFolderSales = "";

	m_strSchedLogAllFilename = "";
	m_strSchedLogOldFilename = "";
	m_strNetworkLogAllFilename = "";
	m_strNetworkLogOldFilename = "";

	m_lMaxSchedLogFileSize = 0;
	m_lMaxNetworkLogFileSize = 0;

	m_strPosUtilityStopFile = "";
	m_bPosUtilityStopFlag = FALSE;
	m_nMaxUsers = 1;
}

/**********************************************************************/

UINT CRealTimeCommsThread::DoComms( LPVOID pParam )
{
	int nForceQuit = 0;
	CRealTimeCommsThreadInfo* pInfo = (CRealTimeCommsThreadInfo*) pParam;
	
	DoCommsInternal( pInfo, nForceQuit );

	if ( nForceQuit != 0 )
	{
		CString strMsg;
		strMsg.Format( "QUIT COMMS (%d)", nForceQuit );
		LogRealTimeCommsMessage( pInfo, strMsg );
	}

	return 0;
}

/**********************************************************************/

void CRealTimeCommsThread::DoCommsInternal( CRealTimeCommsThreadInfo* pInfo, int& nForceQuit )
{
	if ( ( TRUE == pInfo -> m_bCASHRSP ) && ( FALSE == pInfo -> m_bPosUtilityStopFlag ) )
	{
		CArray<CRealTimeFileBufferInfo,CRealTimeFileBufferInfo> arrayBuffer;

		BuildCASHRSPBuffer( pInfo, arrayBuffer, nForceQuit );

		if ( nForceQuit != 0 )
			return;
		
		if ( arrayBuffer.GetSize() > 0 )
			ProcessCASHRSPBuffer( pInfo, arrayBuffer, nForceQuit );

		if ( nForceQuit != 0 )
			return;

		ProcessHandheldExports( pInfo, nForceQuit );

		if ( nForceQuit != 0 )
			return;
	}

	if ( ( TRUE == pInfo -> m_bProducts ) && ( FALSE == pInfo -> m_bPosUtilityStopFlag ) )
	{
		CStringArray arrayBuffer;
		BuildProductBuffer( pInfo, arrayBuffer, nForceQuit );

		if ( nForceQuit != 0 )
			return;

		if ( arrayBuffer.GetSize() > 0 )
			ProcessProductBuffer( pInfo, arrayBuffer, nForceQuit );

		if ( nForceQuit != 0 )
			return;
	}

	if ( ( TRUE == pInfo -> m_bCustomer ) && ( FALSE == pInfo -> m_bPosUtilityStopFlag ) )
	{
		CStringArray arrayBuffer;
		BuildCustomerBuffer( pInfo, arrayBuffer, nForceQuit );

		if ( nForceQuit != 0 )
			return;

		if ( arrayBuffer.GetSize() > 0 )
			ProcessCustomerBuffer( pInfo, arrayBuffer, nForceQuit );

		if ( nForceQuit != 0 )
			return;
	}

	if ( ( TRUE == pInfo -> m_bPMS ) && ( FALSE == pInfo -> m_bPosUtilityStopFlag ) )
	{
		DownloadPMSRooms( pInfo, nForceQuit );

		if ( nForceQuit != 0 )
			return;

		ImportPMSSales( pInfo, nForceQuit );

		if ( nForceQuit != 0 )
			return;
	}
}

/**********************************************************************/

void CRealTimeCommsThread::ProcessHandheldExports( CRealTimeCommsThreadInfo* pInfo, int& nForceQuit )
{
	CCSV csvExport ( pInfo -> m_siteInfoSales.m_strExportControlLocs );

	for ( int nIdx = 0; nIdx < csvExport.GetSize(); nIdx += 4)
	{
		CString strTokenFolder = csvExport.GetString( nIdx );
		CString strTokenFile1 = strTokenFolder + "\\FILE3031.DAT";
		CString strTokenFile2 = strTokenFolder + "\\FILE3032.DAT";

		CString strDownloadFolder = csvExport.GetString( nIdx + 1 );
		CString strDownloadFile = strDownloadFolder + "\\PRODUCTS.SQL";
			
		CString strCommandFolder = csvExport.GetString( nIdx + 2 );
		CString strCommandFile = strCommandFolder + "\\HHDATAEXPORT.TXT";

		int nDelay = csvExport.GetInt( nIdx + 3 );

		if ( ::FileExists( strTokenFile1 ) == TRUE )
		{
			if ( ::FileExists( strDownloadFile ) == FALSE )
			{
				if ( ::ExistSubdirectory( strDownloadFolder ) == TRUE )
				{
					CFileRemover FileRemover( strTokenFile1 );

					if ( ::FileExists( strTokenFile1 ) == FALSE )
					{
						CSSFile fileToken2;
						if ( fileToken2.Open( strTokenFile2, "wb" ) == TRUE )
						{
							fileToken2.WriteLine( "Microtrend" );
							fileToken2.Close();
						}
					}
				}
			}
		}
		else if ( ::FileExists( strTokenFile2 ) == TRUE )
		{
			CFileStatus FileStatus;
			CFile::GetStatus( strTokenFile2, FileStatus );
			CTime timeCommand = FileStatus.m_mtime + CTimeSpan( 0, 0, nDelay, 0 );

			if ( CTime::GetCurrentTime() > timeCommand )
			{
				CSSFile fileCommand;
				if ( fileCommand.Open( strCommandFile, "wb" ) == TRUE )
				{
					fileCommand.WriteLine( "EXPORT 0" );
					fileCommand.Close();

					CFileRemover FileRemover( strTokenFile2 );
				}
			}
		}

		if ( TRUE == pInfo -> m_bForceQuit )
		{
			nForceQuit = 201;
			return;
		}
	}
}

/**********************************************************************/

int CRealTimeCommsThread::GetFilesToProcess( int nSpeed )
{
	switch( nSpeed )
	{
	case 1:		return 1;
	case 2:		return 5;
	case 3:		return 10;
	case 4:		return 20;
	case 5:		return 30;
	case 6:		return 45;
	case 7:		return 60;
	case 8:		return 75;
	case 9:		return 90;
	case 10:	return 100;
	default:	return 1;
	}
}

/**********************************************************************/

void CRealTimeCommsThread::BuildCASHRSPBuffer ( CRealTimeCommsThreadInfo* pInfo, CArray<CRealTimeFileBufferInfo,CRealTimeFileBufferInfo>& arrayBuffer, int& nForceQuit )
{
	CFileFind finder;

	CString strPath = pInfo -> m_siteInfoSales.m_strEcrDataPath;

	BOOL bWorking = finder.FindFile ( strPath + "*.*" );

	if ( TRUE == pInfo -> m_bForceQuit )
	{
		nForceQuit = 1;
		return;
	}

	int nFileCount = 0;
	int nZeroFileCount = 0;
	CString strZeroFileSource = "";
	int nFilesToProcess = GetFilesToProcess ( pInfo -> m_nSpeed );

	int nBufferPos = 0;
	while ( ( bWorking == TRUE ) && ( nFileCount < nFilesToProcess ) )
	{
		bWorking = finder.FindNextFile();

		if ( TRUE == pInfo -> m_bForceQuit )
		{
			nForceQuit = 2;
			return;
		}

		CRealTimeFileBufferInfo FileBufferInfo;
		if ( CCashRSPFlusher::CheckCashRSPFile( finder, FileBufferInfo ) == TRUE )
		{
			bool bFailed = FALSE;

			if ( 0 == FileBufferInfo.m_nRenameType )
			{
				CFileStatus FileStatus1;
				CString strOriginalPath = strPath + FileBufferInfo.GetOriginalFilename();
				if ( CFile::GetStatus( strOriginalPath, FileStatus1 ) == FALSE )
				{
					LogRealTimeCommsResult( pInfo, "Unable to check file status", "File", strOriginalPath, "", "" );
					bFailed = TRUE;
				}

				if ( ( FALSE == bFailed ) && ( 0 == FileStatus1.m_size ) )
				{
					nZeroFileCount++;
					strZeroFileSource = strOriginalPath;
					bFailed = TRUE;
				}
			}
			
			if ( FALSE == bFailed )
			{
				arrayBuffer.InsertAt( nBufferPos++, FileBufferInfo );
				nFileCount++;
				
				if ( 0 == FileBufferInfo.m_nRenameType )
				{
					FileBufferInfo.m_nRenameType = 1;
					arrayBuffer.Add( FileBufferInfo );
					nFileCount++;
				}
			}

			if ( TRUE == pInfo -> m_bForceQuit )
			{
				nForceQuit = 3;
				return;
			}
		}
	}

	if ( nZeroFileCount > pInfo -> m_nZeroFileCount )
	{
		pInfo -> m_nZeroFileCount = nZeroFileCount;

		CString strMsg;
		strMsg.Format( "Zero sized file (%d)", nZeroFileCount );
		LogRealTimeCommsResult( pInfo, strMsg, "Source", strZeroFileSource, "", "" );
	}
}

/**********************************************************************/

bool CRealTimeCommsThread::CheckPosTrayStopFile( CRealTimeCommsThreadInfo* pInfo )
{
	bool bResult = FALSE;
	CString strBaseFile = pInfo -> m_strPosUtilityStopFile;

	int nLength = strBaseFile.GetLength();
	
	if ( nLength > 4 )
	{
		strBaseFile = strBaseFile.Left( nLength - 4 );

		CString strNum = "";
		for ( int n = 1; n <= pInfo -> m_nMaxUsers ; n++ )
		{
			if ( n != 1 )
				strNum.Format( "%2.2d", n );

			if ( ::FileExists( strBaseFile + strNum + ".dat" ) )
			{
				bResult = TRUE;
				break;
			}
		}
	}

	return bResult;
}

/**********************************************************************/

void CRealTimeCommsThread::ProcessCASHRSPBuffer( CRealTimeCommsThreadInfo* pInfo, CArray<CRealTimeFileBufferInfo,CRealTimeFileBufferInfo>& arrayBuffer, int& nForceQuit )
{
	CSSFile fileLock;
	if ( fileLock.Open( pInfo -> m_siteInfoSales.m_strNetworkLockFilename, "wb" ) == FALSE )
		return;	
		
	CString strEcrDataPath = pInfo -> m_siteInfoSales.m_strEcrDataPath;
	
	while( arrayBuffer.GetSize() != 0 )
	{

#ifdef POSTRAY_UTILITY

		if ( CheckPosTrayStopFile( pInfo ) == TRUE )
		{
			pInfo -> m_bPosUtilityStopFlag = TRUE;
			break;
		}

#endif

		int nFileType = arrayBuffer[0].m_nFileType;
		int nRenameType = arrayBuffer[0].m_nRenameType;
		
		switch ( nRenameType )
		{
		case 0:
		case 1:
			{
				CString strOriginalFilename = "";
				strOriginalFilename += strEcrDataPath;
				strOriginalFilename += arrayBuffer[0].GetOriginalFilename();
				
				CString strRenamedFilename = "";
				strRenamedFilename += strEcrDataPath;
				strRenamedFilename += arrayBuffer[0].GetRenamedFilename();
			
				CString strLocalTempFile = "";
				strLocalTempFile += pInfo -> m_siteInfoSales.m_strLocalDestPath;
				strLocalTempFile += arrayBuffer[0].GetDestinationFilename();
		
				if ( CCashRSPFlusher::ProcessRealTimeFile ( pInfo, nRenameType, strOriginalFilename, strRenamedFilename, strLocalTempFile, nFileType ) == TRUE )
				{
					if ( 1 == nRenameType )
					{
						CFileRemover FileRemover ( strRenamedFilename );
						pInfo -> m_nTransactionCount++;
					}
				}

				CFileRemover FileRemover( strLocalTempFile );

				if ( TRUE == pInfo -> m_bForceQuit )
					nForceQuit = 101;
			}
			break;

		case 99:
			CString strOriginalFilename = "";
			strOriginalFilename += strEcrDataPath;
			strOriginalFilename += arrayBuffer[0].GetOriginalFilename();
			CFileRemover FileRemover( strOriginalFilename );

			if ( TRUE == pInfo -> m_bForceQuit )
				nForceQuit = 102;
			
			break;
		}

		arrayBuffer.RemoveAt(0);

		if ( nForceQuit != 0 )
			break;
	}

	fileLock.Close();
}

/**********************************************************************/

void CRealTimeCommsThread::BuildProductBuffer ( CRealTimeCommsThreadInfo* pInfo, CStringArray& arrayBuffer, int& nForceQuit )
{
	CFileFind finder;

	CString strPath = pInfo -> m_siteInfoProducts.m_strEcrDataPath;
	
	BOOL bWorking = finder.FindFile ( strPath + "*.*" );

	if ( TRUE == pInfo -> m_bForceQuit )
	{
		nForceQuit = 301;
		return;
	}

	int nFilesToProcess = GetFilesToProcess ( pInfo -> m_nSpeed );

	while ( ( bWorking == TRUE ) && ( ( arrayBuffer.GetSize() < nFilesToProcess ) ) )
	{
		bWorking = finder.FindNextFile();

		if ( TRUE == pInfo -> m_bForceQuit )
		{
			nForceQuit = 302;
			return;
		}

		if ( finder.IsDirectory() == TRUE )
			continue;

		if ( finder.IsDots() == TRUE )
			continue;

		CString strFileTitle = finder.GetFileTitle();
		CString strFileName = finder.GetFileName();

		int nFileNameLen = strFileName.GetLength();
		if ( nFileNameLen != strFileTitle.GetLength() + 4 )
			continue;

		if ( nFileNameLen < 8 )
			continue;

		strFileName.MakeUpper();

		if ( strFileName.Right(4) != ".CSV" )
			continue;

		if ( strFileName.Left(4) != "PROD" )
			continue;

		CString strPlu = "";
		
		if ( nFileNameLen > 8 )
		{
			strPlu = strFileName.Mid( 4, nFileNameLen - 8 );
			if ( ::TestNumeric( strPlu ) == FALSE )
				continue;
		}

		arrayBuffer.Add ( strPlu );	
	}
}

/**********************************************************************/

void CRealTimeCommsThread::ProcessProductBuffer( CRealTimeCommsThreadInfo* pInfo, CStringArray& arrayBuffer, int &nForceQuit )
{
	CSSFile fileLock;
	if ( fileLock.Open( pInfo -> m_siteInfoSales.m_strNetworkLockFilename, "wb" ) == FALSE )
		return;	
		
	CString strEcrDataPath = pInfo -> m_siteInfoProducts.m_strEcrDataPath;
	
	while( arrayBuffer.GetSize() != 0 )
	{

#ifdef POSTRAY_UTILITY

		if ( CheckPosTrayStopFile( pInfo ) == TRUE )
		{
			pInfo -> m_bPosUtilityStopFlag = TRUE;
			break;
		}

#endif

		CString strPlu = arrayBuffer.GetAt(0);
		
		CString strOriginalFolder = "";
		strOriginalFolder += pInfo -> m_siteInfoProducts.m_strEcrDataPath;
		
		CString strOriginalFilename = "";
		strOriginalFilename += "PROD";
		strOriginalFilename += strPlu;
		strOriginalFilename += ".CSV";

		CString strLocalTempFolder = "";
		strLocalTempFolder += pInfo -> m_siteInfoProducts.m_strLocalDestPath;

		CString strLocalTempFilename = strOriginalFilename + ".TMP";
	
		if ( ProcessProductFileInternal( pInfo, strOriginalFolder, strOriginalFilename, strLocalTempFolder, strLocalTempFilename ) == TRUE )
			CFileRemover FileRemover ( strOriginalFolder + strOriginalFilename );
	
		CFileRemover FileRemover( strOriginalFolder + strOriginalFilename );
		arrayBuffer.RemoveAt(0);

		if ( TRUE == pInfo -> m_bForceQuit )
		{
			nForceQuit = 401;
			break;
		}
	}

	fileLock.Close();
}

/**********************************************************************/

void CRealTimeCommsThread::BuildCustomerBuffer ( CRealTimeCommsThreadInfo* pInfo, CStringArray& arrayBuffer, int& nForceQuit )
{
	CFileFind finder;

	CString strPath = pInfo -> m_siteInfoCustomer.m_strEcrDataPath;
	
	BOOL bWorking = finder.FindFile ( strPath + "CUSTOMER*.SQL" );

	if ( TRUE == pInfo -> m_bForceQuit )
	{
		nForceQuit = 501;
		return;
	}

	int nFilesToProcess = GetFilesToProcess ( pInfo -> m_nSpeed );

	while ( ( bWorking == TRUE ) && ( ( arrayBuffer.GetSize() < nFilesToProcess ) ) )
	{
		bWorking = finder.FindNextFile();

		if ( TRUE == pInfo -> m_bForceQuit )
		{
			nForceQuit = 502;
			return;
		}

		if ( finder.IsDirectory() == TRUE )
			continue;

		if ( finder.IsDots() == TRUE )
			continue;

		CString strFileTitle = finder.GetFileTitle();
		CString strFileName = finder.GetFileName();

		int nFileNameLen = strFileName.GetLength();

		CString strCustomer = strFileName.Mid( 8, nFileNameLen - 12 );		
		strCustomer.MakeUpper();

		if ( ( strCustomer == "S" ) || ( ::TestNumeric( strCustomer ) == TRUE ) )
			arrayBuffer.Add ( strCustomer );
	}
}

/**********************************************************************/

void CRealTimeCommsThread::ProcessCustomerBuffer( CRealTimeCommsThreadInfo* pInfo, CStringArray& arrayBuffer, int& nForceQuit )
{
	CSSFile fileLock;
	if ( fileLock.Open( pInfo -> m_siteInfoSales.m_strNetworkLockFilename, "wb" ) == FALSE )
		return;	
		
	CString strEcrDataPath = pInfo -> m_siteInfoCustomer.m_strEcrDataPath;
	
	while( arrayBuffer.GetSize() != 0 )
	{

#ifdef POSTRAY_UTILITY

		if ( CheckPosTrayStopFile( pInfo ) == TRUE )
		{
			pInfo -> m_bPosUtilityStopFlag = TRUE;
			break;
		}

#endif

		CString strOriginalFolder = "";
		strOriginalFolder += pInfo -> m_siteInfoCustomer.m_strEcrDataPath;
		
		CString strOriginalFilename = "";
		strOriginalFilename += "CUSTOMER";
		strOriginalFilename += arrayBuffer.GetAt(0);
		strOriginalFilename += ".SQL";
		
		CString strLocalTempFolder = "";
		strLocalTempFolder += pInfo -> m_siteInfoCustomer.m_strLocalDestPath;

		CString strLocalTempFilename = strOriginalFilename + ".TMP";
	
		if ( ProcessCustomerFileInternal( pInfo, strOriginalFolder, strOriginalFilename, strLocalTempFolder, strLocalTempFilename ) == TRUE )
			CFileRemover FileRemover ( strOriginalFolder + strOriginalFilename );
	
		CFileRemover FileRemover( strOriginalFolder + strOriginalFilename );

		arrayBuffer.RemoveAt(0);

		if ( pInfo -> m_bForceQuit == TRUE )
		{
			nForceQuit = 601;
			break;
		}
	}

	fileLock.Close();
}

/**********************************************************************/

bool CRealTimeCommsThread::ProcessProductFileInternal( CRealTimeCommsThreadInfo* pInfo, CString& strOriginalFolder, CString& strOriginalFilename, CString& strLocalTmpFolder, CString& strLocalTmpFilename )
{		
	CString strOriginalFile = strOriginalFolder + strOriginalFilename;
	CString strLocalTmpFile = strLocalTmpFolder + strLocalTmpFilename;

	if ( ::FileExists( strOriginalFile ) == FALSE )
		return FALSE;

	if ( CopyFile( strOriginalFile, strLocalTmpFile, FALSE ) == 0 )
	{
		CString strMsg = "Unable to copy file ";
		strMsg += strOriginalFile;
		LogRealTimeCommsMessage( pInfo, strMsg );
		return FALSE;
	}

	CFileStatus FileStatus1;
	if ( CFile::GetStatus( strOriginalFile, FileStatus1 ) == FALSE )
	{
		CString strMsg = "Unable to check file status ";
		strMsg += strOriginalFilename;
		LogRealTimeCommsMessage( pInfo, strMsg );
		return FALSE;
	}
	
	CFileStatus FileStatus2;
	if ( CFile::GetStatus( strLocalTmpFile, FileStatus2 ) == FALSE )
	{
		CString strMsg = "Unable to check file status ";
		strMsg += strLocalTmpFile;
		LogRealTimeCommsMessage( pInfo, strMsg );
		return FALSE;
	}

	if ( FileStatus1.m_size != FileStatus2.m_size )
	{
		CString strMsg = "Error in copied file ";
		strMsg += strOriginalFile;
		LogRealTimeCommsMessage( pInfo, strMsg );
		return FALSE;
	}

	if ( FileStatus1.m_size == 0 )
		return FALSE;

	CString strLocalProductFile = "";
	strLocalProductFile += pInfo -> m_siteInfoProducts.m_strLocalDestPath;
	strLocalProductFile += strOriginalFilename;

	if ( ::CopyFile( strLocalTmpFile, strLocalProductFile, FALSE ) == FALSE )
	{
		CString strMsg = "Unable to copy file ";
		strMsg += strLocalTmpFile;
		LogRealTimeCommsMessage( pInfo, strMsg );
		return FALSE;
	}

	CFileRemover FileRemover( strLocalTmpFile );

	return TRUE;
}

/**********************************************************************/

bool CRealTimeCommsThread::ProcessCustomerFileInternal( CRealTimeCommsThreadInfo* pInfo, CString& strOriginalFolder, CString& strOriginalFilename, CString& strLocalTmpFolder, CString& strLocalTmpFilename )
{		
	CString strOriginalFile = strOriginalFolder + strOriginalFilename;
	CString strLocalTmpFile = strLocalTmpFolder + strLocalTmpFilename;

	if ( ::FileExists( strOriginalFile ) == FALSE )
		return FALSE;

	if ( CopyFile( strOriginalFile, strLocalTmpFile, FALSE ) == 0 )
	{
		CString strMsg = "Unable to copy file ";
		strMsg += strOriginalFile;
		LogRealTimeCommsMessage( pInfo, strMsg );
		return FALSE;
	}

	CFileStatus FileStatus1;
	if ( CFile::GetStatus( strOriginalFile, FileStatus1 ) == FALSE )
	{
		CString strMsg = "Unable to check file status ";
		strMsg += strOriginalFilename;
		LogRealTimeCommsMessage( pInfo, strMsg );
		return FALSE;
	}
	
	CFileStatus FileStatus2;
	if ( CFile::GetStatus( strLocalTmpFile, FileStatus2 ) == FALSE )
	{
		CString strMsg = "Unable to check file status ";
		strMsg += strLocalTmpFile;
		LogRealTimeCommsMessage( pInfo, strMsg );
		return FALSE;
	}

	if ( FileStatus1.m_size != FileStatus2.m_size )
	{
		CString strMsg = "Error in copied file ";
		strMsg += strOriginalFile;
		LogRealTimeCommsMessage( pInfo, strMsg );
		return FALSE;
	}

	if ( FileStatus1.m_size == 0 )
		return FALSE;

	CString strLocalCustomerFile = "";
	strLocalCustomerFile += pInfo -> m_siteInfoCustomer.m_strLocalDestPath;
	strLocalCustomerFile += strOriginalFilename;

	if ( ::CopyFile( strLocalTmpFile, strLocalCustomerFile, FALSE ) == FALSE )
	{
		CString strMsg = "Unable to copy file ";
		strMsg += strLocalTmpFile;
		LogRealTimeCommsMessage( pInfo, strMsg );
		return FALSE;
	}

	CFileRemover FileRemover( strLocalTmpFile );

	return TRUE;
}

/**********************************************************************/

void CRealTimeCommsThread::DownloadPMSRooms( CRealTimeCommsThreadInfo* pInfo, int& nForceQuit )
{
	CString strSourceFolder = pInfo -> m_strPMSDownloadFolder;
	strSourceFolder += "\\";

	CString strDestFolder = pInfo -> m_strPMSRoomFolder;
	if ( strDestFolder.GetLength() < 5 )
		return;

	strDestFolder.MakeUpper();
	if ( strDestFolder.Right(5) != "\\ROOM" )
		return;

	strDestFolder += "\\";

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strSourceFolder + "*.*" );

	if ( TRUE == pInfo -> m_bForceQuit )
	{
		nForceQuit = 701;
		return;
	}

	CStringArray arrayFilesToDelete;

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if ( TRUE == pInfo -> m_bForceQuit )
		{
			nForceQuit = 702;
			return;
		}

		if ( finder.IsDots() )
			continue;

		if ( finder.IsDirectory() )
			continue;

		CString strFilename = finder.GetFileName();

		if ( CopyFile( strSourceFolder + strFilename, strDestFolder + strFilename, FALSE ) == TRUE )
			arrayFilesToDelete.Add( strFilename );
		else
			break;
	} 

	for ( int n = 0; n < arrayFilesToDelete.GetSize(); n++ )
		CFileRemover FileRemover( strSourceFolder + arrayFilesToDelete.GetAt(n) );
}

/**********************************************************************/

void CRealTimeCommsThread::GetSchedulerLogLine( const char* szText, bool bHide, CString& strLine )
{
	CCSV csv('\t');
	csv.Add( "" );
	csv.Add( szText );

	CTime tm = CTime::GetCurrentTime();
		
	CString strDate = tm.Format ( "%a %b %d %Y" );
	csv.Add( strDate );
	
	CString strTime = tm.Format ( "%H:%M:%S" );
	csv.Add( strTime );

	if ( TRUE == bHide )
		strLine.Format ( "<HIDE1>%s", csv.GetLine() );
	else
		strLine = csv.GetLine();
}	

/**********************************************************************/

void CRealTimeCommsThread::CheckMaxLogFileSize( CString& strNew, CString& strOld, long lSize )
{
	CFileStatus Status;
	if ( CFile::GetStatus ( strNew, Status ) == TRUE )
	{
		if ( Status.m_size > lSize )	
		{
			CopyFile( strNew, strOld, FALSE );
			CFileRemover FileRemover( strNew );
		}
	}
}

/**********************************************************************/

void CRealTimeCommsThread::LogRealTimeCommsResult( CRealTimeCommsThreadInfo* pInfo, const char* szHeader, const char* szLine1, const char* szFile1, const char* szLine2, const char* szFile2 )
{
	LogRealTimeCommsMessage( pInfo, szHeader, TRUE );

	CString strLine1 = szLine1;
	if ( strLine1 != "" )
	{
		CString strLine = strLine1;
		strLine += " :";
		strLine += szFile1;
		LogRealTimeCommsMessage( pInfo, strLine, FALSE );
	}

	CString strLine2 = szLine2;
	if ( strLine2 != "" )
	{
		CString strLine = strLine2;
		strLine += " :";
		strLine += szFile2;
		LogRealTimeCommsMessage( pInfo, strLine, FALSE );
	}
}

/**********************************************************************/

void CRealTimeCommsThread::LogRealTimeCommsMessage( CRealTimeCommsThreadInfo* pInfo, const char* szText, bool bLineBefore, bool bHide )
{
	CString strText = "Server5000 ";
	strText += szText;

	CreateSyssetSubdirectory ( "SchedLog" );
	CreateSyssetSubdirectory ( "SchedOld" );

	CString strLine;
	GetSchedulerLogLine( strText, bHide, strLine );

	//SCHEDULER LOG
	{
		CString strNew = pInfo -> m_strSchedLogAllFilename; 
		CString strOld = pInfo -> m_strSchedLogOldFilename; 
		CheckMaxLogFileSize( strNew, strOld, pInfo -> m_lMaxSchedLogFileSize );
		
		if ( ::FileExists ( strNew ) == FALSE )
		{
			CReportFile ReportFile;
			ReportFile.Open ( strNew );
			ReportFile.SetStyle1 ( "Communications Log" );
			ReportFile.AddColumn ( "",TA_LEFT, 300 );
			ReportFile.AddColumn ( "",TA_LEFT, 900 );
			ReportFile.AddColumn ( "",TA_RIGHT, 400 );
			ReportFile.AddColumn ( "",TA_LEFT, 1200 );
			ReportFile.WriteHeader();
			ReportFile.Close();
		}

		CSSFile fileSched;
		if ( fileSched.Open( strNew, "ab" ) == TRUE )
		{
			if ( TRUE == bLineBefore )
				fileSched.WriteLine( "<LI>" );

			fileSched.WriteLine( strLine );
		}
	}

	//NETWORK LOG
	{
		CString strNew = pInfo -> m_strNetworkLogAllFilename;
		CString strOld = pInfo -> m_strNetworkLogOldFilename;

		CheckMaxLogFileSize( strNew, strOld, pInfo -> m_lMaxNetworkLogFileSize );

		if ( ::FileExists ( strNew ) == FALSE )
		{
			CReportFile ReportFile;
			ReportFile.Open ( strNew );
			ReportFile.SetStyle1 ( "Communications Log" );
			ReportFile.AddColumn ( "",TA_LEFT, 300 );
			ReportFile.AddColumn ( "",TA_LEFT, 900 );
			ReportFile.AddColumn ( "",TA_RIGHT, 400 );
			ReportFile.AddColumn ( "",TA_LEFT, 1200 );
			ReportFile.WriteHeader();
			ReportFile.Close();
		}

		CSSFile fileSched;
		if ( fileSched.Open( strNew, "ab" ) == TRUE )
		{
			if ( TRUE == bLineBefore )
				fileSched.WriteLine( "<LI>" );

			fileSched.WriteLine( strLine );
		}
	}
}

/**********************************************************************/

void CRealTimeCommsThread::ImportPMSSales( CRealTimeCommsThreadInfo* pInfo, int& nForceQuit )
{
	CSSFile fileLock;
	if ( fileLock.Open( pInfo -> m_strPMSLockFile, "wb" ) == TRUE )
	{
		ImportPMSSalesInternal( pInfo, nForceQuit );
		fileLock.Close();
	}
}

/**********************************************************************/

void CRealTimeCommsThread::ImportPMSSalesInternal( CRealTimeCommsThreadInfo* pInfo, int& nForceQuit )
{
	CString strPostPath = pInfo -> m_strPMSPostPath;
	if ( strPostPath.GetLength() < 5 )
		return;

	strPostPath.MakeUpper();
	if ( strPostPath.Right(5) != "\\POST" )
		return;
 
	strPostPath += "\\";

	CStringArray arrayFilenames;
	BuildPMSBuffer( pInfo, strPostPath, arrayFilenames, nForceQuit );

	if ( nForceQuit != 0 )
		return;

	ProcessPMSBuffer( pInfo, strPostPath, arrayFilenames, nForceQuit );

	if ( nForceQuit != 0 )
		return;
}

/**********************************************************************/

void CRealTimeCommsThread::BuildPMSBuffer( CRealTimeCommsThreadInfo* pInfo, CString& strPostPath, CStringArray& arrayFilenames, int& nForceQuit )
{
	CFileFind finder;

	BOOL bWorking = finder.FindFile ( strPostPath + "*.*" );

	if ( TRUE == pInfo -> m_bForceQuit )
	{
		nForceQuit = 801;
		return;
	}

	int nFilesToProcess = GetFilesToProcess ( pInfo -> m_nSpeed );

	while ( ( bWorking == TRUE ) && ( ( arrayFilenames.GetSize() < nFilesToProcess ) || ( pInfo -> m_bAllPMS == TRUE ) ) )
	{
		bWorking = finder.FindNextFile();

		if ( TRUE == pInfo -> m_bForceQuit )
		{
			nForceQuit = 802;
			return;
		}

		if ( finder.IsDirectory() == TRUE )
			continue;

		if ( finder.IsDots() == TRUE )
			continue;

		CString strFileTitle = finder.GetFileTitle();
		CString strFileName = finder.GetFileName();

		int nFileNameLen = strFileName.GetLength();
		int nFileTitleLen = strFileTitle.GetLength();
		
		if ( nFileNameLen < nFileTitleLen + 2 )
			continue;

		if ( nFileNameLen < 3 )
			continue;

		CString strExt = strFileName.Right( nFileNameLen - nFileTitleLen - 1 );	
		if ( ::TestNumeric( strExt ) == FALSE )
			continue;

		bool bAccept = FALSE;
		if ( ::TestNumeric( strFileTitle ) == TRUE )
		{
			bAccept = TRUE;
		}
		else if ( nFileTitleLen >= 24 )
		{
			strFileTitle.MakeUpper();
			if ( strFileTitle.Left(7) == "SSFILE_" )
			{
				CString strTest = strFileTitle.Right( nFileTitleLen - 23 );
				bAccept = ::TestNumeric( strTest );
			}
		}

		if ( TRUE == bAccept )
			arrayFilenames.Add( strFileName );
	}
}

/**********************************************************************/

void CRealTimeCommsThread::ProcessPMSBuffer( CRealTimeCommsThreadInfo* pInfo, CString& strPostPath, CStringArray& arrayFilenames, int& nForceQuit )
{
	for ( int n = 0; n < arrayFilenames.GetSize(); n++ )
	{
		CString strTrans = arrayFilenames.GetAt(n);

		CString strOriginalFilename = "";
		strOriginalFilename = strPostPath;
		strOriginalFilename += strTrans;

		CString strRenamedFilename = "";
		CString strLocalTempFile = "";

		if ( strTrans.GetLength() > 23 )
		{
			strRenamedFilename = strOriginalFilename;

			strLocalTempFile = pInfo -> m_strPMSFolderPending;
			strLocalTempFile += "\\";
			strLocalTempFile += strTrans;
		}
		else
		{
			COleDateTime timeNow = COleDateTime::GetCurrentTime();

			CString strSSFilePrefix;
			strSSFilePrefix.Format( "SSFILE_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d_",
				timeNow.GetYear(),
				timeNow.GetMonth(),
				timeNow.GetDay(),
				timeNow.GetHour(),
				timeNow.GetMinute(),
				timeNow.GetSecond() );

			strRenamedFilename = strPostPath;
			strRenamedFilename += strSSFilePrefix;
			strRenamedFilename += strTrans;

			strLocalTempFile = pInfo -> m_strPMSFolderPending;
			strLocalTempFile += "\\";
			strLocalTempFile += strSSFilePrefix;
			strLocalTempFile += strTrans;
		}

		CString strLocalSalesFile = pInfo -> m_strPMSFolderSales;
		strLocalSalesFile += "\\";
		strLocalSalesFile += strTrans;

		if ( ProcessPMSFile ( strOriginalFilename, strRenamedFilename, strLocalTempFile, strLocalSalesFile ) == TRUE )
			CFileRemover FileRemover ( strRenamedFilename );	

		if ( TRUE == pInfo -> m_bForceQuit )
		{
			nForceQuit = 901;
			break;
		}
	}
}

/**********************************************************************/

bool CRealTimeCommsThread::ProcessPMSFile ( CString& strOriginalFilename, CString& strRenamedFilename, CString& strLocalTempFile, CString& strLocalSalesFile )
{
	if ( strOriginalFilename != strRenamedFilename )
		if ( rename( strOriginalFilename, strRenamedFilename ) != 0 )
			return FALSE;

	if ( CopyFile( strRenamedFilename, strLocalTempFile, FALSE ) == 0 )
		return FALSE;

	CFileStatus FileStatus1, FileStatus2;
	CFile::GetStatus( strRenamedFilename, FileStatus1 );
	CFile::GetStatus( strLocalTempFile, FileStatus2 );

	if ( FileStatus1.m_size != FileStatus2.m_size )
		return FALSE;

	if ( FileStatus1.m_size == 0 )
		return FALSE;

	{
		CSSFile fileLocalTemp, fileLocalSales;

		if ( fileLocalTemp.Open( strLocalTempFile, "rb" ) == FALSE )
			return FALSE;
	
		if ( fileLocalSales.Open( strLocalSalesFile, "ab" ) == FALSE )
			return FALSE;
		
		CString strBuffer;
		while( fileLocalTemp.ReadString( strBuffer ) == TRUE )
			fileLocalSales.WriteLine( strBuffer );
	}

	CFileRemover FileRemover( strLocalTempFile );
	
	return TRUE;
}

/**********************************************************************/
