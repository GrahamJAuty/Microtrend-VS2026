/**********************************************************************/
 
/**********************************************************************/
 
/**********************************************************************/
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "MessageLogger.h"
/**********************************************************************/
#include "PosTrayExportSender.h"
/**********************************************************************/

CPosTrayExportSender::CPosTrayExportSender()
{
	m_strSlash = "\\";
}

/**********************************************************************/

void CPosTrayExportSender::LookForReports()
{
	CString strSource = "PosTray\\Pending";
	::GetSyssetProgramPath( strSource );
	ExportReports( strSource, "", TRUE  );
}

/**********************************************************************/

void CPosTrayExportSender::ExportReports( const char* szSourceFolder, const char* szParent, bool bRoot )
{	
	CString strSourceFolder = szSourceFolder;
	
	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strSourceFolder + m_strSlash + "*.*" ) != 0 );

	CStringArray arrayFoldersToDelete;
	CStringArray arrayFilesToDelete;

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		//IGNORE DIRECTORIES
		if ( FileFinder.IsDirectory() == FALSE )
		{
			if ( FALSE == bRoot )
			{
				CString strSourceFilename = FileFinder.GetFileName();
				strSourceFilename.MakeUpper();

				CString strExt = strSourceFilename.Right(4);

				if ( strExt != ".001" )
				{
					bool bValidFilename = FALSE;
					
					if ( strSourceFilename.GetLength() == 36 )
						if ( ( strExt == ".HTM" ) || ( strExt == ".CSV" ) )
							bValidFilename = TRUE;
						
					if ( FALSE == bValidFilename )
					{
						CString strError = "Failed : ";
						strError += GetExportErrorText( EXPORT_ERROR_FILENAME );
						LogExportJobSystem( szParent, "", strSourceFilename, strError );
						arrayFilesToDelete.Add( strSourceFilename );
					}
					else
					{
						CString strErrorText = "";
						CString strTargetFolder = "";
						CString strTargetFilename = "";
						int nResult = ExportReport( strSourceFolder, strSourceFilename, strTargetFolder, strTargetFilename, strErrorText );

						CString strReportResult = "";
						bool bCanDelete = FALSE;

						switch( nResult )
						{
						case EXPORT_ERROR_NONE:
							strReportResult = "OK";
							bCanDelete = TRUE;
							break;

						case EXPORT_ERROR_INFOFILE_TARGETFOLDER:
						case EXPORT_ERROR_INFOFILE_TASKFILENAME:
						case EXPORT_ERROR_INFOFILE_FIND:
							strReportResult = "FAILED : ";
							strReportResult += GetExportErrorText( nResult );
							bCanDelete = TRUE;
							break;
							
						default:
							strReportResult = "FAILED : ";
							strReportResult += GetExportErrorText( nResult );
							break;
						}

						if ( strReportResult != "" )
							LogExportJobSystem( szParent, strTargetFolder, strTargetFilename, strReportResult );	

						if ( TRUE == bCanDelete )
						{
							arrayFilesToDelete.Add( strSourceFilename );
							
							CString str001Filename;
							if ( ::ChangeFileExtension( strSourceFilename, "001", str001Filename ) )
								arrayFilesToDelete.Add( str001Filename );
						}
					}
				}
			}
		}
		else
		{
			CString strSubFolder = FileFinder.GetFileName();
			
			bool bProcessSubFolder = FALSE;
			
			if ( strSubFolder.GetLength() > 3 )
			{
				strSubFolder.MakeUpper();
				bProcessSubFolder = TRUE;
			}

			if ( TRUE == bProcessSubFolder )
			{
				CString strPendingFolder = strSourceFolder + m_strSlash + strSubFolder;
				ExportReports ( strPendingFolder, strSubFolder, FALSE );
			}
		}
	}

	CFileRemover FileRemover;
	for ( int n = 0; n < arrayFilesToDelete.GetSize(); n++ )
	{
		CString strFileToDelete = strSourceFolder + m_strSlash + arrayFilesToDelete.GetAt(n); 
		FileRemover.RemoveFile( strFileToDelete  );
	}
}

/**********************************************************************/

int CPosTrayExportSender::ExportReport( CString& strSourceFolder, CString& strSourceFilename, CString& strTargetFolder, CString& strTargetFilename, CString& strErrorText )
{
	CString strExportFilename = strSourceFilename;
	strExportFilename.MakeUpper();
	
	CString strInfoFilename;
	if ( ChangeFileExtension( strExportFilename, "001", strInfoFilename ) == FALSE )
		return EXPORT_ERROR_INFOFILE_FIND;
	
	if ( ::FileExists( strSourceFolder + m_strSlash + strInfoFilename ) == FALSE )
		return EXPORT_ERROR_INFOFILE_FIND;

	CSSFile fileInfo;
	if ( fileInfo.Open( strSourceFolder + m_strSlash + strInfoFilename, "rb" ) == FALSE )
		return EXPORT_ERROR_INFOFILE_READ;

	if ( fileInfo.ReadString( strTargetFilename ) == FALSE )
		return EXPORT_ERROR_INFOFILE_TASKFILENAME;

	CString strTaskFilename;
	if ( fileInfo.ReadString( strTaskFilename ) == FALSE )
		return EXPORT_ERROR_INFOFILE_TASKFILENAME;

	CPosTrayTask Task( FALSE );
	Task.QuickLoadExportFolder( strTaskFilename );

	strTargetFolder = Task.GetTaskExportFolder();
	::TrimSpaces( strTargetFolder, FALSE );
		
	if ( strTargetFolder == "" )
	{
		strTargetFolder = "PosTray\\Exports";
		::GetSyssetProgramPath( strTargetFolder );
	}

	fileInfo.Close();

	CString strSourcePath = strSourceFolder + m_strSlash + strSourceFilename;
	CString strDestPath = strTargetFolder + m_strSlash + strTargetFilename;
	
	CFileStatus statusSource;
	if ( CFile::GetStatus( strSourcePath, statusSource ) == FALSE )
		return EXPORT_ERROR_STATUS_SOURCE;

	::CreateSubdirectory( strTargetFolder );

	if ( ExistSubdirectory( strTargetFolder ) == FALSE )
		return EXPORT_ERROR_TARGET_FOLDER;
	
	if ( ::CopyFile( strSourcePath, strDestPath, false ) == 0 )
		return EXPORT_ERROR_COPYFILE1;

	CFileStatus statusDest;
	if ( CFile::GetStatus( strDestPath, statusDest ) == FALSE )
		return EXPORT_ERROR_STATUS_DEST;

	if ( statusSource.m_size != statusDest.m_size )
		return EXPORT_ERROR_COPYFILE2;

	return EXPORT_ERROR_NONE;
}

/**********************************************************************/

const char* CPosTrayExportSender::GetExportErrorText( int nError )
{
	m_strExportErrorText = "";

	switch( nError )
	{
	case EXPORT_ERROR_FILENAME:					m_strExportErrorText = "Invalid filename";					break;
	case EXPORT_ERROR_INFOFILE_FIND:			m_strExportErrorText = "Missing info file";					break;
	case EXPORT_ERROR_INFOFILE_READ:			m_strExportErrorText = "Unable to open info file";			break;
	case EXPORT_ERROR_INFOFILE_TARGETFOLDER:	m_strExportErrorText = "No export folder specified";		break;
	case EXPORT_ERROR_INFOFILE_TASKFILENAME:	m_strExportErrorText = "No task filename specified";		break;
	case EXPORT_ERROR_COPYFILE1:				m_strExportErrorText = "Unable to copy file";				break;
	case EXPORT_ERROR_COPYFILE2:				m_strExportErrorText = "Target file has incorrect size";	break;
	case EXPORT_ERROR_STATUS_SOURCE:			m_strExportErrorText = "Unable to check source file status";break;
	case EXPORT_ERROR_STATUS_DEST:				m_strExportErrorText = "Unable to check target file status";break;
	case EXPORT_ERROR_TARGET_FOLDER:			m_strExportErrorText = "Unable to create target folder"		;break;
	default:
		m_strExportErrorText.Format ( "Unknown Error Code (%d)", nError ); 
	}

	return m_strExportErrorText;
}

/**********************************************************************/

void CPosTrayExportSender::LogExportJobSystem( const char* szType, const char* szTargetFolder, const char* szTargetFile, const char* szResult )
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CExportLoggerInfo info;

	info.m_strDate.Format( "%2.2d/%2.2d/%4.4d", 
		timeNow.GetDay(),
		timeNow.GetMonth(),
		timeNow.GetYear() );

	info.m_strTime.Format( "%2.2d:%2.2d:%2.2d", 
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond() );

	info.m_strType = szType;
	info.m_strTargetFolder = szTargetFolder;
	info.m_strTargetFile = szTargetFile;
	info.m_strResult = szResult;

	MessageLogger.LogExportSystem( info );
}

/**********************************************************************/
