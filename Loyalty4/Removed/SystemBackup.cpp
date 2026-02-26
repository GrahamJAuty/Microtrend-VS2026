/**********************************************************************/
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
/**********************************************************************/
#include "AutoBackupProcessor.h"
#include "BackupErrors.h"
#include "BackupTypeDlg.h"
#include "ListDataRestoreDlg.h"
#include "SQLTranScriptImport.h"
/**********************************************************************/
#include "SystemBackup.h"
/**********************************************************************/
extern const char* szVERSION_LOYALTY_ABOUT;
/**********************************************************************/

CSystemBackup::CSystemBackup(bool bBackup, bool bRestoreToFolder, bool bFromScript, CLockManagerInfo& infoLock)
{
	m_bAuto = FALSE;
	m_nFilesPerMiniZip = 1000;
	m_bRestoreToFolder = (FALSE == bBackup) && (TRUE == bRestoreToFolder);
	m_bFromScript = (FALSE == bBackup) && (TRUE == bFromScript);

	m_strFolderPathSystemData = Sysset.GetProgramFolder(SYSSET_LOYALTY4);
	m_strFolderPathRestoreSave = m_strFolderPathSystemData + ".bak";
	m_strFolderPathZipTemp = m_strFolderPathSystemData + "$$$$$";

	//INFORMATION FOR THE CLIENT FUNCTION
	m_bComplete = FALSE;
	m_bTerminatedLoyalty = FALSE;
	m_strOnScreenError = "";

	//USE VERSION NUMBER TO MAKE FILENAME
	m_BackupFilter.LoadRules();
	SetBackupFilename(bBackup);

	if (bBackup == TRUE)
	{
		BackupFiles();
	}
	else
	{
		RestoreFiles(infoLock);
	}
}

/**********************************************************************/

CSystemBackup::CSystemBackup( CString& strFilePath )
{
	m_bAuto = TRUE;
	m_nFilesPerMiniZip = 1000;
	m_bRestoreToFolder = FALSE;
	m_bFromScript = FALSE;

	m_strFolderPathSystemData = Sysset.GetProgramFolder( SYSSET_LOYALTY4 );
	
	m_strFolderPathRestoreSave = m_strFolderPathSystemData + ".bak";
	m_strFolderPathZipTemp = m_strFolderPathSystemData + "$$$$$";

	//INFORMATION FOR THE CLIENT FUNCTION
	m_bComplete = FALSE;
	m_bTerminatedLoyalty = FALSE;
	m_strOnScreenError = "";

	//USE VERSION NUMBER TO MAKE FILENAME
	m_BackupFilter.LoadRules();
	SetBackupFilename( TRUE );

	m_strBackupFilter = "";
	CString strFileName = m_strBackupFilename + ".zip"; 

	strFilePath += "\\";
	strFilePath += strFileName;
	
	BackupFiles( strFileName, strFilePath ); 
}

/**********************************************************************/

void CSystemBackup::SetBackupFilename( bool bBackup )
{
	CString strVersion = szVERSION_LOYALTY_ABOUT;
	
	CString strNumber = "000";

	int nLength = strVersion.GetLength();
	
	if ( nLength >= 4 ) //VX.YY
	{
		CString strSubVer = strVersion.Mid(1,4);

		if (::TestNumeric(strSubVer.Left(1)) == TRUE)
		{
			if (strSubVer.Mid(1, 1) == ".")
			{
				if (::TestNumeric(strSubVer.Right(2)) == TRUE)
				{
					strNumber = strSubVer.Left(1) + strSubVer.Right(2);
				}
			}
		}
	}

	CString strBeta = "";
	System.GetBetaTypeForBackup( strBeta );

	CString strType = "System";
	CString strFirstLetter = "b";
	if ( TRUE == bBackup )
	{
		if ( m_BackupFilter.GotRules() )
		{
			strFirstLetter = "x";
			strType = "Partial";
		}
	}
	

	m_strBackupFilename.Format ( "%s%2.2d%s%s",
		(const char*) strFirstLetter,
		SYSSET_LOYALTY4, 
		(const char*) strNumber,
		(const char*) strBeta );

	m_strBackupFilter.Format ( "%s Backup Files (%s.zip)|%s.zip||",
		(const char*) strType,
		(const char*) m_strBackupFilename,
		(const char*) m_strBackupFilename );

	m_strRestoreFilter.Format ( "System Backup Files (%s*.zip)|%s*.zip||",
		(const char*) m_strBackupFilename.Left(3),
		(const char*) m_strBackupFilename.Left(3) );
}

/**********************************************************************/

void CSystemBackup::LogBackupMessage ( const char* szFilename, int nContext, int nFamily, int nError )
{
	CString strMsg;

	CString strAuto = ( m_bAuto ) ? "Automatic " : "";
	
	if (nContext != ERROR_CONTEXT_NONE)
	{
		strMsg.Format("<..>%sBackup failed (%s) (%d:%d:%d)", (const char*)strAuto, szFilename, nContext, nFamily, nError);
	}
	else
	{
		strMsg.Format("<..>%sBackup complete (%s)", (const char*)strAuto, szFilename);
	}

	LogBackupMessage ( strMsg );
}

/**********************************************************************/

void CSystemBackup::LogRestoreMessage ( const char* szFilename, int nContext, int nFamily, int nError )
{
	CString strMsg;

	CString strToFolder = ( m_bRestoreToFolder ) ? " to folder " : " ";

	if (nContext != ERROR_CONTEXT_NONE)
	{
		strMsg.Format("<..>Restore%sfailed (%s) (%d:%d:%d)", (const char*)strToFolder, szFilename, nContext, nFamily, nError);
	}
	else
	{
		strMsg.Format("<..>Restore%scomplete (%s)", (const char*)strToFolder, szFilename);
	}

	LogBackupMessage ( strMsg );
}

/**********************************************************************/

const char* CSystemBackup::GetMiniZipFilename( int n )
{
	m_strMiniZipFilename.Format ( "%s\\bak%5.5d.zip", (const char*) m_strFolderPathZipTemp, n );
	return m_strMiniZipFilename;
}

/**********************************************************************/

const char* CSystemBackup::GetMiniZipExtendedFilename( int n )
{
	m_strMiniZipExtendedFilename.Format ( "%s\\bak%5.5d_%5.5d.zip", 
		(const char*) m_strFolderPathZipTemp, 
		n,
		m_arrayFilenames.GetSize() );
	
	return m_strMiniZipExtendedFilename;
}

/**********************************************************************/

bool CSystemBackup::BackupCallback ( DWORD dwFileSize, int nBytesRead, void *pData) 
{
	CSystemBackup* pSystemBackup = (CSystemBackup*) pData;

	__int64 nTotalFileSize = pSystemBackup -> m_nTotalFileSize;
	__int64 nProcessedSoFar = pSystemBackup -> m_nProcessedSoFar + nBytesRead;

	if (nTotalFileSize != 0)
	{
		pSystemBackup->m_BackupInfoDlg.SetPos(FALSE, nProcessedSoFar, nTotalFileSize, 10, 90);
	}

	return TRUE;
}

/**********************************************************************/

bool CSystemBackup::ChangeDisk( DWORD dwNumber, int iCode, void* pData)
{
	CSystemBackup* pSystemBackup = (CSystemBackup*) pData;

	switch ( iCode )
	{
	case -1:
		{
			CString strMsg;
			strMsg.Format ( "Please insert disk number %d from your backup set.", dwNumber ); 
			
			if (Prompter.OkCancel(strMsg) != IDOK)
			{
				return FALSE;
			}

			while ( TRUE )
			{	
				DWORD dwFlags;
				DWORD dwComponentLength;
				char cVolumeBuffer [ 100 ];
				
				UINT nErrorMode = SetErrorMode ( SEM_FAILCRITICALERRORS );
				int nResult = GetVolumeInformation ( pSystemBackup -> m_strDrive, cVolumeBuffer, 100, NULL, &dwComponentLength, &dwFlags, NULL, 0 );
				SetErrorMode ( nErrorMode );

				if ( nResult != 0 )
				{
					CString strVolumeName = cVolumeBuffer;
					strVolumeName.MakeUpper();

					CString strExpectedVolumeName;
					strExpectedVolumeName.Format ( "PKBACK# %3.3d", dwNumber );
				
					if (strVolumeName == strExpectedVolumeName)
					{
						return TRUE;
					}
				}
				
				CString strNewMsg;
				strNewMsg = "Incorrect disk or no disk inserted\n\n";
				strNewMsg += strMsg;

				if (Prompter.OkCancel(strNewMsg) == IDCANCEL)
				{
					return FALSE;
				}
			}
		}
		break;

	case -2:
		{
			CString strMsg;
			strMsg.Format("An existing archive file was found on this disk\n\nPlease insert another disk");

			if (Prompter.OkCancel(strMsg) == IDOK)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		break;

	case -3:
		{
			CString strMsg;
			strMsg.Format ( "The disk appears to be write protected\n\nPlease insert another disk" );

			if (Prompter.OkCancel(strMsg) == IDOK)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		break;

	case -4:
		//UNABLE TO CREATE FILE SO GIVE UP IMMEDIATELY
		return FALSE;

	default:
		if (iCode >= 0)
		{
			CString strMsg;
			strMsg.Format("Please insert disk %d to continue your backup", dwNumber);

			if (Prompter.OkCancel(strMsg) == IDOK)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
}

/**********************************************************************/

void CSystemBackup::OpenArchive ( int& nErrorFamily, int& nErrorNumber, const char* szPath, int nMode, int nVolumeSize )
{
	nErrorFamily = ERROR_FAMILY_NONE;
	nErrorNumber = 0;

	try
	{
		m_ZipArchive.Open ( szPath, nMode, nVolumeSize ); 
	}
	catch (CZipException* e)
	{
		nErrorFamily = ERROR_FAMILY_ZIPARCHIVE;
		nErrorNumber = e -> m_iCause;
		e->Delete();
	}
	catch ( CFileException* e )
	{
		nErrorFamily = ERROR_FAMILY_CFILEEXCEPTION;
		nErrorNumber = 1;
		e->Delete();
	}
	catch ( CMemoryException* e )
	{
		nErrorFamily = ERROR_FAMILY_CMEMORYEXCEPTION;
		nErrorNumber = 1;
		e->Delete();
	}
	catch (CException* e)
	{
		nErrorFamily = ERROR_FAMILY_CEXCEPTION;
		nErrorNumber = 1;
		e->Delete();		
	}
}

/**********************************************************************/
	
void CSystemBackup::CloseArchive ( int& nErrorFamily, int& nErrorNumber )
{
	nErrorFamily = ERROR_FAMILY_NONE;
	nErrorNumber = 0;

	try
	{
		m_ZipArchive.Close();
	}
	catch (CZipException* e)
	{
		nErrorFamily = ERROR_FAMILY_ZIPARCHIVE;
		nErrorNumber = e -> m_iCause;
		e->Delete();
	}
	catch ( CFileException* e )
	{
		nErrorFamily = ERROR_FAMILY_CFILEEXCEPTION;
		nErrorNumber = 1;
		e->Delete();
	}
	catch ( CMemoryException* e )
	{
		nErrorFamily = ERROR_FAMILY_CMEMORYEXCEPTION;
		nErrorNumber = 1;
		e->Delete();
	}
	catch (CException* e)
	{
		nErrorFamily = ERROR_FAMILY_CEXCEPTION;
		nErrorNumber = 1;
		e->Delete();
	}
}

/**********************************************************************/

void CSystemBackup::AddNewFile ( int& nErrorFamily, int& nErrorNumber, const char* szPath )
{
	nErrorFamily = 0;
	nErrorNumber = 0;

	try
	{
		if ( m_ZipArchive.AddNewFile ( szPath, Z_DEFAULT_COMPRESSION, TRUE, CSystemBackup::BackupCallback, this ) == FALSE )
		{
			nErrorFamily = ERROR_FAMILY_MICROTREND;
			nErrorNumber = MICROTREND_ERROR_ADDFILE;
		}
	}
	catch (CZipException* e)
	{
		nErrorFamily = ERROR_FAMILY_ZIPARCHIVE;
		nErrorNumber = e -> m_iCause;
		e->Delete();
	}
	catch ( CFileException* e )
	{
		nErrorFamily = ERROR_FAMILY_CFILEEXCEPTION;
		nErrorNumber = 1;
		e->Delete();
	}
	catch ( CMemoryException* e )
	{
		nErrorFamily = ERROR_FAMILY_CMEMORYEXCEPTION;
		nErrorNumber = 1;
		e->Delete();
	}
	catch (CException* e)
	{
		nErrorFamily = ERROR_FAMILY_CEXCEPTION;
		nErrorNumber = 1;
		e->Delete();
	}
}

/**********************************************************************/

void CSystemBackup::ExtractFile ( int& nErrorFamily, int& nErrorNumber, int nIndex )
{
	nErrorFamily = 0;
	nErrorNumber = 0;

	try
	{
		if ( m_ZipArchive.ExtractFile ( nIndex, ".\\", TRUE, NULL, CSystemBackup::BackupCallback, this ) == FALSE )
		{
			nErrorFamily = ERROR_FAMILY_MICROTREND;
			nErrorNumber = MICROTREND_ERROR_EXTRACTFILE;
		}
	}
	catch (CZipException* e)
	{
		nErrorFamily = ERROR_FAMILY_ZIPARCHIVE;
		nErrorNumber = e -> m_iCause;
		e->Delete();
	}
	catch ( CFileException* e )
	{
		nErrorFamily = ERROR_FAMILY_CFILEEXCEPTION;
		nErrorNumber = 1;
		e->Delete();
	}
	catch ( CMemoryException* e )
	{
		nErrorFamily = ERROR_FAMILY_CMEMORYEXCEPTION;
		nErrorNumber = 1;
		e->Delete();
	}
	catch (CException* e)
	{
		nErrorFamily = ERROR_FAMILY_CEXCEPTION;
		nErrorNumber = 1;
		e->Delete();
	}
}

/**********************************************************************/

const char* CSystemBackup::GetDefaultBackupFolder()
{
	m_strDefaultBackupFolder = "";
	GetDataProgramPath(m_strDefaultBackupFolder);

	CString strFileToCheck = "sys001.dat";

	CSSFile fileCheck;
	if (fileCheck.Open(strFileToCheck, "rb"))
	{
		CString strBuffer;
		fileCheck.ReadString(strBuffer);

		if (::ExistSubdirectory(strBuffer) == TRUE)
		{
			m_strDefaultBackupFolder = strBuffer;
		}
	}

	return m_strDefaultBackupFolder;
}

/**********************************************************************/

void CSystemBackup::SetDefaultBackupFolder(const char* sz)
{
	CString strFileCheck = "sys001.dat";

	CSSFile fileCheck;
	if (fileCheck.Open(strFileCheck, "wb"))
	{
		fileCheck.WriteLine(sz);
	}
}

/**********************************************************************/

void CSystemBackup::BackupFiles()
{
	CString strFileName = "";
	CString strPathName = "";

	if ( AutoBackupOptions.GetAutoBackupType() != AUTOBACKUP_TYPE_NONE )
	{
		CBackupTypeDlg dlgType ( NULL );
		
		if ( dlgType.DoModal() == IDCANCEL )
		{
			m_strOnScreenError = "The backup was abandoned";
			return;
		}
	
		if ( 0 == dlgType.m_nType )
		{
			strFileName = m_strBackupFilename + ".zip";
			strPathName = AutoBackupOptions.GetPath();
			if ( "" != strPathName ) strPathName += "\\";

			COleDateTime timeNow = COleDateTime::GetCurrentTime();

			CString strTime;
			strTime.Format( "%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
				timeNow.GetYear(),
				timeNow.GetMonth(),
				timeNow.GetDay(),
				timeNow.GetHour(),
				timeNow.GetMinute(),
				timeNow.GetSecond() );

			strPathName += strTime;
			::CreateSubdirectory( strPathName );

			strPathName += "\\";
			strPathName += strFileName;

			m_bAuto = TRUE;
		}
	}
	
	if ( "" == strFileName )
	{
		CFileDialog dlgFile ( FALSE, "zip", m_strBackupFilename + ".zip", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, m_strBackupFilter, NULL );
		dlgFile.m_ofn.lpstrTitle = "Backup System";

		dlgFile.m_ofn.lpstrInitialDir = GetDefaultBackupFolder();
	
		if ( dlgFile.DoModal() == IDCANCEL )
		{
			m_strOnScreenError = "The backup was abandoned";
			return;
		}

		strFileName = dlgFile.GetFileName();
		strPathName = dlgFile.GetPathName();
	}

	BackupFiles( strFileName, strPathName );
}

/**********************************************************************/

void CSystemBackup::BackupFiles(const char* szFileName, const char* szPathName)
{
	//MAKE SURE THE USER HAS NOT CHANGED THE FILENAME
	CString strUserFileName = szFileName;
	CString strUserFilePath = szPathName;
	CString strGoodFileName = m_strBackupFilename + ".zip";

	strUserFileName.MakeLower();
	strUserFilePath.MakeLower();
	strGoodFileName.MakeLower();

	if (strUserFileName != strGoodFileName)
	{
		LogBackupMessage(strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_BADNAME);
		m_strOnScreenError = "Invalid backup filename selected";
		return;
	}

	//GIVE UP IF THE BACKUP FOLDER DOES NOT EXIST
	CString strUserPathName = strUserFilePath.Left(strUserFilePath.GetLength() - strGoodFileName.GetLength());
	if (::ExistSubdirectory(strUserPathName) == FALSE)
	{
		LogBackupMessage(strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_NOTEXIST);
		m_strOnScreenError = "Unable to locate backup folder";
		return;
	}

	char drive[_MAX_DRIVE];
	_splitpath_s(strUserPathName, drive, _MAX_DRIVE, NULL, 0, NULL, 0, NULL, 0);
	m_strDrive = drive;
	m_strDrive += "\\";

	int nResult = GetDriveType(m_strDrive);
	bool bSpanning = FALSE;

	switch (nResult)
	{
	case DRIVE_CDROM:
	case DRIVE_FIXED:
	case DRIVE_REMOTE:
		bSpanning = FALSE;
		break;

	case DRIVE_REMOVABLE:
		if (FALSE == m_bAuto)
			bSpanning = TRUE;
		break;

	case DRIVE_NO_ROOT_DIR:
		LogBackupMessage(strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_NOROOT);
		m_strOnScreenError = "Unable to find the backup root folder";
		return;

	case DRIVE_RAMDISK:
		LogBackupMessage(strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_RAMDISK);
		m_strOnScreenError = "Unable to backup to a RAM disk";
		return;

	case DRIVE_UNKNOWN:
	default:
		LogBackupMessage(strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_DRIVETYPE);
		m_strOnScreenError = "Unable to identify the backup drive type";
		return;
	}

	m_strDrive.MakeLower();
	strUserPathName.MakeLower();

	if (m_strDrive != strUserPathName)
		bSpanning = FALSE;

	if (bSpanning == TRUE)
	{
		DWORD dwSectorsPerCluster;
		DWORD dwBytesPerSector;
		DWORD dwNumberOfFreeClusters;
		DWORD dwTotalNumberOfClusters;

		if (GetDiskFreeSpace(m_strDrive, &dwSectorsPerCluster, &dwBytesPerSector, &dwNumberOfFreeClusters, &dwTotalNumberOfClusters) == FALSE)
		{
			LogBackupMessage(strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_DRIVESIZE);
			m_strOnScreenError = "Unable to confirm the backup drive size";
			return;
		}

		__int64 nTotalBytes = (dwBytesPerSector * dwSectorsPerCluster * dwTotalNumberOfClusters);
		__int64 nFloppyApprox = 2097152;

		if (nTotalBytes > nFloppyApprox)
		{
			bSpanning = FALSE;
		}
	}

	if ((bSpanning == FALSE) && (::FileExists(strUserFilePath) == TRUE))
	{
		CString strMsg = "An existing backup file was found in the specified folder.\n\n";
		strMsg += "Are you sure you wish to overwrite it.";

		if (Prompter.YesNo(strMsg) != IDYES)
		{
			m_strOnScreenError = "The backup was abandoned";
			return;
		}
	}

	//CREATE DIALOG TO SHOW PROGRESS AND INFO
	m_BackupInfoDlg.CreateOnMainWnd(FALSE);
	CFileRemoverBackup FileRemover(&m_BackupInfoDlg);

	//ATTEMPT TO REMOVE TEMPORARY ZIP FOLDER
	FileRemover.RemoveFolder(m_strFolderPathZipTemp);
	if (::ExistSubdirectory(m_strFolderPathZipTemp) == TRUE)
	{
		LogBackupMessage(strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_REMOVEFOLDER2);
		m_strOnScreenError = "Unable to rename a previous temporary folder";
		return;
	}

	//ATTEMPT TO RECREATE TEMPORARY ZIP FOLDER
	if (::MakeSubdirectory(m_strFolderPathZipTemp) != 0)
	{
		LogBackupMessage(strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_MAKEFOLDER);
		m_strOnScreenError = "Unable to create temporary folder";
		return;
	}

	CString strDest = m_strFolderPathSystemData + "\\backup.001";
	CString strSource = "backup.log";

	CopyFile(strSource, strDest, FALSE);

	__int64 nFilesSoFar = 0;
	__int64 nFilesToBackup = 0;
	CountFilesToBackup(m_strFolderPathSystemData + "\\", nFilesToBackup);

	//CREATE INTERMEDIATE ZIP FILES
	m_nNextZipFileNo = 1;
	m_nTotalFileSize = 0;
	m_arrayFilenames.RemoveAll();

	if (CreateMiniZipFiles(m_strFolderPathSystemData + "\\", nFilesSoFar, nFilesToBackup) == FALSE)
	{
		return;
	}

	//TIDY UP ANY LEFT OVER FILES
	if (m_arrayFilenames.GetSize() > 0)
	{
		CString strZipFilename = GetMiniZipFilename(m_nNextZipFileNo);
		if (CreateZipFile(strZipFilename, FALSE, nFilesSoFar, nFilesToBackup, TRUE) == FALSE)
		{
			return;
		}

		m_nNextZipFileNo++;
	}

	m_nTotalFileSize = 0;
	m_arrayFilenames.RemoveAll();

	for (int n = 1; n < m_nNextZipFileNo; n++)
	{
		CString strIntermediateBackupFilename = GetMiniZipFilename(n);

		if (::FileExists(strIntermediateBackupFilename) == TRUE)
		{
			CFileStatus FileStatus;
			CFile::GetStatus(strIntermediateBackupFilename, FileStatus);
			m_nTotalFileSize += FileStatus.m_size;
			m_arrayFilenames.Add(strIntermediateBackupFilename);
		}
	}

	m_BackupInfoDlg.SetText(FALSE, BACKUP_PROGRESS_BACKUP, m_nNextZipFileNo);

	if (FALSE == m_bAuto)
	{
		if (CreateZipFile(strUserFilePath, bSpanning, nFilesSoFar, nFilesToBackup, FALSE) == FALSE)
		{
			return;
		}
	}
	else
	{
		int nFileNumber = 1;
		int nFileListPos = 0;
		int nResult = 0;

		do
		{
			nResult = CreateZipFile(strUserFilePath, nFileNumber++, nFileListPos);
		} 
		while (nResult == 1);

		if (nResult != 0)
		{
			return;
		}
	}

	LogBackupMessage(strUserFilePath, 0, 0, 0);
	m_bComplete = TRUE;

	//ATTEMPT TO REMOVE TEMPORARY ZIP FOLDER
	FileRemover.RemoveFolder(m_strFolderPathZipTemp);

	if (FALSE == m_bAuto)
	{
		SetDefaultBackupFolder(strUserPathName);
	}
}

/**********************************************************************/
//return 0 for OK
//return 1 for Not Finished
//return 2 for Error
/**********************************************************************/

int CSystemBackup::CreateZipFile( CString& strBaseZipFilename, int nFileNumber, int& nFileListPos )
{	
	CString strZipFilenameToUse = strBaseZipFilename;

	if ( 1 == nFileNumber )
		m_nProcessedSoFar = 0;
	else
	{	
		strZipFilenameToUse.Format( "%s.%3.3d",
			(const char*) strBaseZipFilename.Left( strBaseZipFilename.GetLength() - 4 ),
			nFileNumber );
	}
		
	int nErrorFamily, nErrorNumber; 
	m_ZipArchive.SetSpanCallback(CSystemBackup::ChangeDisk, this);
	OpenArchive ( nErrorFamily, nErrorNumber, strZipFilenameToUse, CZipArchive::zipCreate, -1 );
	if ( nErrorFamily != ERROR_FAMILY_NONE )
	{
		LogBackupMessage ( strZipFilenameToUse, ERROR_CONTEXT_ZIP_CREATE_NOSPAN, nErrorFamily, nErrorNumber );
		m_strOnScreenError = "Unable to create the backup file";
		return 2;
	}
	
	__int64 nProcessedThisFile = 0;

	for ( ; nFileListPos < m_arrayFilenames.GetSize(); nFileListPos++ )
	{
		if ( nProcessedThisFile >= 0x40000000 )
			break;

		CString strFilenameOld = GetMiniZipFilename( nFileListPos + 1 );
		CString strFilenameNew = GetMiniZipExtendedFilename( nFileListPos + 1 );
		::rename( strFilenameOld, strFilenameNew );

		CFileStatus FileStatus;
		CFile::GetStatus ( strFilenameNew, FileStatus );
	
		int nErrorFamily, nErrorNumber; 
		AddNewFile ( nErrorFamily, nErrorNumber, strFilenameNew );
		m_nProcessedSoFar += FileStatus.m_size;
		
		if ( nErrorFamily != ERROR_FAMILY_NONE )
		{
			LogBackupMessage ( strZipFilenameToUse, ERROR_CONTEXT_ZIP_ADD, nErrorFamily, nErrorNumber );
			
			int nErrorFamily, nErrorNumber; 
			CloseArchive ( nErrorFamily, nErrorNumber );
			if (nErrorFamily != 0)
			{
				LogBackupMessage(strZipFilenameToUse, ERROR_CONTEXT_ZIP_CLOSE, nErrorFamily, nErrorNumber);
			}

			CString strMsg = "<..>Backup failed on file ";
			strMsg += strFilenameNew;
			LogBackupMessage ( strMsg );

			m_strOnScreenError = "Backup failed on file ";
			m_strOnScreenError += strFilenameNew;
			return 2;
		}

		nProcessedThisFile += FileStatus.m_size;
	}
	
	CloseArchive ( nErrorFamily, nErrorNumber );
	if ( nErrorFamily != ERROR_FAMILY_NONE )
	{
		LogBackupMessage ( strZipFilenameToUse, ERROR_CONTEXT_ZIP_CLOSE, nErrorFamily, nErrorNumber );
		m_strOnScreenError = "Unable to close the backup file";
		return 2;
	}

	if (nFileListPos >= m_arrayFilenames.GetSize())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**********************************************************************/

void CSystemBackup::CountFilesToBackup( const char* szLocalPath, __int64& nCountSoFar )
{
	CFileFind FileFinder;	
	CString strLocalPath = szLocalPath;
	bool bWorking = ( FileFinder.FindFile ( strLocalPath + "*.*" ) != 0 );

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		CString strFilename = FileFinder.GetFileName();
		CString strFilePath = strLocalPath + strFilename;

		if (FileFinder.IsDirectory() == TRUE)
		{
			bool bIsTempFolder = FALSE;
			if (strFilename.GetLength() == 6)
			{
				strFilename.MakeUpper();
				if (strFilename.Left(4) == "TEMP")
				{
					if (::TestNumeric(strFilename.Right(2)) == TRUE)
					{
						bIsTempFolder = TRUE;
					}
				}
			}

			if (bIsTempFolder == FALSE)
			{
				CountFilesToBackup(strFilePath + "\\", nCountSoFar);
			}
		}
		else
		{
			if ( GetFileIncludeType( strLocalPath, strFilename ) == BACKUP_FILE_INCLUDE_YES )
			{
				nCountSoFar++;
				m_BackupInfoDlg.SetText( TRUE, BACKUP_PROGRESS_COUNTFILE, nCountSoFar );
			}
		}
	}
}

/**********************************************************************/

bool CSystemBackup::CreateMiniZipFiles ( const char* szLocalPath, __int64& nFilesSoFar, __int64& nFilesToBackup )
{
	m_BackupInfoDlg.SetText ( FALSE, BACKUP_PROGRESS_BACKUP, m_nNextZipFileNo  );
	m_BackupInfoDlg.SetText ( TRUE, BACKUP_PROGRESS_BACKUPSOFAR, nFilesSoFar, nFilesToBackup  );
	
	CString strLocalPath = szLocalPath;
	
	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strLocalPath + "*.*" ) != 0 );

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		CString strFilename = FileFinder.GetFileName();
		CString strFilePath = strLocalPath + strFilename;

		if ( FileFinder.IsDirectory() == TRUE )
		{
			bool bIsTempFolder = FALSE;
			if ( strFilename.GetLength() == 6 )
			{
				strFilename.MakeUpper();
				if ( strFilename.Left(4) == "TEMP" )
					if ( ::TestNumeric( strFilename.Right(2) ) == TRUE )
						bIsTempFolder = TRUE;
			}

			if ( bIsTempFolder == FALSE )
				if ( CreateMiniZipFiles ( strFilePath + "\\", nFilesSoFar, nFilesToBackup ) == FALSE )
					return FALSE;
		}
		else
		{
			switch ( GetFileIncludeType( strLocalPath, strFilename ) )
			{
			case BACKUP_FILE_INCLUDE_YES:
				{
					CFileStatus FileStatus;
					CFile::GetStatus ( strFilePath, FileStatus );

					if ( ( FileStatus.m_size != 0 ) || ( AutoBackupOptions.GetExcludeZeroSizeFilesFlag() == FALSE ) )
					{
						m_nTotalFileSize += FileStatus.m_size;
						m_arrayFilenames.Add ( strFilePath );
					}
					else
						m_arrayFilenames.Add( "" );

					m_BackupInfoDlg.SetPos( FALSE, m_arrayFilenames.GetSize(), m_nFilesPerMiniZip, 0, 10 );
				}
				break;

			case BACKUP_FILE_INCLUDE_NO:
				break;

			case BACKUP_FILE_INCLUDE_ABORT:
			default:
				{
					CString strZipFilename = GetMiniZipFilename( m_nNextZipFileNo );
					LogBackupMessage ( strZipFilename, ERROR_CONTEXT_ZIP_ADD, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_ILLEGAL_FILE );

					CString strFailFile = "";
					strFailFile += strLocalPath;
					strFailFile += "\\";
					strFailFile += strFilename;

					CString strMsg = "<..>Backup failed on lock file ";
					strMsg += strFailFile;
					LogBackupMessage ( strMsg );

					m_strOnScreenError = "The backup was prevented by a lock file ";
					m_strOnScreenError += strFailFile;
					
					return FALSE;
				}
				break;
			}
		}

		if ( m_arrayFilenames.GetSize() >= m_nFilesPerMiniZip )
		{
			CString strZipFilename = GetMiniZipFilename( m_nNextZipFileNo );
			if ( CreateZipFile( strZipFilename, FALSE, nFilesSoFar, nFilesToBackup, TRUE ) == FALSE )
				return FALSE;

			m_arrayFilenames.RemoveAll();
			m_nTotalFileSize = 0;
			m_nNextZipFileNo++;
		
			m_BackupInfoDlg.SetText ( FALSE, BACKUP_PROGRESS_BACKUP, m_nNextZipFileNo  );
			m_BackupInfoDlg.SetPos( FALSE, 0, m_nFilesPerMiniZip, 0, 10 );
		}
	}

	return TRUE;
}

/**********************************************************************/

int CSystemBackup::GetFileIncludeType( const char* szLocalPath, const char* szFile )
{
	CString strFile = szFile;
	strFile.MakeUpper();

	CString strPath = szLocalPath;
	strPath.MakeUpper();

	int nPathLen = strPath.GetLength();

	if (nPathLen > 9)
	{
		if (strPath.Right(9) == "SYSFILES\\")
		{
			if ((strFile == "LOY.RUN") || (strFile == "LOY.NO") || (strFile == "PFILE.RUN") || (strFile == "REQ.010") || (strFile == "PFILE004.DAT"))
			{
				return BACKUP_FILE_INCLUDE_NO;
			}
		}
	}

	if (m_BackupFilter.CheckFile(strPath, strFile) == TRUE)
	{
		return BACKUP_FILE_INCLUDE_YES;
	}
	else
	{
		return BACKUP_FILE_INCLUDE_NO;
	}
}

/**********************************************************************/

bool CSystemBackup::CreateZipFile( CString& strZipFilename, bool bSpanning, __int64& nFilesSoFar, __int64& nFilesToBackup, bool bShowProgress )
{			
	//CREATE THE APPROPRIATE TYPE OF ZIP ARCHIVE
	if ( bSpanning == TRUE )
	{
		int nErrorFamily, nErrorNumber; 
		m_ZipArchive.SetSpanCallback(CSystemBackup::ChangeDisk, this);
		OpenArchive ( nErrorFamily, nErrorNumber, strZipFilename, CZipArchive::zipCreateSpan, -1 );
		if ( nErrorFamily != ERROR_FAMILY_NONE )
		{
			LogBackupMessage ( strZipFilename, ERROR_CONTEXT_ZIP_CREATE_SPAN, nErrorFamily, nErrorNumber );
			m_strOnScreenError = "Unable to create the backup file";
			return FALSE;
		}	
	}
	else
	{
		int nErrorFamily, nErrorNumber; 
		m_ZipArchive.SetSpanCallback(CSystemBackup::ChangeDisk, this);
		OpenArchive ( nErrorFamily, nErrorNumber, strZipFilename, CZipArchive::zipCreate, -1 );
		if ( nErrorFamily != ERROR_FAMILY_NONE )
		{
			LogBackupMessage ( strZipFilename, ERROR_CONTEXT_ZIP_CREATE_NOSPAN, nErrorFamily, nErrorNumber );
			m_strOnScreenError = "Unable to create the backup file";
			return FALSE;
		}
	}
	
	m_nProcessedSoFar = 0;
	for ( int nIndex = 0; nIndex < m_arrayFilenames.GetSize(); nIndex++ )
	{
		CString strFilename = m_arrayFilenames.GetAt ( nIndex );

		if ( strFilename != "" )
		{
			CFileStatus FileStatus;
			CFile::GetStatus ( strFilename, FileStatus );
		
			int nErrorFamily, nErrorNumber; 
			AddNewFile ( nErrorFamily, nErrorNumber, strFilename );
			m_nProcessedSoFar += FileStatus.m_size;
			
			if ( TRUE == bShowProgress )
			{
				nFilesSoFar++;
				m_BackupInfoDlg.SetPos( TRUE, nFilesSoFar, nFilesToBackup );
				m_BackupInfoDlg.SetText( TRUE, BACKUP_PROGRESS_BACKUPSOFAR, nFilesSoFar, nFilesToBackup );
			}
			
			if ( nErrorFamily != ERROR_FAMILY_NONE )
			{
				LogBackupMessage ( strZipFilename, ERROR_CONTEXT_ZIP_ADD, nErrorFamily, nErrorNumber );
				
				int nErrorFamily, nErrorNumber; 
				CloseArchive ( nErrorFamily, nErrorNumber );
				if ( nErrorFamily != 0 )
					LogBackupMessage ( strZipFilename, ERROR_CONTEXT_ZIP_CLOSE, nErrorFamily, nErrorNumber );

				CString strMsg = "<..>Backup failed on file ";
				strMsg += strFilename;
				LogBackupMessage ( strMsg );

				m_strOnScreenError = "Backup failed on file ";
				m_strOnScreenError += strFilename;
				return FALSE;
			}
		}
		else
		{
			if ( TRUE == bShowProgress )
			{
				nFilesSoFar++;
				m_BackupInfoDlg.SetPos( TRUE, nFilesSoFar, nFilesToBackup );
				m_BackupInfoDlg.SetText( TRUE, BACKUP_PROGRESS_BACKUPSOFAR, nFilesSoFar, nFilesToBackup );
			}
		}
	}
	
	int nErrorFamily, nErrorNumber;
	CloseArchive ( nErrorFamily, nErrorNumber );
	if ( nErrorFamily != ERROR_FAMILY_NONE )
	{
		LogBackupMessage ( strZipFilename, ERROR_CONTEXT_ZIP_CLOSE, nErrorFamily, nErrorNumber );
		m_strOnScreenError = "Unable to close the backup file";
		return FALSE;
	}

	return TRUE;		
}

/**********************************************************************/

void CSystemBackup::CleanUpRestore( CFileRemoverBackup& FileRemover )
{
	if ( FALSE == m_bRestoreToFolder )
	{
		FileRemover.RemoveFolder ( m_strFolderPathSystemData );
		rename ( m_strFolderPathRestoreSave, m_strFolderPathSystemData );
	}
}

/**********************************************************************/

void CSystemBackup::RestoreFiles( CLockManagerInfo& infoLock )
{
	CString strUserFileName = "";
	CString strUserFilePath = "";

	if ( AutoBackupOptions.GetAutoBackupType() != AUTOBACKUP_TYPE_NONE )
	{
		CStringArray arrayBackups;
		AutoBackupProcessor.BuildAutoBackupList( arrayBackups );

		if ( arrayBackups.GetSize() != 0 )
		{
			CListDataRestoreDlg dlg ( arrayBackups, NULL );
			if ( dlg.DoModal() == IDCANCEL )
			{
				m_strOnScreenError = "The system restore has been abandoned\n\nYour system data has not been changed";
				return;
			}

			int nSel = dlg.GetListSelection();
			if ( ( nSel >= 0 ) && ( nSel < arrayBackups.GetSize() ) )
			{
				CCSV csv( arrayBackups.GetAt( nSel ) );

				CString strNum;
				strNum.Format( "%3.3d", csv.GetInt(3) );

				strUserFilePath = AutoBackupOptions.GetPath();
				if ( "" != strUserFilePath ) strUserFilePath += "\\";
				strUserFilePath += csv.GetString(0);
				strUserFilePath += "\\";
				strUserFilePath += m_strBackupFilename.Left(3);
				strUserFilePath += strNum;
				strUserFilePath += csv.GetString(5);
				strUserFilePath += ".ZIP";
				strUserFileName = strUserFilePath.Right(11);
				m_bAuto = TRUE;
			}
		}
	}

	//REQUEST BACKUP FOLDER NAME FROM WINDOWS FILE DIALOG
	if ( strUserFileName == "" )
	{
		CFileDialog dlgFile ( TRUE, "zip", m_strBackupFilename + ".zip", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, m_strRestoreFilter, NULL );
		dlgFile.m_ofn.lpstrTitle = "Restore System";
	
		dlgFile.m_ofn.lpstrInitialDir = GetDefaultBackupFolder();
	
		if ( dlgFile.DoModal() == IDCANCEL )
		{
			m_strOnScreenError = "The system restore has been abandoned\n\nYour system data has not been changed";
			return;
		}
	
		//MAKE SURE THE USER HAS NOT CHANGED THE FILENAME
		strUserFileName = dlgFile.GetFileName();
		strUserFilePath = dlgFile.GetPathName();
	}

	CString strGoodFileName = m_strBackupFilename + ".zip";
	
	strUserFileName.MakeLower();
	strUserFilePath.MakeLower();
	strGoodFileName.MakeLower();
	
	if ( strUserFileName.GetLength() != 11 || 
		strUserFileName.Right(4) != strGoodFileName.Right(4) || 
		strUserFileName.Left(3) != strGoodFileName.Left(3) ||
		strUserFileName.GetAt(3) < '0' || strUserFileName.GetAt(3) > '9' ||
		strUserFileName.GetAt(4) < '0' || strUserFileName.GetAt(4) > '9' ||
		strUserFileName.GetAt(5) < '0' || strUserFileName.GetAt(5) > '9' )
	{
		LogRestoreMessage( strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_BADNAME );
		m_strOnScreenError.Format ( "You must specify a %s file to restore from.", ( const char* ) strGoodFileName );
		return;
	}
	
	//HANDLE FILE NOT FOUND
	if ( ::FileExists ( strUserFilePath ) == FALSE )
	{
		LogRestoreMessage( strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_NOTEXIST );
		m_strOnScreenError = "The selected backup file was not found";
		return;
	}

	CString strAppName = AfxGetAppName();

	if ( strUserFileName != strGoodFileName )
	{
		CString strMsg = "";
		strMsg += "The backup file that you have selected appears to be from\n";
		strMsg += "a different version of ";
		strMsg += strAppName;
		strMsg += ".\n\n";
		strMsg += "Some of the data from this backup may not work correctly\n";
		strMsg += "with your system.\n\n";
		strMsg += "Are you sure you wish to restore this backup ?";
		
		if ( Prompter.YesNo ( strMsg ) != IDYES )
		{
			m_strOnScreenError = "The system restore has been abandoned\n\nYour system data has not been changed";
			return;
		}
	}

	CStringArray arrayWarning;
	
	{
		CString strTitle = "Restore ";
		strTitle += strAppName;
		strTitle += " System";
		arrayWarning.Add ( strTitle );
	}

	arrayWarning.Add ( "Your system will be restored from the selected backup file." );
	arrayWarning.Add ( "" );
	
	if (FALSE == m_bRestoreToFolder)
	{
		arrayWarning.Add("Your current system folder will be renamed.");
	}
	else
	{
		arrayWarning.Add("The restored files will be placed in a separate folder.");
	}

	arrayWarning.Add ( "" );

	{
		CString strLine = "";
		strLine += "You will need to restart ";
		strLine += strAppName;
		strLine += ".";
		arrayWarning.Add( strLine );
	}

	if ( Prompter.Warning( arrayWarning ) != IDYES )
	{
		m_strOnScreenError = "The system restore has been abandoned\n\nYour system data has not been changed";
		return;
	}

	m_bTerminatedLoyalty = TRUE;
	
	//CHECK THAT THE DRIVE TYPE IS VALID
	CString strUserPathName = strUserFilePath.Left ( strUserFilePath.GetLength() - strGoodFileName.GetLength() );
	
	char drive[_MAX_DRIVE];
	_splitpath_s ( strUserPathName, drive, _MAX_DRIVE, NULL, 0, NULL, 0, NULL, 0 );
	m_strDrive = drive;
	m_strDrive += "\\";

	int nResult = GetDriveType ( m_strDrive );

	switch ( nResult )
	{
	case DRIVE_CDROM:
	case DRIVE_REMOVABLE:
	case DRIVE_FIXED:
	case DRIVE_REMOTE:
		break;

	case DRIVE_NO_ROOT_DIR:
		LogRestoreMessage( strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_NOROOT );
		m_strOnScreenError = "The restore failed because the root directory could not be found\n\n";
		m_strOnScreenError += "Your system data has not been changed.";
		return;

	case DRIVE_RAMDISK:
		LogRestoreMessage( strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_RAMDISK );
		m_strOnScreenError = "The restore failed because the selected drive is a ramdisk\n\n";
		m_strOnScreenError +=	"Your system data has not been changed.";
		return;

	case DRIVE_UNKNOWN:
	default:
		LogRestoreMessage( strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_DRIVETYPE );
		m_strOnScreenError = "The restore failed because the drive type is unknown\n\n";
		m_strOnScreenError += "Your system data has not been changed.";
		return;
	}

	//CREATE DIALOG FOR PROGRESS AND INFO
	m_BackupInfoDlg.CreateOnMainWnd( TRUE );	
	CFileRemoverBackup FileRemover( &m_BackupInfoDlg );

	{
		CWaitCursor wait;

		//ATTEMPT TO REMOVE TEMPORARY ZIP FOLDER
		FileRemover.RemoveFolder ( m_strFolderPathZipTemp );
		if ( ::ExistSubdirectory ( m_strFolderPathZipTemp ) == TRUE )
		{
			LogRestoreMessage ( strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_REMOVEFOLDER2 );
			m_strOnScreenError = "The restore failed when removing a previous temporary folder\n\n";
			m_strOnScreenError += "Your system data has not been changed.";
			return;
		}

		//ATTEMPT TO RECREATE TEMPORARY ZIP FOLDER
		if ( ::MakeSubdirectory ( m_strFolderPathZipTemp ) != 0 )
		{	
			LogRestoreMessage ( strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_MAKEFOLDER );
			m_strOnScreenError = "The restore failed when creating a temporary folder\n\n";
			m_strOnScreenError += "Your system data has not been changed.";
			return;
		}

		if ( TRUE == m_bRestoreToFolder )
		{
			CSSFile fileSourceInfo;
			if ( fileSourceInfo.Open( ".\\restoredfile.txt", "wb" ) == TRUE )
			{
				fileSourceInfo.WriteLine( strUserFilePath );
				fileSourceInfo.Close();
			}
		}
		else
		{
			//ATTEMPT TO REMOVE BACKUP SAVE FOLDERS
			FileRemover.RemoveFolder ( m_strFolderPathRestoreSave );
			if ( ::ExistSubdirectory ( m_strFolderPathRestoreSave ) == TRUE )
			{
				LogRestoreMessage ( strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_REMOVEFOLDER1 );
				m_strOnScreenError = "The restore failed when removing a previous temporary folder\n\n";
				m_strOnScreenError += "Your system data has not been changed.";
				return;
			}

			//RENAME SYSTEM FOLDER TO BACKUP FOLDER 
			if ( rename ( m_strFolderPathSystemData, m_strFolderPathRestoreSave ) != 0 )
			{
				LogRestoreMessage ( strUserFilePath, ERROR_CONTEXT_PREZIP, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_RENAMEFOLDER1 );
				m_strOnScreenError = "The restore failed when renaming the current system folder\n\n";
				m_strOnScreenError += "Your system data has not been changed.";
				return;
			}
		}
	}

	//RESTORE FROM THE MAIN ZIP FILE
	m_nNextZipFileNo = 0;
	
	CString strFileToRestore = strUserFilePath;
	int nNextOuterFileNo = 2;

	do
	{
		m_nNextZipFileNo++;

		__int64 nDummy1, nDummy2;
		if ( RestoreFromZipFile( strFileToRestore, nDummy1, nDummy2, FALSE ) == FALSE )
		{
			CleanUpRestore( FileRemover );	
			return;
		}

		strFileToRestore.Format( "%s.%3.3d",
			(const char*) strUserFilePath.Left( strUserFilePath.GetLength() - 4 ),
			nNextOuterFileNo++ );
	}
	while ( ::FileExists( strFileToRestore ) == TRUE );

	CReportConsolidationArray<CSortedStringItem> arrayFileLog;
	CString strFileCount = "";

	//COUNT INTERMEDIATE FILES
	__int64 nFilesSoFar = 0;
	__int64 nFilesToRestore = 0;

	if ( ::ExistSubdirectory ( m_strFolderPathZipTemp ) == TRUE )
	{
		CString strPath = m_strFolderPathZipTemp + "\\*.*";
		CFileFind FileFinder;
		
		bool bWorking = ( FileFinder.FindFile ( strPath ) != 0 );
	
		while (bWorking)   
		{
			bWorking = ( FileFinder.FindNextFile() != 0 );

			if ( FileFinder.IsDots() == TRUE )
				continue;

			if ( FileFinder.IsDirectory() == TRUE )
				continue;

			CString strIntermediateZipFilepath = FileFinder.GetFilePath();
			strIntermediateZipFilepath.MakeLower();

			CString strIntermediateZipFilename = FileFinder.GetFileName();
			strIntermediateZipFilename.MakeLower();

			int nLength = strIntermediateZipFilename.GetLength();
			if ( ( nLength != 12 ) && ( nLength != 18 ) )
				continue;

			if ( strIntermediateZipFilename.Right(4) != ".zip" )
				continue;

			nFilesToRestore++;
		}
	}

	nFilesToRestore *= m_nFilesPerMiniZip;

	//NOW RESTORE FROM INTERMEDIATE FILES
	if ( ::ExistSubdirectory ( m_strFolderPathZipTemp ) == TRUE )
	{
		CString strPath = m_strFolderPathZipTemp + "\\*.*";
		CFileFind FileFinder;
		
		bool bWorking = ( FileFinder.FindFile ( strPath ) != 0 );
	
		while (bWorking)
		{
			bWorking = (FileFinder.FindNextFile() != 0);

			if (FileFinder.IsDots() == TRUE)
			{
				continue;
			}

			if (FileFinder.IsDirectory() == TRUE)
			{
				continue;
			}

			CString strIntermediateZipFilepath = FileFinder.GetFilePath();
			strIntermediateZipFilepath.MakeLower();

			CString strIntermediateZipFilename = FileFinder.GetFileName();
			strIntermediateZipFilename.MakeLower();

			int nLength = strIntermediateZipFilename.GetLength();
			if ((nLength != 12) && (nLength != 18))
			{
				continue;
			}

			if (strIntermediateZipFilename.Right(4) != ".zip")
			{
				continue;
			}

			if ( nLength == 18 )
			{
				CString strToken = strIntermediateZipFilename.Mid(3,11);

				CSortedStringItem item;
				item.m_strItem = strToken;

				int nPos;
				if ( arrayFileLog.Find( item, nPos ) == TRUE )
				{
					LogRestoreMessage ( strIntermediateZipFilepath, ERROR_CONTEXT_ZIP_EXTRACT, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_FILENO_DUPLICATED );
					m_strOnScreenError = "The restore failed because an internal file number was duplicated.\n\n";
					m_strOnScreenError += "Your system data has not been changed.";
					CleanUpRestore( FileRemover );
					return;
				}

				if (strFileCount == "")
				{
					strFileCount = strToken.Right(5);
				}

				if ( strFileCount != strToken.Right(5) )
				{
					LogRestoreMessage ( strIntermediateZipFilepath, ERROR_CONTEXT_ZIP_EXTRACT, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_FILENO_COUNT );
					m_strOnScreenError = "The restore failed because an internal file number was invalid.\n\n";
					m_strOnScreenError += "Your system data has not been changed.";
					CleanUpRestore( FileRemover );
					return;
				}

				bool bValidNumber = TRUE;
				if ( ::TestNumeric( strToken.Left(5) ) == FALSE )
					bValidNumber = FALSE;
				else if ( ::TestNumeric( strToken.Right(5) ) == FALSE )
					bValidNumber = FALSE;

				int nFileNo = atoi( strToken.Left(5) );
				int nMaxFileNo = atoi( strToken.Right(5) );

				if ( ( nFileNo < 1 ) || ( nFileNo > nMaxFileNo ) )
					bValidNumber = FALSE;

				if ( FALSE == bValidNumber )
				{
					LogRestoreMessage ( strIntermediateZipFilepath, ERROR_CONTEXT_ZIP_EXTRACT, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_FILENO_RANGE );
					m_strOnScreenError = "The restore failed because an internal file number was invalid.\n\n";
					m_strOnScreenError += "Your system data has not been changed.";
					CleanUpRestore( FileRemover );
					return;
				}
				
				arrayFileLog.Consolidate( item );
			}

			m_nNextZipFileNo++;
			if ( RestoreFromZipFile( strIntermediateZipFilepath, nFilesSoFar, nFilesToRestore, TRUE ) == FALSE )
			{
				CleanUpRestore( FileRemover );
				return;
			}
		}
	}

	if ( strFileCount != "" )
	{
		if ( atoi( strFileCount ) != arrayFileLog.GetSize() )
		{
			LogRestoreMessage ( strUserFilePath, ERROR_CONTEXT_ZIP_EXTRACT, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_FILENO_MISSING );
			m_strOnScreenError = "The restore failed because some expected files were missing.\n\n";
			m_strOnScreenError += "Your system data has not been changed.";
			CleanUpRestore( FileRemover );
		}
	}

	if (FALSE == m_bRestoreToFolder)
	{
		bool bRestoreFromScript = FALSE;
		bool bUseFallbackScript = FALSE;

		if (::FileExists(Filenames.GetSQLBackupFilename()) == TRUE)
		{
			if (TRUE == m_bFromScript)
			{
				bRestoreFromScript = TRUE;
				bUseFallbackScript = TRUE;
			}
			else
			{
				CSQLDb SQLDb;
				if (SQLDb.RestoreDatabase() != SQLCRUD_ERR_NONE)
				{
					LogRestoreMessage("SQL Restore", ERROR_CONTEXT_ZIP_EXTRACT, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_DBVERSION);
					m_strOnScreenError = "The restore failed because the database could not be recreated from an SQL backup file.\n\n";
					m_strOnScreenError += "Your system data has not been changed.";
					CleanUpRestore(FileRemover);
					return;
				}
			}
		}
		else
		{
			bRestoreFromScript = TRUE;
			bUseFallbackScript = FALSE;
		}

		if ( TRUE == bRestoreFromScript)
		{
			CSQLTranScriptImport Script;
			Script.BeginTrans();
			Script.DoWork(bUseFallbackScript);

			if (Script.EndTrans() != SQLTRAN_STATE_COMMITOK)
			{
				if (Script.GetBadDbVersionFlag() == TRUE)
				{
					LogRestoreMessage("SQL Script", ERROR_CONTEXT_ZIP_EXTRACT, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_DBVERSION);
					m_strOnScreenError = "The restore failed because the SQL Database Version of the backup did not match your system.\n\n";
					m_strOnScreenError += "Your system data has not been changed.";

				}
				else
				{
					LogRestoreMessage("SQL Script", ERROR_CONTEXT_ZIP_EXTRACT, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_SQLSCRIPT);
					m_strOnScreenError = "The restore failed because the SQL database could not be recreated from script.\n\n";
					m_strOnScreenError += "Your system data has not been changed.";
				}

				CleanUpRestore(FileRemover);
				return;
			}
		}
	}
	
	LogRestoreMessage ( strUserFilePath, 0, 0, 0 );
	m_bComplete = TRUE;

	//ATTEMPT TO REMOVE TEMPORARY ZIP FOLDER
	FileRemover.RemoveFolder ( m_strFolderPathZipTemp );

	if (FALSE == m_bAuto)
	{
		SetDefaultBackupFolder(strUserPathName);
	}
}

/**********************************************************************/

bool CSystemBackup::IsValidTargetFolder( CString& strFilename, CStringArray& arrayTargetFolders )
{
	strFilename.MakeLower();
	int nFilenameLen = strFilename.GetLength();

	for ( int n = 0; n < arrayTargetFolders.GetSize(); n++ )
	{
		CString strTargetFolder = arrayTargetFolders.GetAt(n);
		
		strTargetFolder.MakeLower();
		int nTargetLen = strTargetFolder.GetLength();

		if ( ( nFilenameLen > nTargetLen ) && ( strFilename.Left( nTargetLen ) == strTargetFolder ) )
			return TRUE;
	}
	
	return FALSE;
}

/**********************************************************************/

bool CSystemBackup::RestoreFromZipFile( CString& strZipFilename, __int64& nFilesSoFar, __int64& nTarget, bool bShowProgress )
{
	//ATTEMPT TO OPEN THE SOURCE ARCHIVE
	while ( TRUE )
	{
		int nErrorFamily; 
		int nErrorNumber;
		m_ZipArchive.SetSpanCallback(CSystemBackup::ChangeDisk, this);
		OpenArchive ( nErrorFamily, nErrorNumber, strZipFilename, CZipArchive::zipOpenReadOnly, 0 );
		
		if ( nErrorFamily == ERROR_FAMILY_NONE )
			break;
		else if ( nErrorFamily == ERROR_FAMILY_ZIPARCHIVE && nErrorNumber == CZipException::cdirNotFound )
		{
			CString strMsg = "The selected file is part of a backup spanning several disks\n\n";
			strMsg += "Please insert the last disk from this backup to continue.";

			if ( Prompter.OkCancel ( strMsg ) != IDOK )
			{
				m_strOnScreenError = "The system restore has been abandoned\n\nYour system data has not been changed";
				return FALSE;
			}
		}
		else
		{
			LogRestoreMessage ( strZipFilename, ERROR_CONTEXT_ZIP_OPEN, nErrorFamily, nErrorNumber );
			m_strOnScreenError = "The restore failed because the backup file could not be opened\n\n";
			m_strOnScreenError += "Your system data has not been changed.";
			return FALSE;
		}
	}

	m_BackupInfoDlg.SetText( FALSE, BACKUP_PROGRESS_RESTORE, m_nNextZipFileNo );

	CStringArray arrayTargetFolders;
	arrayTargetFolders.Add( m_strFolderPathSystemData + "$$$$$\\" );
	arrayTargetFolders.Add( m_strFolderPathSystemData + "\\" );
		
	//WORK OUT TOTAL FILE SIZE FOR PROGRESS
	m_nTotalFileSize = 0;
	for ( int nIndex = 0; nIndex < m_ZipArchive.GetNoEntries(); nIndex++ )
	{
		m_BackupInfoDlg.SetPos( FALSE, nIndex, m_ZipArchive.GetNoEntries(), 0, 10 );

		CZipFileHeader ZipFileHeader;
		if ( m_ZipArchive.GetFileInfo( ZipFileHeader, nIndex ) == FALSE )
		{
			LogRestoreMessage ( strZipFilename, ERROR_CONTEXT_ZIP_OPEN, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_FILEHEADER );
	
			int nErrorFamily; 
			int nErrorNumber; 
			CloseArchive ( nErrorFamily, nErrorNumber );
			if ( nErrorFamily != ERROR_FAMILY_NONE )
				LogRestoreMessage ( strZipFilename, ERROR_CONTEXT_ZIP_CLOSE, nErrorFamily, nErrorNumber );

			m_strOnScreenError = "The restore failed because the backup file could not be processed\n\n";
			m_strOnScreenError += "Your system data has not been changed.";
			return FALSE;
		}
		
		CString strFilename;
		strFilename = ZipFileHeader.GetFileName();
		
		if ( IsValidTargetFolder( strFilename, arrayTargetFolders ) == FALSE )
		{				
			LogRestoreMessage ( strZipFilename, ERROR_CONTEXT_ZIP_OPEN, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_BADFOLDERNAME );
	
			int nErrorFamily; 
			int nErrorNumber; 
			CloseArchive ( nErrorFamily, nErrorNumber );
			if ( nErrorFamily != ERROR_FAMILY_NONE )
				LogRestoreMessage ( strZipFilename, ERROR_CONTEXT_ZIP_CLOSE, nErrorFamily, nErrorNumber );

			m_strOnScreenError = "The restore failed because the backup contained unrecognised folders\n\n";
			m_strOnScreenError += "Your system data has not been changed.";
			return FALSE;
		}
		
		m_nTotalFileSize += ZipFileHeader.m_uUncomprSize;
	}

	m_nProcessedSoFar = 0;
	for ( int nIndex = 0; nIndex < m_ZipArchive.GetNoEntries(); nIndex++ )
	{
		int nErrorFamily = ERROR_FAMILY_NONE;
		int nErrorNumber = 0;

		CZipFileHeader ZipFileHeader;
		if ( m_ZipArchive.GetFileInfo( ZipFileHeader, nIndex ) == FALSE )
		{
			LogRestoreMessage ( strZipFilename, ERROR_CONTEXT_ZIP_OPEN, ERROR_FAMILY_MICROTREND, MICROTREND_ERROR_FILEHEADER );
		
			int nErrorFamily; 
			int nErrorNumber; 
			CloseArchive ( nErrorFamily, nErrorNumber );
			if ( nErrorFamily != ERROR_FAMILY_NONE )
				LogRestoreMessage ( strZipFilename, ERROR_CONTEXT_ZIP_CLOSE, nErrorFamily, nErrorNumber );
		
			m_strOnScreenError = "The restore failed because the backup file could not be processed\n\n";
			m_strOnScreenError += "Your system data has not been changed.";
			return FALSE;
		}
		
		CString strFilename;
		strFilename = ZipFileHeader.GetFileName();
		
		if ( IsValidTargetFolder( strFilename, arrayTargetFolders ) == FALSE )
		{
			m_nProcessedSoFar += ZipFileHeader.m_uUncomprSize;
			continue;
		}

		ExtractFile ( nErrorFamily, nErrorNumber, nIndex );

		if ( TRUE == bShowProgress )
		{
			nFilesSoFar++;
			m_BackupInfoDlg.SetPos( TRUE, nFilesSoFar, nTarget );
			m_BackupInfoDlg.SetText( TRUE, BACKUP_PROGRESS_RESTORESOFAR, nFilesSoFar, nTarget );
		}

		if ( nErrorFamily != ERROR_FAMILY_NONE )
		{
			LogRestoreMessage ( strZipFilename, ERROR_CONTEXT_ZIP_EXTRACT, nErrorFamily, nErrorNumber );

			int nErrorFamily; 
			int nErrorNumber; 
			CloseArchive ( nErrorFamily, nErrorNumber );
			if ( nErrorFamily != ERROR_FAMILY_NONE )
				LogRestoreMessage ( strZipFilename, ERROR_CONTEXT_ZIP_CLOSE, nErrorFamily, nErrorNumber );
		
			CString strMsg = "<..>Restore failed on file ";
			strMsg += strFilename;
			LogBackupMessage ( strMsg );

			m_strOnScreenError = "The restore failed when reading the file shown below\n\n";
			m_strOnScreenError += strFilename + "\n\n";
			m_strOnScreenError += "Your system data has not been changed.";
			return FALSE;
		}
		m_nProcessedSoFar += ZipFileHeader.m_uUncomprSize;
	}

	int nErrorFamily; 
	int nErrorNumber; 
	CloseArchive ( nErrorFamily, nErrorNumber );
	if ( nErrorFamily != ERROR_FAMILY_NONE )
	{
		LogRestoreMessage ( strZipFilename, ERROR_CONTEXT_ZIP_CLOSE, nErrorFamily, nErrorNumber );
		m_strOnScreenError = "The restore failed when closing the backup file.";
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CSystemBackup::LogBackupMessage( const char* szMsg )
{
	CString strBackupLogFolder = "";
	CString strBackupLogFile = "";

	if ( FALSE == m_bRestoreToFolder )
	{
		strBackupLogFolder = Filenames.GetBackupLogToWriteFolder();
		strBackupLogFile = Filenames.GetBackupLogToWriteFilename();
	}
	else
	{
		strBackupLogFolder = "..\\Sysset\\P47\\BackupLog";
		strBackupLogFile = strBackupLogFolder + "\\Backup.log";
	}

	if (::ExistSubdirectory(strBackupLogFolder) == FALSE)
	{
		::MakeSubdirectory(strBackupLogFolder);
	}

	CTime tm = CTime::GetCurrentTime();
	CString strTime = tm.Format ( "%H:%M:%S" );
	CString strDate = tm.Format ( "%a %b %d %Y" );

	CCSV csv ( '\t' );

	csv.Add ( szMsg );
	csv.Add ( strDate );
	csv.Add ( strTime );
	
	CString strLine = "";
	strLine.Format ( "%s", 
		(const char*) csv.GetLine() );

	if ( ::FileExists ( strBackupLogFile ) == FALSE )
	{
		CReportFile ReportFile;
		ReportFile.Open ( strBackupLogFile );
		ReportFile.SetStyle1 ( "Backup Log" );
		ReportFile.AddColumn ( "",TA_LEFT, 1500 );
		ReportFile.AddColumn ( "",TA_RIGHT, 400 );
		ReportFile.AddColumn ( "",TA_LEFT, 250 );
		ReportFile.WriteLine ( strLine );
		ReportFile.Close();
	}
	else
	{
		CSSFile fileLog;

		if (fileLog.Open(strBackupLogFile, "ab") == TRUE)
		{
			fileLog.WriteLine(strLine);
		}
	}
}

/**********************************************************************/


