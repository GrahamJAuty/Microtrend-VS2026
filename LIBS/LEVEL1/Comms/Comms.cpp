/**********************************************************************/

volatile bool CComms::m_bReadThreadActive;
volatile bool CComms::m_bTimerThreadActive;
volatile DWORD CComms::m_dwTimeMSecs;
HANDLE CComms::m_hTimerEvent;

/**********************************************************************/

CComms::CComms()
{
	m_hComm = NULL;
	m_hTimerEvent = NULL;
	m_bTimerThreadActive = FALSE;
	m_bReadThreadActive = FALSE;
	m_hTimerThread = NULL;
}

/**********************************************************************/

CComms::CComms ( HANDLE hComm )
{
	m_hComm = hComm;
	m_hTimerEvent = NULL;
	m_bTimerThreadActive = FALSE;
	m_bReadThreadActive = FALSE;
	m_hTimerThread = NULL;
}

/**********************************************************************/

CComms::~CComms()
{
	CommDisableNotification();
	EndTimerThread();
}

/**********************************************************************/

bool CComms::Open ( const char* szMode, UINT cbInQueue, UINT cbOutQueue )
{
	CString strPort = "\\\\.\\";		// required for COM10 and above
	CString strMode;
	bool bColon = FALSE;

	for ( int i = 0 ; i < (int) strlen ( szMode ) ; i++ )
	{
		char c = szMode[i];

		if ( bColon == FALSE )
		{
			if ( c == ':' )
				bColon = TRUE;
			else
				strPort += c;		// extract COM1 from "COM1:9600,n,8,1"
		}
		else
			strMode += c;		// extract 9600,n,8,1 from "COM1:9600,n,8,1"
	}

	SECURITY_ATTRIBUTES sa{};
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	HANDLE hReply = ::CreateFile ( strPort,
						GENERIC_READ | GENERIC_WRITE,
						0,
						&sa,
						OPEN_EXISTING,
						FILE_FLAG_OVERLAPPED,
						NULL );

	if ( hReply != INVALID_HANDLE_VALUE )
	{
		m_hComm = hReply;

		if ( ::SetupComm ( m_hComm, (DWORD)cbInQueue, (DWORD)cbOutQueue ) != 0 )
		{
			DCB dcb;

			if ( ::GetCommState ( m_hComm, &dcb ) != 0 )		// fill dcb with sensible values
			{
				if ( ::BuildCommDCB ( strMode, &dcb ) != 0 )
				{
					dcb.fOutX = 0;			// disable xon/xoff
					dcb.fInX = 0;			// disable xon/xoff

					if ( ::SetCommState ( m_hComm, &dcb ) != 0 )
					{
						COMMTIMEOUTS cmto;

						if ( ::GetCommTimeouts ( m_hComm, &cmto ) != 0 )	// set tx timeout
						{
							cmto.WriteTotalTimeoutMultiplier = 0;
							cmto.WriteTotalTimeoutConstant = 5000;		// 5 seconds

							return ::SetCommTimeouts ( m_hComm, &cmto ) != 0;
						}
					}
				}
			}
		}
	}
	return FALSE;
}

/**********************************************************************/

bool CComms::Close()
{
	if ( m_hComm != NULL )
	{
		::CloseHandle ( m_hComm );
		m_hComm = NULL;
	}
	return TRUE;
}

/**********************************************************************/

bool CComms::CommGetCTS()
{
	DWORD dwStatus;

	::GetCommModemStatus ( m_hComm, &dwStatus );

	if (( dwStatus & MS_CTS_ON ) != 0 )
		return TRUE;					// cts on

	return FALSE;						// cts off
}

/**********************************************************************/

bool CComms::CommGetDCD()
{
	DWORD dwStatus;

	::GetCommModemStatus ( m_hComm, &dwStatus );

	if (( dwStatus & MS_RLSD_ON ) != 0 )
		return TRUE;					// dcd on

	return FALSE;						// dcd off
}

/**********************************************************************/

bool CComms::CommGetRI()
{
	DWORD dwStatus;

	::GetCommModemStatus ( m_hComm, &dwStatus );

	if (( dwStatus & MS_RING_ON ) != 0 )
		return TRUE;					// ri on

	return FALSE;						// ri off
}

/**********************************************************************/

bool CComms::CommTxChar ( const char c )
{
	return CommTxBlock ( &c, 1 );
}

/**********************************************************************/

bool CComms::CommTxString ( const char* szBuf )
{
	return CommTxBlock ( szBuf, lstrlen(szBuf) );
}

/**********************************************************************/

bool CComms::CommTxBlock ( const char* pBuf, DWORD dwCount )
{
	OVERLAPPED osWrite = { 0, 0, 0, 0, NULL };
	DWORD dwBufPtr = 0;
	DWORD dwBytesWritten;
	bool bReply = TRUE;

	osWrite.hEvent = CreateEvent ( NULL, TRUE, FALSE, NULL );	// create this write operation's OVERLAPPED structure's hEvent.

	if ( osWrite.hEvent == NULL )
		return FALSE;				// error creating overlapped event handle

	do
	{
		bool bWriteOk = FALSE;

		if ( WriteFile ( m_hComm, &pBuf[dwBufPtr], dwCount, &dwBytesWritten, &osWrite ) != 0 )	// issue write.
		{
			bWriteOk = TRUE;
		}
		else
		{
			if ( GetLastError() == ERROR_IO_PENDING )	// write is pending
			{
				DWORD dwRes = WaitForSingleObject ( osWrite.hEvent, INFINITE );

				if ( dwRes == WAIT_OBJECT_0 )		// OVERLAPPED structure's event has been signaled. 
				{
					if ( GetOverlappedResult ( m_hComm, &osWrite, &dwBytesWritten, TRUE ) != 0 )
						bWriteOk = TRUE;
				}
			}
		}

		if ( bWriteOk == TRUE && dwBytesWritten > 0 )	// bytes written = 0 if tx timeout
		{
			dwCount -= dwBytesWritten;
			dwBufPtr += dwBytesWritten;
		}
		else
		{
			bReply = FALSE;
			break;
		}
	}
	while ( dwCount > 0 );	// keep looping until all characters have been written

	CloseHandle ( osWrite.hEvent );

	return bReply;
}

/**********************************************************************/
/*	Read Thread											*/
/**********************************************************************/

bool CComms::CommEnableNotification ( DWORD dwEventMask )
{
	if ( m_bReadThreadActive == FALSE )
	{
		m_bReadThreadActive = TRUE;

		if ( ::SetCommMask ( m_hComm, dwEventMask ) != 0 )		// specify events to look for in thread
		{
			if ( AfxBeginThread ( ReadThread, this ) != NULL )	// thread started
				return TRUE;
		}

		m_bReadThreadActive = FALSE;
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CComms::CommDisableNotification()
{
	if ( m_bReadThreadActive == TRUE )
	{
		m_bReadThreadActive = FALSE;

		::SetCommMask ( m_hComm, 0 );		// terminate waitcommevent in readthread
	}
}

/**********************************************************************/

UINT CComms::ReadThread ( LPVOID pParam )
{
	CComms* pComms = (CComms*) pParam;
	HANDLE hComm = pComms->GetHandle();
	OVERLAPPED oRead = { 0, 0, 0, 0, NULL };
	OVERLAPPED oCommEvent = { 0, 0, 0, 0, NULL };
	DWORD dwEvent = 0;
	COMSTAT cs;

	oRead.hEvent = CreateEvent ( NULL, TRUE, FALSE, NULL );
	oCommEvent.hEvent = CreateEvent ( NULL, TRUE, FALSE, NULL );

	while ( m_bReadThreadActive == TRUE )
	{
		bool bCommEventOk = FALSE;

		if ( ::WaitCommEvent ( hComm, &dwEvent, &oCommEvent ) != 0 )
		{
			bCommEventOk = TRUE;
		}
		else
		{
			if ( GetLastError() == ERROR_IO_PENDING )	// event is pending
			{
				DWORD dwRes = WaitForSingleObject ( oCommEvent.hEvent, INFINITE );

				if ( dwRes == WAIT_OBJECT_0 )		// OVERLAPPED structure's event has been signaled. 
				{
					DWORD dwDummy;

					if ( GetOverlappedResult ( hComm, &oCommEvent, &dwDummy, TRUE ) != 0 )
						bCommEventOk = TRUE;
				}
			}
		}

		if ( bCommEventOk == TRUE )
		{
			DWORD dwError;

			::ClearCommError ( hComm, &dwError, &cs );

			if ( (dwEvent & EV_RXCHAR) != 0 && cs.cbInQue != 0 )
			{
				bool bReadOk = FALSE;
				DWORD dwBufSize = cs.cbInQue;
				char* pBuf = new char[dwBufSize];
				
				DWORD dwBytesRead = 0;

				if ( ::ReadFile ( hComm, pBuf, cs.cbInQue, &dwBytesRead, &oRead ) != 0 )
				{
					bReadOk = TRUE;
				}
				else
				{
					if ( GetLastError() == ERROR_IO_PENDING )	// read is pending
					{
						DWORD dwRes = WaitForSingleObject ( oRead.hEvent, INFINITE );

						if ( dwRes == WAIT_OBJECT_0 )		// OVERLAPPED structure's event has been signaled. 
						{
							if ( GetOverlappedResult ( hComm, &oRead, &dwBytesRead, TRUE ) != 0 )
								bReadOk = TRUE;
						}
					}
				}

				if ( ( bReadOk == TRUE ) && ( dwBytesRead <= dwBufSize ) )
				{
					for ( DWORD i = 0 ; i < dwBytesRead; i++ )
					{
						pComms->OnCommRxChar ( pBuf[i] );
					}
				}

				delete [] pBuf;
			}
			if (( dwEvent & ~(EV_RXCHAR | EV_TXEMPTY) ) != 0 )	// anything other than rxchar or txempty
				pComms->OnCommEventMask ( dwEvent );
		}
	}

	::PurgeComm ( hComm, PURGE_RXCLEAR );	// clean out any pending bytes in the receive buffer

	CloseHandle ( oRead.hEvent );
	CloseHandle ( oCommEvent.hEvent );

	return 0;
}

/**********************************************************************/
/*	Timer Thread											*/
/**********************************************************************/

void CComms::BeginTimerThread()
{
	m_hTimerEvent = CreateEvent ( NULL, FALSE, FALSE, NULL );
	if ( m_hTimerEvent != NULL )
	{
		m_bTimerThreadActive = TRUE;
		m_dwTimeMSecs = INFINITE;

		DWORD dwThreadID;

		m_hTimerThread = CreateThread ( NULL, 0, (LPTHREAD_START_ROUTINE) TimerThread, this, 0, &dwThreadID );

		if ( m_hTimerThread == INVALID_HANDLE_VALUE )
		{
			CloseHandle ( m_hTimerEvent );
			m_hTimerEvent = NULL;
		}
	}
}

/**********************************************************************/

void CComms::EndTimerThread()
{
	if ( m_hTimerEvent != NULL )
	{
		DWORD dwThreadID;

		m_bTimerThreadActive = FALSE;		// break out of timer thread

		SetEvent ( m_hTimerEvent );		// break out of WaitForSingleObject()

		while ( GetExitCodeThread ( m_hTimerThread, &dwThreadID ) != 0 )	// wait for thread to finish
		{
			if ( dwThreadID != STILL_ACTIVE )
				break;
		}

		CloseHandle ( m_hTimerThread );
		CloseHandle ( m_hTimerEvent );
		m_hTimerEvent = NULL;
	}
}

/**********************************************************************/

bool CComms::CommStartTimer ( DWORD dwTimeMSecs )
{
	if ( m_hTimerEvent == NULL )		// timer thread not started
		BeginTimerThread();

	if ( m_hTimerEvent != NULL )
	{
		m_dwTimeMSecs = dwTimeMSecs;

		SetEvent ( m_hTimerEvent );	// break out of WaitForSingleObject()

		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

void CComms::CommStopTimer()
{
	if ( m_hTimerEvent != NULL )
	{
		m_dwTimeMSecs = INFINITE;

		SetEvent ( m_hTimerEvent );	// break out of WaitForSingleObject()
	}
}

/**********************************************************************/

UINT CComms::TimerThread ( LPVOID pParam )
{
	CComms* pComms = (CComms*) pParam;

	while ( m_bTimerThreadActive == TRUE )
	{
		while ( WaitForSingleObject ( m_hTimerEvent, m_dwTimeMSecs ) == WAIT_TIMEOUT )
			pComms->OnCommTimer();
	}

	return 0;
}

/**********************************************************************/
