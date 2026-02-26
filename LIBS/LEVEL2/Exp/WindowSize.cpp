/**********************************************************************/

#include <shlobj.h>

/**********************************************************************/

void SS_SaveWindowSize ( CWnd* pWnd, const char* szFilename )
{
	if ( pWnd != NULL )
	{
		WINDOWPLACEMENT wp;
		pWnd->GetWindowPlacement ( &wp );

		CSSFile file;
		if ( file.Open ( szFilename, "wb" ) == TRUE )
		{
			file.Write ( &wp, sizeof(wp), 1 );
			file.Close();
		}
	}
}

/**********************************************************************/

void SS_RestoreWindowSize ( CWnd* pWnd, const char* szFilename )
{
	if ( pWnd != NULL )
	{
//		if ( pWnd->IsWindowVisible() == FALSE )		// window does not have WS_VISIBLE bit set (stops disph working)
		{
			CSSFile file;
			if ( file.Open ( szFilename, "rb" ) == TRUE )
			{
				WINDOWPLACEMENT wp;
				if ( file.Read ( &wp, sizeof(wp), 1 ) == 1 )
				{
					if ( wp.length == sizeof(wp) )		// make sure it is same version of structure
						pWnd->SetWindowPlacement ( &wp );
				}
				file.Close();
			}
		}
	}
}

/**********************************************************************/

void SS_SaveUserWindowSize ( CWnd* pWnd, const char* szProgram, const char* szId )
{
	char szPath[MAX_PATH];

	if ( SUCCEEDED ( SHGetFolderPath ( NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath )))	// Get path for local user data.
	{
		// windows 7 returns:  C:\Users\<-Username->\AppData\Local
		// windows XP returns: C:\Documents and Settings\<-Username->\Local Settings\Application Data

		CString strCompanyFolder = SS_PathAppend ( szPath, "SharpSoft" );
		::MakeSubdirectory ( strCompanyFolder );	// make directory if it does not exist

		CString strProgramDataFolder = SS_PathAppend ( strCompanyFolder, szProgram );
		::MakeSubdirectory ( strProgramDataFolder );	// make directory if it does not exist

		CString strWindowSizeFolder = SS_PathAppend ( strProgramDataFolder, "ws" );
		::MakeSubdirectory ( strWindowSizeFolder );	// make directory if it does not exist

		CString strMD5 = CSSMD5::CalcMD5 ( szId );

		CString strFilename = SS_PathAppend ( strWindowSizeFolder, strMD5 );

		SS_SaveWindowSize ( pWnd, strFilename );
	}
}

/**********************************************************************/

void SS_RestoreUserWindowSize ( CWnd* pWnd, const char* szProgram, const char* szId )
{
	char szPath[MAX_PATH];

	if ( SUCCEEDED ( SHGetFolderPath ( NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath )))	// Get path for local user data.
	{
		// windows 7 returns:  C:\Users\<-Username->\AppData\Local
		// windows XP returns: C:\Documents and Settings\<-Username->\Local Settings\Application Data

		CString strCompanyFolder = SS_PathAppend ( szPath, "SharpSoft" );

		CString strProgramDataFolder = SS_PathAppend ( strCompanyFolder, szProgram );

		CString strWindowSizeFolder = SS_PathAppend ( strProgramDataFolder, "ws" );

		CString strMD5 = CSSMD5::CalcMD5 ( szId );

		CString strFilename = SS_PathAppend ( strWindowSizeFolder, strMD5 );

		SS_RestoreWindowSize ( pWnd, strFilename );
	}
}

/**********************************************************************/

CString SS_GetIniFilename ( const char* szProgram )
{
	CString strFilename;

	char szPath[MAX_PATH];

	if ( SUCCEEDED ( SHGetFolderPath ( NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath )))	// Get path for local user data.
	{
		// windows 7 returns:  C:\Users\<-Username->\AppData\Local
		// windows XP returns: C:\Documents and Settings\<-Username->\Local Settings\Application Data

		CString strCompanyFolder = SS_PathAppend ( szPath, "SharpSoft" );
		::MakeSubdirectory ( strCompanyFolder );	// make directory if it does not exist

		CString strProgramDataFolder = SS_PathAppend ( strCompanyFolder, szProgram );
		::MakeSubdirectory ( strProgramDataFolder );	// make directory if it does not exist

		strFilename = strProgramDataFolder;
		strFilename += "\\";
	}

	strFilename += szProgram;
	strFilename += ".ini";

	return strFilename;
}

/**********************************************************************/
