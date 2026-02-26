/**********************************************************************/

CSSSocket::CSSSocket()
{
	m_hSocket = NULL;

	SetRxTimeout ( 0 );		// default (no timeout)
	SetTxTimeout ( 0 );		// default (no timeout)
}

/**********************************************************************/

CSSSocket::CSSSocket ( DWORD dwConnectTimeout, DWORD dwRxTimeout, DWORD dwTxTimeout )
{
	m_hSocket = NULL;

	SetRxTimeout ( dwRxTimeout );		// milliseconds
	SetTxTimeout ( dwTxTimeout );		// milliseconds
}

/**********************************************************************/

CSSSocket::~CSSSocket()
{
	Close();
}

/**********************************************************************/

void CSSSocket::SetRxTimeout ( DWORD dwRxTimeout )
{
	m_tvRx.tv_sec = dwRxTimeout / 1000;			// seconds
	m_tvRx.tv_usec = (dwRxTimeout % 1000 ) * 1000;	// microseconds
}

/**********************************************************************/

void CSSSocket::SetTxTimeout ( DWORD dwTxTimeout )
{
	m_tvTx.tv_sec = dwTxTimeout / 1000;			// seconds
	m_tvTx.tv_usec = (dwTxTimeout % 1000 ) * 1000;	// microseconds
}

/**********************************************************************/

BOOL CSSSocket::Accept ( CSSSocket& sock )
{
	BOOL bReply = FALSE;

	SOCKET hSocket;
	if (( hSocket = accept ( m_hSocket, NULL, NULL )) != INVALID_SOCKET )
	{
		sock.m_hSocket = hSocket;
		bReply = TRUE;
	}
	else
		m_strError = GetLastErrorMessage ( WSAGetLastError() );

	return bReply;
}

/**********************************************************************/

void CSSSocket::Attach ( SOCKET hSocket )
{
	m_hSocket = hSocket;
}

/**********************************************************************/

BOOL CSSSocket::Bind ( UINT uiPort, const char* szIPAddress )
{
	BOOL bReply = FALSE;

	SOCKADDR_IN sa;
	memset ( &sa, 0, sizeof(sa) );
	sa.sin_family = AF_INET;

	if (szIPAddress != NULL)
	{
		inet_pton(AF_INET, szIPAddress, &sa.sin_addr.s_addr);
	}
	else
	{
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	sa.sin_port = htons ( uiPort );

	if ( bind ( m_hSocket, (SOCKADDR*)&sa, sizeof(sa) ) != SOCKET_ERROR )
		bReply = TRUE;
	else
		m_strError = GetLastErrorMessage ( WSAGetLastError() );

	return bReply;
}

/**********************************************************************/

BOOL CSSSocket::Connect ( const char* szIPAddress, UINT uiPort )
{
	BOOL bReply = FALSE;

	SOCKADDR_IN sa;
	memset ( &sa, 0, sizeof(sa) );
	sa.sin_family = AF_INET;
	inet_pton(AF_INET, szIPAddress, &sa.sin_addr.s_addr);
	sa.sin_port = htons ( uiPort );

	if ( connect ( m_hSocket, (SOCKADDR*)&sa, sizeof(sa) ) != SOCKET_ERROR )
		bReply = TRUE;
	else
		m_strError = GetLastErrorMessage ( WSAGetLastError() );

	return bReply;
}

/**********************************************************************/

void CSSSocket::Close()
{
	if ( m_hSocket != NULL )
	{
		closesocket ( m_hSocket );
		m_hSocket = NULL;
	}
}

/**********************************************************************/

BOOL CSSSocket::Create ( UINT uiPort, int nSocketType )
{
	BOOL bReply = FALSE;

	if (( m_hSocket = socket ( AF_INET, nSocketType, IPPROTO_TCP )) != INVALID_SOCKET )
		bReply = TRUE;
	else
		m_strError = GetLastErrorMessage ( WSAGetLastError() );

	return bReply;
}

/**********************************************************************/

SOCKET CSSSocket::Detach()
{
	SOCKET hSocket = m_hSocket;
	m_hSocket = NULL;
	return hSocket;
}

/**********************************************************************/

BOOL CSSSocket::Listen ( int nBacklog )
{
	BOOL bReply = FALSE;

	if ( listen ( m_hSocket, nBacklog ) != SOCKET_ERROR )
		bReply = TRUE;
	else
		m_strError = GetLastErrorMessage ( WSAGetLastError() );

	return bReply;
}

/**********************************************************************/

int CSSSocket::Shutdown ( int nHow )
{
	return shutdown ( m_hSocket, nHow );	// SD_RECEIVE, SD_SEND, SD_BOTH
}

/**********************************************************************/

int CSSSocket::Receive ( void* lpBuf, int nBufLen )
{
	int nBytesReceived = SOCKET_ERROR;

	FD_SET fd = { 1, m_hSocket };
	int nReply;

	if (( m_tvRx.tv_sec + m_tvRx.tv_usec ) != 0 )
		nReply = select ( 0, &fd, NULL, NULL, &m_tvRx );	// set timeout
	else
		nReply = select ( 0, &fd, NULL, NULL, NULL );	// infinite

	if ( nReply == SOCKET_ERROR )
	{
		m_strError = GetLastErrorMessage ( WSAGetLastError() );
	}
	else if ( nReply == 0 )
	{
		m_strError = "Receive timeout";
	}
	else
	{
		if (( nBytesReceived = recv ( m_hSocket, (char*) lpBuf, nBufLen, 0 )) == SOCKET_ERROR )
			m_strError = GetLastErrorMessage ( WSAGetLastError() );
	}

	return nBytesReceived;
}

/**********************************************************************/

int CSSSocket::Send ( void* lpBuf, int nBufLen )
{
	int nBytesSent = SOCKET_ERROR;

	FD_SET fd = { 1, m_hSocket };
	int nReply;

	if (( m_tvTx.tv_sec + m_tvTx.tv_usec ) != 0 )
		nReply = select ( 0, NULL, &fd, NULL, &m_tvTx );	// set timeout
	else
		nReply = select ( 0, NULL, &fd, NULL, NULL );	// infinite

	if ( nReply == SOCKET_ERROR )
	{
		m_strError = GetLastErrorMessage ( WSAGetLastError() );
	}
	else if ( nReply == 0 )
	{
		m_strError = "Send timeout";
	}
	else
	{
		if (( nBytesSent = send ( m_hSocket, (char*) lpBuf, nBufLen, 0 )) == SOCKET_ERROR )
			m_strError = GetLastErrorMessage ( WSAGetLastError() );
	}

	return nBytesSent;
}

/**********************************************************************/

bool CSSSocket::RxData ( unsigned char* pBuf, int nLength )
{
	bool bReply = FALSE;

	int nTotal = 0;

	while ( TRUE )
	{
		int nBytesReceived = Receive ( &pBuf[nTotal], nLength-nTotal );

		if ( nBytesReceived == SOCKET_ERROR )
			break;

		else if ( nBytesReceived == 0 )
		{
			m_strError = "Connection closed";
			break;
		}
		else
		{
			nTotal += nBytesReceived;
			if ( nTotal == nLength )
			{
				bReply = TRUE;
				break;
			}
		}
	}

	return bReply;
}

/**********************************************************************/

bool CSSSocket::TxData ( unsigned char* pBuf, int nLength )
{
	bool bReply = FALSE;

	int nTotal = 0;

	while ( TRUE )
	{
		int nBytesSent = Send ( &pBuf[nTotal], nLength-nTotal );

		if ( nBytesSent == SOCKET_ERROR )
			break;

		nTotal += nBytesSent;
		if ( nTotal == nLength )
		{
			bReply = TRUE;
			break;
		}
	}

	return bReply;
}

/**********************************************************************/
