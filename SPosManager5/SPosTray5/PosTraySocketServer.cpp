/**********************************************************************/
#ifdef POSTRAY_SOCKET
/**********************************************************************/
#include "NodeTypes.h"
#include "ReportTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTraySocketServer.h"
/**********************************************************************/

CPosTraySocketServer::CPosTraySocketServer ( int nMaxConnections, DWORD dwExitTimeout )
	: CSSServer ( nMaxConnections, dwExitTimeout )
{
	m_bEnabled = TRUE;			// enabled
	m_bLogData = FALSE;			// dont log data
	m_strLogFilename = "";		// no logging at all
}

/**********************************************************************/

bool CPosTraySocketServer::StartListening ( UINT uiPort )
{
	CString strLog;
	bool bReply;

	if (( bReply = CSSServer::StartListening ( uiPort )) == TRUE )
		strLog = "Server started";
	else
		strLog.Format ( "Server not started (%s)", GetError() );

	WriteLog ( strLog );

	return bReply;
}

/**********************************************************************/

bool CPosTraySocketServer::StopListening()
{
	CString strLog;
	bool bReply;

	if (( bReply = CSSServer::StopListening()) == TRUE )
		strLog = "Server stopped";
	else
		strLog.Format ( "Server stopped (%s)", GetError() );

	WriteLog ( strLog );

	return bReply;
}

/**********************************************************************/

bool CPosTraySocketServer::RemoteStopListening()
{
	CString strLog;
	bool bReply;

	if (( bReply = CSSServer::RemoteStopListening()) == TRUE )
		strLog = "Server stopped";
	else
		strLog.Format ( "Server stopped (%s)", GetError() );

	WriteLog ( strLog );

	return bReply;
}

/**********************************************************************/

void CPosTraySocketServer::OnConnect ( CSSSocket& sockConnect, int nThreadNo, bool& bTerminate )
{
	CString strLogFilename;
	if ( m_bLogData == TRUE )
	{
		strLogFilename = m_strLogFilename;
	}

	CString strThreadNo;
	strThreadNo.Format( "%2.2d ", nThreadNo );
	
	WriteLog( strThreadNo + "Socket Connect" );

	CPosTraySocket sock ( 30000, 30000, 30000, strLogFilename );
	sock.Attach ( sockConnect.Detach() );

	CPosTraySocketMessage Message;
	if ( sock.GetMessage ( Message ) == TRUE )
	{
		WriteLog( strThreadNo + "Got Message" );

		Message.CreateParamsFilename();
		Message.CreatePendingFolder();
		Message.WriteParamsFile();

		CString strExe = ".\\SPosTray.Exe";
		CString strStockmanFlag = ( SysInfo.IsStockSystem() ) ? "1" : "0";
				
		CString strDataPathNo;
		strDataPathNo.Format( "%d", SysInfo.GetDataPathNo() );

		SSspawnl( NULL, P_NOWAIT, strExe, strExe, 
			"901",
			strStockmanFlag,
			strDataPathNo,
			Message.GetParamsFileName(),
			NULL );

		if ( ( Message.AwaitReply( 60000, bTerminate ) == TRUE ) && ( FALSE == bTerminate ) )
		{
			CString strFilePath = Message.GetHTMLFilePath(); 
			sock.SendRMTIFileData( strFilePath );
			WriteLog( strThreadNo + "Sent Report Reply" );
		}
		else if ( FALSE == bTerminate )
		{
			sock.SendRMTIString( Message.CreateHTMLResponseString( "TIMEOUT" ) );
			WriteLog( strThreadNo + "Sent TimeOut Reply" );
		}
		else
		{
			WriteLog( strThreadNo + "Got Terminate Message" );
		}

		Message.DeletePendingFiles();
	}
	else
	{
		sock.SendRMTIString( Message.CreateHTMLResponseString( "BAD MESSAGE" ) );
		WriteLog( strThreadNo + "Sent Bad Message Reply" );
	}

	sock.Close();
}

/**********************************************************************/

void CPosTraySocketServer::WriteLog ( const char* szText )
{
	if ( m_strLogFilename != "" )
	{
		SYSTEMTIME t;
		GetLocalTime ( &t );

		CString strLog;
		strLog.Format ( "%2.2d-%2.2d-%4.4d %2.2d:%2.2d:%2.2d.%3.3d  %s", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds, szText );

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

/*
void CSmartServer::DoDisableServer ( CSmartSocket* pSock, CString& strData, int nLength )
{
	if ( nLength == SMART_SERVER_LENGTH_DISABLE_SERVER )
	{
		Disable();
		pSock->SendAck ( SMART_SERVER_ERROR_OK );
		OnRemoteDisableServer();	// inform user
	}
	else
		pSock->SendAck ( SMART_SERVER_ERROR_BLOCK_LENGTH );
}
*/

/**********************************************************************/

/*
void CSmartServer::DoEnableServer ( CSmartSocket* pSock, CString& strData, int nLength )
{
	if ( nLength == SMART_SERVER_LENGTH_ENABLE_SERVER )
	{
		Enable();
		pSock->SendAck ( SMART_SERVER_ERROR_OK );
		OnRemoteEnableServer();	// inform user
	}
	else
		pSock->SendAck ( SMART_SERVER_ERROR_BLOCK_LENGTH );
}
*/

/**********************************************************************/

/*
void CSmartServer::DoExitServer ( CSmartSocket* pSock, CString& strData, int nLength )
{
	if ( nLength == SMART_SERVER_LENGTH_EXIT_SERVER )
	{
		pSock->SendAck ( SMART_SERVER_ERROR_OK );
		RemoteStopListening();
		OnRemoteExitServer();	// inform user
	}
	else
		pSock->SendAck ( SMART_SERVER_ERROR_BLOCK_LENGTH );
}
*/

/**********************************************************************/
