/**********************************************************************/

CCSVArray::CCSVArray()
{
	m_cSeparator = ',';
	m_cQuote = '"';
	m_bRemoveTrailing = FALSE;
	m_bRemoveZeros = FALSE;
	m_bMinimizeFloat = FALSE;
	m_bModified = FALSE;
}

CCSVArray::CCSVArray ( char cSeparator, char cQuote, bool bRemoveTrailing, bool bRemoveZeros, bool bMinimizeFloat )
{
	m_cSeparator = cSeparator;
	m_cQuote = cQuote;
	m_bRemoveTrailing = bRemoveTrailing;
	m_bRemoveZeros = bRemoveZeros;
	m_bMinimizeFloat = bMinimizeFloat;
	m_bModified = FALSE;
}

/**********************************************************************/

bool CCSVArray::Read ( const char* szFilename )
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
	return FALSE;
}

/**********************************************************************/

bool CCSVArray::Write ( const char* szFilename )
{
	bool bReply;

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

	return bReply;
}

/**********************************************************************/

const char* CCSVArray::GetAt ( int nIndex )
{
	if ( nIndex >= 0 && nIndex < m_Array.GetSize() )
		return m_Array.GetAt(nIndex);

	return "";
}

/**********************************************************************/

bool CCSVArray::GetAt ( int nIndex, CCSVRecord& Record )
{
	if ( nIndex >= 0 && nIndex < m_Array.GetSize() )
	{
		CCSV csv ( m_Array.GetAt(nIndex) );
		Record.ConvertFromCSV ( csv );
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

bool CCSVArray::SetAt ( int nIndex, CCSVRecord& Record )
{
	if ( nIndex >= 0 && nIndex < m_Array.GetSize() )
	{
		CCSV csv ( m_cSeparator, m_cQuote, m_bRemoveTrailing, m_bRemoveZeros, m_bMinimizeFloat );

		Record.ConvertToCSV ( csv );
		if ( m_Array.GetAt(nIndex) != csv.GetLine() )
		{
			m_Array.SetAt ( nIndex, csv.GetLine() );
			m_bModified = TRUE;
		}
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

int CCSVArray::Add ( CCSVRecord& Record )
{
	int nIndex;
	CCSV csv ( m_cSeparator, m_cQuote, m_bRemoveTrailing, m_bRemoveZeros, m_bMinimizeFloat );
	Record.ConvertToCSV ( csv );

	nIndex = m_Array.Add ( csv.GetLine() );
	m_bModified = TRUE;
	return nIndex;
}

/**********************************************************************/

bool CCSVArray::RemoveAt ( int nIndex, int nCount )
{
	if ( nIndex >= 0 && nIndex < m_Array.GetSize() )
	{
		m_Array.RemoveAt ( nIndex, nCount );
		m_bModified = TRUE;
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

bool CCSVArray::InsertAt ( int nIndex, CCSVRecord& Record, int nCount )
{
	if ( nIndex >= 0 )
	{
		CCSV csv ( m_cSeparator, m_cQuote, m_bRemoveTrailing, m_bRemoveZeros, m_bMinimizeFloat );

		Record.ConvertToCSV ( csv );

		if ( nIndex < m_Array.GetSize() )
			m_Array.InsertAt ( nIndex, csv.GetLine(), nCount );
		else
			m_Array.Add ( csv.GetLine() );

		m_bModified = TRUE;
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

