/**********************************************************************/

CSSInt64Database::CSSInt64Database()
{
	m_bIndexed = TRUE;
}

/**********************************************************************/

bool CSSInt64Database::FindRecord ( __int64 n64Key )
{
	return Find ( n64Key );
}

/**********************************************************************/

bool CSSInt64Database::AddRecord ( __int64 n64Key )
{
	if ( FindRecord ( n64Key ) == FALSE )	// record does not exist
	{
		CString strBlank ( m_cSeparator, GetHeaderSize()-1 );	// blank record filled with separators
		CCSV csv ( strBlank, m_cSeparator, m_cQuote );
		csv.SetAt ( m_nIndexCol, n64Key );
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

bool CSSInt64Database::Find ( __int64 n64Key )
{
	int nLow = 0;
	int nHigh = m_arRecord.GetSize() - 1;

	while ( nLow <= nHigh )
	{
		m_nRow = ( nLow + nHigh ) / 2;

		int nComp = SortCompare ( n64Key, GetInt64Key(m_nRow) );

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

int CSSInt64Database::SortCompare ( __int64 n64Key0, __int64 n64Key1 )
{
	if ( m_bAscending == TRUE )
	{
		if ( n64Key0 > n64Key1 )
			return 1;
		else if ( n64Key0 < n64Key1 )
			return -1;
	}
	else
	{
		if ( n64Key1 > n64Key0 )
			return 1;
		else if ( n64Key1 < n64Key0 )
			return -1;
	}
	return 0;
}

/**********************************************************************/

__int64 CSSInt64Database::GetInt64Key ( int nRow )
{
	return ( m_arRecord.GetAt(nRow)->GetInt64Key(m_nIndexCol) );
}

/**********************************************************************/

bool CSSInt64Database::IsSorted ( int i )
{
	static __int64 n64Key0;
	static __int64 n64Key1;

	if ( i == 0 )
		n64Key0 = GetInt64Key(0);
	else
	{
		n64Key1 = GetInt64Key(i);

		if ( SortCompare ( n64Key0, n64Key1 ) >= 0 )
		{
			m_strError.Format ( "Unsorted record in line %d", i+2 );
			return FALSE;
		}
		n64Key0 = n64Key1;
	}
	return TRUE;
}

/**********************************************************************/
