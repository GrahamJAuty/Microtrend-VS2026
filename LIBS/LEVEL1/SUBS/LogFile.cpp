/**********************************************************************/

CSSLogFile::CSSLogFile()
	: m_strFilename ( "" )
{
}

CSSLogFile::CSSLogFile ( const char* szFilename )
	: m_strFilename ( szFilename )
{
}

/**********************************************************************/

CSSLogFile::~CSSLogFile()
{
	if ( m_strArray.GetSize() > 0 )
	{
		WriteArray();
		m_strArray.RemoveAll();
	}
}

/**********************************************************************/

void CSSLogFile::SetFilename ( const char* szFilename )
{
	m_strFilename = szFilename;
}

/**********************************************************************/

void CSSLogFile::WriteLine ( const char* szText )
{
	if ( m_strFilename != "" )
	{
		bool bReply;

		CSSFile file;
		if (( bReply = file.Open ( m_strFilename, "ab", _SH_DENYWR )) == TRUE )
		{
			if ( m_strArray.GetSize() > 0 )	// write lines in array first
			{
				for ( int i = 0 ; i < m_strArray.GetSize() ; i++ )
				{
					if (( bReply = file.WriteLine ( m_strArray.GetAt(0) )) == TRUE )
						m_strArray.RemoveAt(0);		// successfully written, delete line from array
					else
						break;		// unable to write to disk
				}
			}

			if ( bReply == TRUE )
				bReply = file.WriteLine ( szText );	// try and write new line to disk

			file.Close();
		}

		if ( bReply == FALSE )			// unable to write to disk
			m_strArray.Add ( szText );	// add line to array
	}
}

/**********************************************************************/

void CSSLogFile::AppendFile ( const char* szFilename )
{
	if ( m_strFilename != "" )
	{
		CSSFile file;
		if ( file.Open ( szFilename, "rb", _SH_DENYNO ) == TRUE )
		{
			CString strBuf;

			while ( file.ReadString ( strBuf ) == TRUE )
				m_strArray.Add ( strBuf );

			file.Close();

			WriteArray();
		}
	}
}

/**********************************************************************/

void CSSLogFile::WriteArray()
{
	if ( m_strFilename != "" )
	{
		if ( m_strArray.GetSize() > 0 )
		{
			CSSFile file;
			if ( file.Open ( m_strFilename, "ab", _SH_DENYWR ) == TRUE )
			{
				for ( int i = 0 ; i < m_strArray.GetSize() ; i++ )
				{
					if ( file.WriteLine ( m_strArray.GetAt(0) ) == TRUE )
						m_strArray.RemoveAt(0);	// successfully written, delete line
					else
						break;	// unable to write to disk
				}

				file.Close();
			}
		}
	}
}

/**********************************************************************/
