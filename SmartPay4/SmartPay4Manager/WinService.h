/**********************************************************************/
#pragma once
/**********************************************************************/
#include <WinSvc.h>
/**********************************************************************/

class CWinService
{
public:
	CWinService( bool bBackground );
	virtual ~CWinService();

public:
	void Init( const char* szServiceName, const char* szServiceDisplayName, const char* szServiceDescription, DWORD dwControlsAccepted = SERVICE_ACCEPT_STOP );
	const char* GetError() { return m_strError; }
	void IsInstalled( bool& bIsKnown, bool& bIsInstalled );
	void IsRunning(  bool& bIsKnown, bool& bIsRunning );

public:
	BOOL ServiceInstall();		// installs the service if not already installed ( if false display GetError() )
	BOOL ServiceUninstall();		// uninstalls the service if installed ( if false display GetError() )
	BOOL ServiceStart();		// starts the service ( if false display GetError() )
	BOOL ServiceStop();			// stops the service ( if false display GetError() )

private:
	BOOL Uninstall();
	BOOL Install();

private:
	CString GetLastErrorMessage();

private:
	char m_szServiceName[64];
	const char* m_szServiceDisplayName;
	const char* m_szServiceDescription;
	SERVICE_STATUS_HANDLE m_hServiceStatus;
	SERVICE_STATUS m_Status;
	CString m_strError;

private:
	bool m_bBackground;
};

/**********************************************************************/
