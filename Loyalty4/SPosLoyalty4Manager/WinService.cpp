/**********************************************************************/
#include "..\SPosLoyalty4Shared\ServiceOptions.h"
/**********************************************************************/
#include "WinService.h"
/**********************************************************************/

CWinService::CWinService()
{
}

/**********************************************************************/

void CWinService::Init( const char* szServiceName, const char* szServiceDisplayName, const char* szServiceDescription, DWORD dwControlsAccepted )
{
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

BOOL CWinService::ServiceInstall()
{
	BOOL bReply;

	bool bInstallStatusKnown, bIsInstalled;
	IsInstalled( bInstallStatusKnown, bIsInstalled );

	if ( TRUE == bIsInstalled )
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

	bool bInstallStatusKnown, bIsInstalled;
	IsInstalled( bInstallStatusKnown, bIsInstalled );

	if ( FALSE == bIsInstalled )
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
// Install the service
/**********************************************************************/

BOOL CWinService::Install()
{
	// Get the executable file path
	char szFilePath[_MAX_PATH];
	::GetModuleFileName ( NULL, szFilePath, sizeof(szFilePath) );

	CString strPath = szFilePath;
	strPath.MakeUpper();
	if ( strPath.Right(24) != "\\SPOSLOYALTY4MANAGER.EXE" )
	{
		m_strError = "Incorrect EXE filepath";
		return FALSE;
	}

	strPath = strPath.Left( strPath.GetLength() - 24 );
	
	CString strFolder = strPath;

	strPath += "\\SPosLoyalty4Server.exe";
	
	strPath += " <service> ";
	strPath += strFolder;
	
	// Open the Service Control Manager with full access
	SC_HANDLE hSCM = ::OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );

	if ( hSCM == NULL )
		return FALSE;

	bool bWindowsLogon = FALSE;
	CString strWindowsUsername = ".\\";
	CString strWindowsPassword = "";

	bWindowsLogon = ServerServiceOptions.GetWindowsLogonFlag();
	strWindowsUsername += ServerServiceOptions.GetWindowsUsername();
	strWindowsPassword = ServerServiceOptions.GetWindowsPassword();
	
	SC_HANDLE hService = NULL;

	if ( FALSE == bWindowsLogon )
	{
		// Create the service
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
			NULL,						// name of the account under which the service should run
			NULL );						// password
	}
	else
	{
		// Create the service
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
			strWindowsPassword );		// password
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
	BOOL bResult = FALSE;

	// Open the Service Control Manager with full access
	SC_HANDLE hSCM = ::OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	
	if ( hSCM != NULL )
	{
		SC_HANDLE hService = ::OpenService ( hSCM, m_szServiceName, DELETE );
		
		if ( hService != NULL )
		{
			if ( ::DeleteService ( hService ) == TRUE )
			{
				bResult = TRUE;
			}
	
			::CloseServiceHandle ( hService );
		}

		::CloseServiceHandle ( hSCM );
	}

	return bResult;
}

/**********************************************************************/
