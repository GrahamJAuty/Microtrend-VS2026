/**********************************************************************/

bool CQVConsolidateArray::Add ( const char* szLabel, double dQty, double dValue )
{
	int nIndex = 0;

	if ( Find ( szLabel, nIndex ) == TRUE )	// label exists
	{
		((CQVConsolidateObject*) (CObArray::GetAt(nIndex)))->Consolidate ( dQty, dValue );

		return TRUE;
	}
	else								// label not found
	{
		CQVConsolidateObject* pObj = new CQVConsolidateObject ( szLabel, dQty, dValue );

		return CConsolidateArray::Add ( nIndex, pObj );
	}
}

/**********************************************************************/

bool CQVConsolidateArray::AddIfExists ( const char* szLabel, double dQty, double dValue )
{
	bool bResult = FALSE;

	if ( dQty != 0 || dValue != 0 )
	{
		int nIndex = 0;

		if ( Find ( szLabel, nIndex ) == TRUE )	// label exists
		{
			((CQVConsolidateObject*) (CObArray::GetAt(nIndex)))->Consolidate ( dQty, dValue );
			bResult = TRUE;
		}
	}
	return bResult;
}

/**********************************************************************/
