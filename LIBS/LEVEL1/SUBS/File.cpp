/**********************************************************************/

#define BUFLEN 2048		// used for ReadLine(CString&), ReadString(CString&) and Read(CCSV&)

/**********************************************************************/
/*	CSSFile												*/
/**********************************************************************/

CSSFile::CSSFile()
{
	m_fp = NULL;
}

/**********************************************************************/

CSSFile::~CSSFile()
{
	Close();
}

/**********************************************************************/

bool CSSFile::Open ( const char* szFilename, const char* szMode )
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

bool CSSFile::Open ( const char* szFilename, const char* szMode, int nShare, int nRetries )
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

int CSSFile::Close()
{
	int nReply = 0;

	if ( m_fp != NULL )
		if (( nReply = fclose ( m_fp )) == 0 )
			m_fp = NULL;

	return nReply;
}

/**********************************************************************/

int CSSFile::Read ( void* Buffer, int nSize, int nCount )
{
	return ( m_fp != NULL ) ? fread ( Buffer, nSize, nCount, m_fp ) : 0;
}

/**********************************************************************/

int CSSFile::Write ( const void* Buffer, int nSize, int nCount )
{
	return ( m_fp != NULL ) ? fwrite ( Buffer, nSize, nCount, m_fp ) : 0;
}

/**********************************************************************/

int CSSFile::GetChar()
{
	return ( m_fp != NULL ) ? fgetc ( m_fp ) : EOF;
}

/**********************************************************************/

int CSSFile::PutChar ( int nChar )
{
	return ( m_fp != NULL ) ? fputc ( nChar, m_fp ) : EOF;
}

/**********************************************************************/

int CSSFile::Seek ( long lOffset, int nOrigin )
{
	return ( m_fp != NULL ) ? fseek ( m_fp, lOffset, nOrigin ) : 0;
}

/**********************************************************************/

long CSSFile::Tell()
{
	return ( m_fp != NULL ) ? ftell ( m_fp ) : 0L;
}

/**********************************************************************/

int CSSFile::Flush()
{
	return ( m_fp != NULL ) ? fflush ( m_fp ) : EOF;
}

/**********************************************************************/

long CSSFile::GetLength()
{
	return ( m_fp != NULL ) ? _filelength(_fileno(m_fp)) : 0L;
}

/**********************************************************************/

int CSSFile::SetLength ( long lSize )
{
	return ( m_fp != NULL ) ? _chsize (_fileno(m_fp), lSize) : -1;
}

/**********************************************************************/

bool CSSFile::ReadLine ( CString& strBuf )
{
	strBuf = "";

	if ( m_fp != NULL )
	{
		char* pBuf = new char[BUFLEN];
		if ( pBuf != NULL )
		{
			while ( fgets ( pBuf, BUFLEN, m_fp ) != NULL )
			{
				strBuf += pBuf;

				if ( strBuf.Right(1) == '\n' )	// end of line
				{
					delete [] pBuf;
					return TRUE;
				}
			}

			delete [] pBuf;

			if ( strBuf != "" )		// eof - last line not terminated with lf
				return TRUE;
		}
	}

	return FALSE;
}

/**********************************************************************/

bool CSSFile::WriteLine ( const char* szBuf )
{
	if ( m_fp != NULL )
	{
		if ( fputs ( szBuf, m_fp ) >= 0 )
			if ( fputs ( "\r\n", m_fp ) >= 0 )
				return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

bool CSSFile::ReadString ( CString& strBuf )
{
	if ( ReadLine ( strBuf ) == TRUE )
	{
		strBuf.TrimRight ( "\r\n" );
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CSSFile::WriteString ( const char* szBuf )
{
	if ( m_fp != NULL )
	{
		if ( fputs ( szBuf, m_fp ) >= 0 )
			return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

bool CSSFile::Read ( CCSV& csv, int nMaxFields )
{
	CString strBuf;
	if ( ReadString ( strBuf ) == TRUE )
	{
		csv.ParseLine ( strBuf, nMaxFields );
		return TRUE;
	}

	csv.RemoveAll();
	return FALSE;
}

/**********************************************************************/

bool CSSFile::Write ( CCSV& csv )
{
	return WriteLine ( csv.GetLine() );
}

/**********************************************************************/

int CSSFile::GetPercentPosition()
{
	long lFileLength = GetLength();

	if ( lFileLength == 0L )
		return 0;

	return (int) ( ( (double) Tell() * 100.0 ) / (double) lFileLength );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

int MakeSubdirectory ( const char* szPath )
{
	if ( _access ( szPath, 0 ) != 0 )	// check if subdirectory exists
		return ( _mkdir ( szPath ) );	// create subdirectory

	return 0;
}

/**********************************************************************/

bool ExistSubdirectory ( const char* szPath )
{
	if ( _access ( szPath, 0 ) == 0 )	// check if subdirectory exists
		return TRUE;

	return FALSE;
}

/**********************************************************************/

CString SS_PathAppend ( const char* szPath, const char* szMore )
{
	CString strMore = szMore;
	strMore.TrimLeft ( '\\' );		// remove leading backslash

	CString strPath = szPath;
	strPath.TrimRight ( '\\' );		// remove trailing backslash
	strPath += '\\';				// add backslash
	strPath += strMore;

	return strPath;
}

/**********************************************************************/
