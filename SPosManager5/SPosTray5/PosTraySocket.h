#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#ifdef POSTRAY_SOCKET
/**********************************************************************/
#include <socket.hpp>
/**********************************************************************/
#include "PosTraySocketMessage.h"
/**********************************************************************/

class CPosTraySocket : public CSSSocket
{
public:
	CPosTraySocket ( DWORD dwConnectTimeout, DWORD dwRxTimeout, DWORD dwTxTimeout, const char* szLogFilename );
	bool GetMessage ( CPosTraySocketMessage& Message );
	bool SendMessage ( CString strData );
	
public:
	void SendRMTIFileData( CString& strFilename );
	void SendRMTIString( const char* sz );

private:
	bool GetBytes ( char* pBuf, int nBufSize, int& nBytesReceived );
	bool GetChunkedString( CString& strTarget, int nExpectedLen );
	bool PutString ( char* szText );
	bool PutString ( CString str );
	void LogData ( const char* szData );

private:
	CString m_strLogFilename;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
