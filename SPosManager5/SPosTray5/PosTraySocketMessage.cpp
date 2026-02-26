/**********************************************************************/
#ifdef POSTRAY_SOCKET
/**********************************************************************/
#include "PosTraySocketMessage.h"
/**********************************************************************/

CPosTraySocketMessage::CPosTraySocketMessage(void)
{
	Reset();
}

/**********************************************************************/

void CPosTraySocketMessage::Reset()
{
	m_strIntro = "";
	m_strHeader = "";
	m_strBody = "";
}

/**********************************************************************/

void CPosTraySocketMessage::GetPosTraySocketFilePath( CString& strFilename )
{
	CString strPath = "PosTray\\RMTIPending";
	
	if ( strFilename != "" )
	{
		strPath += "\\";
		strPath += strFilename;
	}
	
	strFilename = strPath;
	::GetSyssetProgramPath( strFilename );
}

/**********************************************************************/

void CPosTraySocketMessage::ParseData()
{
	ParseDataInternal ( m_strHeader, m_arrayHeaderProperties );
	ParseDataInternal ( m_strBody, m_arrayBodyProperties );
	m_strHeader = "";
	m_strBody = "";
}

/**********************************************************************/

void CPosTraySocketMessage::ParseDataInternal( CString& strData, CReportConsolidationArray<CSortedStringByString>& arrayProperties )
{
	arrayProperties.RemoveAll();
	
	int nStartPos = 0;
	int nNewLinePos = 0;

	int nLength = strData.GetLength();

	while ( ( nNewLinePos = strData.Find( "\x0A", nStartPos ) ) != -1 )
	{
		CString strChunk = strData.Mid( nStartPos, nNewLinePos - nStartPos );
		ParseChunk( strChunk, arrayProperties );
		nStartPos = nNewLinePos + 1;
	}

	if ( nStartPos < nLength - 1 )
	{
		CString strChunk = strData.Mid( nStartPos, nLength - nStartPos  );
		ParseChunk( strChunk, arrayProperties );
	}
}

/**********************************************************************/

void CPosTraySocketMessage::ParseChunk( CString& strChunk, CReportConsolidationArray<CSortedStringByString>& arrayProperties )
{
	int nChunkLength = strChunk.GetLength();

	int nEqualsPos = strChunk.Find( "=" );
	if ( ( nEqualsPos > 0 ) && ( nEqualsPos < nChunkLength - 1 ) )
	{
		CSortedStringByString item;
		item.m_strKey = strChunk.Left(nEqualsPos);
		item.m_strData = strChunk.Right( nChunkLength - ( nEqualsPos + 1 ) );
		arrayProperties.Consolidate( item );
	}
}

/**********************************************************************/

void CPosTraySocketMessage::CreateParamsFilename()
{
	m_strParamsFilename.Format( "%10.10d", GetTickCount() );
}

/**********************************************************************/

void CPosTraySocketMessage::SetParamsFilename( const char* sz )
{
	m_strParamsFilename = sz;
}

/**********************************************************************/

void CPosTraySocketMessage::CreatePendingFolder()
{
	CString strFolder = "";
	GetPosTraySocketFilePath( strFolder );
	::CreateSubdirectory( strFolder );
}

/**********************************************************************/

void CPosTraySocketMessage::WriteParamsFile()
{
	CSSFile fileParams;
	if ( fileParams.Open( GetParamsFilePath(), "wb" ) == TRUE )
	{
		for ( int n = 0; n < m_arrayBodyProperties.GetSize(); n++ )
		{
			CSortedStringByString item;
			m_arrayBodyProperties.GetAt( n, item );
			
			CString strLine = "";
			strLine += item.m_strKey;
			strLine += "=";
			strLine += item.m_strData;
			fileParams.WriteLine( strLine );
		}
	}
}

/**********************************************************************/

void CPosTraySocketMessage::DeletePendingFiles()
{
	if ( m_strParamsFilename != "" )
	{
		CString strParamsPath = GetParamsFilePath();
		remove( strParamsPath );
		remove( strParamsPath + ".htm" );
		remove( strParamsPath + ".rep" );
	}
}

/**********************************************************************/

const char* CPosTraySocketMessage::GetParamsFilePath()
{
	m_strParamsFilePath = m_strParamsFilename;
	GetPosTraySocketFilePath( m_strParamsFilePath );
	return m_strParamsFilePath;
}

/**********************************************************************/

const char* CPosTraySocketMessage::GetHTMLFilePath()
{
	m_strHTMLFilePath = GetParamsFilePath();
	m_strHTMLFilePath += ".HTM";
	return m_strHTMLFilePath;
}

/**********************************************************************/

bool CPosTraySocketMessage::AwaitReply( int nTimeOut, bool& bTerminate )
{
	DWORD dwTickCountQuit = GetTickCount() + nTimeOut;
	CString strFilePath = GetHTMLFilePath();

	while ( ( GetTickCount() < dwTickCountQuit ) && ( FALSE == bTerminate ) )
	{
		if ( ::FileExists( strFilePath ) == TRUE )
		{
			return TRUE;
		}

		Sleep(100);
	}

	return ::FileExists( strFilePath );
}

/**********************************************************************/

void CPosTraySocketMessage::CreateHTMLResponseFile( const char* szMessage )
{
	CSSFile fileResponse;
	if ( fileResponse.Open( GetHTMLFilePath(), "wb" ) == TRUE )
	{
		CString strMessage;
		strMessage.Format( "<body><p>%s</p></body>", szMessage );
		fileResponse.WriteLine( strMessage );	
	}
}

/**********************************************************************/

const char* CPosTraySocketMessage::CreateHTMLResponseString( const char* szMessage )
{
	m_strHTMLResponse.Format( "<body><p>%s</p></body>", szMessage );
	return m_strHTMLResponse;
}

/**********************************************************************/
#endif
/**********************************************************************/


