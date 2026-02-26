#pragma once
/**********************************************************************/

static const char* szEnqFilename = "sync.enq";
static const char* szAckFilename = "sync.ack";
static const char* szEnqFileTmp = "sync.tmp";

/**********************************************************************/

class CSPOSCommsConnect
{
public:
	CSPOSCommsConnect();
	~CSPOSCommsConnect();
	bool Connect ( int nNetworkIdx, const char* szLocationName, const char* szNetworkPath, CWnd* pStatusWnd );
	void Disconnect( bool bForceLog = FALSE );
	void Terminate();

private:
	bool AccessWait ( const char* szPath, DWORD dwMSecs );
	void UserWaitLoop ( DWORD dwMSecs );
	void DisplayStatus ( const char* szText );
	void LogMessage ( const char* szType, const char* szError, bool bHide=FALSE );

private:
	int m_nNetworkIdx;
	CWnd* m_pStatusWnd;
	bool m_bUserExit;
	CString m_strError;
};

/**********************************************************************/

