/**********************************************************************/
#include "FileRemover.h"
//#include "globalfunctions.h"
/**********************************************************************/
#include "PosTrayTokenFiles.h"
/**********************************************************************/

void CPosTrayTokenFiles::CreateStopFile()
{
	CSSFile fileToken;
	CFilenameUpdater FnUp(SysFiles::PosTrayStopFile );

	if ( fileToken.Open( FnUp.GetInstanceFilenameToUse(), "wb" ) == TRUE )
	{
		fileToken.WriteLine( "PosTray" );
		fileToken.Close();
	}
}

/**********************************************************************/

void CPosTrayTokenFiles::DeleteStopFile()
{
	CFilenameUpdater FnUp(SysFiles::PosTrayStopFile );
	CFileRemover FileRemover( FnUp.GetInstanceFilenameToUse() );
}

/**********************************************************************/

bool CPosTrayTokenFiles::StopFileExists()
{
	CFilenameUpdater FnUp(SysFiles::PosTrayStopFile );
	return ::FileExists( FnUp.GetInstanceFilenameToUse() );
}

/**********************************************************************/

void CPosTrayTokenFiles::CreateSystemUpdateFile()
{
	CSSFile fileToken;
	CFilenameUpdater FnUp(SysFiles::PosTrayUpdateFile );
	if ( fileToken.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		fileToken.WriteLine( "PosTray" );
		fileToken.Close();
	}
}

/**********************************************************************/

void CPosTrayTokenFiles::DeleteSystemUpdateFile()
{
	CFilenameUpdater FnUp(SysFiles::PosTrayUpdateFile );
	CFileRemover FileRemover( FnUp.GetFilenameToUse() );
}

/**********************************************************************/

bool CPosTrayTokenFiles::SystemUpdateFileExists()
{
	CFilenameUpdater FnUp(SysFiles::PosTrayUpdateFile );
	return ::FileExists( FnUp.GetFilenameToUse() );
}

/**********************************************************************/

void CPosTrayTokenFiles::GetSystemUpdateFileTime( COleDateTime& oleTime)
{
	CFilenameUpdater FnUp(SysFiles::PosTrayUpdateFile );
	
	CFileStatus fileStatusUpdate;
	if ( CFile::GetStatus( FnUp.GetFilenameToUse(), fileStatusUpdate ) == TRUE )
	{
		CTime t = fileStatusUpdate.m_mtime;
		oleTime = COleDateTime( t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond() );
	}
	else
		oleTime = COleDateTime( 1500, 1, 1, 0, 0, 0 );
}

/**********************************************************************/

void CPosTrayTokenFiles::CreateRetryEmailFile()
{
	COleDateTime time = COleDateTime::GetCurrentTime();

	CString strDate;
	strDate.Format( "%4.4d%2.2d%2.2d-%2.2d%2.2d%2.2d",
		time.GetYear(),
		time.GetMonth(),
		time.GetDay(),
		time.GetHour(),
		time.GetMinute(),
		time.GetSecond() );

	CString strFilename = "PosTray\\File9999.dat";
	::GetSyssetProgramPath( strFilename );

	CSSFile fileDate;
	if ( fileDate.Open( strFilename, "wb" ) == TRUE )
		fileDate.WriteLine( strDate );
}

/**********************************************************************/

void CPosTrayTokenFiles::DeleteRetryEmailFile()
{
	CString strFilename = "PosTray\\File9999.dat";
	::GetSyssetProgramPath( strFilename );
	CFileRemover FileRemover( strFilename );
}

/**********************************************************************/

bool CPosTrayTokenFiles::RetryEmailFileExists()
{
	CString strFilename = "PosTray\\File9999.dat";
	::GetSyssetProgramPath( strFilename );
	return ::FileExists( strFilename );
}

/**********************************************************************/
