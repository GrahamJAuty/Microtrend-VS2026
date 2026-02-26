/**********************************************************************/
#include "..\CommonEcrStock\CustomExportManager.h"
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "StockpointCSVArray.h"
#include "UnitechExportCreator.h"
#include "UnitechOptions.h"
#include "WinUser.h"
/**********************************************************************/
#include "UnitechBatchJob.h"
/**********************************************************************/

CUnitechBatchJob::CUnitechBatchJob( int nDbIdx, int nSpIdx )
{
	m_nDbIdx = nDbIdx;
	m_nSpIdx = nSpIdx;

	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	m_strTimeStamp.Format( "%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond() );

	m_nNetUseDrive = UnitechOptions.GetNetUseFlag();
	
	m_strNetUseRootPath = "";
	if ( ( m_nNetUseDrive >= 1 ) && ( m_nNetUseDrive <= 20 ) )
		::GetDataProgramPath( m_strNetUseRootPath );
	
	int nLength = m_strNetUseRootPath.GetLength();
	if ( ( nLength < 8 ) || ( m_strNetUseRootPath.Left(2) != "\\\\" ) )
	{
		m_nNetUseDrive = 0;
		m_strNetUseRootPath = "";
	}
	else
		m_strNetUseRootPath = m_strNetUseRootPath.Left( nLength - 1 );
}

/**********************************************************************/

int CUnitechBatchJob::RunBatch( bool bUploadPlu, bool bDownloadPlu, bool bUploadStock, CUnitechBatchJobResults& results, CWnd* pOwner )
{
	CSSFile fileLockDb, fileLockSp;

	int nResult = UNITECH_BATCH_ERROR_NONE;

	if ( ( TRUE == bDownloadPlu ) || ( TRUE == bUploadPlu ) )
	{
		CFilenameUpdater FnUp( SysFiles::UnitechDbLock, m_nDbIdx );
		
		if ( fileLockDb.Open( FnUp.GetFilenameToUse(), "wb" ) == FALSE )
			nResult = UNITECH_BATCH_ERROR_LOCK;
	}

	if ( ( TRUE == bUploadStock ) && ( UNITECH_BATCH_ERROR_NONE == nResult ) )
	{
		CFilenameUpdater FnUp( SysFiles::UnitechSpLock, m_nSpIdx );
		
		if ( fileLockSp.Open( FnUp.GetFilenameToUse(), "wb" ) == FALSE )
			nResult = UNITECH_BATCH_ERROR_LOCK;
	}

	if ( UNITECH_BATCH_ERROR_NONE == nResult )
	{
		nResult = RunBatchInternal( bUploadPlu, bDownloadPlu, bUploadStock, pOwner );

		if ( UNITECH_BATCH_ERROR_NONE == nResult )
		{
			if( TRUE == bUploadStock )
				FilterStockUploads( results );

			if( TRUE == bUploadPlu )
				FilterPluUploads();
		}
	}

	return nResult;
}

/**********************************************************************/

int CUnitechBatchJob::RunBatchInternal( bool bUploadPlu, bool bDownloadPlu, bool bUploadStock, CWnd* pOwner )
{
	m_bWrittenHeader = FALSE;
	
	// "C:\\ptcomm\\ptcomm.exe";
	CString strProgram = UnitechOptions.GetFullProgramPath();
	if ( ::FileExists( strProgram ) == FALSE )
		return UNITECH_BATCH_ERROR_PROGRAM;
	
	CSSFile fileBatch;
	CFilenameUpdater FnUpBatch ( SysFiles::UnitechBatch );
	if ( fileBatch.Open ( FnUpBatch.GetFilenameToUse(), "wb" ) == FALSE )
		return UNITECH_BATCH_ERROR_JOBFILE;

	if ( TRUE == bUploadStock )
	{
		CString strDestFolder;
		strDestFolder = dbStockpoint.GetFolderPath( m_nSpIdx );
		strDestFolder += "\\Unitech";
		
		if ( ::ExistSubdirectory( strDestFolder ) == FALSE )
			::CreateSubdirectory( strDestFolder );

		if ( ::ExistSubdirectory( strDestFolder ) == FALSE )
			return UNITECH_BATCH_ERROR_CREATEFOLDER;
	}

	if ( TRUE == bUploadPlu )
	{
		CString strDestFolder;
		strDestFolder = dbDatabase.GetFolderPathData( m_nDbIdx );
		strDestFolder += "\\Unitech";
		
		if ( ::ExistSubdirectory( strDestFolder ) == FALSE )
			::CreateSubdirectory( strDestFolder );

		if ( ::ExistSubdirectory( strDestFolder ) == FALSE )
			return UNITECH_BATCH_ERROR_CREATEFOLDER;
	}

	CSSFile fileClear;
	CFilenameUpdater FnUpClear ( SysFiles::UnitechClear );
	if ( fileClear.Open( FnUpClear.GetFilenameToUse(), "wb" ) == TRUE )
	{
		fileClear.WriteLine( "" );
		fileClear.Close();
	}

// downlod csv plu data
	if ( TRUE == bDownloadPlu )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( m_nDbIdx, info );
		CUnitechExportCreator ExportCreator;

		WriteHeader( fileBatch, TRUE, "Download Plu File" );

		CFilenameUpdater FnUpPlu( SysFiles::UnitechPlu, m_nDbIdx );

		CString strLine;
		strLine.Format ( "FILE(OVERWRITE): \x022%s\x022 <> plu.csv",
			GetNetUseFolderPath( FnUpPlu.GetFilenameToUse() ) );
		
		fileBatch.WriteLine ( strLine );
	}

	if  ( ( TRUE == bUploadStock ) || ( TRUE == bUploadPlu ) )
	{
// upoad csv files from device
		WriteHeader( fileBatch, FALSE, "Upload data files" );

		if ( TRUE == bUploadStock )
		{
			WriteAppend( fileBatch, "delivery" );
			WriteAppend( fileBatch, "wrapper" );
			WriteAppend( fileBatch, "returns" );
			WriteAppend( fileBatch, "sales" );
			WriteAppend( fileBatch, "adjust" );
			WriteAppend( fileBatch, "closing" );
		}

		if ( TRUE == bUploadPlu )
			WriteFetchPlu( fileBatch, "pluxfer" );
			
		
// Delete csv files in device
		WriteHeader( fileBatch, TRUE, "Reset data files" );

		if ( TRUE == bUploadStock )
		{
			WriteClear( fileBatch, "delivery" );
			WriteClear( fileBatch, "wrapper" );
			WriteClear( fileBatch, "returns" );
			WriteClear( fileBatch, "sales" );
			WriteClear( fileBatch, "adjust" );
			WriteClear( fileBatch, "closing" );
		}

		if ( TRUE == bUploadPlu )
			WriteClear( fileBatch, "pluxfer" );
	}

	fileBatch.Close();

	// terminate batch if all ok
	CString strP1 = "/exit";								

	// "/comport:6"
	CString strP2;											
	strP2.Format ( "/comport:%d", UnitechOptions.GetComPortNo() );
	
	// "/ptaddr:A"
	CString strP3 = "/ptaddr:A";											
	
	CString strP4;
	strP4.Format ( "/batch:\x022%s\x022",
		FnUpBatch.GetFilenameToUse() );

	CreateNetUseBatchFiles();

	// run unitech program with newly create batch file
	//	C:\\ptcomm\\ptcomm.exe /exit /comport:6 /ptaddr:A /batch:stock.txt 

	bool bLaunchFail = FALSE;

	{
        DWORD dwForegroundLockTimeout;
        if ( pOwner != NULL )
        {			
			::SystemParametersInfo ( SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &dwForegroundLockTimeout, 0 );
            ::SystemParametersInfo ( SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (PVOID)0, 0 );
            pOwner->EnableWindow ( FALSE );
        }

		NetUseOn();
 
        DWORD dwExitCode = -1;
        HANDLE hProcess = (HANDLE) _spawnl ( _P_NOWAIT, strProgram, strProgram, strP1, strP2, strP3, strP4, NULL );
        if ( (int) hProcess != -1 )
        {
            while ( WaitForSingleObject ( hProcess, 1 ) == WAIT_TIMEOUT )
            {
                MSG msg;
                while ( ::PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) != 0 )
                {
                    ::TranslateMessage ( &msg );
                    ::DispatchMessage ( &msg );
                }
            }
 
            GetExitCodeProcess ( hProcess, &dwExitCode );
        }
		else
			bLaunchFail = TRUE;
     
        if ( pOwner != NULL )
        {       
			pOwner->SetForegroundWindow();         		
			::SystemParametersInfo ( SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (PVOID)dwForegroundLockTimeout, 0 );          
			pOwner->EnableWindow ( TRUE );
		}

		NetUseOff();
    }
		
	if ( TRUE == bLaunchFail )
		return UNITECH_BATCH_ERROR_SPAWN;
	else
		return UNITECH_BATCH_ERROR_NONE; 
}

/**********************************************************************/

void CUnitechBatchJob::WriteHeader( CSSFile& file, bool bDownload, const char* szComment )
{
	CString strComment = "ECHO: ";
	strComment += szComment;

	CString strDirection = "DIRECTION: ";
	if ( TRUE == bDownload )
		strDirection += "Download";
	else
		strDirection += "Upload";

	file.WriteLine( strComment );
	
	if ( FALSE == m_bWrittenHeader )
	{
		CString strFastSpeed;
		if ( UnitechOptions.GetFastSpeedFlag() == TRUE )
			strFastSpeed = "FASTSPEED: yes";
		else
			strFastSpeed = "FASTSPEED: no";

		file.WriteLine( strFastSpeed );
		m_bWrittenHeader = TRUE;
	}

	file.WriteLine( strDirection );
}

/**********************************************************************/

void CUnitechBatchJob::WriteAppend( CSSFile& file, const char* szFilename )
{
	CString strLine;
	strLine.Format ( "FILE(OVERWRITE): %s.csv <> \x022%s\\unitech\\%s_%s.tmp\x022",
		szFilename,
		GetNetUseFolderPath( dbStockpoint.GetFolderPath( m_nSpIdx ) ),
		(const char*) m_strTimeStamp,
		szFilename );

	file.WriteLine ( strLine );

	strLine.Format( "DELETE: %s.csv", szFilename );
	file.WriteLine ( strLine );
}

/**********************************************************************/

void CUnitechBatchJob::WriteFetchPlu( CSSFile& file, const char* szFilename )
{
	CString strLine;
	
	strLine.Format ( "FILE(OVERWRITE): %s.csv <> \x022%s\\unitech\\%s_%s.tmp\x022",
		szFilename,
		GetNetUseFolderPath( dbDatabase.GetFolderPathData( m_nDbIdx ) ),
		szFilename,
		(const char*) m_strTimeStamp );
	file.WriteLine ( strLine );
	
	strLine.Format( "DELETE: %s.csv", szFilename );
	file.WriteLine ( strLine );
}

/**********************************************************************/

void CUnitechBatchJob::WriteClear( CSSFile& file, const char* szFilename )
{
	CFilenameUpdater FnUpClear ( SysFiles::UnitechClear );
	
	CString strLine;
	strLine.Format ( "FILE: \x022%s\x022 <> %s.csv", 
		GetNetUseFolderPath( FnUpClear.GetFilenameToUse() ),
		szFilename );
	
	file.WriteLine ( strLine );
}

/**********************************************************************/

void CUnitechBatchJob::FilterStockUploads( CUnitechBatchJobResults& results )
{
	CArray<CUnitechFileInfo,CUnitechFileInfo> arrayFiles;
	
	CString strFileSpec;
	strFileSpec.Format( "%s\\unitech\\*.tmp", dbStockpoint.GetFolderPath( m_nSpIdx ) );

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strFileSpec );

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDirectory() == TRUE )
			continue;

		if ( finder.IsDots() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();

		int nNameLen = strFilename.GetLength();
		if ( nNameLen < 21 )
			continue;

		strFilename.MakeUpper();

		if ( ::TestNumeric( strFilename.Left(8) ) == FALSE )
			continue;

		if ( strFilename.Mid( 8,1 ) != "_" )
			continue;

		if ( ::TestNumeric( strFilename.Mid(9,6) ) == FALSE )
			continue;

		if ( strFilename.Mid( 15,1 ) != "_" )
			continue;

		if ( strFilename.Right(4) != ".TMP" )
			continue;
	
		CString strType = strFilename.Mid( 16, nNameLen - 20 );

		strType.MakeLower();

		int nFileType = 0;
		if ( strType == "delivery" )		nFileType = 1;
		else if ( strType == "wrapper" )	nFileType = 2;
		else if ( strType == "returns" )	nFileType = 3;
		else if ( strType == "sales" )		nFileType = 4;
		else if ( strType == "adjust" )		nFileType = 5;
		else if ( strType == "closing" )	nFileType = 6;

		if ( 0 != nFileType )
		{
			CUnitechFileInfo info;
			info.m_strTimeStamp = strFilename.Left(15);
			info.m_nFileType = nFileType;
			arrayFiles.Add( info );
		}
	}

	CString strFolderPath;
	strFolderPath.Format( "%s\\unitech\\",
		dbStockpoint.GetFolderPath( m_nSpIdx ) );

	for ( int n = 0; n < arrayFiles.GetSize(); n++ )
	{
		CString strFilename = strFolderPath;
		strFilename += arrayFiles[n].m_strTimeStamp;
		strFilename += "_";

		CString strFileType = "";
		switch( arrayFiles[n].m_nFileType )
		{
		case 1:	strFileType += "delivery";		break;
		case 2:	strFileType += "wrapper";		break;
		case 3:	strFileType += "returns";		break;
		case 4:	strFileType += "sales";			break;
		case 5:	strFileType += "adjust";		break;
		case 6:	strFileType += "closing";		break;
		}

		strFilename += strFileType;
		strFilename += ".tmp";

		CString strRenamedFilename = strFilename;
		strRenamedFilename.SetAt( strRenamedFilename.GetLength() - 1, '2' );

		if ( rename( strFilename, strRenamedFilename ) != 0 )
			continue;

		CString strDest;
		strDest.Format( "%s\\%s.csv",
			dbStockpoint.GetFolderPath( m_nSpIdx ),
			(const char*) strFileType );

		int nLineCount = 0;
		bool bFileFail = FALSE;

		//SCOPE TO AUTO CLOSE FILES
		{
			CSSFile fileSource;
			if ( fileSource.Open( strRenamedFilename, "rb" ) == FALSE )
				bFileFail = TRUE;
			else
			{
				CSSFile fileDest;

				CString strBuffer;
				while ( ( fileSource.ReadString( strBuffer ) == TRUE ) && ( FALSE == bFileFail ) )
				{
					if ( strBuffer != "" )
					{
						if ( 0 == nLineCount )
						{
							if ( fileDest.Open( strDest, "ab" ) == FALSE )
								bFileFail = TRUE;
						}

						if ( FALSE == bFileFail )
						{
							fileDest.WriteLine( strBuffer );
							nLineCount++;
						}
					}
				}
			}
		}

		if ( FALSE == bFileFail )
		{
			switch( arrayFiles[n].m_nFileType )
			{
			case 1:	results.m_nDelivery += nLineCount;	break;
			case 2:	results.m_nWrapper += nLineCount;	break;
			case 3:	results.m_nReturn += nLineCount;	break;
			case 4:	results.m_nSale += nLineCount;		break;
			case 5:	results.m_nAdjust += nLineCount;	break;
			case 6:	results.m_nClosing += nLineCount;	break;
			}

			CFileRemover FileRemover( strRenamedFilename );
		}
	}
}

/**********************************************************************/

void CUnitechBatchJob::FilterPluUploads()
{
	CString strFileSpec;
	strFileSpec.Format( "%s\\unitech\\pluxfer*.tmp", dbDatabase.GetFolderPathData( m_nDbIdx ) );

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strFileSpec );

	CStringArray arrayFilenames;

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDirectory() == TRUE )
			continue;

		if ( finder.IsDots() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();
		strFilename.MakeUpper();

		if ( strFilename.GetLength() == 27 )
			if ( strFilename.Left(8) == "PLUXFER_" )
				if ( strFilename.Right(4) == ".TMP" )
					if ( strFilename.Mid(16,1) == "_" )
						if ( ::TestNumeric( strFilename.Mid(8,8) ) == TRUE )
							if ( ::TestNumeric( strFilename.Mid(17,6) ) == TRUE )
								arrayFilenames.Add( strFilename );
	}

	CString strFolderPath;
	strFolderPath.Format( "%s\\unitech\\",
		dbDatabase.GetFolderPathData( m_nDbIdx ) );

	for ( int n = 0; n < arrayFilenames.GetSize(); n++ )
	{
		CString strFilename = arrayFilenames.GetAt(n);
		CString strFilePath = strFolderPath + strFilename;
		
		CString strRenamedFilePath = strFilePath;
		strRenamedFilePath.SetAt( strRenamedFilePath.GetLength() - 1, '2' );

		if ( rename( strFilePath, strRenamedFilePath ) != 0 )
			continue;

		CString strDest;
		strDest.Format( "%s\\pluxfer_%s.csv",
			dbDatabase.GetFolderPathData( m_nDbIdx ),
			(const char*) strFilename.Mid(8,15) );

		int nLineCount = 0;
		bool bFileFail = FALSE;

		//SCOPE TO AUTO CLOSE FILES
		{
			CSSFile fileSource;
			if ( fileSource.Open( strRenamedFilePath, "rb" ) == FALSE )
				bFileFail = TRUE;
			else
			{
				CSSFile fileDest;

				CString strBuffer;
				while ( ( fileSource.ReadString( strBuffer ) == TRUE ) && ( FALSE == bFileFail ) )
				{
					if ( strBuffer != "" )
					{
						if ( 0 == nLineCount )
						{
							if ( fileDest.Open( strDest, "wb", _SH_DENYWR ) == FALSE )
								bFileFail = TRUE;
						}

						if ( FALSE == bFileFail )
						{
							if ( 0 == nLineCount )
								fileDest.WriteLine( "PluNo,EcrText,Price1,Price2,Price3,DeptNo,TaxCode" );

							fileDest.WriteLine( strBuffer );
							nLineCount++;
						}
					}
				}
			}
		}

		if ( FALSE == bFileFail )
			CFileRemover FileRemover( strRenamedFilePath );
	}
}

/**********************************************************************/

bool CUnitechBatchJob::GetStatusMessage( CString& strMessage )
{
	bool bResult = TRUE;
	strMessage = "";
	
	switch( UnitechOptions.GetStatusType() )
	{
	case UNITECH_STATUS_TYPE_REGISTRY:
		bResult = GetStatusMessageRegistry( strMessage );
		break;

	case UNITECH_STATUS_TYPE_FILE:
		bResult = GetStatusMessageFile( strMessage);
		break;

	case UNITECH_STATUS_TYPE_NONE:
	default:
		bResult = TRUE;
		strMessage = "Batch job attempt completed.";
		break;
	}
	
	return bResult;
}

/**********************************************************************/

bool CUnitechBatchJob::GetStatusMessageRegistry( CString& strMessage )
{
	strMessage = "Batch job attempt completed.\n\n";

	CString strRegError = "Unable to read job status from registry.\n\n(Reason code ";

	HKEY hKey;
	LONG lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Unitech America Inc.\\PTComm Manager\\Settings", 0, KEY_READ, &hKey);
	bool bExistsAndSuccess (lRes == ERROR_SUCCESS);
	
	if ( lRes == ERROR_FILE_NOT_FOUND )
	{
		strMessage += strRegError + " 1)";
		return FALSE;
	}

	if ( lRes != ERROR_SUCCESS )
	{
		CString strCode;
		strCode.Format( " 2.%d)", lRes );
		strMessage += strRegError + strCode;
		return FALSE;
	}

	DWORD dwBatchDone, dwBatchError;
	GetDWORDRegKey(hKey, L"BatchDone", dwBatchDone, 999 );
	
	if ( 999 == dwBatchDone )
	{
		strMessage += strRegError + " 3)";
		return FALSE;
	}
	
	if ( 0 == dwBatchDone )
	{
		strMessage += "ERROR : Batch job incomplete.";
		return FALSE;
	}
	
	GetDWORDRegKey(hKey, L"BatchError", dwBatchError, 999 );

	if ( 999 == dwBatchError )
	{
		strMessage += strRegError + " 4)";
		return FALSE;
	}

	if ( 0 != dwBatchError )
	{
		strMessage += "ERROR : Batch job failed";
		return FALSE;
	}

	strMessage = "Batch job completed successfully";
	return TRUE;
}

/**********************************************************************/

LONG CUnitechBatchJob::GetDWORDRegKey(HKEY hKey, LPCWSTR szValueName, DWORD &nValue, DWORD nDefaultValue)
{
    nValue = nDefaultValue;
    DWORD dwBufferSize(sizeof(DWORD));
    DWORD nResult(0);
    LONG nError = ::RegQueryValueExW(hKey,
       szValueName,
        0,
        NULL,
        reinterpret_cast<LPBYTE>(&nResult),
        &dwBufferSize);
    if (ERROR_SUCCESS == nError)
    {
        nValue = nResult;
    }
    return nError;
}

/**********************************************************************/

bool CUnitechBatchJob::GetStatusMessageFile( CString& strMessage )
{
	strMessage = "Batch job attempt completed.\n\n";

	CString strFilename = UnitechOptions.GetFullStatusPath();

	if ( ::FileExists( strFilename ) == FALSE )
	{
		strMessage += "ERROR : Status file not found";
		return FALSE;
	}

	CSSFile fileStatus;
	if ( fileStatus.Open( strFilename, "rb" ) == FALSE )
	{
		strMessage += "ERROR : Unable to open status file";
		return FALSE;
	}

	CString strBuffer;
	bool bGotBatchDone = FALSE;
	while ( fileStatus.ReadString( strBuffer ) == TRUE )
	{
		strBuffer.MakeUpper();
		if ( strBuffer.Left(9) == "BATCHDONE" )
		{
			bGotBatchDone = TRUE;
			break;
		}
	}

	if ( FALSE == bGotBatchDone )
	{
		strMessage += "ERROR : No job result in status file";
		return FALSE;
	}

	strBuffer = strBuffer.Right( strBuffer.GetLength() - 10 );

	if ( strBuffer== "FAIL" )
	{
		strMessage += "ERROR : Status file reports job failed";
		return FALSE;
	}

	if ( strBuffer== "" )
	{
		strMessage += "ERROR : Status file reports job not started.\n\n";
		strMessage += "Check that device is in cradle.\n\n";
		strMessage += "Check that cradle is connected to PC.\n\n";
		strMessage += "Check that correct COM Port is selected.";
		return FALSE;
	}

	if ( strBuffer == "SUCCESS" )
	{
		strMessage = "Batch job completed successfully";
		return TRUE;
	}

	strMessage += "ERROR : Unknown job status (";
	strMessage += strBuffer;
	strMessage += ")";

	return FALSE;
}

/**********************************************************************/

const char* CUnitechBatchJob::GetNetUseFolderPath( const char* szSource )
{
	m_strNetUseFolderPath = szSource;

	if ( 0 != m_nNetUseDrive )
	{
		int nRootLength = m_strNetUseRootPath.GetLength();
		int nPathLength = m_strNetUseFolderPath.GetLength();

		if ( ( nRootLength > 0 ) && ( nRootLength < nPathLength ) )
		{
			CString strTemp1 = m_strNetUseRootPath;
			CString strTemp2 = m_strNetUseFolderPath.Left( nRootLength );
			CString strTemp3 = m_strNetUseFolderPath.Right( nPathLength - nRootLength );
			
			strTemp1.MakeUpper();
			strTemp2.MakeUpper();

			if ( strTemp1 == strTemp2 )
			{
				m_strNetUseFolderPath = "";
				m_strNetUseFolderPath += "g:";
				m_strNetUseFolderPath += strTemp3;
				m_strNetUseFolderPath.SetAt( 0, 'f' + m_nNetUseDrive );
			}
		}
	}

	return m_strNetUseFolderPath;
}

/**********************************************************************/

void CUnitechBatchJob::CreateNetUseBatchFiles()
{
	if ( 0 != m_nNetUseDrive )
	{
		{
			CSSFile fileBatch;
			CFilenameUpdater FnUp( SysFiles::UnitechNetUseOn );
	
			CString strFilename = FnUp.GetFilenameToUse();
			strFilename.SetAt( strFilename.GetLength() - 3, 'b' );

			if ( fileBatch.Open( strFilename, "wb" ) == TRUE )
			{
				CString str = "";
				str += "net use g: ";
				str += "\x022";
				str += m_strNetUseRootPath;
				str += "\x022";
				str.SetAt( 8, 'f' + m_nNetUseDrive );
				fileBatch.WriteLine( str );
			}
		}

		{
			CSSFile fileBatch;
			CFilenameUpdater FnUp( SysFiles::UnitechNetUseOff );
			
			CString strFilename = FnUp.GetFilenameToUse();
			strFilename.SetAt( strFilename.GetLength() - 3, 'b' );

			if ( fileBatch.Open( strFilename, "wb" ) == TRUE )
			{
				CString str = "";
				str += "net use g: ";
				str += "/delete /y";
				str.SetAt( 8, 'f' + m_nNetUseDrive );
				fileBatch.WriteLine( str );
			}
		}
	}
}

/**********************************************************************/

void CUnitechBatchJob::NetUseOn()
{
	if ( 0 != m_nNetUseDrive )
	{
		CString strQuote = "\x022";
		CFilenameUpdater FnUp( SysFiles::UnitechNetUseOn );
		CString strFilename = FnUp.GetFilenameToUse();
		strFilename.SetAt( strFilename.GetLength() - 3, 'b' );
		system( strQuote + strFilename + strQuote );
	}
}

/**********************************************************************/

void CUnitechBatchJob::NetUseOff()
{
	if ( 0 != m_nNetUseDrive )
	{
		CString strQuote = "\x022";
		CFilenameUpdater FnUp( SysFiles::UnitechNetUseOff );
		CString strFilename = FnUp.GetFilenameToUse();
		strFilename.SetAt( strFilename.GetLength() - 3, 'b' );
		system( strQuote + strFilename + strQuote );
	}
}

/**********************************************************************/
