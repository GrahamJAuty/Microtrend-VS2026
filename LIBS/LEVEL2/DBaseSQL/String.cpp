/**********************************************************************/

CSSStringDatabase::CSSStringDatabase()
{
	m_bIndexed = TRUE;
}

/**********************************************************************/

bool CSSStringDatabase::FindRecord ( const char* szKey )
{
	return Find ( szKey );
}

/**********************************************************************/

bool CSSStringDatabase::AddRecord ( const char* szKey )
{
	if ( FindRecord ( szKey ) == FALSE )	// record does not exist
	{
		CString strBlank ( m_cSeparator, GetHeaderSize()-1 );	// blank record filled with separators
		CCSV csv ( strBlank, m_cSeparator, m_cQuote );
		csv.SetAt ( m_nIndexCol, szKey );
		if ( m_arRecord.InsertRecord ( m_nRow, csv.GetLine(), m_cSeparator, m_cQuote, m_nIndexCol ) == TRUE )
		{
			m_bDatabaseModified = TRUE;	// database has been modified
			m_bUpdateView = TRUE;		// need to update view
			return TRUE;
		}
	}
	return FALSE;		// record already exists
}

/**********************************************************************/

bool CSSStringDatabase::Find ( const char* szKey )
{
	int nLow = 0;
	int nHigh = m_arRecord.GetSize() - 1;
	CString strKey = FormatKey(szKey);

	while ( nLow <= nHigh )
	{
		m_nRow = ( nLow + nHigh ) / 2;

		int nComp = SortCompare ( strKey, GetStringKey(m_nRow) );

		if ( nComp < 0 )
			nHigh = m_nRow - 1;

		else if ( nComp > 0 )
			nLow = m_nRow + 1;

		else						// found key
			return TRUE;
	}

	m_nRow = nLow;
	return FALSE;
}

/**********************************************************************/

int CSSStringDatabase::SortCompare ( const char* szKey0, const char* szKey1 )
{
	if ( m_bIgnoreCase == TRUE )
	{
		if ( m_bAscending == TRUE )
			return _stricmp ( szKey0, szKey1 );
		else
			return _stricmp ( szKey1, szKey0 );
	}
	else
	{
		if ( m_bAscending == TRUE )
			return strcmp ( szKey0, szKey1 );
		else
			return strcmp ( szKey1, szKey0 );
	}
}

/**********************************************************************/

const char* CSSStringDatabase::GetStringKey ( int nRow )
{
	return FormatKey ( m_arRecord.GetAt(nRow)->GetStringKey(m_nIndexCol) );
}

/**********************************************************************/

bool CSSStringDatabase::IsSorted ( int i )
{
	static CString strKey0;
	static CString strKey1;

	if ( i == 0 )
		strKey0 = GetStringKey(0);
	else
	{
		strKey1 = GetStringKey(i);

		if ( SortCompare ( strKey0, strKey1 ) >= 0 )
		{
			m_strError.Format ( "Unsorted record in line %d", i+2 );
			return FALSE;
		}
		strKey0 = strKey1;
	}
	return TRUE;
}

/**********************************************************************/
