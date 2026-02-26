/**********************************************************************/
#pragma once
/**********************************************************************/
#include <WinSvc.h>
/**********************************************************************/
#define SERVICE_CONTROL_USER 128
/**********************************************************************/
#include "..\SPosLoyalty4Shared\LoyaltyCommandLineInfo.h"
/**********************************************************************/

class CWinService
{
public:
	CWinService();
	virtual ~CWinService();

public:
	void Init( const char* szServiceName, const char* szServiceDisplayName, const char* szServiceDescription, DWORD dwControlsAccepted = SERVICE_ACCEPT_STOP );
	
	BOOL ServiceRun();			// runs the service ( returns when service is stopped ) ( if false display GetError() )
	
	BOOL ParseStandardArgs(CLoyaltyCommandLineInfo& cmdInfo);
	
	const char* GetError() { return m_strError; }

public:
	void Run();						// function to do the real work (must be implemented)
	virtual BOOL OnInit();			// Called when the service is first initialized (return true if ok)
	virtual void OnStop();			// Called when the service control manager wants to stop the service
	virtual void OnInterrogate();		// called when the service control manager wants to interrogate the service
	virtual void OnPause();			// called when the service control manager wants to pause the service
	virtual void OnContinue();		// called when the service control manager wants to continue the service
	virtual void OnShutdown();		// called when the service control manager wants to shut down the service
	virtual void OnPreShutdown();	// called when the service control manager wants to shut down the service

	virtual BOOL OnUserControl ( DWORD dwOpcode );	// called when the service gets a user control message (return true to say handled)

	// the following functions are used by CServiceSetupDlg()

public:
	BOOL ServiceInstall( bool bLogon, CString strUsername, CString strPassword );		// installs the service if not already installed ( if false display GetError() )
	BOOL ServiceUninstall();	// uninstalls the service if installed ( if false display GetError() )
	BOOL ServiceStart();		// starts the service ( if false display GetError() )
	BOOL ServiceStop();			// stops the service ( if false display GetError() )
	void ServiceStatus();		// checks whether the service is installed and running

private:
	void IsInstalled( bool& bIsKnown, bool& bIsInstalled );
	void IsRunning( bool& bIsKnown, bool& bIsRunning );

private:
	BOOL RunService();
	BOOL Install( bool bLogon, CString strUsername, CString strPassword );
	BOOL Uninstall();
	void SetStatus ( DWORD dwState );
	BOOL Initialize();
	DWORD GetExitCode() { return m_Status.dwWin32ExitCode; }
	CString GetLastErrorMessage();

	// static member functions
private:
	static void WINAPI ServiceMain ( DWORD dwArgc, LPTSTR* lpszArgv );
	static DWORD WINAPI Handler ( DWORD dwOpcode, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext );
	static DWORD WINAPI ServiceWorkerThread (LPVOID lpParam);
	static DWORD WINAPI ServicePreShutdownThread (LPVOID lpParam);

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

private:
	CString m_strError;
	CString m_strInstallStatusUnknown;
	CString m_strRunningStatusUnknown;

public:
	static HANDLE ServiceStopEvent;
	static HANDLE ServiceDoneShutdownEvent;
};

/**********************************************************************/

extern const char* szVERSION_WINSERVICE;

/**********************************************************************/
