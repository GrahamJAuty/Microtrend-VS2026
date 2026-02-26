/**********************************************************************/

CSSSocket CSSServer::m_sockListen;		// static listening socket
int CSSServer::m_nMaxThreads;
int CSSServer::m_nThreadCount;
bool CSSServer::m_bTerminate;

/**********************************************************************/

CSSServer::CSSServer ( int nMaxThreads, DWORD dwExitTimeout )
{
	m_nMaxThreads = nMaxThreads;			// max number of simultaneous threads
	m_dwExitTimeout = dwExitTimeout;		// exit timeout

	Init();		// initialize
}

/**********************************************************************/

void CSSServer::Init()
{
	m_nThreadCount = 0;			// number of threads in use
	m_bListening = FALSE;		// not listening
	m_bTerminate = FALSE;		// tell all threads to terminate
}

/**********************************************************************/

UINT CSSServer::ThreadProc ( LPVOID pParam )
{
	CSSServer* pServer = (CSSServer*) pParam;
	pServer->IncThreadCount();

	CSSSocket sockConnect;

	// waits here forever until connection received or socket closed

	if ( pServer->m_sockListen.Accept ( sockConnect ) != 0 )
	{
		if ( AfxBeginThread ( ThreadProc, pServer ) != NULL )
		{
			if ( pServer->m_nThreadCount < pServer->m_nMaxThreads )
			{
				pServer->OnConnect ( sockConnect, pServer->m_nThreadCount, pServer->m_bTerminate );
			}
		}
		sockConnect.Close();
	}
	pServer->DecThreadCount();
	return 0;
}

/**********************************************************************/

bool CSSServer::StartListening ( UINT uiPort )
{
	bool bReply = FALSE;

	if ( m_bListening == FALSE )	// not listening
	{
		Init();	// initialize

		if ( m_sockListen.Create ( 0, SOCK_STREAM ) != 0 )
		{
			if ( m_sockListen.Bind ( uiPort ) != 0 )
			{
				if ( m_sockListen.Listen() != 0 )
				{
					m_nThreadCount = 0;
					m_bListening = TRUE;	// listening

					if ( AfxBeginThread ( ThreadProc, this ) != NULL )
						bReply = TRUE;
					else
					{
						m_strError = "Cannot start server thread";
						m_sockListen.Close();
						m_bListening = FALSE;	// not listening
					}
				}
				else
				{
					m_strError = m_sockListen.GetError();
					m_sockListen.Close();
				}
			}
			else
			{
				m_strError = m_sockListen.GetError();
				m_sockListen.Close();
			}
		}
		else
			m_strError = m_sockListen.GetError();
	}
	else
		bReply = TRUE;		// server already listening

	return bReply;
}

/**********************************************************************/

bool CSSServer::StopListening()
{
	if ( m_bListening == TRUE )
	{
		m_sockListen.Close();	// close socket (this will exit accept loop in thread)

		m_bTerminate = TRUE;	// tell all threads to terminate

		// wait for all threads to finish

		DWORD dwEndTime = ::GetTickCount() + m_dwExitTimeout;	// now + timeout

		while ( m_nThreadCount > 0 )
		{
			if ( dwEndTime <= ::GetTickCount() )
				break;

			for ( int i = 0 ; i < 50 ; i++ )
				PumpMessage();

			Sleep ( 1 );		// millisecs
		}

		m_bListening = FALSE;	// not listening

		switch ( m_nThreadCount )
		{
		case 0:	return TRUE;	
		case 1:	m_strError = "1 thread could not be terminated";		break;
		default:	m_strError.Format ( "%d threads could not be terminated", m_nThreadCount );	break;
		}
		return FALSE;
	}
	return TRUE;
}

/**********************************************************************/

bool CSSServer::RemoteStopListening()
{
	DecThreadCount();			// because we are in thread now
	return StopListening();		// wait for connections to be completed
}

/**********************************************************************/
