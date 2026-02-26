#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#ifdef POSTRAY_SOCKET
/**********************************************************************/
#include <socket.hpp>
#include <subs.hpp>
/**********************************************************************/
#include "PosTraySocket.h"
/**********************************************************************/

class CPosTraySocketServer : public CSSServer
{
public:
	CPosTraySocketServer ( int nMaxConnections, DWORD dwExitTimeout );

	bool StartListening ( UINT uiPort );
	bool StopListening();

	void EnableLogging ( const char* szLogFilename, bool bLogData=FALSE ) { m_strLogFilename = szLogFilename; m_bLogData = bLogData; }
	void Enable() { m_bEnabled = TRUE; }
	void Disable() { m_bEnabled = FALSE; }

	virtual int OnSendData ( int nDataType, const char* szData, CString& strReply ){ return 0; }

	virtual void OnRemoteEnableServer() {}
	virtual void OnRemoteDisableServer() {}
	virtual void OnRemoteExitServer() {}

protected:
	virtual void OnConnect ( CSSSocket& sockConnect, int nThreadNo, bool& bTerminate );

private:
	bool RemoteStopListening();	// must only be called from OnConnect() routine
	void WriteLog ( const char* szText );

private:
	CString m_strLogFilename;
	bool m_bLogData;
	bool m_bEnabled;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
