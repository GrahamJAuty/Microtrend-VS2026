/**********************************************************************/

CSortStringArray::CSortStringArray ( int nOffset, bool bAscending, bool bIgnoreCase )
{
	m_nOffset = nOffset;
	m_bAscending = bAscending;
	m_bIgnoreCase = bIgnoreCase;
}

/**********************************************************************/

CString CSortStringArray::GetLabel ( const char* szRecord )
{
	CString strLabel ( szRecord );

	return strLabel.Mid ( m_nOffset );
}

/**********************************************************************/

int CSortStringArray::Add ( const char* szText )
{
	int nIndex = 0;
	CString strLabel = GetLabel ( szText );

	Find ( strLabel, nIndex );

	if ( nIndex >= GetSize() )
		nIndex = CStringArray::Add ( szText );
	else
		InsertAt ( nIndex, szText );

	return nIndex;
}

/**********************************************************************/

bool CSortStringArray::Find ( const char* szLabel, int& nIndex )
{
	int nLow = 0;
	int nHigh = GetSize() - 1;

	while ( nLow <= nHigh )
	{
		nIndex = ( nLow + nHigh ) / 2;

		int nComp;

		if ( m_bIgnoreCase == TRUE )
		{
			if ( m_bAscending == TRUE )
				nComp = _stricmp ( szLabel, GetLabel(GetAt(nIndex)) );	/* compare strings */
			else
				nComp = _stricmp ( GetLabel(GetAt(nIndex)), szLabel );	/* compare strings */
		}
		else
		{
			if ( m_bAscending == TRUE )
				nComp = strcmp ( szLabel, GetLabel(GetAt(nIndex)) );	/* compare strings */
			else
				nComp = strcmp ( GetLabel(GetAt(nIndex)), szLabel );	/* compare strings */
		}

		if ( nComp < 0 )
			nHigh = nIndex - 1;

		else if ( nComp > 0 )
			nLow = nIndex + 1;

		else						/* strings compare */
			return TRUE;
	}

	nIndex = nLow;
	return FALSE;
}

/**********************************************************************/
