/**********************************************************************/
#include "CkTask.h"
#include "CKZip.h"
#include "CKZipEntry.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\GlobalState.h"
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
	
	if ( nLength >= 5 ) //VX.YY
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
	CString strPrefix = "mb";
	if ( TRUE == bBackup )
	{
		if ( m_BackupFilter.GotRules() )
		{
			strPrefix = "xx";
			strType = "Partial";
		}
	}
	

	m_strBackupFilename.Format ( "%s%2.2d%s%s",
		(const char*) strPrefix,
		SYSSET_LOYALTY4, 
		(const char*) strNumber,
		(const char*) strBeta );

	m_strBackupFilter.Format ( "%s Backup Files (%s.zip)|%s.zip||",
		(const char*) strType,
		(const char*) m_strBackupFilename,
		(const char*) m_strBackupFilename );

	m_strRestoreFilter.Format ( "System Backup Files (%s*.zip)|%s*.zip||",
		(const char*) m_strBackupFilename.Left(4),
		(const char*) m_strBackupFilename.Left(4) );
}

/**********************************************************************/

void CSystemBackup::LogBackupMessage ( const char* szFilename, int nContext, int nError )
{
	CString strMsg;

	CString strAuto = ( m_bAuto ) ? "Automatic " : "";
	
	if (nContext != BKERROR_CONTEXT_NONE)
	{
		strMsg.Format("<..>%sBackup failed (%s) (%d:%d)", (const char*)strAuto, szFilename, nContext, nError);
	}
	else
	{
		strMsg.Format("<..>%sBackup complete (%s)", (const char*)strAuto, szFilename);
	}

	LogBackupMessage ( strMsg );
}

/**********************************************************************/

void CSystemBackup::LogRestoreMessage ( const char* szFilename, int nContext, int nError )
{
	CString strMsg = "";

	CString strToFolder = ( m_bRestoreToFolder ) ? " to folder " : " ";

	if (nContext != BKERROR_CONTEXT_NONE)
	{
		strMsg.Format("<..>Restore%sfailed (%s) (%d:%d)", (const char*)strToFolder, szFilename, nContext, nError);
	}
	else
	{
		strMsg.Format("<..>Restore%scomplete (%s)", (const char*)strToFolder, szFilename);
	}

	LogBackupMessage ( strMsg );
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
		LogBackupMessage(strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_BADNAME);
		m_strOnScreenError = "Invalid backup filename selected";
		return;
	}

	//GIVE UP IF THE BACKUP FOLDER DOES NOT EXIST
	CString strUserPathName = strUserFilePath.Left(strUserFilePath.GetLength() - strGoodFileName.GetLength());
	if (::ExistSubdirectory(strUserPathName) == FALSE)
	{
		LogBackupMessage(strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_NOTEXIST);
		m_strOnScreenError = "Unable to locate backup folder";
		return;
	}

	char drive[_MAX_DRIVE];
	_splitpath_s(strUserPathName, drive, _MAX_DRIVE, NULL, 0, NULL, 0, NULL, 0);
	m_strDrive = drive;
	m_strDrive += "\\";

	int nResult = GetDriveType(m_strDrive);
	
	switch (nResult)
	{
	case DRIVE_CDROM:
	case DRIVE_FIXED:
	case DRIVE_REMOTE:
	case DRIVE_REMOVABLE:
		break;

	case DRIVE_NO_ROOT_DIR:
		LogBackupMessage(strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_NOROOT);
		m_strOnScreenError = "Unable to find the backup root folder";
		return;

	case DRIVE_RAMDISK:
		LogBackupMessage(strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_RAMDISK);
		m_strOnScreenError = "Unable to backup to a RAM disk";
		return;

	case DRIVE_UNKNOWN:
	default:
		LogBackupMessage(strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_DRIVETYPE);
		m_strOnScreenError = "Unable to identify the backup drive type";
		return;
	}

	m_strDrive.MakeLower();
	strUserPathName.MakeLower();

	if (::FileExists(strUserFilePath) == TRUE)
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
	m_BackupProgressDlg.CreateOnMainWnd(FALSE);
	CFileRemoverBackup FileRemover(&m_BackupProgressDlg);

	//ATTEMPT TO REMOVE TEMPORARY ZIP FOLDER
	FileRemover.RemoveFolder(m_strFolderPathZipTemp);
	if (::ExistSubdirectory(m_strFolderPathZipTemp) == TRUE)
	{
		LogBackupMessage(strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_REMOVETEMPFOLDER);
		m_strOnScreenError = "Unable to remove a previous temporary folder";
		return;
	}

	//ATTEMPT TO RECREATE TEMPORARY ZIP FOLDER
	if (::MakeSubdirectory(m_strFolderPathZipTemp) != 0)
	{
		LogBackupMessage(strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_MAKEFOLDER);
		m_strOnScreenError = "Unable to create temporary folder";
		return;
	}

	//MAKE A COPY OF THE BACKUP LOG FILE TO INCLUDE IN THE BACKUP
	CString strDest = m_strFolderPathSystemData + "\\backup.001";
	CString strSource = "backup.log";
	CopyFile(strSource, strDest, FALSE);

	CkZip ZipArchive;
	
	int nCompressionLevel = AutoBackupOptions.GetCompressionLevel();
	if (nCompressionLevel < 0 || nCompressionLevel > 9)
	{
		nCompressionLevel = 6;
	}
	ZipArchive.SetCompressionLevel(nCompressionLevel);

	CString strTempZipFilename = m_strFolderPathZipTemp + "\\backup.zip";

	if (ZipArchive.NewZip(strTempZipFilename) == FALSE)
	{
		LogBackupMessage(strUserFilePath, BKERROR_CONTEXT_ZIP_CREATE, BKERROR_TYPE_ZIPCREATEARCHIVE);
		m_strOnScreenError = "Unable to create the backup file";
		return;
	}

	__int64 nFilesSoFar = 0;
	__int64 nFilesToBackup = 0;
	CountFilesToBackup(m_strFolderPathSystemData + "\\", nFilesToBackup);

	if (CreateZipFile( ZipArchive, m_strFolderPathSystemData + "\\", nFilesSoFar, nFilesToBackup) == FALSE)
	{
		return;
	}

    // Use Chilkat async support for writing the zip file
    CkTask* pTask = ZipArchive.WriteZipAndCloseAsync();
    if (pTask == nullptr) {
        LogBackupMessage(strUserFilePath, BKERROR_CONTEXT_ZIP_CREATE, BKERROR_TYPE_ZIPWRITEARCHIVE1);
        m_strOnScreenError = "Unable to start writing the backup file";
        return;
    }

    // Start the async task
    pTask->Run();
 
	{
		int nPercentOld = -1;
		while (!pTask->get_Finished()) 
		{
			int nPercentDone = pTask->get_PercentDone();

			if (nPercentDone > nPercentOld)
			{
				m_BackupProgressDlg.SetText(BACKUP_PROGRESS_FINALISE, nPercentDone, 100);
				m_BackupProgressDlg.SetPos(nPercentDone, 100);
				nPercentOld = nPercentDone;
			}

			Sleep(100); // Sleep for 100ms
		}
	}

	bool bSuccess = pTask->GetResultBool();
	pTask->dispose();
	delete pTask;
    
	if ( FALSE == bSuccess) 
	{
        LogBackupMessage(strUserFilePath, BKERROR_CONTEXT_ZIP_CREATE, BKERROR_TYPE_ZIPWRITEARCHIVE2);
        m_strOnScreenError = "Unable to write the backup file";
        return;
    }

	if (CopyFile(strTempZipFilename, strUserFilePath, FALSE) == FALSE)
	{
		LogBackupMessage(strUserFilePath, BKERROR_CONTEXT_ZIP_CREATE, BKERROR_TYPE_COPYFILE);
		m_strOnScreenError = "Unable to copy the backup file";
		return;
	}

	LogBackupMessage(strUserFilePath, 0, 0);
	m_bComplete = TRUE;

	//ATTEMPT TO REMOVE TEMPORARY ZIP FOLDER
	FileRemover.RemoveFolder(m_strFolderPathZipTemp);

	if (FALSE == m_bAuto)
	{
		SetDefaultBackupFolder(strUserPathName);
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
		if (FileFinder.IsDots() == TRUE)
		{
			continue;
		}

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
				m_BackupProgressDlg.SetText( BACKUP_PROGRESS_COUNTFILE, nCountSoFar );
			}
		}
	}
}

/**********************************************************************/

bool CSystemBackup::CreateZipFile ( CkZip& ZipArchive, const char* szLocalPath, __int64& nFilesSoFar, __int64& nFilesToBackup )
{
	CString strLocalPath = szLocalPath;
	
	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strLocalPath + "*.*" ) != 0 );

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if (FileFinder.IsDots() == TRUE)
		{
			continue;
		}

		CString strFilename = FileFinder.GetFileName();
		CString strFilePath = strLocalPath + strFilename;

		if ( FileFinder.IsDirectory() == TRUE )
		{
			bool bIsTempFolder = FALSE;
			if ( strFilename.GetLength() == 6 )
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
				if (CreateZipFile(ZipArchive, strFilePath + "\\", nFilesSoFar, nFilesToBackup) == FALSE)
				{
					return FALSE;
				}
			}
		}
		else
		{
			switch ( GetFileIncludeType( strLocalPath, strFilename ) )
			{
			case BACKUP_FILE_INCLUDE_YES:
				{
					CFileStatus FileStatus;
					CFile::GetStatus ( strFilePath, FileStatus );

					if ((FileStatus.m_size != 0) || (AutoBackupOptions.GetExcludeZeroSizeFilesFlag() == FALSE))
					{
						if (ZipArchive.AddFile(strFilePath, FALSE) == FALSE)
						{
							LogBackupMessage(strFilePath, BKERROR_CONTEXT_ZIP_CREATE, BKERROR_TYPE_ADDFILE);
							m_strOnScreenError = "Unable to add file to the backup";
							return FALSE;
						}
					}
					
					m_BackupProgressDlg.SetPos(++nFilesSoFar, nFilesToBackup);
					m_BackupProgressDlg.SetText(BACKUP_PROGRESS_BACKUPSOFAR, nFilesSoFar, nFilesToBackup);
				}
				break;

			case BACKUP_FILE_INCLUDE_NO:
			default:
				break;
			}
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
				strUserFilePath += m_strBackupFilename.Left(4);
				strUserFilePath += strNum;
				strUserFilePath += csv.GetString(5);
				strUserFilePath += ".ZIP";
				strUserFileName = strUserFilePath.Right(12);
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
	
	if ( strUserFileName.GetLength() != 12 || 
		strUserFileName.Right(4) != strGoodFileName.Right(4) || 
		strUserFileName.Left(4) != strGoodFileName.Left(4) ||
		strUserFileName.GetAt(4) < '0' || strUserFileName.GetAt(4) > '9' ||
		strUserFileName.GetAt(5) < '0' || strUserFileName.GetAt(5) > '9' ||
		strUserFileName.GetAt(6) < '0' || strUserFileName.GetAt(6) > '9' )
	{
		LogRestoreMessage( strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_BADNAME );
		m_strOnScreenError.Format ( "You must specify a %s file to restore from.", ( const char* ) strGoodFileName );
		return;
	}
	
	//HANDLE FILE NOT FOUND
	if ( ::FileExists ( strUserFilePath ) == FALSE )
	{
		LogRestoreMessage( strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_NOTEXIST );
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
	case DRIVE_FIXED:
	case DRIVE_REMOTE:
	case DRIVE_REMOVABLE:
		break;

	case DRIVE_NO_ROOT_DIR:
		LogRestoreMessage( strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_NOROOT );
		m_strOnScreenError = "The restore failed because the root directory could not be found\n\n";
		m_strOnScreenError += "Your system data has not been changed.";
		return;

	case DRIVE_RAMDISK:
		LogRestoreMessage( strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_RAMDISK );
		m_strOnScreenError = "The restore failed because the selected drive is a ramdisk\n\n";
		m_strOnScreenError +=	"Your system data has not been changed.";
		return;

	case DRIVE_UNKNOWN:
	default:
		LogRestoreMessage( strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_DRIVETYPE );
		m_strOnScreenError = "The restore failed because the drive type is unknown\n\n";
		m_strOnScreenError += "Your system data has not been changed.";
		return;
	}

	//CREATE DIALOG FOR PROGRESS AND INFO
	m_BackupProgressDlg.CreateOnMainWnd( TRUE );	
	CFileRemoverBackup FileRemover( &m_BackupProgressDlg );


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
				LogRestoreMessage ( strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_REMOVETEMPFOLDER );
				m_strOnScreenError = "The restore failed when removing a previous temporary folder\n\n";
				m_strOnScreenError += "Your system data has not been changed.";
				return;
			}

			//RENAME SYSTEM FOLDER TO BACKUP FOLDER 
			if ( rename ( m_strFolderPathSystemData, m_strFolderPathRestoreSave ) != 0 )
			{
				LogRestoreMessage ( strUserFilePath, BKERROR_CONTEXT_PREZIP, BKERROR_TYPE_RENAMESYSFOLDER );
				m_strOnScreenError = "The restore failed when renaming the current system folder\n\n";
				m_strOnScreenError += "Your system data has not been changed.";
				return;
			}
		}

	CkZip ZipArchive;
	if (ZipArchive.OpenZip(strUserFilePath) == FALSE)
	{
		LogRestoreMessage(strUserFilePath, BKERROR_CONTEXT_ZIP_EXTRACT, BKERROR_TYPE_ZIPOPENARCHIVE);
		m_strOnScreenError = "The restore failed because the backup file could not be opened\n\n";
		m_strOnScreenError += "Your system data has not been changed.";
		return;
	}

	int nFilesToExtract = ZipArchive.get_NumEntries();
	for (int n = 0; n < nFilesToExtract; n++)
	{
		CkZipEntry* entry = ZipArchive.GetEntryByIndex(n);

		if (entry == NULL)
		{
			LogRestoreMessage(strUserFilePath, BKERROR_CONTEXT_ZIP_EXTRACT, BKERROR_TYPE_ZIPREADENTRY);
			m_strOnScreenError = "The restore failed because an entry in the backup file could not be read\n\n";
			m_strOnScreenError += "Your system data has not been changed.";
			return;
		}

		if (entry->get_IsDirectory() == FALSE)
		{
			CString strTargetPath = m_strFolderPathSystemData.Mid(4);

			CkString CkPath;
			entry->get_FileName(CkPath);
			CString strPath = CkPath.getString();
			strPath.MakeUpper();

			if (strPath.Left(4) != "P47/")
			{
				LogRestoreMessage(strUserFilePath, BKERROR_CONTEXT_ZIP_EXTRACT, BKERROR_TYPE_BADFILEPATH);
				m_strOnScreenError = "The restore failed because an entry in the backup file has an invalid path\n\n";
				m_strOnScreenError += "Your system data has not been changed.";
				delete entry;
				return;
			}

			if (entry->Extract(strTargetPath) == FALSE)
			{
				LogRestoreMessage(strUserFilePath, BKERROR_CONTEXT_ZIP_EXTRACT, BKERROR_TYPE_ZIPEXTRACTFILE);
				m_strOnScreenError = "The restore failed because an entry in the backup file could not be extracted\n\n";
				m_strOnScreenError += "Your system data has not been changed.";
				delete entry;
				return;
			}

			m_BackupProgressDlg.SetText(BACKUP_PROGRESS_RESTORESOFAR, n + 1, nFilesToExtract);
			m_BackupProgressDlg.SetPos(n + 1, nFilesToExtract);
		}

		delete entry;
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
					LogRestoreMessage("SQL Restore", BKERROR_CONTEXT_ZIP_EXTRACT, BKERROR_TYPE_RESTORESQL);
					
					CString strError = g_GlobalState.GetExecuteSQLLastError();
					
					if (strError != "")
					{
						LogBackupMessage(strError);
					}

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
					LogRestoreMessage("SQL Script", BKERROR_CONTEXT_ZIP_EXTRACT, BKERROR_TYPE_SCRIPTDBVERSION);
					m_strOnScreenError = "The restore failed because the SQL Database Version of the backup did not match your system.\n\n";
					m_strOnScreenError += "Your system data has not been changed.";

				}
				else
				{
					LogRestoreMessage("SQL Script", BKERROR_CONTEXT_ZIP_EXTRACT, BKERROR_TYPE_RESTORESCRIPT);
					m_strOnScreenError = "The restore failed because the SQL database could not be recreated from script.\n\n";
					m_strOnScreenError += "Your system data has not been changed.";
				}

				CleanUpRestore(FileRemover);
				return;
			}
		}
	}
	
	LogRestoreMessage ( strUserFilePath, 0, 0 );
	m_bComplete = TRUE;

	//ATTEMPT TO REMOVE TEMPORARY ZIP FOLDER
	FileRemover.RemoveFolder ( m_strFolderPathZipTemp );

	if (FALSE == m_bAuto)
	{
		SetDefaultBackupFolder(strUserPathName);
	}
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


