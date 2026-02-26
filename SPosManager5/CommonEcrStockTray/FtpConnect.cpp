/**********************************************************************/
#include "CommsErrors.h"
#include "FNBDatabaseOptions.h"
#include "..\CommonEcrStock\IDraughtOptions.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include "FtpConnect.h"
/**********************************************************************/

CFtpTransferInfo::CFtpTransferInfo()
{
	Reset();
}

/**********************************************************************/

void CFtpTransferInfo::Reset()
{
	m_nFailedRename = 0;
	m_nFailedTransfer = 0;
	m_nFilesCopied = 0;
	m_nFailedDelete = 0;
	m_nPutAside = 0;
}

/**********************************************************************/

CFtpConnect::CFtpConnect() : m_InternetSession( "Microtrend" )
{
	m_pFtpConnection = NULL;
	m_nLastError = 0;
	m_bWantErrorDisconnect = FALSE;

	m_strCurrentServer = "";
	m_strCurrentUserName = "";
	m_strCurrentPassword = "";
	m_nCurrentPortNo = 0;
}

/**********************************************************************/

CFtpConnect::~CFtpConnect(void)
{
	Disconnect();
}

/**********************************************************************/

void CFtpConnect::Disconnect()
{
	if ( m_pFtpConnection != NULL )
	{
		m_pFtpConnection -> Close();
		delete m_pFtpConnection;
		m_pFtpConnection = NULL;

		m_strCurrentServer = "";
		m_strCurrentUserName = "";
		m_strCurrentPassword = "";
		m_nCurrentPortNo = 0;
		m_bWantErrorDisconnect = FALSE;
	}
}

/**********************************************************************/

bool CFtpConnect::ConnectIDraught()
{
#ifdef POSTRAY_UTILITY
	return FALSE;
#else

	CString strServer = IDraughtOptions.GetFTPServer();
	CString strUserName = IDraughtOptions.GetFTPUserName();
	CString strPassword = IDraughtOptions.GetFTPPassword();
	int nPortNo = 21;

	Disconnect();

	try
	{
		m_pFtpConnection = m_InternetSession.GetFtpConnection( strServer, strUserName, strPassword, nPortNo, TRUE );
	}
	catch (CInternetException* pEx) 
	{
		m_nLastError = pEx -> m_dwError;		
		pEx->Delete();
	}

	if ( m_pFtpConnection != NULL )
	{
		m_bWantErrorDisconnect = FALSE;
		return TRUE;
	}
	else
		return FALSE;
#endif
}

/**********************************************************************/

bool CFtpConnect::ConnectFNBExport( CFNBDatabaseOptions& Options )
{
	CString strServer = Options.GetFTPServer();
	CString strUserName = Options.GetFTPUserName();
	CString strPassword = Options.GetFTPPassword();
	int nPortNo = 21;

	Disconnect();

	try
	{
		m_pFtpConnection = m_InternetSession.GetFtpConnection( strServer, strUserName, strPassword, nPortNo, TRUE );
	}
	catch (CInternetException* pEx) 
	{
		m_nLastError = pEx -> m_dwError;		
		pEx->Delete();
	}

	if ( m_pFtpConnection != NULL )
	{
		m_bWantErrorDisconnect = FALSE;
		return TRUE;
	}
	else
		return FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CFtpConnect::ConnectChartwellsExport(CChartwellsOptions& Options)
{
	CString strServer = Options.GetFTPServer();
	CString strUserName = Options.GetFTPUserName();
	CString strPassword = Options.GetFTPPassword();
	int nPortNo = 21;

	Disconnect();

	try
	{
		m_pFtpConnection = m_InternetSession.GetFtpConnection(strServer, strUserName, strPassword, nPortNo, TRUE);
	}
	catch (CInternetException* pEx)
	{
		m_nLastError = pEx->m_dwError;
		pEx->Delete();
	}

	if (m_pFtpConnection != NULL)
	{
		m_bWantErrorDisconnect = FALSE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CFtpConnect::ConnectS4LabourExport()
{
	CString strServer = PosTrayOptions.GetS4LabourFTPServer();
	CString strUserName = PosTrayOptions.GetS4LabourFTPUserName();
	CString strPassword = PosTrayOptions.GetS4LabourFTPPassword();
	int nPortNo = 21;

	Disconnect();

	try
	{
		m_pFtpConnection = m_InternetSession.GetFtpConnection( strServer, strUserName, strPassword, nPortNo, TRUE );
	}
	catch (CInternetException* pEx) 
	{
		m_nLastError = pEx -> m_dwError;		
		pEx->Delete();
	}

	if ( m_pFtpConnection != NULL )
	{
		m_bWantErrorDisconnect = FALSE;
		return TRUE;
	}
	else
		return FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CFtpConnect::ConnectPosTrayExport()
{
	CString strServer = PosTrayOptions.GetFTPServer();
	CString strUserName = PosTrayOptions.GetFTPUserName();
	CString strPassword = PosTrayOptions.GetFTPPassword();
	int nPortNo = 21;

	Disconnect();

	try
	{
		m_pFtpConnection = m_InternetSession.GetFtpConnection( strServer, strUserName, strPassword, nPortNo, TRUE );
	}
	catch (CInternetException* pEx) 
	{
		m_nLastError = pEx -> m_dwError;		
		pEx->Delete();
	}

	if ( m_pFtpConnection != NULL )
	{
		m_bWantErrorDisconnect = FALSE;
		return TRUE;
	}
	else
		return FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CFtpConnect::PushDirectory( const char* szPath )
{
	bool bResult = PushDirectoryInternal( szPath );

	if ( FALSE == bResult )
		m_bWantErrorDisconnect = TRUE;

	return bResult;

}

/**********************************************************************/

bool CFtpConnect::PushDirectoryInternal( const char* szPath )
{
	if ( m_pFtpConnection == NULL )
		return FALSE;

	CString strOldPath;
	if ( GetCurrentDirectory( strOldPath ) == FALSE )
		return FALSE;

	bool bResult = FALSE;

	try
	{
		bResult = ( m_pFtpConnection -> SetCurrentDirectory( szPath ) == TRUE );
	}
	catch(CInternetException* pEx)
	{
		m_nLastError = pEx -> m_dwError;		
		pEx -> Delete();
		return FALSE;
	}

	if ( bResult == FALSE )
		m_nLastError = GetLastError();
	else
		m_arrayFolderStack.Add( strOldPath );

	return bResult;
}

/**********************************************************************/

bool CFtpConnect::PopDirectory( CString& strOldPath )
{
	strOldPath = "";
	bool bResult = PopDirectoryInternal( strOldPath );

	if ( FALSE == bResult )
		m_bWantErrorDisconnect = TRUE;

	return bResult;
}

/**********************************************************************/

bool CFtpConnect::PopDirectoryInternal( CString& strOldPath )
{
	if ( m_pFtpConnection == NULL )
		return FALSE;

	if ( m_arrayFolderStack.GetSize() == 0 )
		return FALSE;

	strOldPath = m_arrayFolderStack.GetAt( m_arrayFolderStack.GetSize() - 1 );

	bool bResult = FALSE;

	try
	{
		bResult = ( m_pFtpConnection -> SetCurrentDirectory( strOldPath ) == TRUE );
	}
	catch(CInternetException* pEx)
	{
		m_nLastError = pEx -> m_dwError;		
		pEx -> Delete();
		return FALSE;
	}

	if ( bResult == FALSE )
	{
		m_nLastError = GetLastError();
		return FALSE;
	}

	CString strTestPath;
	if ( GetCurrentDirectory( strTestPath ) == FALSE )
		return FALSE;

	if ( strTestPath != strOldPath )
		return FALSE;

	m_arrayFolderStack.RemoveAt( m_arrayFolderStack.GetSize() - 1 );
	return TRUE;
}

/**********************************************************************/

bool CFtpConnect::GetCurrentDirectory( CString& strDirectory )
{
	if ( m_pFtpConnection == NULL )
		return FALSE;

	bool bResult = FALSE;

	try
	{
		bResult = ( m_pFtpConnection -> GetCurrentDirectory( strDirectory ) == TRUE );
	}
	catch(CInternetException* pEx)
	{
		m_nLastError = pEx -> m_dwError;		
		pEx -> Delete();
		return FALSE;
	}

	if ( bResult == FALSE )
		m_nLastError = GetLastError();

	return bResult;
}

/**********************************************************************/

bool CFtpConnect::CreateDirectory( const char* szPath )
{
	if ( m_pFtpConnection == NULL )
		return FALSE;

	bool bResult = FALSE;

	try
	{
		bResult = ( m_pFtpConnection -> CreateDirectory( szPath ) == TRUE );
	}
	catch(CInternetException* pEx)
	{
		m_nLastError = pEx -> m_dwError;		
		pEx -> Delete();
		return FALSE;
	}

	if ( bResult == FALSE )
		m_nLastError = GetLastError();

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CFtpConnect::OpenFile( CInternetFile** pFile, const char* szFilename, DWORD dwType )
{
	*pFile = NULL;

	if ( m_pFtpConnection == NULL )
		return FALSE;

	try
	{
		*pFile = m_pFtpConnection -> OpenFile( szFilename, dwType );
	}
	catch(CInternetException* pEx)
	{
		pEx -> Delete();
		return FALSE;
	}

	return ( *pFile != NULL );
}

/**********************************************************************/

bool CFtpConnect::ReadFromFile( CInternetFile* pFile, CString& strBuffer )
{
	if ( pFile == NULL )
		return FALSE;
	
	bool bResult = FALSE;

	try
	{
		bResult = ( pFile -> ReadString( strBuffer ) == TRUE );
	}
	catch(CInternetException* pEx)
	{
		pEx -> Delete();
		return FALSE;
	}

	return bResult;
}

/**********************************************************************/

bool CFtpConnect::WriteToFile( CInternetFile* pFile, const char* sz )
{
	if ( pFile == NULL )
		return FALSE;
	
	bool bResult = FALSE;

	try
	{
		CString str = sz;
		pFile -> Write( str, str.GetLength() );
		bResult = TRUE;
	}
	catch(CInternetException* pEx)
	{
		pEx -> Delete();
		return FALSE;
	}

	return bResult;
}

/**********************************************************************/

bool CFtpConnect::CloseFile( CInternetFile* pFile )
{
	if ( pFile == NULL )
		return FALSE;
	
	bool bResult = FALSE;

	try
	{
		pFile -> Close();
		bResult = TRUE;
	}
	catch(CInternetException* pEx)
	{
		pEx -> Delete();
		return FALSE;
	}

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CFtpConnect::GetFile( const char* szSource, const char* szDest, bool bFailIfExists )
{
	if ( m_pFtpConnection == NULL )
		return FALSE;

	bool bResult = FALSE;

	try
	{
		bResult = ( m_pFtpConnection -> GetFile( szSource, szDest, bFailIfExists ) == TRUE );
	}
	catch(CInternetException* pEx)
	{
		m_nLastError = pEx -> m_dwError;		
		pEx -> Delete();
		return FALSE;
	}

	if ( bResult == FALSE )
		m_nLastError = GetLastError();

	return bResult;
}

/**********************************************************************/

bool CFtpConnect::PutFile( const char* szSource, const char* szDest )
{
	if ( m_pFtpConnection == NULL )
		return FALSE;

	bool bResult = FALSE;

	try
	{
		bResult = ( m_pFtpConnection -> PutFile( szSource, szDest ) == TRUE );
	}
	catch(CInternetException* pEx)
	{
		m_nLastError = pEx -> m_dwError;		
		pEx -> Delete();
		return FALSE;
	}

	if ( bResult == FALSE )
		m_nLastError = GetLastError();

	return bResult;
}

/**********************************************************************/

bool CFtpConnect::DeleteFile( const char* szFile )
{
	if ( m_pFtpConnection == NULL )
		return FALSE;

	bool bResult = FALSE;

	try
	{
		bResult = ( m_pFtpConnection -> Remove ( szFile ) == TRUE );
	}
	catch(CInternetException* pEx)
	{
		m_nLastError = pEx -> m_dwError;		
		pEx -> Delete();
		return FALSE;
	}

	if ( bResult == FALSE )
		m_nLastError = GetLastError();

	return bResult;
}

/**********************************************************************/

bool CFtpConnect::RenameFile( const char* szOld, const char* szNew )
{
	if ( m_pFtpConnection == NULL )
		return FALSE;

	bool bResult = FALSE;

	try
	{
		bResult = ( m_pFtpConnection ->Rename( szOld, szNew ) == TRUE );
	}
	catch(CInternetException* pEx)
	{
		m_nLastError = pEx -> m_dwError;		
		pEx -> Delete();
		return FALSE;
	}

	if ( bResult == FALSE )
		m_nLastError = GetLastError();

	return bResult;
}

/**********************************************************************/

bool CFtpConnect::FileExists( const char* szFile )
{
	if ( m_pFtpConnection == NULL )
		return FALSE;

	bool bResult = FALSE;

	try
	{
		CFtpFileFind finder( m_pFtpConnection );
		bResult = ( finder.FindFile( szFile ) == TRUE );
	}
	catch(CInternetException* pEx)
	{
		pEx -> Delete();
		return FALSE;
	}

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CFtpConnect::GetFilteredFileList( CReportConsolidationArray<CSortedStringItem>& arrayFiles, CStatic& staticInfo, int nFilterType )
{
	arrayFiles.RemoveAll();

	bool bResult = GetFilteredFileListInternal( arrayFiles, staticInfo, nFilterType );

	if ( FALSE == bResult )
		m_bWantErrorDisconnect = FALSE;

	return bResult;
}

/**********************************************************************/

bool CFtpConnect::GetFilteredFileListInternal( CReportConsolidationArray<CSortedStringItem>& arrayFiles, CStatic& staticInfo, int nFilterType )
{
	arrayFiles.RemoveAll();

	if ( m_pFtpConnection == NULL )
		return FALSE;

	bool bResult = FALSE;

	try
	{
		CInternetFile* pFile = m_pFtpConnection ->Command( "NLST", CFtpConnection::CmdRespRead );
	
		if ( pFile != 0 )
		{
			CString strFilename;
			while ( pFile -> ReadString( strFilename ) == TRUE )
			{
				CString strInfo = "";

				switch( nFilterType )
				{
				case FTP_FILTERTYPE_FILE_SSFILE:
					strInfo.Format( "Finding CASHRSP files %d", arrayFiles.GetSize() + 1 );
					break;

				case FTP_FILTERTYPE_FOLDER_EOD:
					strInfo.Format( "Finding EOD folders %d", arrayFiles.GetSize() + 1 );
					break;

				case FTP_FILTERTYPE_FOLDER_DATE:
					strInfo.Format( "Finding date folders %d", arrayFiles.GetSize() + 1 );
					break;

				case FTP_FILTERTYPE_FILE_REASON:
					strInfo.Format( "Finding EOD reason files %d", arrayFiles.GetSize() + 1 );
					break;

				case FTP_FILTERTYPE_FILE_PRODCSV:
					strInfo.Format( "Finding EOD product files %d", arrayFiles.GetSize() + 1 );
					break;

				case FTP_FILTERTYPE_FILE_CUSTOMER:
					strInfo.Format( "Finding EOD customer files %d", arrayFiles.GetSize() + 1 );
					break;
				}

				staticInfo.SetWindowText( strInfo );

				strFilename.MakeUpper();

				switch( nFilterType )
				{
				case FTP_FILTERTYPE_FILE_SSFILE:
					{
						if ( strFilename.GetLength() == 31 )
						{
							if ( strFilename.Left(7) == "SSFILE_" )
								if ( ::TestNumeric( strFilename.Mid(7,8) ) == TRUE )
									if ( strFilename.Mid(15,1) == "_" )
										if ( ::TestNumeric( strFilename.Mid(16,6) ) == TRUE )
											if ( strFilename.Mid(22,6) == "_TERM." ) 
												if ( ::TestNumeric( strFilename.Right(3) ) == TRUE )
												{
													CSortedStringItem item;
													item.m_strItem = strFilename;
													arrayFiles.Consolidate( item );
												}
						}
						else if ( strFilename.GetLength() == 35 )
						{
							if ( strFilename.Left(7) == "SSFILE_" )
								if ( ::TestNumeric( strFilename.Mid(7,8) ) == TRUE )
									if ( strFilename.Mid(15,1) == "_" )
										if ( ::TestNumeric( strFilename.Mid(16,6) ) == TRUE )
											if ( strFilename.Mid(22,10) == "_TERMDPST." ) 
												if ( ::TestNumeric( strFilename.Right(3) ) == TRUE )
												{
													CSortedStringItem item;
													item.m_strItem = strFilename;
													arrayFiles.Consolidate( item );
												}
						}
					}
					break;

				case FTP_FILTERTYPE_FOLDER_EOD:
					{
						if ( strFilename.GetLength() >= 3 )
							if ( strFilename.Left(3) == "EOD" )
							{
								CSortedStringItem item;
								item.m_strItem = strFilename;
								arrayFiles.Consolidate( item );
							}
					}
					break;

				case FTP_FILTERTYPE_FILE_PRODCSV:
					{
						if ( strFilename.GetLength() >= 8 )
							if ( strFilename.Left(4) == "PROD" )
								if ( strFilename.Right(4) == ".CSV" )
								{
									CSortedStringItem item;
									item.m_strItem = strFilename;
									arrayFiles.Consolidate( item );
								}
					}
					break;

				case FTP_FILTERTYPE_FILE_CUSTOMER:
					{
						if ( strFilename.GetLength() >= 13 )
							if ( strFilename.Left(8) == "CUSTOMER" )
								if ( strFilename.Right(4) == ".SQL" )
								{
									CSortedStringItem item;
									item.m_strItem = strFilename;
									arrayFiles.Consolidate( item );
								}
					}
					break;

				case FTP_FILTERTYPE_FOLDER_DATE:
					{
						if ( strFilename.GetLength() == 8 )
							if ( ::TestNumeric( strFilename ) == TRUE )
							{
								CSortedStringItem item;
								item.m_strItem = strFilename;
								arrayFiles.Consolidate( item );
							}
					}
					break;

				case FTP_FILTERTYPE_FILE_REASON:
					{
						if ( ( strFilename == "REASONS.CON" ) || ( strFilename == "REASONS.CSV" ) )
						{
							CSortedStringItem item;
							item.m_strItem = strFilename;
							arrayFiles.Consolidate( item );
						}
					}
					break;
				}
			}
		
			pFile -> Close();
			delete pFile;
			
			bResult = TRUE;
		}
	}
	catch(CInternetException* pEx)
	{
		m_nLastError = pEx -> m_dwError;		
		pEx -> Delete();
		return FALSE;
	}

	if ( bResult == FALSE )
		m_nLastError = GetLastError();

	return bResult;
}

/**********************************************************************/

