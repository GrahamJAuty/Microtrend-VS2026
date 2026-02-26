/**********************************************************************/
#pragma once
/**********************************************************************/

class CCommsConnect
{
public:
	CCommsConnect ( const char* szDialFilename, const char* szDllFilename = "modem.dll" );
	~CCommsConnect();
	bool Connect ( const char* szCommMode, const char* szPhoneParams, CWnd* pParent=NULL, bool bProtect=TRUE );
	bool ConnectNetwork ( int nSyssetNetworkComms );
	void Disconnect();
	const char* GetError() { return m_strError; }
	CComms* GetPort() { return &m_Port; }
	HANDLE GetHandle() { return m_Port.GetHandle(); }

private:
	bool TestProtect();
	void DropLine();

private:
	CComms m_Port;
	CString m_strDialFilename;
	CString m_strDllFilename;
	CString m_strError;
	bool m_bPortOpen;
	bool m_bRemote;
	bool m_bNetworkComms;
};

/**********************************************************************/

class CModemAnswerSetupDialog
{
public:
	CModemAnswerSetupDialog ( const char* szAnswerFilename, const char* szDllFilename = "modem.dll", CWnd* pParent = NULL );
	int DoModal();
	const char* GetError() { return m_strError; }

private:
	CString m_strAnswerFilename;
	CString m_strDllFilename;
	CString m_strError;
};

/**********************************************************************/

class CModemDialSetupDialog
{
public:
	CModemDialSetupDialog ( const char* szDialFilename, const char* szDllFilename = "modem.dll", CWnd* pParent = NULL );
	int DoModal();
	const char* GetError() { return m_strError; }

private:
	CString m_strDialFilename;
	CString m_strDllFilename;
	CString m_strError;
};

/**********************************************************************/

class CModemDial
{
public:
	CModemDial ( const char* szDialFilename, const char* szDllFilename = "modem.dll", CWnd* pParent = NULL );
	bool Dial ( CComms* pPort, const char* szPhone, const char* szMessage = "" );
	const char* GetError() { return m_strError; }

private:
	CString m_strDialFilename;
	CString m_strDllFilename;
	CString m_strError;
};

/**********************************************************************/

bool ModemVersion ( CString& strVersion, const char* szDllFilename = "modem.dll" );

/**********************************************************************/

extern const char* szVERSION_CONNECT;

/**********************************************************************/

