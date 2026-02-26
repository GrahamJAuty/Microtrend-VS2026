/**********************************************************************/

CConsolidateArray::CConsolidateArray ( bool bAscending, bool bIgnoreCase )
{
	m_bAscending = bAscending;
	m_bIgnoreCase = bIgnoreCase;
}

/**********************************************************************/

CConsolidateArray::~CConsolidateArray()
{
	RemoveAll();
}

/**********************************************************************/

void CConsolidateArray::RemoveAll()
{
	for ( int i = 0 ; i < GetSize() ; i++ )
		delete GetAt(i);

	CObArray::RemoveAll();
}

/**********************************************************************/

bool CConsolidateArray::Add ( const char* szLabel )
{
	int nIndex;

	if ( Find ( szLabel, nIndex ) == TRUE )	/* label exists */
	{
		((CConsolidateObject*) (CObArray::GetAt(nIndex)))->Consolidate();

		return TRUE;
	}
	else			/* label not found */
	{
		CConsolidateObject* pObj = new CConsolidateObject ( szLabel );

		return Add ( nIndex, pObj );
	}
}

/**********************************************************************/

bool CConsolidateArray::Add ( int nIndex, CConsolidateObject* pObj )
{
	if ( pObj != NULL )
	{
		TRY
		{
			if ( nIndex >= GetSize() )
				CObArray::Add ( pObj );
			else
				InsertAt ( nIndex, pObj );

			return TRUE;
		}
		CATCH ( CMemoryException, e )
		{
			delete pObj;
		}
		END_CATCH
	}

	return FALSE;
}

/**********************************************************************/

bool CConsolidateArray::Find ( const char* szLabel, int& nIndex )
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
				nComp = _stricmp ( szLabel, ((CConsolidateObject*) (CObArray::GetAt(nIndex)))->GetLabel() );	/* compare strings */
			else
				nComp = _stricmp ( ((CConsolidateObject*) (CObArray::GetAt(nIndex)))->GetLabel(), szLabel );	/* compare strings */
		}
		else
		{
			if ( m_bAscending == TRUE )
				nComp = strcmp ( szLabel, ((CConsolidateObject*) (CObArray::GetAt(nIndex)))->GetLabel() );		/* compare strings */
			else
				nComp = strcmp ( ((CConsolidateObject*) (CObArray::GetAt(nIndex)))->GetLabel(), szLabel );		/* compare strings */
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
