/**********************************************************************/

#define DB_CLOSED		0

/**********************************************************************/

CSharedStringArray::CSharedStringArray()
{
	m_bModified = FALSE;
	m_nMode = DB_CLOSED;
}

CSharedStringArray::~CSharedStringArray()
{
	Close();
}

/**********************************************************************/

int CSharedStringArray::Open ( const char* szFilename, int nMode )
{
	return Open ( szFilename, nMode, 0 );		// no retries
}

/**********************************************************************/

int CSharedStringArray::Open ( const char* szFilename, int nMode, int nRetries )
{
	if ( m_nMode != DB_CLOSED )
	{
		m_strError = "File is already open";
		return DB_ERR_FILE;
	}

	m_strFilename = szFilename;
	m_Array.RemoveAll();
	m_bModified = FALSE;			// array same as disk
	bool bReply = FALSE;

	if ( nMode == DB_READONLY )
	{
		if ( m_File.Open ( szFilename, "rb", _SH_DENYNO, nRetries ) == TRUE )		// deny none
		{
			CString strBuf;

			while ( m_File.ReadString ( strBuf ) == TRUE )
				m_Array.Add ( strBuf );

			m_File.Close();

			bReply = TRUE;
		}
	}
	else if ( nMode == DB_READWRITE )
	{
		if ( _access ( szFilename, 0 ) == 0 )
		{
			if ( m_File.Open ( szFilename, "r+b", _SH_DENYWR, nRetries ) == TRUE )	// deny write
			{
				CString strBuf;

				while ( m_File.ReadString ( strBuf ) == TRUE )
					m_Array.Add ( strBuf );

				bReply = TRUE;
			}
		}
		else				// file does not exist
		{
			if ( m_File.Open ( szFilename, "w+b", _SH_DENYWR, nRetries ) == TRUE )	// deny write
			{
				bReply = TRUE;
			}
		}
	}
	else
	{
		m_strError = "Invalid open mode";
		return DB_ERR_FILE;
	}

	if ( bReply == TRUE )
	{
		m_nMode = nMode;
		m_strError = "Ok";
		return DB_ERR_NONE;
	}

	DWORD dwError = GetLastError();

	if ( dwError == ERROR_SHARING_VIOLATION || dwError == 0 )
	{
		m_strError = "File is in use";
		return DB_ERR_SHARE;
	}
	else
	{
		m_strError = GetLastErrorMessage();
		return DB_ERR_FILE;
	}
}

/**********************************************************************/

bool CSharedStringArray::Close()
{
	if ( m_nMode == DB_READWRITE )
	{
		if ( m_bModified == TRUE )
		{
			if ( m_File.Seek ( 0, SEEK_SET ) == 0 )
			{
				if ( m_File.SetLength ( 0L ) == 0 )
				{
					for ( int i = 0 ; i < m_Array.GetSize() ; i++ )
					{
						if ( m_File.WriteLine ( m_Array.GetAt(i) ) == FALSE )
							return FALSE;
					}
				}
			}
		}

		if ( m_File.Close() != 0 )
			return FALSE;
	}

	m_bModified = FALSE;
	m_Array.RemoveAll();
	m_nMode = DB_CLOSED;
	return TRUE;
}

/**********************************************************************/

bool CSharedStringArray::ForceRead()
{
	if ( m_nMode == DB_READWRITE )
	{
		if ( m_File.Seek ( 0, SEEK_SET ) == 0 )
		{
			m_Array.RemoveAll();

			CString strBuf;

			while ( m_File.ReadString ( strBuf ) == TRUE )
				m_Array.Add ( strBuf );

			m_bModified = FALSE;	// array same as disk
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

/**********************************************************************/

bool CSharedStringArray::ForceWrite()
{
	if ( m_nMode == DB_READWRITE )
	{
		if ( m_File.Seek ( 0, SEEK_SET ) == 0 )
		{
			if ( m_File.SetLength ( 0L ) == 0 )
			{
				for ( int i = 0 ; i < m_Array.GetSize() ; i++ )
				{
					if ( m_File.WriteLine ( m_Array.GetAt(i) ) == FALSE )
						return FALSE;
				}

				if ( m_File.Flush() == 0 )	// force write to disk
				{
					m_bModified = FALSE;	// array same as disk
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

/**********************************************************************/

const char* CSharedStringArray::GetAt ( int nIndex )
{
	if ( nIndex >= 0 && nIndex < m_Array.GetSize() )
		return m_Array.GetAt(nIndex);

	return "";
}

/**********************************************************************/

void CSharedStringArray::SetAt ( int nIndex, const char* szText )
{
	if ( nIndex >= 0 && nIndex < m_Array.GetSize() )
	{
		if ( m_Array.GetAt(nIndex) != szText )
		{
			m_Array.SetAt ( nIndex, szText );
			m_bModified = TRUE;
		}
	}
}

/**********************************************************************/

int CSharedStringArray::Add ( const char* szText )
{
	int nIndex = m_Array.Add ( szText );
	m_bModified = TRUE;
	return nIndex;
}

/**********************************************************************/

void CSharedStringArray::RemoveAt ( int nIndex, int nCount )
{
	if ( nIndex >= 0 && nIndex < m_Array.GetSize() )
	{
		m_Array.RemoveAt ( nIndex, nCount );
		m_bModified = TRUE;
	}
}

/**********************************************************************/

void CSharedStringArray::InsertAt ( int nIndex, const char* szText, int nCount )
{
	if ( nIndex >= 0 )
	{
		if ( nIndex < m_Array.GetSize() )
			m_Array.InsertAt ( nIndex, szText, nCount );
		else
			m_Array.Add ( szText );

		m_bModified = TRUE;
	}
}

/**********************************************************************/

void CSharedStringArray::SetModifiedFlag ( bool b )
{
	if ( m_nMode == DB_READWRITE || m_nMode == DB_CLOSED )		// (db_closed for compatibilty with pre v4 software)
		m_bModified = b;
}

/**********************************************************************/
// for compatibilty with pre v4 software only

bool CSharedStringArray::Read ( const char* szFilename )
{
	if ( m_nMode == DB_CLOSED )
	{
		m_Array.RemoveAll();
		m_bModified = FALSE;		/* array same as disk */

		CSSFile file;

		if ( file.Open ( szFilename, "rb" ) == TRUE )
		{
			CString strBuf;

			while ( file.ReadString ( strBuf ) == TRUE )
				m_Array.Add ( strBuf );

			file.Close();

			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/
// for compatibilty with pre v4 software only

bool CSharedStringArray::Write ( const char* szFilename )
{
	bool bReply = FALSE;

	if ( m_nMode == DB_CLOSED )
	{
		CSSFile file;
		if (( bReply = file.Open ( szFilename, "wb" )) == TRUE )
		{
			for ( int i = 0 ; i < m_Array.GetSize() ; i++ )
			{
				bReply = file.WriteLine ( m_Array.GetAt(i) );
				if ( bReply == FALSE )
					break;
			}

			file.Close();

			if ( bReply == TRUE )		/* all lines written */
				m_bModified = FALSE;	/* array same as disk */
		}
	}

	return bReply;
}

/**********************************************************************/
