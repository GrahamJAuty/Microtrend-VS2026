/**********************************************************************/
#include "..\SPosLoyalty4Shared\MessageLogger.h"
/**********************************************************************/
#include "ServerRegistration.h"
#include "ServerWrapper.h"
/**********************************************************************/
#include "WinService.h"
/**********************************************************************/

/**********************************************************************/
// static variables
/**********************************************************************/

CWinService* CWinService::m_pThis = NULL;
HANDLE CWinService::ServiceStopEvent = INVALID_HANDLE_VALUE; 
HANDLE CWinService::ServiceDoneShutdownEvent = INVALID_HANDLE_VALUE; 

/**********************************************************************/

CWinService::CWinService()
{
	CString strTemp = "Unable to confirm whether service is already ";
	m_strInstallStatusUnknown = strTemp + "installed";
	m_strRunningStatusUnknown = strTemp + "running";
}

/**********************************************************************/

void CWinService::Init( const char* szServiceName, const char* szServiceDisplayName, const char* szServiceDescription, DWORD dwControlsAccepted )
{
	// copy the address of the current object so we can access it from
	// the static member callback functions. 
	// WARNING: This limits the application to only one CWinService object. 

	m_pThis = this;

	strcpy_s ( m_szServiceName, sizeof(m_szServiceName), szServiceName );
	m_szServiceDisplayName = szServiceDisplayName;
	m_szServiceDescription = szServiceDescription;

	// set up the initial service status 
	m_hServiceStatus = NULL;
	m_Status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_Status.dwCurrentState = SERVICE_STOPPED;
	m_Status.dwControlsAccepted = dwControlsAccepted;
	m_Status.dwWin32ExitCode = 0;
	m_Status.dwServiceSpecificExitCode = 0;
	m_Status.dwCheckPoint = 0;
	m_Status.dwWaitHint = 0;
}

/**********************************************************************/

CWinService::~CWinService()
{
}

/**********************************************************************/

BOOL CWinService::ServiceRun()
{
	BOOL bReply = FALSE;

	CString strFail = "Run Service FAIL : ";

	bool bIsKnown = FALSE;
	bool bIsInstalled = FALSE;
	IsInstalled( bIsKnown, bIsInstalled );

	if ( FALSE == bIsKnown )
	{
		MessageLogger.LogWinServiceMessage( strFail + m_strInstallStatusUnknown );
	}
	else if ( FALSE == bIsInstalled )
	{
		MessageLogger.LogWinServiceMessage( strFail + "Not Installed" );
	}
	else if ( RunService() == FALSE )
	{
		MessageLogger.LogWinServiceMessage( strFail + m_strError );
	}
	else
	{
		bReply = TRUE;
	}

	return bReply;
}

/**********************************************************************/
// command line argument parsing
// returns TRUE true if argument has been processed
/**********************************************************************/

BOOL CWinService::ParseStandardArgs(CLoyaltyCommandLineInfo& cmdInfo)
{
	if (cmdInfo.GetSize() == 2)		// See if we have any command line args we recognise
	{
		CString strArg = cmdInfo.GetAt(1);
		strArg.MakeLower();

		if (strArg == "install")			// request to install service
		{
			m_strError = (ServiceInstall(FALSE, "", "") == TRUE) ? "The service has been installed" : GetError();
			return TRUE;					// say we processed the argument
		}
		else if (strArg == "uninstall")		// request to uninstall service
		{
			m_strError = (ServiceUninstall() == TRUE) ? "The service has been uninstalled" : GetError();
			return TRUE;					// say we processed the argument
		}
		else if (strArg == "start")		// request to start service
		{
			m_strError = (ServiceStart() == TRUE) ? "The service has started" : GetError();
			return TRUE;					// say we processed the argument
		}
		else if (strArg == "stop")			// request to stop service
		{
			m_strError = (ServiceStop() == TRUE) ? "The service has stopped" : GetError();
			return TRUE;					// say we processed the argument
		}
		else if (strArg == "status")			// check service status
		{
			ServiceStatus();
			return TRUE;						// say we processed the argument
		}
	}

	if (cmdInfo.GetSize() == 4)		// See if we have any command line args we recognise
	{
		CString strArg = cmdInfo.GetAt(1);	
		strArg.MakeLower();

		CString strUsername = cmdInfo.GetAt(2);
		CString strPassword = cmdInfo.GetAt(3);

		if (strArg == "/install")			// request to install service
		{
			m_strError = (ServiceInstall(TRUE, strUsername, strPassword) == TRUE) ? "The service has been installed" : GetError();
			return TRUE;					// say we processed the argument
		}
	}

	char szFilePath[_MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));	// get fully qualified path for the file

	char szFilename[_MAX_FNAME];
	_splitpath_s(szFilePath, NULL, 0, NULL, 0, szFilename, _MAX_FNAME, NULL, 0);		// extract just the filename

	m_strError = "The following commands are available:\n\n";

	CString str;
	str.Format("%s install\n", szFilename);
	m_strError += str;
	str.Format("%s install windows-username windows-password\n", szFilename);
	m_strError += str;
	str.Format("%s uninstall\n", szFilename);
	m_strError += str;
	str.Format("%s start\n", szFilename);
	m_strError += str;
	str.Format("%s stop\n", szFilename);
	m_strError += str;
	str.Format("%s status\n", szFilename);
	m_strError += str;

	return FALSE;		// say we have not processed the argument
}

/**********************************************************************/

BOOL CWinService::ServiceInstall( bool bLogon, CString strUsername, CString strPassword )
{
	BOOL bReply;

	bool bIsKnown = FALSE;
	bool bIsInstalled = FALSE;

	IsInstalled( bIsKnown, bIsInstalled );
	
	if ( FALSE == bIsKnown )
	{
		m_strError = m_strInstallStatusUnknown;
		bReply = FALSE;
	}
	else if ( TRUE == bIsInstalled )
	{
		m_strError = "The service is already installed";
		bReply = FALSE;
	}
	else
	{
		if ( Install( bLogon, strUsername, strPassword ) == TRUE )		// try and install the copy that's running
		{
			bReply = TRUE;			// the service has been installed
		}
		else
		{
			m_strError = GetLastErrorMessage();
			bReply = FALSE;
		}
	}

	return bReply;
}

/**********************************************************************/

BOOL CWinService::ServiceUninstall()
{
	BOOL bReply;

	bool bIsKnown = FALSE;
	bool bIsInstalled = FALSE;
	IsInstalled( bIsKnown, bIsInstalled );

	if ( FALSE == bIsKnown )
	{
		m_strError = m_strInstallStatusUnknown;
		bReply = FALSE;
	}
	else if ( FALSE == bIsInstalled )
	{
		m_strError = "The service is not installed";
		bReply = FALSE;
	}
	else
	{
		if ( Uninstall() == TRUE )	// try and remove the copy that's installed
		{
			bReply = TRUE;			// the service has been uninstalled
		}
		else
		{
			m_strError = GetLastErrorMessage();
			bReply = FALSE;
		}
	}

	return bReply;
}

/**********************************************************************/

BOOL CWinService::ServiceStart()
{
	BOOL bReply = FALSE;

	bool bIsKnown = FALSE;
	bool bIsInstalled = FALSE;

	IsInstalled( bIsKnown, bIsInstalled );

	if ( FALSE == bIsKnown )
	{
		m_strError = m_strInstallStatusUnknown;
		bReply = FALSE;
	}
	else if ( FALSE == bIsInstalled )
	{
		m_strError = "The service is not installed";
		bReply = FALSE;
	}
	else
	{
		bool bIsRunning = FALSE;
		IsRunning( bIsKnown, bIsRunning );

		if ( FALSE == bIsKnown )
		{
			m_strError = m_strRunningStatusUnknown;
			bReply = FALSE;
		}
		else if ( TRUE == bIsRunning )
		{
			m_strError = "The service is already running";
			bReply = FALSE;
		}
		else
		{
			CString str;

			SC_HANDLE hSCM = ::OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );	// test if the service is currently installed
			
			if ( hSCM != NULL )
			{
				SC_HANDLE hService = ::OpenService ( hSCM, m_szServiceName, SERVICE_START );
				if ( hService != NULL )
				{
					bReply = ::StartService ( hService, 0, NULL );	
					::CloseServiceHandle ( hService );
				}

				::CloseServiceHandle ( hSCM );
			}
			
			if ( bReply == FALSE )
			{
				m_strError = GetLastErrorMessage();
			}
		}
	}

	return bReply;
}

/**********************************************************************/

BOOL CWinService::ServiceStop()
{
	BOOL bReply = FALSE;

	bool bIsKnown = FALSE;
	bool bIsInstalled = FALSE;

	IsInstalled( bIsKnown, bIsInstalled );

	if ( FALSE == bIsKnown )
	{
		m_strError = m_strInstallStatusUnknown;
		bReply = FALSE;
	}
	else if ( FALSE == bIsInstalled )
	{
		m_strError = "The service is not installed";
		bReply = FALSE;
	}
	else
	{
		bool bIsRunning = FALSE;
		IsRunning( bIsKnown, bIsRunning );

		if ( FALSE == bIsKnown )
		{
			m_strError = m_strRunningStatusUnknown;
			bReply = FALSE;
		}
		else if ( FALSE == bIsRunning )
		{
			m_strError = "The service is not running";
			bReply = FALSE;
		}
		else
		{
			CString str;

			SC_HANDLE hSCM = ::OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );	// Test if the service is currently installed
			if ( hSCM != NULL )
			{
				SC_HANDLE hService = ::OpenService ( hSCM, m_szServiceName, SERVICE_STOP );
				if ( hService != NULL )
				{
					SERVICE_STATUS ServiceStatus;
					bReply = ::ControlService ( hService, SERVICE_CONTROL_STOP, &ServiceStatus );
					::CloseServiceHandle ( hService );
				}

				::CloseServiceHandle ( hSCM );
			}

			if ( bReply == FALSE )
				m_strError = GetLastErrorMessage();
		}
	}

	return bReply;
}

/**********************************************************************/

void CWinService::ServiceStatus()
{
	bool bIsKnown = FALSE;
	bool bIsInstalled = FALSE;

	IsInstalled( bIsKnown, bIsInstalled );

	if ( FALSE == bIsKnown )
	{
		m_strError = m_strInstallStatusUnknown;
	}
	else if ( FALSE == bIsInstalled )
	{
		m_strError = "The service is not installed";
	}
	else
	{
		m_strError = "The service is installed";

		bool bIsRunning = FALSE;
		IsRunning( bIsKnown, bIsRunning );

		if ( FALSE == bIsKnown )
		{
			m_strError += "\n";
			m_strError += m_strRunningStatusUnknown;
		}
		else if ( FALSE == bIsRunning )
		{
			m_strError += " but not running";
		}
		else
		{
			m_strError += " and running";
		}
	}
}

/**********************************************************************/

CString CWinService::GetLastErrorMessage()
{
	DWORD dwLastError = GetLastError(); 

	LPVOID lpMsgBuf = NULL;
	FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
					NULL,
					dwLastError,
					MAKELANGID ( LANG_NEUTRAL, SUBLANG_DEFAULT ),
					(LPTSTR)&lpMsgBuf,
					0,
					NULL );

	CString str = (LPTSTR) lpMsgBuf;

	LocalFree ( lpMsgBuf );

	return str;
}

/**********************************************************************/
// Install/uninstall routines
/**********************************************************************/

void CWinService::IsInstalled( bool& bIsKnown, bool& bIsInstalled )
{
	bIsKnown = FALSE;
	bIsInstalled = FALSE;

	// Open the Service Control Manager with full access
	SC_HANDLE hSCM = ::OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );	// Test if the service is currently installed
	if ( hSCM != NULL )
	{
		bIsKnown = TRUE;

		// Try to open the service
		SC_HANDLE hService = ::OpenService ( hSCM, m_szServiceName, SERVICE_QUERY_CONFIG );
		if ( hService != NULL )
		{
			bIsInstalled = TRUE;
			::CloseServiceHandle ( hService );
		}
		else
		{
			m_strError = GetLastErrorMessage();
		}

		::CloseServiceHandle ( hSCM );
	}
	else
	{
		m_strError = GetLastErrorMessage();
	}
}

/**********************************************************************/

void CWinService::IsRunning( bool& bIsKnown, bool& bIsRunning )
{
	bIsKnown = FALSE;
	bIsRunning = FALSE;

	SC_HANDLE hSCM = ::OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );	// Test if the service is currently installed
	
	if ( hSCM != NULL )
	{
		SC_HANDLE hService = ::OpenService ( hSCM, m_szServiceName, SERVICE_QUERY_STATUS );
		
		if ( hService != NULL )
		{
			SERVICE_STATUS ServiceStatus;
			if ( QueryServiceStatus( hService, &ServiceStatus ) == TRUE )
			{
				bIsKnown = TRUE;
				bIsRunning = ( ServiceStatus.dwCurrentState == SERVICE_RUNNING );
			}
			
			::CloseServiceHandle ( hService );
		}
		
		::CloseServiceHandle ( hSCM );
	}
}

/**********************************************************************/
// Install the service
/**********************************************************************/

BOOL CWinService::Install( bool bLogon, CString strUsername, CString strPassword )
{
	// Open the Service Control Manager with full access
	SC_HANDLE hSCM = ::OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );

	if ( hSCM == NULL )
		return FALSE;

	// Get the executable file path
	char szFilePath[_MAX_PATH];
	::GetModuleFileName ( NULL, szFilePath, sizeof(szFilePath) );

	CString strPath = szFilePath;
	strPath.MakeUpper();
	if ( strPath.Right(22) == "SPOSLOYALTY4SERVER.EXE" )
	{
		CString strFolder = strPath.Left( strPath.GetLength() - 23 );
		strPath += " <service> ";
		strPath += strFolder;
	}

	CString strWindowsUsername = ".\\";
	strWindowsUsername += strUsername;

	// Create the service
	SC_HANDLE hService = NULL;
		
	if ( FALSE == bLogon )
	{	
		hService = ::CreateService(	
							hSCM,						// handle to the service control manager database
							m_szServiceName,			// service name
							m_szServiceDisplayName,		// display name
							SERVICE_ALL_ACCESS,			// desired access
							SERVICE_WIN32_OWN_PROCESS,	// service type
							SERVICE_AUTO_START,			// start type
							SERVICE_ERROR_NORMAL,		// error control
							strPath,					// fully qualified path to the service binary file
							NULL,						// load order group
							NULL,						// tag id
							NULL,						// dependencies
							NULL,						// name of the account under which the service should run
							NULL );						// password
	}
	else
	{
		hService = ::CreateService (	
							hSCM,						// handle to the service control manager database
							m_szServiceName,			// service name
							m_szServiceDisplayName,		// display name
							SERVICE_ALL_ACCESS,			// desired access
							SERVICE_WIN32_OWN_PROCESS,	// service type
							SERVICE_AUTO_START,			// start type
							SERVICE_ERROR_NORMAL,		// error control
							strPath,					// fully qualified path to the service binary file
							NULL,						// load order group
							NULL,						// tag id
							NULL,						// dependencies
							strWindowsUsername,			// name of the account under which the service should run
							strPassword );				// password
	}

	if ( hService == NULL )
	{
		::CloseServiceHandle ( hSCM );
		return FALSE;
	}

	// Set the description of the service
	SERVICE_DESCRIPTION Desc{};
	Desc.lpDescription = (char*) m_szServiceDescription;		// Command Line Service description goes here...
	ChangeServiceConfig2 ( hService, SERVICE_CONFIG_DESCRIPTION, &Desc );

	SERVICE_PRESHUTDOWN_INFO info{};
	info.dwPreshutdownTimeout = 5 * 60 * 1000; // 5 minutes
	ChangeServiceConfig2(hService, SERVICE_CONFIG_PRESHUTDOWN_INFO, &info);

	// Tidy up
	::CloseServiceHandle ( hService );
	::CloseServiceHandle ( hSCM );
	return TRUE;
}

/**********************************************************************/
// Uninstall the service
/**********************************************************************/

BOOL CWinService::Uninstall()
{
	// Open the Service Control Manager with full access
	SC_HANDLE hSCM = ::OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	
	if ( hSCM == NULL )
		return FALSE;

	BOOL bResult = FALSE;
	SC_HANDLE hService = ::OpenService ( hSCM, m_szServiceName, DELETE );
	if ( hService != NULL )
	{
		bResult = ::DeleteService ( hService );

		::CloseServiceHandle ( hService );
	}

	::CloseServiceHandle ( hSCM );
	return bResult;
}

/**********************************************************************/
// Service startup and registration
/**********************************************************************/

BOOL CWinService::RunService()
{
	SERVICE_TABLE_ENTRY st[] = 
	{
		{ m_szServiceName, ServiceMain },
		{ NULL, NULL }
	};

	BOOL bReply = ::StartServiceCtrlDispatcher ( st );
	if ( bReply == FALSE )
	{
		m_strError = GetLastErrorMessage();
	}
	return bReply;
}

/**********************************************************************/
// static member function (callback)
/**********************************************************************/

void CWinService::ServiceMain ( DWORD dwArgc, LPTSTR* lpszArgv )
{
	// Get a pointer to the C++ object
	CWinService* pService = m_pThis;

	// Register the control request handler
	pService->m_Status.dwCurrentState = SERVICE_START_PENDING;
	pService->m_hServiceStatus = RegisterServiceCtrlHandlerEx ( pService->m_szServiceName, Handler, NULL );
	if ( pService->m_hServiceStatus == NULL )
	{
		return;
	}

	// Start the initialisation
	if ( pService->Initialize() == TRUE )
	{
		// Do the real work. 
		// When the Run function returns, the service has stopped.
		pService->m_Status.dwWin32ExitCode = 0;
		pService->m_Status.dwCheckPoint = 0;
		pService->m_Status.dwWaitHint = 0;
		pService->Run();
	}

	// Tell the service manager we are stopped
	pService->SetStatus ( SERVICE_STOPPED );
}

/**********************************************************************/
// status functions
/**********************************************************************/

void CWinService::SetStatus ( DWORD dwState )
{
	m_Status.dwCurrentState = dwState;
	::SetServiceStatus ( m_hServiceStatus, &m_Status );
}

/**********************************************************************/
// Service initialization
/**********************************************************************/

BOOL CWinService::Initialize()
{
	// Start the initialization
	SetStatus ( SERVICE_START_PENDING );

	// Perform the actual initialization
	BOOL bResult = OnInit(); 

	// Set final state
	m_Status.dwWin32ExitCode = GetLastError();
	m_Status.dwCheckPoint = 0;
	m_Status.dwWaitHint = 0;
	if ( bResult == FALSE )
	{
		SetStatus ( SERVICE_STOPPED );
		return FALSE;    
	}

	SetStatus ( SERVICE_RUNNING );

	return TRUE;
}

/**********************************************************************/
// Control request handlers
// static member function (callback) to handle commands from the
// service control manager
/**********************************************************************/

DWORD CWinService::Handler ( DWORD dwOpcode,
  DWORD dwEventType,
  LPVOID lpEventData,
  LPVOID lpContext ) 
{
	DWORD dwResult = NO_ERROR;

	// Get a pointer to the object
	CWinService* pService = m_pThis;

	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP: 
		pService->SetStatus ( SERVICE_STOP_PENDING );
		pService->OnStop();
		break;

	case SERVICE_CONTROL_PAUSE: 
		pService->OnPause();
		break;

	case SERVICE_CONTROL_CONTINUE: 
		pService->OnContinue();
		break;

	case SERVICE_CONTROL_INTERROGATE: 
		pService->OnInterrogate();
		break;

	case SERVICE_CONTROL_PRESHUTDOWN:
		pService->SetStatus ( SERVICE_STOP_PENDING );
		pService->OnPreShutdown();
		break;

	case SERVICE_CONTROL_SHUTDOWN:
		pService->SetStatus ( SERVICE_STOP_PENDING );
		pService->OnShutdown();
		break;

	default:
		if ( dwOpcode >= SERVICE_CONTROL_USER )
		{
			if ( pService->OnUserControl ( dwOpcode ) == FALSE )
			{
				dwResult = ERROR_CALL_NOT_IMPLEMENTED;
			}
		}
		else
		{
			dwResult = ERROR_CALL_NOT_IMPLEMENTED;
		}
		break;
	}

	// Report current status
	::SetServiceStatus ( pService->m_hServiceStatus, &pService->m_Status );
	return dwResult;
}

/**********************************************************************/
// Called when the service is first initialized
/**********************************************************************/

BOOL CWinService::OnInit()
{
	return TRUE;
}

/**********************************************************************/
// Called when the service control manager wants to stop the service
/**********************************************************************/

void CWinService::OnStop()
{
	MessageLogger.LogWinServiceMessage( "Windows request : Stop Service" );
	SetEvent (ServiceStopEvent);
}

/**********************************************************************/
// called when the service is interrogated
/**********************************************************************/

void CWinService::OnInterrogate()
{
}

/**********************************************************************/
// called when the service is paused
/**********************************************************************/

void CWinService::OnPause()
{
	MessageLogger.LogWinServiceMessage( "Windows request : Pause Service" );
}

/**********************************************************************/
// called when the service is continued
/**********************************************************************/

void CWinService::OnContinue()
{
	MessageLogger.LogWinServiceMessage( "Windows request : Continue Service" );
}

/**********************************************************************/
// called before the service is shut down
/**********************************************************************/

void CWinService::OnPreShutdown()
{
	CWinService::ServiceDoneShutdownEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
	HANDLE hThread = CreateThread (NULL, 0, ServicePreShutdownThread, NULL, 0, NULL);
	MessageLogger.LogWinServiceMessage( "Windows request : Pre Shutdown Service" );
	SetEvent (ServiceStopEvent);
}

/**********************************************************************/
// called when the service is shut down
/**********************************************************************/

void CWinService::OnShutdown()
{
	MessageLogger.LogWinServiceMessage( "Windows request : Shutdown Service" );
	SetEvent (ServiceStopEvent);
}

/**********************************************************************/
// called when the service gets a user control message
/**********************************************************************/

BOOL CWinService::OnUserControl ( DWORD dwOpcode )
{
	switch ( dwOpcode )
	{
	case SERVICE_CONTROL_USER + 0:
		// do something
		return TRUE;		// message handled

	default:
		break;
	}

	return FALSE;		 // message not handled}
}

/**********************************************************************/

void CWinService::Run()
{
	MessageLogger.LogWinServiceMessage( "Service thread start" );
	CWinService::ServiceStopEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
	HANDLE hThread = CreateThread (NULL, 0, ServiceWorkerThread, NULL, 0, NULL);
	
	if (hThread != 0)
	{
		WaitForSingleObject(hThread, INFINITE);
		MessageLogger.LogWinServiceMessage("Service thread stop");
	}
	else
	{
		MessageLogger.LogWinServiceMessage("Service thread create fail");
	}
}

/**********************************************************************/
 
DWORD WINAPI CWinService::ServiceWorkerThread (LPVOID lpParam)
{
	bool bGotStopEvent = FALSE;

	while (TRUE)
	{
		if (WaitForSingleObject(CWinService::ServiceStopEvent, 0) == WAIT_OBJECT_0)
		{
			bGotStopEvent = TRUE;
			break;
		}

		ServerRegistration.CheckRegistration();

		if (ServerRegistration.IsRegistered() == TRUE)
		{
			break;
		}

		if (ServerRegistration.CanGiveUp() == TRUE)
		{
			bGotStopEvent = TRUE;
			break;
		}

		if (SolutionGlobalFunctions::FileExists(Filenames.GetServerServiceStopFilename()) == TRUE)
		{
			MessageLogger.LogSystemMessage("Service shutdown request by token file");
			bGotStopEvent = TRUE;
			break;
		}

		Sleep(50);
	}

	if ((ServerRegistration.IsRegistered() == TRUE) && (FALSE == bGotStopEvent))
	{
		CString strMsg = "";
		strMsg.Format("Service Registration OK (%d:%d)",
			ServerRegistration.GetAttemptCount1(),
			ServerRegistration.GetAttemptCount2());

		MessageLogger.LogSystemMessage(strMsg);

		if (ServerRegistration.CheckDbAccess() == FALSE)
		{
			MessageLogger.LogSystemMessage(ServerRegistration.GetErrorText1());
			MessageLogger.LogSystemMessage(ServerRegistration.GetErrorText2());
		}
		else
		{
			CServerWrapper ServerWrapper;
			ServerWrapper.InitSystem(TRUE);

			while (TRUE)
			{
				ServerWrapper.Pulse();

				if (WaitForSingleObject(CWinService::ServiceStopEvent, 0) == WAIT_OBJECT_0)
				{
					break;
				}

				if (SolutionGlobalFunctions::FileExists(Filenames.GetServerServiceStopFilename()) == TRUE)
				{
					MessageLogger.LogSystemMessage("Service shutdown request by token file");
					break;
				}

				if (SolutionGlobalFunctions::FileExists(Filenames.GetServerUpdatePluReqFilename()) == TRUE)
				{
					ServerWrapper.ProcessUpdatePluRequest();
				}

				Sleep(50);
			}
		}
	}

	SetEvent(ServiceDoneShutdownEvent);

	return ERROR_SUCCESS;
}

/**********************************************************************/

DWORD WINAPI CWinService::ServicePreShutdownThread (LPVOID lpParam)
{
	int x = 1;
	while (WaitForSingleObject(CWinService::ServiceDoneShutdownEvent, 0) != WAIT_OBJECT_0)
    {        
		Sleep(100);

		CWinService* pService = m_pThis;
		pService ->	m_Status.dwCheckPoint = x++;
		::SetServiceStatus ( pService->m_hServiceStatus, &pService->m_Status );
	}

	return ERROR_SUCCESS;
}

/**********************************************************************/
