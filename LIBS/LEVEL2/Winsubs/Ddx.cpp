/**********************************************************************/

static int Compare ( const char* szNum1, const char* szNum2 )
{
	int nLen1 = strlen ( szNum1 );
	int nLen2 = strlen ( szNum2 );

	if ( nLen1 != nLen2 )
		return ( nLen1 - nLen2 );
	else
		return ( strcmp ( szNum1, szNum2 ) );
}

/**********************************************************************/

static bool ValidateIntegerStringInRange ( const char* szNumber, const char* szFirst, const char* szLast )
{
	CString strNumber = szNumber;			/* the number to be checked for within range */
	strNumber.TrimLeft ( '0' );
	CString strFirst = szFirst;			/* range low */
	strFirst.TrimLeft ( '0' );
	CString strLast = szLast;			/* range high */
	strLast.TrimLeft ( '0' );

	for ( int n = 0; n < strNumber.GetLength(); n++ )
	{
		char c = strNumber.GetAt(n);
		if ( ( c < '0' ) || ( c > '9' ) )
			return FALSE;
	}

	if ( Compare ( strNumber, strFirst ) >= 0 && Compare ( strNumber, strLast ) <= 0 )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

void AFXAPI DDV_IntegerString(CDataExchange* pDX, CString const& strValue, CString const& strLow, CString const& strHigh )
{
	if ( pDX -> m_bSaveAndValidate )
	{
		if ( ::ValidateIntegerStringInRange ( strValue, strLow, strHigh ) == FALSE )
		{
			CString strMessage;
			
			strMessage.Format ( "Please enter an integer\nbetween %s and %s",
					(const char*) strLow, 
					(const char*) strHigh );
			
			AfxMessageBox ( strMessage, MB_OK | MB_ICONEXCLAMATION );
			
			//the Fail() function throws a user exception so
			//we empty the string now to prevent memory leaks
			
			strMessage.Empty();
			pDX -> Fail();
		}
	}
}

/**********************************************************************/
