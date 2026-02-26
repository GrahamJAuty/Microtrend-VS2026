/**********************************************************************/

#define BOM	0xfeff	// byte order mark

/**********************************************************************/

CSSUnicodeFile::CSSUnicodeFile() : m_wBuf{ 0 }
{
	m_fp = NULL;
}

/**********************************************************************/

CSSUnicodeFile::~CSSUnicodeFile()
{
	Close();
}

/**********************************************************************/

bool CSSUnicodeFile::Open ( const char* szFilename, const char* szMode )
{
	Close();

	CString strMode ( szMode );
	int nShare = _SH_DENYNO;

	if ( strMode.Left(1) == "w" || strMode.Left(1) == "a" || strMode.Left(2) == "r+" )
		nShare = _SH_DENYWR;

	int i = 0;
	do
	{
		if (( m_fp = _fsopen ( szFilename, szMode, nShare )) != NULL )
			return TRUE;

		if ( GetLastError() != ERROR_SHARING_VIOLATION )
			break;

		WaitLoop ( 100 );		// wait 100 msecs
	}
	while ( i++ < 10 );			// retry upto 10 times

	return FALSE;
}

/**********************************************************************/

bool CSSUnicodeFile::Open ( const char* szFilename, const char* szMode, int nShare, int nRetries )
{
	Close();

	int i = 0;
	do
	{
		if (( m_fp = _fsopen ( szFilename, szMode, nShare )) != NULL )
			return TRUE;

		if ( GetLastError() != ERROR_SHARING_VIOLATION )
			break;

		WaitLoop ( 100 );		// wait 100 msecs
	}
	while ( i++ < nRetries );	// number of retries

	return FALSE;
}

/**********************************************************************/

int CSSUnicodeFile::Close()
{
	int nReply = 0;

	if ( m_fp != NULL )
		if (( nReply = fclose ( m_fp )) == 0 )
			m_fp = NULL;

	return nReply;
}

/**********************************************************************/

bool CSSUnicodeFile::ReadString ( CString& strBuf )
{
	strBuf = "";

	if ( m_fp != NULL )
	{
		wchar_t* wBuf = m_wBuf;

		if ( ftell ( m_fp ) == 0 )		// beginning of file
			wBuf++;					// skip byte order mark

		if ( fgetws ( m_wBuf, sizeof(m_wBuf)/sizeof(unsigned short), m_fp ) != NULL )
		{
			unsigned char* ucBuf = (unsigned char*) m_wBuf;
			unsigned short w;

			while (( w = *wBuf++ ) != 0 )
			{
				if ( w == 0x0d || w == 0x0a )	// discard trailing crlf
					break;

				*ucBuf++ = (unsigned char) w;
			}

			*ucBuf = 0;	// end of line
			strBuf = (unsigned char*) m_wBuf;
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/

bool CSSUnicodeFile::WriteString ( const char* szBuf )
{
	if ( m_fp != NULL )
	{
		wchar_t* wBuf = m_wBuf;

		if ( ftell ( m_fp ) == 0 )	// beginning of file
			*wBuf++ = BOM;			// byte order mark

		unsigned char c;

		while (( c = *szBuf++ ) != 0 )
			*wBuf++ = (unsigned short) c;

		*wBuf = 0x00;

		if ( fputws ( m_wBuf, m_fp ) >= 0 )
			return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

bool CSSUnicodeFile::ReadLine ( CString& strBuf )
{
	strBuf = "";

	if ( m_fp != NULL )
	{
		wchar_t* wBuf = m_wBuf;

		if ( ftell ( m_fp ) == 0 )		// beginning of file
			wBuf++;					// skip byte order mark

		if ( fgetws ( m_wBuf, sizeof(m_wBuf)/sizeof(unsigned short), m_fp ) != NULL )
		{
			unsigned char* ucBuf = (unsigned char*) m_wBuf;
			unsigned short w;

			while (( w = *wBuf++ ) != 0 )
				*ucBuf++ = (unsigned char) w;

			*ucBuf = 0;	// end of line
			strBuf = (unsigned char*) m_wBuf;
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/

bool CSSUnicodeFile::WriteLine ( const char* szBuf )
{
	if ( m_fp != NULL )
	{
		wchar_t* wBuf = m_wBuf;

		if ( ftell ( m_fp ) == 0 )	// beginning of file
			*wBuf++ = BOM;			// byte order mark

		unsigned char c;

		while (( c = *szBuf++ ) != 0 )
			*wBuf++ = (unsigned short) c;

		*wBuf++ = 0x0d;
		*wBuf++ = 0x0a;
		*wBuf = 0x00;

		if ( fputws ( m_wBuf, m_fp ) >= 0 )
			return TRUE;
	}
	return FALSE;
}

/**********************************************************************/
