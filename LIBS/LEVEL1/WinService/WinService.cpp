
/**********************************************************************/
// static variables
/**********************************************************************/

CWinService* CWinService::m_pThis = NULL;

/**********************************************************************/

CWinService::CWinService ( const char* szServiceName, const char* szServiceDisplayName, const char* szServiceDescription, DWORD dwControlsAccepted )
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
	BOOL bReply;

	if (( bReply = IsInstalled() ) == TRUE )
		bReply = RunService();		// run service

	return bReply;
}

/**********************************************************************/
// command line argument parsing
// returns TRUE true if argument has been processed
/**********************************************************************/

BOOL CWinService::ParseStandardArgs ( int argc, char* argv[] )
{
	if ( argc == 2 )		// See if we have any command line args we recognise
	{
		CString strArg = argv[1];
		strArg.MakeLower();

		if ( strArg == "/install" )			// request to install service
		{
			m_strError = ( ServiceInstall() == TRUE ) ? "The service has been installed" : GetError();
			return TRUE;					// say we processed the argument
		}
		else if ( strArg == "/uninstall" )		// request to uninstall service
		{
			m_strError = ( ServiceUninstall() == TRUE ) ? "The service has been uninstalled" : GetError();
			return TRUE;					// say we processed the argument
		}
		else if ( strArg == "/start"  )		// request to start service
		{
			m_strError = ( ServiceStart() == TRUE ) ? "The service has started" : GetError();
			return TRUE;					// say we processed the argument
		}
		else if ( strArg == "/stop" )			// request to stop service
		{
			m_strError = ( ServiceStop() == TRUE ) ? "The service has stopped" : GetError();
			return TRUE;					// say we processed the argument
		}
	}

	char szFilePath[_MAX_PATH];
	::GetModuleFileName ( NULL, szFilePath, sizeof(szFilePath) );	// get fully qualified path for the file

	char szFilename[_MAX_FNAME];
	_splitpath_s ( szFilePath, NULL, 0, NULL, 0, szFilename, _MAX_FNAME, NULL, 0 );		// extract just the filename

	m_strError = "The following commands are available:\n\n";

	CString str;
	str.Format ( "%s /install\n", szFilename );
	m_strError += str;
	str.Format ( "%s /uninstall\n", szFilename );
	m_strError += str;
	str.Format ( "%s /start\n", szFilename );
	m_strError += str;
	str.Format ( "%s /stop\n", szFilename );
	m_strError += str;
	str.Format ( "%s /setup\n", szFilename );
	m_strError += str;

	return FALSE;		// say we have not processed the argument
}

/**********************************************************************/

BOOL CWinService::ServiceInstall()
{
	BOOL bReply;

	if ( IsInstalled() == TRUE )
	{
		m_strError = "The service is already installed";
		bReply = FALSE;
	}
	else
	{
		if ( Install() == TRUE )		// try and install the copy that's running
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

	if ( IsInstalled() == FALSE )
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
		m_strError = GetLastErrorMessage();

	return bReply;
}

/**********************************************************************/

BOOL CWinService::ServiceStop()
{
	BOOL bReply = FALSE;

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

	return bReply;
}

/**********************************************************************/

CString CWinService::GetLastErrorMessage()
{
	DWORD dwLastError = GetLastError(); 

	LPVOID lpMsgBuf = NULL;
	FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
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

BOOL CWinService::IsInstalled()
{
	BOOL bResult = FALSE;

	// Open the Service Control Manager with full access
	SC_HANDLE hSCM = ::OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );	// Test if the service is currently installed
	if ( hSCM != NULL )
	{
		// Try to open the service
		SC_HANDLE hService = ::OpenService ( hSCM, m_szServiceName, SERVICE_QUERY_CONFIG );
		if ( hService != NULL )
		{
			bResult = TRUE;
			::CloseServiceHandle ( hService );
		}
		else
		{
			m_strError = GetLastErrorMessage();
		}

		::CloseServiceHandle ( hSCM );
	}
	else
		m_strError = GetLastErrorMessage();

	return bResult;
}

/**********************************************************************/
// Install the service
/**********************************************************************/

BOOL CWinService::Install()
{
	// Open the Service Control Manager with full access
	SC_HANDLE hSCM = ::OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );

	if ( hSCM == NULL )
		return FALSE;

	// Get the executable file path
	char szFilePath[_MAX_PATH];
	::GetModuleFileName ( NULL, szFilePath, sizeof(szFilePath) );

	// Create the service
	SC_HANDLE hService = ::CreateService (	hSCM,					// handle to the service control manager database
									m_szServiceName,			// service name
									m_szServiceDisplayName,		// display name
									SERVICE_ALL_ACCESS,			// desired access
									SERVICE_WIN32_OWN_PROCESS,	// service type
									SERVICE_AUTO_START,			// start type
									SERVICE_ERROR_NORMAL,		// error control
									szFilePath,				// fully qualified path to the service binary file
									NULL,					// load order group
									NULL,					// tag id
									NULL,					// dependencies
									NULL,					// name of the account under which the service should run
									NULL );					// password
	if ( hService == NULL )
	{
		::CloseServiceHandle ( hSCM );
		return FALSE;
	}

	// Set the description of the service
	SERVICE_DESCRIPTION Desc{};
	Desc.lpDescription = (char*) m_szServiceDescription;		// Command Line Service description goes here...
	ChangeServiceConfig2 ( hService, SERVICE_CONFIG_DESCRIPTION, &Desc );

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
		m_strError = GetLastErrorMessage();

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
	pService->m_hServiceStatus = RegisterServiceCtrlHandler ( pService->m_szServiceName, Handler );
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

void CWinService::Handler ( DWORD dwOpcode )
{
	// Get a pointer to the object
	CWinService* pService = m_pThis;

	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP: // 1
		pService->SetStatus ( SERVICE_STOP_PENDING );
		pService->OnStop();
		break;

	case SERVICE_CONTROL_PAUSE: // 2
		pService->OnPause();
		break;

	case SERVICE_CONTROL_CONTINUE: // 3
		pService->OnContinue();
		break;

	case SERVICE_CONTROL_INTERROGATE: // 4
		pService->OnInterrogate();
		break;

	case SERVICE_CONTROL_SHUTDOWN: // 5
		pService->OnShutdown();
		break;

	default:
		if ( dwOpcode >= SERVICE_CONTROL_USER )
		{
			if ( pService->OnUserControl ( dwOpcode ) == FALSE )
			{
			}
		}
		else
		{
		}
		break;
	}

	// Report current status

	::SetServiceStatus ( pService->m_hServiceStatus, &pService->m_Status );
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
}

/**********************************************************************/
// called when the service is continued
/**********************************************************************/

void CWinService::OnContinue()
{
}

/**********************************************************************/
// called when the service is shut down
/**********************************************************************/

void CWinService::OnShutdown()
{
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
