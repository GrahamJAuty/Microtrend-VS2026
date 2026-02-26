/**********************************************************************/

#include <math.h>

/**********************************************************************/

bool GetParamValue ( CCSV& csvParams, const char* szLabel, CString& strValue )
{
	for ( int i = 0 ; i < csvParams.GetSize() ; i++ )
	{
		CCSV csvItem ( csvParams.GetString(i), '=' );
		if ( csvItem.GetSize() == 2 )
		{
			CString strLabel = csvItem.GetString(0);
			if ( strLabel == szLabel )
			{
				strValue = csvItem.GetString(1);
				return TRUE;
			}
		}
	}
	return FALSE;
}

/**********************************************************************/

bool GetDouble ( CCSV& csvParams, const char* szLabel, double& dValue )
{
	CString strValue;
	if ( GetParamValue ( csvParams, szLabel, strValue ) == TRUE )
	{
		strValue.TrimLeft(' ');
		strValue.TrimRight(' ');

		if ( strValue != "" )
		{
			if ( strValue.SpanExcluding ( "0123456789.-" ).GetLength() == 0 )	// just got valid characters
			{
				dValue = atof(strValue);
				return TRUE;
			}
		}
	}
	return FALSE;
}

/**********************************************************************/

double GetDouble ( CCSV& csvParams, const char* szLabel, double dDefault, double dMin, double dMax )
{
	double dValue = dDefault;

	double d;
	if ( GetDouble ( csvParams, szLabel, d ) == TRUE )
	{
		if ( d >= dMin && d <= dMax )
			dValue = d;
	}

	return dValue;
}

/**********************************************************************/

bool GetInt ( CCSV& csvParams, const char* szLabel, int& nValue )
{
	CString strValue;
	if ( GetParamValue ( csvParams, szLabel, strValue ) == TRUE )
	{
		strValue.TrimLeft(' ');
		strValue.TrimRight(' ');

		if ( strValue != "" )
		{
			if ( strValue.SpanExcluding ( "0123456789-" ).GetLength() == 0 )		// just got valid characters
			{
				nValue = atoi(strValue);
				return TRUE;
			}
		}
	}
	return FALSE;
}

/**********************************************************************/

int GetInt ( CCSV& csvParams, const char* szLabel, int nDefault, int nMin, int nMax )
{
	int nValue = nDefault;

	int n;
	if ( GetInt ( csvParams, szLabel, n ) == TRUE )
	{
		if ( n >= nMin && n <= nMax )
			nValue = n;
	}

	return nValue;
}

/**********************************************************************/

static bool FindChar ( char cFind, const char* szBuf )
{
	char c;
	while (( c = *szBuf++ ) != 0 )
	{
		if ( c == cFind )
			return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

void GetTokens ( CCSV& csvTokens, const char* szBuf, const char* szDelimiters, const char* szQuotes )
{
	CString strBuf ( szBuf );

	bool bQuotes = FALSE;
	bool bFindToken = TRUE;

	int n = 0;
	int nStartToken = 0;
	while ( n <= strBuf.GetLength() )
	{
		char c = strBuf.GetAt(n);
		if ( FindChar ( c, szQuotes ) == TRUE )
			bQuotes = ( bQuotes == FALSE ) ? TRUE : FALSE;	// toggle quotes

		if ( bQuotes == FALSE )		// not in quoted field
		{
			if ( bFindToken == TRUE )		// looking for start of token
			{
				if ( FindChar ( c, szDelimiters ) == FALSE )		// normal char (not delimiter) - found start of token
				{
					nStartToken = n;
					bFindToken = FALSE;
				}
			}
			else		// looking for delimiter
			{
				if (( FindChar ( c, szDelimiters ) == TRUE ) || n >= strBuf.GetLength() )	// delimiter - got end of token
				{
					csvTokens.Add ( strBuf.Mid ( nStartToken, n-nStartToken ) );
					bFindToken = TRUE;
				}
			}
		}
		n++;
	}
}

/**********************************************************************/
