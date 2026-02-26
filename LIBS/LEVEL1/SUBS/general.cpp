/**********************************************************************/

void PumpMessage()
{
	MSG msg;

	if ( ::PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) != 0 )
	{
		::TranslateMessage ( &msg );
		::DispatchMessage ( &msg );
	}
}

/**********************************************************************/

void WaitLoop ( DWORD dwMSecs, bool bDisableSleep )
{
	LARGE_INTEGER StartCount = { 0 };

	if ( QueryPerformanceCounter ( &StartCount ) != 0 )	// get start count
	{
		LARGE_INTEGER Frequency = { 0 };

		if ( QueryPerformanceFrequency ( &Frequency ) != 0 )	// counts per seond (2,593,540,000 on my pc)
		{
			LARGE_INTEGER TimeUSecs = { 0 };
			LARGE_INTEGER TimeCounts = { 0 };
			LARGE_INTEGER SleepCounts = { 0 };
			LARGE_INTEGER CurrentCount = { 0 };
			LARGE_INTEGER EndCount = { 0 };
			LARGE_INTEGER RemainingtCount = { 0 };
				
			TimeUSecs.QuadPart = ((__int64) dwMSecs * 1000LL);
			TimeCounts.QuadPart = ( Frequency.QuadPart * TimeUSecs.QuadPart) / 1000000LL;
			SleepCounts.QuadPart = ( Frequency.QuadPart * 23000LL ) / 1000000LL;	// assume sleep(1) wont take more than 23 (prime) msecs (big assumption)
			EndCount.QuadPart = StartCount.QuadPart + TimeCounts.QuadPart;

			while ( TRUE )
			{
				PumpMessage();

				QueryPerformanceCounter ( &CurrentCount );		// get current count
				RemainingtCount.QuadPart = EndCount.QuadPart - CurrentCount.QuadPart;

				if ( RemainingtCount.QuadPart <= 0LL )
					return;

				if ( bDisableSleep == FALSE )
				{
					if ( RemainingtCount.QuadPart > SleepCounts.QuadPart )
						Sleep(1);		// hopefully not more than 23 msecs
				}
			}
		}
	}

	unsigned __int64 nEndTimeMSecs = ::GetTickCount64() + (unsigned __int64)dwMSecs;
	
	while ( nEndTimeMSecs > ::GetTickCount64() )
	{
		PumpMessage();
		if ( bDisableSleep == FALSE )
			Sleep ( 1 );
	}
}

/**********************************************************************/

void WaitExit ( bool& bExit, DWORD dwResolution )
{
	while ( bExit == FALSE )
	{
		for ( int i = 0 ; i < 50 ; i++ )
			PumpMessage();

		if ( bExit == FALSE )
			Sleep ( dwResolution );		// millisecs
	}
}

/**********************************************************************/

bool WaitExit ( bool& bExit, DWORD dwTimeoutMSecs, DWORD dwResolutionMSecs )
{
	unsigned __int64 StartTime = ::GetTickCount64();

	while ( bExit == FALSE )
	{
		if ( ( dwTimeoutMSecs > 0 ) && ( StartTime + ( unsigned __int64) dwTimeoutMSecs <= ::GetTickCount64() ) )
			break;

		for ( int i = 0 ; i < 50 ; i++ )
			PumpMessage();

		if ( bExit == FALSE && dwResolutionMSecs > 0 )
			Sleep ( dwResolutionMSecs );		// millisecs
	}
	return bExit;		// true if exit flag set, false if timeout
}

/**********************************************************************/

const char* SSMakeShortString ( CDC* pDC, const char* szLong, int nWidth )
{
	int nStringLen = strlen ( szLong );

	if ( nStringLen == 0 || nStringLen >= (MAX_PATH-1) || pDC->GetTextExtent(szLong,nStringLen).cx <= nWidth )
		return szLong;

	static char szShort[MAX_PATH];
	static const char* szDot3 = "...";
	static const char* szDot2 = "..";
	static const char* szDot1 = ".";
	int nDot3Len = pDC->GetTextExtent(szDot3,3).cx;

	strcpy_s ( szShort, sizeof(szShort), szLong );

	for ( int i = nStringLen-1 ; i > 0 ; i-- )
	{
		szShort[i] = 0;
		int nTextExtent = pDC->GetTextExtent(szShort,i).cx;

		if ( nTextExtent + nDot3Len <= nWidth )
		{
			strcat_s ( szShort, sizeof(szShort), szDot3 );
			break;
		}
		if ( i == 1 )
		{
			int nDot2Len = pDC->GetTextExtent(szDot2,2).cx;
			int nDot1Len = pDC->GetTextExtent(szDot1,1).cx;

			if ( nTextExtent + nDot2Len <= nWidth )
				strcat_s ( szShort, sizeof(szShort), szDot2 );
			else if ( nTextExtent + nDot1Len <= nWidth )
				strcat_s ( szShort, sizeof(szShort), szDot1 );
			else if ( nTextExtent > nWidth )
				szShort[0] = 0;
		}
	}
	return szShort;
}

/**********************************************************************/

CString GetLastErrorMessage()
{
	return GetLastErrorMessage ( GetLastError() );
}

CString GetLastErrorMessage ( DWORD dwError )
{
	LPVOID lpMessageBuffer = NULL; 

	FormatMessage (
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// the user default language
		(LPTSTR) &lpMessageBuffer,
		0,
		NULL );

	CString strError = (LPTSTR) lpMessageBuffer;
	strError.TrimRight ( "\r\n" );

	LocalFree ( lpMessageBuffer ); 

	return strError;
}

/**********************************************************************/

static CString AddQuotes ( const char* szParam )
{
	CString strParam ( '"' );			// add leading quote
	strParam += szParam;			// add string

	int nLen = strlen ( szParam );

	for ( int i = nLen-1 ; i >= 0 ; i-- )
	{
		if ( szParam[i] == '\\' )	// double up trailing backslashes
			strParam += '\\';
		else
			break;
	}

	strParam += '"';				// add trailing quote

	return strParam;
}

/**********************************************************************/

int SSspawnl ( CWnd* pWndParent, int mode, const char* cmdname, const char* arg0, const char* arg1, ... )
{
	CString strParams;

	va_list marker;
	va_start ( marker, arg1 );	// initialize variable arguments

	int nCount = 0;
	const char* szParam = arg1;
	while ( szParam != NULL )
	{
		if ( nCount++ > 0 )
			strParams += " ";

		strParams += AddQuotes ( szParam );
		szParam = va_arg ( marker, const char* );
	}

	va_end ( marker );			// reset variable arguments

	if ( mode == _P_WAIT )
	{
		DWORD dwForegroundLockTimeout = 0;
		if ( pWndParent != NULL )
		{
			::SystemParametersInfo ( SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &dwForegroundLockTimeout, 0 );
			::SystemParametersInfo ( SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (PVOID)0, 0 );
			pWndParent->EnableWindow ( FALSE );
		}

		DWORD dwExitCode = -1;
		HANDLE hProcess = (HANDLE) _spawnl ( _P_NOWAIT, cmdname, arg0, strParams, NULL );
		if ( (int) hProcess != -1 )
		{
			while ( WaitForSingleObject ( hProcess, 1 ) == WAIT_TIMEOUT )
			{
				MSG msg;
				while ( ::PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) != 0 )
				{
					::TranslateMessage ( &msg );
					::DispatchMessage ( &msg );
				}
			}

			GetExitCodeProcess ( hProcess, &dwExitCode );
		}

		if ( pWndParent != NULL )
		{
			pWndParent->SetForegroundWindow();
			::SystemParametersInfo ( SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (PVOID)dwForegroundLockTimeout, 0 );
			pWndParent->EnableWindow ( TRUE );
		}

		return dwExitCode;
	}
	else
		return _spawnl ( mode, cmdname, arg0, strParams, NULL );
}

/**********************************************************************/
