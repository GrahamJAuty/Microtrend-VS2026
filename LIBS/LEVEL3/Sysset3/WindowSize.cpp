/**********************************************************************/

void CSysset::SaveWindowSize ( CWnd* pWnd, const char* szId )
{
	if ( m_IdentityInfo.m_strComputerName != "" )
	{
		// Sysset\Pnn\PCnnnnnnnnnn folder did not exist in sysset v1.11 and earlier
		// Sysset\Pnn\PCnnnnnnnnnn would normally only be created for full systems
		// need to create Sysset\Pnn\PCnnnnnnnnnn for upgrades to systems using sysset v1.11 and earlier

		CString strPath = GetSyssetPCPathClient();		// <DataPath\>Sysset\Pnn\PCnnnnnnnnnn
		::MakeSubdirectory ( strPath );			// make folder (if it doesn't exist)

		strPath = GetSyssetWindowSizePath();		// <DataPath\>Sysset\Pnn\PCnnnnnnnnnn\ws
		::MakeSubdirectory ( strPath );			// make folder (if it doesn't exist)

		CString strMD5 = CSSMD5::CalcMD5 ( szId );	// make unique 32 char md5 hash from id

		CString strFilename;
		strFilename.Format ( "%s\\%s", (const char*) strPath, (const char*) strMD5 );		// <DataPath\>Sysset\Pnn\PCnnnnnnnnnn\ws\xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

		SS_SaveWindowSize ( pWnd, strFilename );
	}
}

/**********************************************************************/

void CSysset::RestoreWindowSize ( CWnd* pWnd, const char* szId )
{
	if ( m_IdentityInfo.m_strComputerName != "" )
	{
		CString strPath = GetSyssetWindowSizePath();		// <DataPath\>Sysset\Pnn\PCnnnnnnnnnn\ws

		CString strMD5 = CSSMD5::CalcMD5 ( szId );		// make unique 32 char md5 hash from id

		CString strFilename;
		strFilename.Format ( "%s\\%s", (const char*) strPath, (const char*) strMD5 );		// <DataPath\>Sysset\Pnn\PCnnnnnnnnnn\ws\xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

		SS_RestoreWindowSize ( pWnd, strFilename );
	}
}

/**********************************************************************/
