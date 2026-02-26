/**********************************************************************/

CHeaderArray::CHeaderArray()
{
	m_Map.InitHashTable ( 61 );	// prime number for best results
}

/**********************************************************************/

void CHeaderArray::RemoveAll()
{
	m_arrayHeader.RemoveAll();
	m_Map.RemoveAll();
}

/**********************************************************************/

int CHeaderArray::SetHeader ( const char* szHeader, char cSeparator, char cQuote, CString& strError )
{
	CCSV csv ( szHeader, cSeparator, cQuote );

	m_arrayHeader.RemoveAll();
	for ( int i = 0 ; i < csv.GetSize() ; i++ )
		m_arrayHeader.Add ( csv.GetString(i) );

	return SetMapArray ( strError );
}

/**********************************************************************/

CString CHeaderArray::GetHeader ( char cSeparator, char cQuote )
{
	CString strHeader;

	CCSV csvHeader ( cSeparator, cQuote );
	for ( int i = 0 ; i < m_arrayHeader.GetSize() ; i++ )
		csvHeader.Add ( m_arrayHeader.GetAt(i) );

	strHeader = csvHeader.GetLine();

	return strHeader;
}

/**********************************************************************/

int CHeaderArray::GetIndex ( const char* szLabel )
{
	int nIndex = -1;

	m_Map.Lookup ( szLabel, nIndex );

	return nIndex;
}

/**********************************************************************/

CString CHeaderArray::GetLabel ( int nCol )
{
	CString strLabel;

	if ( nCol >= 0 && nCol < m_arrayHeader.GetSize() )
		strLabel = m_arrayHeader.GetAt(nCol);

	return strLabel;
}

/**********************************************************************/

int CHeaderArray::AddLabel ( const char* szLabel )
{
	CString strError;
	m_arrayHeader.Add ( szLabel );
	return SetMapArray ( strError );
}

/**********************************************************************/

int CHeaderArray::InsertLabel ( int nCol, const char* szLabel )
{
	CString strError;
	m_arrayHeader.InsertAt ( nCol, szLabel );
	return SetMapArray ( strError );
}

/**********************************************************************/

int CHeaderArray::RenameLabel ( int nCol, const char* szLabel )
{
	CString strError;
	m_arrayHeader.SetAt ( nCol, szLabel );
	return SetMapArray ( strError );
}

/**********************************************************************/

int CHeaderArray::DeleteLabel ( int nCol )
{
	CString strError;
	m_arrayHeader.RemoveAt ( nCol );
	return SetMapArray ( strError );
}

/**********************************************************************/
/*	Private routines										*/
/**********************************************************************/

int CHeaderArray::SetMapArray ( CString& strError )
{
	m_Map.RemoveAll();

	for ( int i = 0 ; i < m_arrayHeader.GetSize() ; i++ )
	{
		if ( AddLabel ( m_arrayHeader.GetAt(i), i ) == FALSE )
		{
			strError.Format ( "Duplicate label in header (%s)", (const char*) m_arrayHeader.GetAt(i) );
			return DB_ERR_HEADER1;
		}
	}
	if ( m_Map.GetSize() == 0 )
	{
		strError = "Invalid header (no column text)";
		return DB_ERR_HEADER2;
	}
	else if ( m_Map.GetSize() < m_arrayHeader.GetSize() )
	{
		strError = "Invalid header (blank column text not allowed)";
		return DB_ERR_HEADER5;
	}

	return DB_ERR_NONE;
}

/**********************************************************************/

bool CHeaderArray::AddLabel ( const char* szLabel, int nCol )
{
	if ( strlen ( szLabel ) == 0 )		// discard blank fields
		return TRUE;

	int n;
	if ( m_Map.Lookup ( szLabel, n ) != 0 )	// error label already exists
		return FALSE;

	m_Map.SetAt ( szLabel, nCol );

	return TRUE;
}

/**********************************************************************/
