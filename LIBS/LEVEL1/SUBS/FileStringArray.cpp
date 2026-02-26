/**********************************************************************/

CFileStringArray::~CFileStringArray()
{
	RemoveAll();
}

/**********************************************************************/

bool CFileStringArray::Read ( const char* filename, int max )
{
	CSSFile file;

	if ( file.Open ( filename, "rb" ) == TRUE )
	{
		CString strBuf;
		int count = 0;

		while ( file.ReadString ( strBuf ) == TRUE )		// read and remove crlf
		{
			Add ( strBuf );

			if ( max != 0 && ++count >= max )
				break;
		}
		file.Close();

		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CFileStringArray::Write ( const char* filename )
{
	bool bReply;

	CSSFile file;
	if (( bReply = file.Open ( filename, "wb" )) == TRUE )
	{
		for ( int i = 0 ; i < GetSize() ; i++ )
		{
			bReply = file.WriteLine ( GetAt(i) );
			if ( bReply == FALSE )
				break;
		}

		file.Close();
	}

	return bReply;
}

/**********************************************************************/
