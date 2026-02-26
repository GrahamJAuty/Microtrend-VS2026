/**********************************************************************/
#pragma once
/**********************************************************************/

#include <WinSvc.h>

#define SERVICE_CONTROL_USER 128

/**********************************************************************/

class CWinService
{
public:
	CWinService ( const char* szServiceName, const char* szServiceDisplayName, const char* szServiceDescription, DWORD dwControlsAccepted = SERVICE_ACCEPT_STOP );
	virtual ~CWinService();

	BOOL ServiceRun();			// runs the service ( returns when service is stopped ) ( if false display GetError() )

	BOOL ParseStandardArgs ( int argc, char* argv[] );	// parse command line arguments ( /install, /uninstall, /start, /stop )
												// true if argument has been processed, display GetError()
												// false if argument has not been processed, display GetError() (may be multiline)
	const char* GetError() { return m_strError; }

public:
	virtual void Run() = 0;			// function to do the real work (must be implemented)
	virtual BOOL OnInit();			// Called when the service is first initialized (return true if ok)
	virtual void OnStop();			// Called when the service control manager wants to stop the service
	virtual void OnInterrogate();		// called when the service control manager wants to interrogate the service
	virtual void OnPause();			// called when the service control manager wants to pause the service
	virtual void OnContinue();		// called when the service control manager wants to continue the service
	virtual void OnShutdown();		// called when the service control manager wants to shut down the service

	virtual BOOL OnUserControl ( DWORD dwOpcode );	// called when the service gets a user control message (return true to say handled)

	// the following functions are used by CServiceSetupDlg()

public:
	BOOL ServiceInstall();		// installs the service if not already installed ( if false display GetError() )
	BOOL ServiceUninstall();		// uninstalls the service if installed ( if false display GetError() )
	BOOL ServiceStart();		// starts the service ( if false display GetError() )
	BOOL ServiceStop();			// stops the service ( if false display GetError() )

private:
	BOOL IsInstalled();
	BOOL RunService();
	BOOL Install();
	BOOL Uninstall();
	void SetStatus ( DWORD dwState );
	BOOL Initialize();
	DWORD GetExitCode() { return m_Status.dwWin32ExitCode; }
	CString GetLastErrorMessage();

	// static member functions
private:
	static void WINAPI ServiceMain ( DWORD dwArgc, LPTSTR* lpszArgv );
	static void WINAPI Handler ( DWORD dwOpcode );

	// static data
private:
	static CWinService* m_pThis;

	// data members
private:
	char m_szServiceName[64];
	const char* m_szServiceDisplayName;
	const char* m_szServiceDescription;
	SERVICE_STATUS_HANDLE m_hServiceStatus;
	SERVICE_STATUS m_Status;
	CString m_strError;
};

/**********************************************************************/

extern const char* szVERSION_WINSERVICE;

/**********************************************************************/
