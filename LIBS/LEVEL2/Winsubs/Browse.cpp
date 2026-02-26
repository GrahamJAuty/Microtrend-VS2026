/**********************************************************************/

static LPITEMIDLIST GetPidl ( const char* szPath )
{
	LPITEMIDLIST pidl = NULL;
	LPSHELLFOLDER pDesktopFolder;
	OLECHAR olePath[MAX_PATH];
	ULONG dwAttributes = 0;
	HRESULT hr;

	if ( SUCCEEDED ( SHGetDesktopFolder(&pDesktopFolder) ))	// get a pointer to the Desktop's IShellFolder interface.
	{
		MultiByteToWideChar ( CP_ACP, MB_PRECOMPOSED, szPath, -1, olePath, MAX_PATH );	// IShellFolder::ParseDisplayName requires the file name be in Unicode.

		hr = pDesktopFolder->ParseDisplayName ( NULL, NULL, olePath, NULL, &pidl, &dwAttributes);	// Convert the path to an ITEMIDLIST.

		pDesktopFolder->Release();		// release the desktop folder object
	}

	return pidl;
}

/**********************************************************************/

bool BrowseFolder ( CString& strPathname, const char* szText, const char* szStartFolder, CWnd* pWnd )
{
	bool bReply = FALSE;
	LPMALLOC pMalloc;

	if ( ::SHGetMalloc ( &pMalloc ) == NOERROR )		// gets the Shell's default allocator
	{
		BROWSEINFO bi;
		char pszBuffer[MAX_PATH];
		LPITEMIDLIST pidl;
		LPITEMIDLIST pidlStartFolder = (szStartFolder != NULL) ? GetPidl(szStartFolder) : NULL;

		bi.hwndOwner = pWnd->GetSafeHwnd();
		bi.pidlRoot = pidlStartFolder;		// use NULL for desktop;
		bi.pszDisplayName = pszBuffer;
		bi.lpszTitle = szText;
		bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
		bi.lpfn = NULL;
		bi.lParam = 0;

		if (( pidl = ::SHBrowseForFolder ( &bi )) != NULL )
		{
			if ( ::SHGetPathFromIDList ( pidl, pszBuffer ) )
			{ 
				strPathname = pszBuffer;
				bReply = TRUE;
			}
			pMalloc->Free ( pidl );
		}
		pMalloc->Free ( pidlStartFolder );
		pMalloc->Release();
	}

	return bReply;
} 

/**********************************************************************/
