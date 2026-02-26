/**********************************************************************/
#include "FileRemover.h"
/**********************************************************************/
#include "NetworkTester.h"
/**********************************************************************/

CNetworkTester::CNetworkTester()
{
	m_nCode = 0;
	m_nError = 0;
}

/**********************************************************************/

int CNetworkTester::TestNetwork()
{
	m_nError = NETWORK_ERROR_NONE;
	m_nCode = 0;

	CString strPath1 = "TestPath1";
	CString strPath2 = "TestPath2";

	::GetTempPath( strPath1 );
	::GetTempPath( strPath2 );
	
	TestNetworkInternal( strPath1, strPath2 );

	if (m_nError != NETWORK_ERROR_NONE)
	{
		return m_nError;
	}

	strPath1.Format( "Test1%d", Sysset.GetInstanceNo() );
	strPath2.Format( "Test2%d", Sysset.GetInstanceNo() );

	::GetSyssetProgramPath( strPath1 );
	::GetSyssetProgramPath( strPath2 );
	
	TestNetworkInternal( strPath1, strPath2 );
	
	return m_nError;
}

/**********************************************************************/

void CNetworkTester::TestNetworkInternal( const char* szPath1, const char* szPath2 )
{
	m_nError = NETWORK_ERROR_NONE;
	m_nCode = 0;

	CString strPath1 = szPath1;
	CString strPath2 = szPath2;

	CString strFile1 = strPath2 + "\\Test1.dat";
	CString strFile2 = strPath2 + "\\Test2.dat";

	CFileRemover FileRemover;
	FileRemover.RemoveFolder( strPath1 );
	
	if ( ::ExistSubdirectory( strPath1 ) == TRUE )
	{
		m_nCode = 1;
		m_nError = NETWORK_ERROR_FOLDER_DELETE;
		return;
	}

	::CreateSubdirectory( strPath1 );

	if ( ::ExistSubdirectory( strPath1 ) == FALSE )
	{
		m_nCode = 2;
		m_nError = NETWORK_ERROR_FOLDER_CREATE;
		return;
	}

	FileRemover.RemoveFolder( strPath2 );
	if ( ::ExistSubdirectory( strPath2 ) == TRUE )
	{
		m_nCode = 3;
		m_nError = NETWORK_ERROR_FOLDER_DELETE;
		return;
	}
	
	rename( strPath1, strPath2 );

	if ( ::ExistSubdirectory( strPath1 ) == TRUE )
	{
		m_nCode = 4;
		m_nError = NETWORK_ERROR_FOLDER_RENAME;
		return;
	}
	
	if ( ::ExistSubdirectory( strPath2 ) == FALSE )
	{
		m_nCode = 5;
		m_nError = NETWORK_ERROR_FOLDER_RENAME;
		return;
	}
	
	FileRemover.RemoveFile( strFile1 );

	if ( ::FileExists( strFile1 ) == TRUE )
	{
		m_nCode = 6;
		m_nError = NETWORK_ERROR_FILE_DELETE;
		return;
	}

	CSSFile fileNew;
	if ( fileNew.Open( strFile1, "wb" ) == TRUE )
	{
		fileNew.WriteLine( "..." );
		fileNew.Close();
	}
	else
	{
		m_nCode = 7;
		m_nError = NETWORK_ERROR_FILE_CREATE;
		return;
	}

	if ( ::FileExists( strFile1 ) == FALSE )
	{
		m_nCode = 8;
		m_nError = NETWORK_ERROR_FILE_CREATE;
		return;
	}

	if ( fileNew.Open( strFile1, "ab" ) == TRUE )
	{
		fileNew.WriteLine( "..." );
		fileNew.Close();
	}
	else
	{
		m_nCode = 9;
		m_nError = NETWORK_ERROR_FILE_APPEND;
		return;
	}

	rename( strFile1, strFile2 );

	if ( ::FileExists( strFile1 ) == TRUE )
	{
		m_nCode = 10;
		m_nError = NETWORK_ERROR_FILE_RENAME;
		return;
	}
	
	if ( ::FileExists( strFile2 ) == FALSE )
	{
		m_nCode = 11;
		m_nError = NETWORK_ERROR_FILE_RENAME;
		return;
	}

	if ( CopyFile( strFile2, strFile1, TRUE ) == FALSE )
	{
		m_nCode = 12;
		m_nError = NETWORK_ERROR_FILE_COPY;
		return;
	}

	if ( ::FileExists( strFile1 ) == FALSE )
	{
		m_nCode = 13;
		m_nError = NETWORK_ERROR_FILE_COPY;
		return;
	}
	
	FileRemover.RemoveFile( strFile1 );

	if ( ::FileExists( strFile1 ) == TRUE )
	{
		m_nCode = 14;	
		m_nError = NETWORK_ERROR_FILE_DELETE;
		return;
	}

	FileRemover.RemoveFile( strFile2 );

	if ( ::FileExists( strFile2 ) == TRUE )
	{
		m_nCode = 15;	
		m_nError = NETWORK_ERROR_FILE_DELETE;
		return;
	}

	FileRemover.RemoveFolder( strPath2 );

	if ( ::FileExists( strFile2 ) == TRUE )
	{
		m_nCode = 16;
		m_nError = NETWORK_ERROR_FOLDER_DELETE;
		return;
	}
}

/**********************************************************************/

void CNetworkTester::ShowError()
{
	CString strApp = AfxGetAppName();

	CString strError;
	switch( m_nError )
	{
	case NETWORK_ERROR_FOLDER_DELETE:
		strError = "Unable to delete an existing folder";
		break;

	case NETWORK_ERROR_FOLDER_RENAME:	
		strError = "Unable to rename an existing folder";
		break;

	case NETWORK_ERROR_FOLDER_CREATE:
		strError = "Unable to create a new folder";	
		break;

	case NETWORK_ERROR_FILE_DELETE:
		strError = "Unable to delete an existing file";
		break;

	case NETWORK_ERROR_FILE_RENAME:
		strError = "Unable to rename an existing file";
		break;

	case NETWORK_ERROR_FILE_CREATE:
		strError = "Unable to create a new file";
		break;

	case NETWORK_ERROR_FILE_APPEND:	
		strError = "Unable to append to an existing file";
		break;

	case NETWORK_ERROR_FILE_COPY:	
		strError = "Unable to copy an existing file";
		break;

	default:						
		strError = "Unknown file or folder error";
		break;
	}

	CString strMsg = "";
	strMsg.Format ( "%s has detected a possible network access problem.\n\nError# %d - %s\n\nPlease check that you have full access rights to %s\ndata folders and then restart %s to try again",
		(const char*) strApp,
		m_nCode,
		(const char*) strError,
		(const char*) strApp,
		(const char*) strApp );

	Prompter.Error( strMsg );
}

/**********************************************************************/
