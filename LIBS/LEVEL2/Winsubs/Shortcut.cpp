/**********************************************************************/

#define szSHELL_FOLDERS	"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"

#define szPROGRAMS	"Programs"
#define szDESKTOP	"Desktop"
#define szSTARTUP	"Startup"

/**********************************************************************/

static void CreateLink ( const char* szLinkFilename, const char* szOrigFilename, const char* szWorkingDirectory, const char* szArgs )
{
	HRESULT hresCoInit = CoInitialize ( NULL );		// Create a shell link object

	IShellLink* psl{};

	HRESULT hres = CoCreateInstance ( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (PVOID *) &psl );

	if ( SUCCEEDED ( hres ) )				// Initialize the shell link object
	{
		psl->SetPath ( szOrigFilename );		// Pathname of original object

		if ( strlen ( szArgs ) > 0 )
			psl->SetArguments ( szArgs );		// Arguments

		if ( strlen ( szWorkingDirectory ) > 0 )
			psl->SetWorkingDirectory ( szWorkingDirectory );	// Working directory when process starts

		IPersistFile* ppf{};

		hres = psl->QueryInterface ( IID_IPersistFile, (PVOID *) &ppf );
		if ( SUCCEEDED ( hres ) )
		{
			#ifndef UNICODE					// Convert the ANSI path to a Unicode path

			WCHAR szPath[MAX_PATH] = { 0 };
			MultiByteToWideChar ( CP_ACP, 0, szLinkFilename, strlen(szLinkFilename), szPath, sizeof(szPath) );
			hres = ppf->Save ( szPath, TRUE );		// Save the shell link object on the disk

			#else

			hres = ppf->Save ( szLinkFilename, TRUE );

			#endif

			ppf->Release();
		}
		psl->Release();
	}

	CoUninitialize();
}

/**********************************************************************/

static CString GetShellPath ( const char* szShellFolder )
{
	CString strPath;
	HKEY hKey;

	if ( RegOpenKeyEx ( HKEY_CURRENT_USER, szSHELL_FOLDERS, 0, KEY_QUERY_VALUE, &hKey ) == ERROR_SUCCESS )
	{
		DWORD dwType;
		DWORD dwSize = MAX_PATH;
		char szLinkPath[MAX_PATH];

		if ( RegQueryValueEx ( hKey, szShellFolder, NULL, &dwType, (BYTE*) &szLinkPath, &dwSize ) == ERROR_SUCCESS )
			strPath = szLinkPath;

		RegCloseKey ( hKey );
	}

	return strPath;
}

/**********************************************************************/

void CreateProgramsLink ( const char* szLinkFilename, const char* szOrigFilename, const char* szWorkingDirectory, const char* szArgs )
{
	CString strLinkPath = GetShellPath ( szPROGRAMS );	// get path to programs (eg c:\windows\start menu\programs)

	if ( strLinkPath != "" )
	{
		CCSV csv ( szLinkFilename, '\\' );

		CString strLinkFolder = strLinkPath;

		for ( int i = 0 ; i < csv.GetSize()-1 ; i++ )
		{
			strLinkFolder += "\\";
			strLinkFolder += csv.GetString(i);

			if (_access(strLinkFolder, 0) != 0)		// check if subdirectory exists
			{
				int nResult = _mkdir(strLinkFolder);				// create subdirectory
			}
		}

		CString strLinkFilename = strLinkPath;
		strLinkFilename += "\\";
		strLinkFilename += szLinkFilename;

		CreateLink ( strLinkFilename, szOrigFilename, szWorkingDirectory, szArgs );
	}
}

/**********************************************************************/

void CreateDesktopLink ( const char* szLinkFilename, const char* szOrigFilename, const char* szWorkingDirectory, const char* szArgs )
{
	CString strLinkPath = GetShellPath ( szDESKTOP );	// get path to desktop (eg c:\windows\desktop)

	if ( strLinkPath != "" )
	{
		strLinkPath += "\\";
		strLinkPath += szLinkFilename;

		CreateLink ( strLinkPath, szOrigFilename, szWorkingDirectory, szArgs );
	}
}

/**********************************************************************/

void CreateStartupLink ( const char* szLinkFilename, const char* szOrigFilename, const char* szWorkingDirectory, const char* szArgs )
{
	CString strLinkPath = GetShellPath ( szSTARTUP );	// get path to startup (eg c:\windows\start menu\programs\startup)

	if ( strLinkPath != "" )
	{
		strLinkPath += "\\";
		strLinkPath += szLinkFilename;

		CreateLink ( strLinkPath, szOrigFilename, szWorkingDirectory, szArgs );
	}
}

/**********************************************************************/

CString GetStartupPath()
{
	CString strPath = GetShellPath ( szSTARTUP );	// get path to startup (eg c:\windows\start menu\programs\startup)

	return strPath;
}

/**********************************************************************/
