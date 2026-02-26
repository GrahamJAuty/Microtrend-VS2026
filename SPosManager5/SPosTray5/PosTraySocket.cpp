/**********************************************************************/
#ifdef POSTRAY_SOCKET
/**********************************************************************/
#include "PosTraySocket.h"
/**********************************************************************/
#define POSTRAY_SOCKET_CHUNKSIZE 1024
/**********************************************************************/

CPosTraySocket::CPosTraySocket ( DWORD dwConnectTimeout, DWORD dwRxTimeout, DWORD dwTxTimeout, const char* szLogFilename )
	: CSSSocket ( dwConnectTimeout, dwRxTimeout, dwTxTimeout )
{
	m_strLogFilename = szLogFilename;
}

/**********************************************************************/

bool CPosTraySocket::GetBytes ( char* pBuf, int nBufSize, int& nBytesReceived )
{
	nBytesReceived = Receive ( pBuf, nBufSize );

	if ( nBytesReceived == SOCKET_ERROR )
	{
		nBytesReceived = 0;
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

bool CPosTraySocket::GetMessage ( CPosTraySocketMessage& Message )
{
	Message.Reset();

	if ( GetChunkedString( Message.m_strIntro, 42 ) == FALSE )
		return FALSE;

	if ( Message.m_strIntro.Left(4) != "RMTI" )
		return FALSE;

	int nHeaderSize = atoi( Message.m_strIntro.Mid(4,9) );
	int nBodySize = atoi( Message.m_strIntro.Mid(13,9) );
	
	if ( ( 0 == nHeaderSize ) || ( 0 == nBodySize ) )
		return FALSE;

	if ( GetChunkedString( Message.m_strHeader, nHeaderSize ) == FALSE )
		return FALSE;

	if ( GetChunkedString( Message.m_strBody, nBodySize ) == FALSE )
		return FALSE;

	Message.ParseData();

	return TRUE;
}

/**********************************************************************/

bool CPosTraySocket::GetChunkedString( CString& strTarget, int nExpectedLen )
{
	strTarget = "";

	char buffer[ POSTRAY_SOCKET_CHUNKSIZE + 1 ];

	{
		int nBytesToGo = nExpectedLen;

		while ( nBytesToGo > 0 )
		{
			int nChunkSize = min ( POSTRAY_SOCKET_CHUNKSIZE, nBytesToGo );
		
			int nBytesReceived = 0;
			if ( GetBytes( buffer, nChunkSize, nBytesReceived ) == FALSE )
				return FALSE;

			if ( nBytesReceived != nChunkSize )
				return FALSE;

			buffer[nBytesReceived] = 0;
			CString strChunk = buffer;

			if ( strChunk.GetLength() != nBytesReceived )
				return FALSE;

			strTarget += strChunk;
			nBytesToGo -= nChunkSize;
		}
	}

	return TRUE;
}

/**********************************************************************/

bool CPosTraySocket::PutString ( char* szText )
{
	int nLen = (int) strlen(szText);

	if ( Send ( szText, nLen ) == SOCKET_ERROR )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

bool CPosTraySocket::PutString ( CString str )
{
	return PutString ( (char*) LPCTSTR(str) );
}

/**********************************************************************/

void CPosTraySocket::SendRMTIFileData( CString& strFilename )
{
	CString strData = "";

	CSSFile file;
	if ( file.Open( strFilename, "rb" ) == TRUE )
	{
		CString strBuffer = "";
		while( file.ReadString( strBuffer ) == TRUE )
			strData += strBuffer;
	}

	SendRMTIString( strData );
}

/**********************************************************************/

void CPosTraySocket::SendRMTIString( const char* sz )
{
	CString strData = sz;
	
	CString strIntro;
	strIntro.Format( "RMTI000000000%9.9d00000000000000000000", strData.GetLength() );
	
	strData.Insert( 0, strIntro );
	PutString( strData );
}

/**********************************************************************/

bool CPosTraySocket::SendMessage( CString strData )
{
	CString strRecord = strData;

	if ( m_strLogFilename != "" )
		LogData ( "PUT " + strRecord );

	return PutString ( (char*) LPCTSTR(strRecord) );
}

/**********************************************************************/

void CPosTraySocket::LogData ( const char* szData )
{
	if ( m_strLogFilename != "" )
	{
		SYSTEMTIME t;
		GetLocalTime ( &t );

		CString strLog;
		strLog.Format ( "%2.2d-%2.2d-%4.4d %2.2d:%2.2d:%2.2d.%3.3d  %s", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds, szData );

		CSSFile file;
		if ( file.Open ( m_strLogFilename, "ab" ) == TRUE )
		{
			file.WriteLine ( strLog );
			file.Close();
		}
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
