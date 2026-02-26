/**********************************************************************/

CIndexSort::CIndexSort()
{
	m_pTextArray = NULL;
	m_lpGetLabel = NULL;
	m_nOffset = 0;
	m_bIgnoreCase = FALSE;
	m_bAscending = TRUE;
}

/**********************************************************************/

CString CIndexSort::GetLabel ( const char* szRecord )
{
	if ( m_lpGetLabel != NULL )
		return m_lpGetLabel ( szRecord );
	else
	{
		CString strLabel ( szRecord );

		return strLabel.Mid ( m_nOffset );
	}
}

/**********************************************************************/

bool CIndexSort::Sort ( CStringArray* pTextArray, CString (CALLBACK EXPORT* lpGetLabel)(const char*), int nOffset, bool bAscending, bool bIgnoreCase )
{
	m_pTextArray = pTextArray;
	m_lpGetLabel = lpGetLabel;
	m_nOffset = nOffset;
	m_bAscending = bAscending;
	m_bIgnoreCase = bIgnoreCase;

	m_IndexArray.RemoveAll();

	for ( int nTextIndex = 0 ; nTextIndex < m_pTextArray->GetSize() ; nTextIndex++ )
	{
		if ( nTextIndex % ( m_pTextArray->GetSize() / 100 ) == 0 )	// 1% steps
			if ( Progress ( nTextIndex ) == TRUE )
				return FALSE;			// cancelled by user

		if ( Add ( nTextIndex ) == -1 )
			return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

CString CIndexSort::GetAt ( int nIndex )
{
	return m_pTextArray->GetAt ( m_IndexArray.GetAt(nIndex) );
}

/**********************************************************************/

int CIndexSort::Add ( int nTextIndex )
{
	int nIndex;
	CString strLabel = GetLabel ( m_pTextArray->GetAt(nTextIndex) );

	Find ( strLabel, nIndex );

	TRY
	{
		if ( nIndex >= m_IndexArray.GetSize() )
			nIndex = m_IndexArray.Add ( nTextIndex );
		else
			m_IndexArray.InsertAt ( nIndex, nTextIndex );

		return nIndex;
	}
	CATCH ( CMemoryException, e )
	{
		return -1;
	}
	END_CATCH
}

/**********************************************************************/

bool CIndexSort::Find ( const char* szLabel, int& nIndex )
{
	int nLow = 0;
	int nHigh = m_IndexArray.GetSize() - 1;

	while ( nLow <= nHigh )
	{
		nIndex = ( nLow + nHigh ) / 2;

		int nComp;

		if ( m_bIgnoreCase == TRUE )
		{
			if ( m_bAscending == TRUE )
				nComp = _stricmp ( szLabel, GetLabel(m_pTextArray->GetAt(m_IndexArray[nIndex])) );	/* compare strings */
			else
				nComp = _stricmp ( GetLabel(m_pTextArray->GetAt(m_IndexArray[nIndex])), szLabel );	/* compare strings */
		}
		else
		{
			if ( m_bAscending == TRUE )
				nComp = strcmp ( szLabel, GetLabel(m_pTextArray->GetAt(m_IndexArray[nIndex])) );	/* compare strings */
			else
				nComp = strcmp ( GetLabel(m_pTextArray->GetAt(m_IndexArray[nIndex])), szLabel );	/* compare strings */
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
