/**********************************************************************/

bool CSSSocket::ReadLine ( CString& strBuf )
{
	bool bReply = FALSE;

	while ( TRUE )
	{
		if ( m_strRxBuf.GetLength() > 0 )		// got some data
		{
			int n = m_strRxBuf.Find ( '\n' );	// look for new line
			if ( n >= 0 )		// found it
			{
				strBuf = m_strRxBuf.Left(n+1);
				m_strRxBuf.Delete ( 0, n+1 );
				bReply = TRUE;
				break;
			}
			else		// not found it
			{
				if ( RxText() == FALSE )	// get more data
					break;
			}
		}
		else		// no data in buffer
		{
			if ( RxText() == FALSE )	// get more data
				break;
		}
	}

	return bReply;
}

/**********************************************************************/

bool CSSSocket::ReadString ( CString& strBuf )
{
	bool bReply = ReadLine ( strBuf );
	if ( bReply == TRUE )
	{
		strBuf.TrimRight ( '\n' );	// discard linefeed
		strBuf.TrimRight ( '\r' );	// discard carriage return
	}

	return bReply;
}

/**********************************************************************/

bool CSSSocket::ReadData ( CString& strBuf, int nLength )
{
	bool bReply = FALSE;

	while ( TRUE )
	{
		if ( m_strRxBuf.GetLength() >= nLength )	// got some data
		{
			strBuf = m_strRxBuf.Left(nLength);
			m_strRxBuf.Delete ( 0, nLength );
			bReply = TRUE;
			break;
		}
		else		// get more data
		{
			if ( RxText() == FALSE )	// get more data
				break;
		}
	}

	return bReply;
}

/**********************************************************************/

bool CSSSocket::WriteLine ( const char* szBuf )
{
	CString strBuf = szBuf;
	strBuf += '\r';	// append carriage return
	strBuf += '\n';	// append linefeed

	return TxData ( (unsigned char*)LPCTSTR(strBuf), strBuf.GetLength() );
}

/**********************************************************************/

bool CSSSocket::WriteString ( const char* szBuf )
{
	return TxData ( (unsigned char*)szBuf, strlen(szBuf) );
}

/**********************************************************************/

bool CSSSocket::RxText()
{
	bool bReply = FALSE;

	char buf[4096+1];		// 4k buffer + space for for trailing '\0' to be added

	int nBytesReceived = Receive ( buf, sizeof(buf)-1 );	// 4k
	if ( nBytesReceived != SOCKET_ERROR )
	{
		if ( nBytesReceived == 0 )
		{
			m_strError = "Connection closed";
		}
		else
		{
			buf[nBytesReceived] = '\0';	// end of string
			m_strRxBuf += buf;			// append to receive buffer
			bReply = TRUE;
		}
	}

	return bReply;
}

/**********************************************************************/
