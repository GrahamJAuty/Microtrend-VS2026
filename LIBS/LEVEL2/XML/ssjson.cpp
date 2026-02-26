/**********************************************************************/
/*	CJsonLabelValueObj										*/
/**********************************************************************/

CJsonLabelValueObj::CJsonLabelValueObj ( const char* szLabel, const char* szValue )
	: m_strLabel(szLabel), m_strValue(szValue)
{
}

/**********************************************************************/
/*	CJsonLabelValueArray									*/
/**********************************************************************/

CJsonLabelValueArray::~CJsonLabelValueArray()
{
	RemoveAll();
}

/**********************************************************************/

void CJsonLabelValueArray::Add ( const char* szLabel, const char* szValue )
{
	CJsonLabelValueObj* pObj = new CJsonLabelValueObj ( szLabel, szValue );
	CPtrArray::Add ( pObj );
}

/**********************************************************************/

CString CJsonLabelValueArray::GetLabel ( int n )
{
	CJsonLabelValueObj* pObj = (CJsonLabelValueObj*) GetAt(n);
	return pObj->m_strLabel;
}

CString CJsonLabelValueArray::GetValue ( int n )
{
	CJsonLabelValueObj* pObj = (CJsonLabelValueObj*) GetAt(n);
	return pObj->m_strValue;
}

/**********************************************************************/

void CJsonLabelValueArray::RemoveAll()
{
	for ( int i = 0 ; i < GetSize() ; i++ )
		delete (CJsonLabelValueObj*) CPtrArray::GetAt(i);

	CPtrArray::RemoveAll();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSSJson::CSSJson()
{
}

/**********************************************************************/

CString CSSJson::GetValue ( const char* szLabel )
{
	CString strValue;

	int nIndex;
	if ( Find ( szLabel, nIndex ) == TRUE )
		strValue = m_arJson.GetValue ( m_arIndex.GetAt(nIndex) );

	return strValue;
}

/**********************************************************************/

bool CSSJson::ParseFile ( const char* szFilename, bool bTestMode )
{
	m_arJson.RemoveAll();
	m_arStack.RemoveAll();
	m_arIndex.RemoveAll();

	if ( DiscardWhitespace ( szFilename, bTestMode ) == TRUE )
	{
		CreateSortIndex();		// sort the labels

		if ( bTestMode == TRUE )
		{
			CString strFilename;
			strFilename = szFilename;

			CSSFile fOut;

			if ( fOut.Open ( strFilename + ".2.txt", "wb" ) == TRUE )
			{
				for ( int i = 0 ; i < m_arJson.GetSize() ; i++ )
				{
					CString str = m_arJson.GetLabel(i);
					str += "=";
					str += m_arJson.GetValue(i);

					fOut.WriteLine ( str );
				}

				fOut.Close();
			}

			if ( fOut.Open ( strFilename + ".3.txt", "wb" ) == TRUE )
			{
				for ( int i = 0 ; i < m_arJson.GetSize() ; i++ )
				{
					CString str = m_arJson.GetLabel(m_arIndex.GetAt(i));
					str += "=";
					str += m_arJson.GetValue(m_arIndex.GetAt(i));

					fOut.WriteLine ( str );
				}

				fOut.Close();
			}
		}

		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/
/*	discard tab, cr, lf and non-quoted spaces					*/
/**********************************************************************/

bool CSSJson::DiscardWhitespace ( const char* szFilename, bool bTestMode )
{
	CSSFile fOut;
	if ( bTestMode == TRUE )
	{
		CString strTestFilename;
		strTestFilename = szFilename;
		if ( fOut.Open ( strTestFilename + ".1.txt", "wb" ) == FALSE )
			return FALSE;
	}

	bool bReply;

	CSSFile fIn;
	if (( bReply = fIn.Open ( szFilename, "rb" )) == TRUE )
	{
		CString strBuf;
		bool bQuoted = FALSE;
		char cPrevChar = NULL;
		int nCount = 0;

		int nChar;
		while (( nChar = fIn.GetChar() ) != EOF )
		{
			if ( ++nCount <= 3 )	// number of characters read
			{
				if ( nCount == 1 && nChar == 0xef )	// UTF-8 byte order mark
					continue;						// discard
				if ( nCount == 2 && nChar == 0xbb )	// UTF-8 byte order mark
					continue;						// discard
				if ( nCount == 3 && nChar == 0xbf )	// UTF-8 byte order mark
					continue;						// discard
			}

			char c = (char) nChar;

			if ( c == '"' )
			{
				if ( cPrevChar != NULL )		// not first character
				{
					if ( cPrevChar != '\\' )							// previous char not backslash - discard quote
						bQuoted = ( bQuoted == FALSE ) ? TRUE : FALSE;	// toggle quoted
					else
						strBuf += c;				// previous char backslash - keep quote
				}
				else											// first char
					bQuoted = ( bQuoted == FALSE ) ? TRUE : FALSE;	// toggle quoted
			}
			else if ( c == 0x09 || c == 0x0d || c == 0x0a )	// discard tab cr and lf. No need to check for quotes as 
			{										// these characters are not allowed in quoted strings
			}
			else if ( c == ' ' )
			{
				if ( bQuoted == TRUE )		// keep space if quoted
				{
					strBuf += c;			// copy to destination
				}
			}
			else if ( c == ',' || c == ':' || c == '}' || c == '[' || c == ']' )
			{
				if ( bQuoted == TRUE )		// keep if quoted
				{
					strBuf += c;			// copy to destination
				}
				else						// not quoted
				{
					strBuf += c;			// copy to destination
					ParseLine ( strBuf );

					if ( bTestMode == TRUE )
						fOut.WriteLine ( strBuf );

					strBuf = "";
				}
			}
			else		// keep all other characters
			{
				strBuf += c;				// copy to destination
			}

			cPrevChar = c;
		}

		fIn.Close();

		if ( bTestMode == TRUE )
			fOut.Close();
	}

	return bReply;
}

/**********************************************************************/

void CSSJson::ParseLine ( CString& strBuf )
{
	if ( strBuf.Left(1) == "{" )			// start of object
	{
		PushLabel ( strBuf.Mid ( 1, strBuf.GetLength()-2 ) );
	}
	else if ( strBuf.Right(1) == ":" )		// end of label
	{
		PushLabel ( strBuf.Mid ( 0, strBuf.GetLength()-1 ) );
	}
	else if ( strBuf == "[" )			// start of array
	{
		CString strLabel = "[0]";
		PushLabel ( strLabel );
	}
	else
	{
		int nLen = strBuf.GetLength();

		char c = strBuf.GetAt ( nLen-1 );			// get last char on line

		if ( c == ',' )						// next label:value or array element value
		{
			if ( nLen > 1 )
			{
				CString strLabel = GetFullLabel();			// label
				CString strValue = strBuf.Left ( nLen-1 );	// value (discard last char on line)
				m_arJson.Add ( strLabel, strValue );
			}

			if ( m_arStack.GetSize() > 0 )
			{
				CString strName = m_arStack.GetAt ( m_arStack.GetSize()-1 );
				if ( IsArray ( strName ) == TRUE )	// [nnn]
				{
					IncrementArray ( strName );	// increment array
					m_arStack.SetAt ( m_arStack.GetSize()-1, strName );
				}
				else							// not array
					PopLabel();
			}
		}
		else if ( c == '}' )					// end of object
		{
			if ( nLen > 1 )
			{
				CString strLabel = GetFullLabel();			// label
				CString strValue = strBuf.Left ( nLen-1 );	// value (discard last char on line)
				m_arJson.Add ( strLabel, strValue );
			}
			PopLabel();
		}
		else if ( c == ']' )					// end of array
		{
			if ( nLen > 1 )
			{
				CString strLabel = GetFullLabel();			// label
				CString strValue = strBuf.Left ( nLen-1 );	// value (discard last char on line)
				m_arJson.Add ( strLabel, strValue );
			}
			PopLabel();
		}
	}
}

/**********************************************************************/

CString CSSJson::GetFullLabel()
{
	CString strLabel;

	for ( int i = 0 ; i < m_arStack.GetSize() ; i++ )
	{
		CString str = m_arStack.GetAt(i);
		if ( i > 0 )
		{
			if ( IsArray ( str ) == FALSE )	// dont add . in front of array
				strLabel += ".";
		}
		strLabel += str;
	}

	return strLabel;
}

/**********************************************************************/

void CSSJson::PushLabel ( CString strLabel )
{
	m_arStack.Add ( strLabel );
}

CString CSSJson::PopLabel()
{
	CString strLabel;

	if ( m_arStack.GetSize() > 1 )
	{
		strLabel = m_arStack.GetAt ( m_arStack.GetSize()-1 );
		m_arStack.RemoveAt ( m_arStack.GetSize()-1 );
	}

	return strLabel;
}

/**********************************************************************/

bool CSSJson::IsArray ( CString strName )
{
	return ( strName.Left(1) == "[" && strName.Right(1) == "]" ) ? TRUE : FALSE;
}

void CSSJson::IncrementArray ( CString& strName )
{
	strName.TrimLeft ( "[" );
	strName.TrimRight ( "]" );
	strName.Format ( "[%d]", atoi(strName) + 1 );
}

/**********************************************************************/

static int CompareRecords ( void* context, const void* arg1, const void* arg2 )
{
	CSSJson* p = (CSSJson*) context;

	CString strLabel1 = p->GetLabel( *(int*)arg1 );
	CString strLabel2 = p->GetLabel( *(int*)arg2 );

	return strcmp ( strLabel1, strLabel2 );
}

/**********************************************************************/

void CSSJson::CreateSortIndex()
{
	m_arIndex.RemoveAll();

	if ( m_arJson.GetSize() > 0 )
	{
		for ( int i = 0 ; i < m_arJson.GetSize() ; i++ )
			m_arIndex.Add (i);

		if ( m_arJson.GetSize() > 1 )
			qsort_s ( m_arIndex.GetData(), m_arIndex.GetSize(), sizeof(UINT), CompareRecords, this );
	}
}

/**********************************************************************/

bool CSSJson::Find ( const char* szLabel, int& nIndex )
{
	int nLow = 0;
	int nHigh = m_arJson.GetSize() - 1;

	while ( nLow <= nHigh )
	{
		nIndex = ( nLow + nHigh ) / 2;

		int nComp;

		nComp = strcmp ( szLabel, m_arJson.GetLabel(m_arIndex.GetAt(nIndex)) );		/* compare strings */

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
