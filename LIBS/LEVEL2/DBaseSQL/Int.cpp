/**********************************************************************/

CSSIntDatabase::CSSIntDatabase()
{
	m_bIndexed = TRUE;
}

/**********************************************************************/

bool CSSIntDatabase::FindRecord ( int nKey )
{
	return Find ( nKey );
}

/**********************************************************************/

bool CSSIntDatabase::AddRecord ( int nKey )
{
	if ( FindRecord ( nKey ) == FALSE )	// record does not exist
	{
		CString strBlank ( m_cSeparator, GetHeaderSize()-1 );	// blank record filled with separators
		CCSV csv ( strBlank, m_cSeparator, m_cQuote );
		csv.SetAt ( m_nIndexCol, nKey );
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

bool CSSIntDatabase::Find ( int nKey )
{
	int nLow = 0;
	int nHigh = m_arRecord.GetSize() - 1;

	while ( nLow <= nHigh )
	{
		m_nRow = ( nLow + nHigh ) / 2;

		int nComp = SortCompare ( nKey, GetIntKey(m_nRow) );

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

int CSSIntDatabase::SortCompare ( int nKey0, int nKey1 )
{
	if ( m_bAscending == TRUE )
		return ( nKey0 - nKey1 );
	else
		return ( nKey1 - nKey0 );
}

/**********************************************************************/

int CSSIntDatabase::GetIntKey ( int nRow )
{
	return ( m_arRecord.GetAt(nRow)->GetIntKey(m_nIndexCol) );
}

/**********************************************************************/

bool CSSIntDatabase::IsSorted ( int i )
{
	static int nKey0;
	static int nKey1;

	if ( i == 0 )
		nKey0 = GetIntKey(0);
	else
	{
		nKey1 = GetIntKey(i);

		if ( SortCompare ( nKey0, nKey1 ) >= 0 )
		{
			m_strError.Format ( "Unsorted record in line %d", i+2 );
			return FALSE;
		}
		nKey0 = nKey1;
	}
	return TRUE;
}

/**********************************************************************/
