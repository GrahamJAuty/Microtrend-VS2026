/**********************************************************************/

CCSV::CCSV ( const char* szCSVLine, char cSeparator, char cQuote, int nMaxFields )
{
	m_cSeparator = cSeparator;
	m_cQuote = cQuote;

	m_bRemoveTrailing = FALSE;
	m_bRemoveZeros = FALSE;
	m_bMinimizeFloat = FALSE;

	ParseLine ( m_strArray, szCSVLine, nMaxFields );
}

CCSV::CCSV ( char cSeparator, char cQuote, bool bRemoveTrailing, bool bRemoveZeros, bool bMinimizeFloat )
{
	m_cSeparator = cSeparator;
	m_cQuote = cQuote;

	m_bRemoveTrailing = bRemoveTrailing;
	m_bRemoveZeros = bRemoveZeros;
	m_bMinimizeFloat = bMinimizeFloat;
}

CCSV::CCSV ( char cSeparator, char cQuote )
{
	m_cSeparator = cSeparator;
	m_cQuote = cQuote;

	m_bRemoveTrailing = FALSE;
	m_bRemoveZeros = FALSE;
	m_bMinimizeFloat = FALSE;
}

/**********************************************************************/

CString CCSV::GetLine ( bool bQuotes )
{
	CString strBuf;

	for ( int i = 0 ; i < m_strArray.GetSize() ; i++ )
	{
		if ( i > 0 )
			strBuf += m_cSeparator;

		CString strTmp = m_strArray.GetAt(i);
		char szTmp[3] = { m_cSeparator, m_cQuote, 0 };

		if ( strpbrk ( strTmp, szTmp ) == NULL && bQuotes == FALSE )	// field does not contain separator or quotes
		{
			strBuf += strTmp;
		}
		else
		{
			strBuf += m_cQuote;			// add leading quotes around field

			int nLen = strTmp.GetLength();

			for ( int n = 0 ; n < nLen ; n++ )		// double up quotes
			{
				char c = strTmp[n];
				strBuf += c;
				if ( c == m_cQuote )
					strBuf += c;
			}

			strBuf += m_cQuote;			// add trailing quotes around field
		}
	}

	if ( m_bRemoveTrailing == TRUE )
		strBuf.TrimRight ( m_cSeparator );

	return strBuf;
}

/**********************************************************************/

CString CCSV::GetLine ( CCSV& csvQuoteArray )
{
	CString strBuf;

	for ( int i = 0 ; i < m_strArray.GetSize() ; i++ )
	{
		if ( i > 0 )
			strBuf += m_cSeparator;

		CString strTmp = m_strArray.GetAt(i);
		char szTmp[3] = { m_cSeparator, m_cQuote, 0 };

		if ( strpbrk ( strTmp, szTmp ) == NULL && csvQuoteArray.GetBool(i) == FALSE )	// field does not contain separator or quotes
		{
			strBuf += strTmp;
		}
		else
		{
			strBuf += m_cQuote;			// add leading quotes around field

			int nLen = strTmp.GetLength();

			for ( int n = 0 ; n < nLen ; n++ )		// double up quotes
			{
				char c = strTmp[n];
				strBuf += c;
				if ( c == m_cQuote )
					strBuf += c;
			}

			strBuf += m_cQuote;			// add trailing quotes around field
		}
	}

	if ( m_bRemoveTrailing == TRUE )
		strBuf.TrimRight ( m_cSeparator );

	return strBuf;
}

/**********************************************************************/

void CCSV::Get ( int nIndex, CString& strText )
{
	strText = GetField ( nIndex );
}

void CCSV::Get ( int nIndex, int& nNum )
{
	nNum = atoi ( GetField ( nIndex ) );
}

void CCSV::Get ( int nIndex, __int64& nNum )
{
	nNum = _atoi64 ( GetField ( nIndex ) );
}

void CCSV::Get ( int nIndex, long& lNum )
{
	lNum = atol ( GetField ( nIndex ) );
}

void CCSV::Get ( int nIndex, double& dNum )
{
	dNum = atof ( GetField ( nIndex ) );
}

void CCSV::Get ( int nIndex, bool& b )
{
	b = ( atoi ( GetField ( nIndex ) ) == 0 ) ? FALSE : TRUE;
}

/**********************************************************************/

const char* CCSV::GetString ( int nIndex )
{
	return GetField ( nIndex );
}

int CCSV::GetInt ( int nIndex )
{
	return atoi ( GetField ( nIndex ) );
}

__int64 CCSV::GetInt64 ( int nIndex )
{
	return _atoi64 ( GetField ( nIndex ) );
}

long CCSV::GetLong ( int nIndex )
{
	return atol ( GetField ( nIndex ) );
}

double CCSV::GetDouble ( int nIndex )
{
	return atof ( GetField ( nIndex ) );
}

bool CCSV::GetBool ( int nIndex )
{
	return ( atoi ( GetField ( nIndex ) ) == 0 ) ? FALSE : TRUE;
}

/**********************************************************************/

int CCSV::Add ( const char* szText )
{
	return m_strArray.Add ( FormatString ( szText ) );
}

int CCSV::Add ( int nNum )
{
	return m_strArray.Add ( FormatString ( nNum ) );
}

int CCSV::Add ( __int64 nNum )
{
	return m_strArray.Add ( FormatString ( nNum ) );
}

int CCSV::Add ( long lNum )
{
	return m_strArray.Add ( FormatString ( lNum ) );
}

int CCSV::Add ( double dNum, int nDp )
{
	return m_strArray.Add ( FormatString ( dNum, nDp, m_bMinimizeFloat ) );
}

int CCSV::Add ( double dNum, int nDp, bool bMinimize )
{
	return m_strArray.Add ( FormatString ( dNum, nDp, bMinimize ) );
}

int CCSV::Add ( bool b )
{
	return m_strArray.Add ( FormatString ( b ) );
}

int CCSV::AddSuffix ( const char* szText, int nSuffix )
{
	CString strBuf;
	strBuf.Format ( "%s%d", szText, nSuffix );
	return Add ( strBuf );
}

/**********************************************************************/

void CCSV::SetAt ( int nIndex, const char* szText )
{
	m_strArray.SetAtGrow ( nIndex, FormatString ( szText ) );
}

void CCSV::SetAt ( int nIndex, int nNum )
{
	m_strArray.SetAtGrow ( nIndex, FormatString ( nNum ) );
}

void CCSV::SetAt ( int nIndex, __int64 nNum )
{
	m_strArray.SetAtGrow ( nIndex, FormatString ( nNum ) );
}

void CCSV::SetAt ( int nIndex, long lNum )
{
	m_strArray.SetAtGrow ( nIndex, FormatString ( lNum ) );
}

void CCSV::SetAt ( int nIndex, double dNum, int nDp )
{
	m_strArray.SetAtGrow ( nIndex, FormatString ( dNum, nDp, m_bMinimizeFloat ) );
}

void CCSV::SetAt ( int nIndex, double dNum, int nDp, bool bMinimize )
{
	m_strArray.SetAtGrow ( nIndex, FormatString ( dNum, nDp, bMinimize ) );
}

void CCSV::SetAt ( int nIndex, bool b )
{
	m_strArray.SetAtGrow ( nIndex, FormatString ( b ) );
}

/**********************************************************************/

void CCSV::InsertAt ( int nIndex, const char* szText )
{
	m_strArray.InsertAt ( nIndex, FormatString ( szText ) );
}

void CCSV::InsertAt ( int nIndex, int nNum )
{
	m_strArray.InsertAt ( nIndex, FormatString ( nNum ) );
}

void CCSV::InsertAt ( int nIndex, __int64 nNum )
{
	m_strArray.InsertAt ( nIndex, FormatString ( nNum ) );
}

void CCSV::InsertAt ( int nIndex, long lNum )
{
	m_strArray.InsertAt ( nIndex, FormatString ( lNum ) );
}

void CCSV::InsertAt ( int nIndex, double dNum, int nDp )
{
	m_strArray.InsertAt ( nIndex, FormatString ( dNum, nDp, m_bMinimizeFloat ) );
}

void CCSV::InsertAt ( int nIndex, double dNum, int nDp, bool bMinimize )
{
	m_strArray.InsertAt ( nIndex, FormatString ( dNum, nDp, bMinimize ) );
}

void CCSV::InsertAt ( int nIndex, bool b )
{
	m_strArray.InsertAt ( nIndex, FormatString ( b ) );
}

/**********************************************************************/

bool CCSV::IsEmpty ( int nIndex )
{
	if ( nIndex < m_strArray.GetSize() )
		return ( m_strArray.GetAt(nIndex).IsEmpty() != 0 );

	return TRUE;
}

/**********************************************************************/

void CCSV::ParseLine ( const char* szBuf, int nMaxFields )
{
	ParseLine ( m_strArray, szBuf, nMaxFields );
}

/**********************************************************************/
/*	private routines										*/
/**********************************************************************/

void CCSV::ParseLine ( CStringArray& strArray, const char* szBuf, int nMaxFields )
{
	int nCount = 0;		// number of fields

	int nLen = strlen ( szBuf );
	if ( nLen > 0 )
	{
		char* szTmp = new char[nLen+1];	// make buffer for largest possible field
		if ( szTmp != NULL )
		{
			int i = 0;				// index to original buffer
			int n = 0;				// index to temp buffer
			int nQuoteToggle;			// for quoted fields
			char c;

			while ( i < nLen )
			{
				if ( szBuf[i] == m_cQuote )		// quoted field
				{
					nQuoteToggle = 0;
					i++;					// move off quote

					while ( i < nLen )
					{
						c = szBuf[i++];

						if ( c == m_cQuote )
						{
							if ( nQuoteToggle == 1 )
								szTmp[n++] = c;

							nQuoteToggle ^= 1;
						}
						else
						{
							if ( c == m_cSeparator && nQuoteToggle == 1 )
							{
								szTmp[n] = 0;
								strArray.SetAtGrow ( nCount++, szTmp );

								if ( nCount == nMaxFields )
								{
									delete [] szTmp;
									strArray.SetSize(nCount);
									return;
								}

								n = 0;	// reset index for next field
								break;
							}
							szTmp[n++] = c;
						}
					}
				}
				else						// normal field
				{
					do
					{
						c = szBuf[i++];

						if ( c == m_cSeparator )
						{
							szTmp[n] = 0;
							strArray.SetAtGrow ( nCount++, szTmp );

							if ( nCount == nMaxFields )
							{
								delete [] szTmp;
								strArray.SetSize(nCount);
								return;
							}

							n = 0;	// reset index for next field
							break;
						}
						else
							szTmp[n++] = c;
					}
					while ( i < nLen );
				}
			}

			szTmp[n] = 0;
			strArray.SetAtGrow ( nCount++, szTmp );	// add last field

			delete [] szTmp;
		}
	}

	strArray.SetSize(nCount);
}

/**********************************************************************/

const char* CCSV::GetField(int nIndex)
{
	if (nIndex < m_strArray.GetSize())
	{
		return m_strArray.GetAt(nIndex).GetString();
	}
	else
	{
		return "";
	}
}

/**********************************************************************/

CString CCSV::FormatString ( const char* szText )
{
	CString strBuf ( szText );

	return strBuf;
}

/**********************************************************************/

CString CCSV::FormatString ( int nNum )
{
	CString strBuf;

	if ( m_bRemoveZeros == FALSE || nNum != 0 )
		strBuf.Format ( "%d", nNum );

	return strBuf;
}

/**********************************************************************/

CString CCSV::FormatString ( __int64 nNum )
{
	CString strBuf;

	if ( m_bRemoveZeros == FALSE || nNum != 0 )
		strBuf.Format ( "%I64d", nNum );

	return strBuf;
}

/**********************************************************************/

CString CCSV::FormatString ( long lNum )
{
	CString strBuf;

	if ( m_bRemoveZeros == FALSE || lNum != 0 )
		strBuf.Format ( "%ld", lNum );

	return strBuf;
}

/**********************************************************************/

CString CCSV::FormatString ( double dNum, int nDp, bool bMinimize )
{
	CString strBuf;

	if ( m_bRemoveZeros == FALSE || dNum != 0.0 )
	{
		strBuf.Format ( "%.*f", nDp, dNum );

		if ( nDp > 0 && bMinimize == TRUE )
		{
			strBuf.TrimRight ( '0' );
			strBuf.TrimRight ( '.' );
		}
	}

	return strBuf;
}

/**********************************************************************/

CString CCSV::FormatString ( bool b )
{
	CString strBuf;

	if ( m_bRemoveZeros == FALSE || b != 0 )
		strBuf = ( b == 0 ) ? "0" : "1";

	return strBuf;
}

/**********************************************************************/
