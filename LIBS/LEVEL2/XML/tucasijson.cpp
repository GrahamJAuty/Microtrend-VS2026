/**********************************************************************/

CJsonObject::CJsonObject ( const char* szObject )
{
	Parse ( szObject );
}

/**********************************************************************/
/* find all label:value pairs in the object, delimited by commas		*/
/**********************************************************************/

void CJsonObject::Parse ( const char* szObject )
{
	CString strObject = szObject;
	strObject.TrimLeft ( '{' );
	strObject.TrimRight ( '}' );

	int nCurPos = 0;
	CString strLabelValuePair;

	while (( strLabelValuePair = GetNextLabelValuePair ( strObject, nCurPos ) ) != "" )
		AddLabelValue ( strLabelValuePair );
}

/**********************************************************************/
/*	get next label:value pair in the object						*/
/*	delimited by non-quoted commas or end of line				*/
/**********************************************************************/

CString CJsonObject::GetNextLabelValuePair ( CString& strObject, int& nCurPos )
{
	CString strLabelValuePair;

	bool bQuoted = FALSE;

	for ( int i = nCurPos ; i < strObject.GetLength() ; i++ )
	{
		char c = strObject.GetAt(i);

		if ( c == '"' )
		{
			if ( i > 0 )		// not first char
			{
				if ( strObject.GetAt(i-1) != '\\' )				// previous char not backslash
					bQuoted = ( bQuoted == FALSE ) ? TRUE : FALSE;	// toggle quoted
			}
			else				// first char
				bQuoted = ( bQuoted == FALSE ) ? TRUE : FALSE;	// toggle quoted
		}
		else if ( c == ',' )		// looking for non-quoted comma
		{
			if ( bQuoted == FALSE )	// found it
			{
				strLabelValuePair = strObject.Mid ( nCurPos, i-nCurPos );
				nCurPos = i+1;
				break;
			}
		}

		if ( i == strObject.GetLength()-1 )		// end of line
		{
			strLabelValuePair = strObject.Mid ( nCurPos, i-nCurPos+1 );
			nCurPos = i+1;
			break;
		}
	}

	return strLabelValuePair;
}

/**********************************************************************/

void CJsonObject::AddLabelValue ( CString& strLabelValuePair )
{
	bool bQuoted = FALSE;

	int nLen = strLabelValuePair.GetLength();
	for ( int i = 0 ; i < nLen ; i++ )
	{
		char c = strLabelValuePair.GetAt(i);

		if ( c == '"' )
		{
			if ( i > 0 )		// not first char
			{
				if ( strLabelValuePair.GetAt(i-1) != '\\' )			// previous char not backslash
					bQuoted = ( bQuoted == FALSE ) ? TRUE : FALSE;	// toggle quoted
			}
			else				// first char
				bQuoted = ( bQuoted == FALSE ) ? TRUE : FALSE;	// toggle quoted
		}
		else if ( c == ':' )		// looking for non-quoted colon
		{
			if ( bQuoted == FALSE )	// found it
			{
				m_arLabel.Add ( TrimNonEscapeQuotes ( strLabelValuePair.Left(i) ) );
				m_arValue.Add ( TrimNonEscapeQuotes ( strLabelValuePair.Right(nLen-i-1) ) );
				break;
			}
		}
	}
}

/**********************************************************************/
/*	discard all quotes that are not escaped with backslash			*/
/**********************************************************************/

CString CJsonObject::TrimNonEscapeQuotes ( CString strLabelValue )
{
	CString strReply;

	bool bQuoted = FALSE;

	for ( int i = 0 ; i < strLabelValue.GetLength() ; i++ )
	{
		char c = strLabelValue.GetAt(i);

		if ( c == '"' )
		{
			if ( i > 0 )		// not first char
			{
				if ( strLabelValue.GetAt(i-1) != '\\' )				// previous char not backslash
					bQuoted = ( bQuoted == FALSE ) ? TRUE : FALSE;	// toggle quoted
				else
					strReply += c;			// keep escaped quote
			}
			else				// first char
				bQuoted = ( bQuoted == FALSE ) ? TRUE : FALSE;	// toggle quoted

		}
		else
			strReply += c;
	}

	return strReply;
}

/**********************************************************************/

CString CJsonObject::GetValue ( const char* szLabel )
{
	CString strValue;

	for ( int i = 0 ; i < m_arLabel.GetSize() ; i++ )
	{
		if ( m_arLabel.GetAt(i) == szLabel )
		{
			strValue = m_arValue.GetAt(i);
			break;
		}
	}
	return strValue;
}

/**********************************************************************/

CString CJsonObject::GetValue ( int nIndex )
{
	CString strValue;

	if ( nIndex >= 0 && nIndex < m_arValue.GetSize() )
		strValue = m_arValue.GetAt ( nIndex );

	return strValue;
}

/**********************************************************************/

CString CJsonObject::GetLabel ( int nIndex )
{
	CString strLabel;

	if ( nIndex >= 0 && nIndex < m_arLabel.GetSize() )
		strLabel = m_arLabel.GetAt ( nIndex );

	return strLabel;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CTucasiJson::~CTucasiJson()
{
	RemoveAll();
}

/**********************************************************************/

void CTucasiJson::RemoveAll()
{
	for ( int i = 0 ; i < m_arObject.GetSize() ; i++ )
		delete (CJsonObject*) m_arObject.GetAt(i);

	m_arObject.RemoveAll();
}

/**********************************************************************/
/*	read all lines and remove white spaces						*/
/*	discard tab cr lf and non-quoted spaces						*/
/**********************************************************************/

bool CTucasiJson::ParseFile ( const char* szFilename )
{
	CString strJson;

	bool bReply;

	CSSFile file;
	if (( bReply = file.Open ( szFilename, "rb" )) == TRUE )
	{
		CString strBuf;
		while ( file.ReadLine ( strBuf ) == TRUE )		// read all lines into strJson buffer
			strJson += strBuf;

		file.Close();
	}

	RemoveAll();		// remove any previous objects

	if ( strJson != "" )
		Parse ( strJson );

	return bReply;
}

/**********************************************************************/
/*	make an array of all json objects							*/
/**********************************************************************/

void CTucasiJson::Parse ( CString& strJson )
{
	DiscardWhitespace ( strJson );	// discard tab, cr, lf and non-quoted spaces

	int nCurPos = 0;
	CString strObject;

	while (( strObject = GetNextObject ( strJson, nCurPos ) ) != "" )
	{
		CJsonObject* pObj = new CJsonObject ( strObject );
		if ( pObj != NULL )
			m_arObject.Add ( pObj );
	}
}

/**********************************************************************/
/*	discard tab, cr, lf and non-quoted spaces					*/
/**********************************************************************/

void CTucasiJson::DiscardWhitespace ( CString& strBuf )
{
	int nSourceLen = strBuf.GetLength();
	int nDestPtr = 0;

	bool bQuoted = FALSE;

	for ( int nSourcePtr = 0 ; nSourcePtr < nSourceLen ; nSourcePtr++ )
	{
		char c = strBuf.GetAt(nSourcePtr);

		if ( c == '"' )
		{
			if ( nSourcePtr > 0 )		// not first character
			{
				if ( strBuf.GetAt ( nSourcePtr-1 ) != '\\' )			// previous char not backslash
					bQuoted = ( bQuoted == FALSE ) ? TRUE : FALSE;	// toggle quoted

				strBuf.SetAt ( nDestPtr++, c );		// copy to destination
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
				strBuf.SetAt ( nDestPtr++, c );	// copy to destination
			}
		}
		else		// keep all other characters
		{
			strBuf.SetAt ( nDestPtr++, c );		// copy to destination
		}
	}

	if ( nDestPtr < nSourceLen )				// some whitespace has been removed
		strBuf.ReleaseBuffer ( nDestPtr );		// shorten strBuf
}

/**********************************************************************/
/*	find next json object surrounded with non-quoted braces		*/
/**********************************************************************/

CString CTucasiJson::GetNextObject ( CString& strJson, int& nCurPos )
{
	bool bQuoted = FALSE;
	bool bFindOpenBrace = TRUE;
	int nFirstPos = 0;
	CString strObject;

	for ( int i = nCurPos ; i < strJson.GetLength() ; i++ )
	{
		char c = strJson.GetAt(i);

		if ( c == '"' )
		{
			if ( i > 0 )		// not first char
			{
				if ( strJson.GetAt(i-1) != '\\' )					// previous char not backslash
					bQuoted = ( bQuoted == FALSE ) ? TRUE : FALSE;	// toggle quotes
			}
			else				// first char
				bQuoted = ( bQuoted == FALSE ) ? TRUE : FALSE;	// toggle quotes
		}
		else
		{
			if ( bFindOpenBrace == TRUE )				// looking for open brace
			{
				if ( c == '{' && bQuoted == FALSE )	// found non-quoted open brace
				{
					nFirstPos = i;
					bFindOpenBrace = FALSE;			// now looking for close brace
				}
			}
			else									// looking for close brace
			{
				if ( c == '}' && bQuoted == FALSE )	// found non-quoted close brace
				{
					strObject = strJson.Mid ( nFirstPos, i-nFirstPos+1 );		// found end of object
					nCurPos = i+1;
					break;
				}
			}
		}
	}
	return strObject;
}

/**********************************************************************/

const char* CTucasiJson::GetValue ( const char* szLabel, int nIdxRecord )
{
	CString strValue;

	if ( nIdxRecord >= 0 && nIdxRecord < m_arObject.GetSize() )
	{
		CJsonObject* pObj = (CJsonObject*) m_arObject.GetAt ( nIdxRecord );
		strValue = pObj->GetValue ( szLabel );
	}

	return strValue;
}

/**********************************************************************/

int CTucasiJson::GetLabelCount()
{
	int nLabelCount = 0;

	if ( m_arObject.GetSize() > 0 )
	{
		CJsonObject* pObj = (CJsonObject*) m_arObject.GetAt ( 0 );		// get 1st object
		nLabelCount = pObj->GetLabelCount();
	}

	return nLabelCount;
}

/**********************************************************************/

const char* CTucasiJson::GetLabel ( int nIdxField )
{
	CString strLabel;

	if ( m_arObject.GetSize() > 0 )
	{
		CJsonObject* pObj = (CJsonObject*) m_arObject.GetAt ( 0 );		// get 1st object
		if ( nIdxField >= 0 && nIdxField < pObj->GetLabelCount() )
			strLabel = pObj->GetLabel ( nIdxField );
	}

	return strLabel;
}

/**********************************************************************/

bool CTucasiJson::FindLabel ( const char* szLabel )
{
	bool bReply = FALSE;

	int n = GetLabelCount();

	for ( int i = 0 ; i < n ; i++ )
	{
		CString strLabel = GetLabel(i);
		if ( strLabel == szLabel )
		{
			bReply = TRUE;
			break;
		}
	}

	return bReply;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

int CTucasiJson::GetLabelCount ( int nIdxRecord )
{
	int nLabelCount = 0;

	if ( nIdxRecord < m_arObject.GetSize() )
	{
		CJsonObject* p = (CJsonObject*) m_arObject.GetAt(nIdxRecord);
		nLabelCount = p->GetLabelCount();
	}

	return nLabelCount;
}

/**********************************************************************/

CString CTucasiJson::GetLabel ( int nIdxRecord, int nIdxField )
{
	CString strLabel;

	if ( nIdxRecord < m_arObject.GetSize() )
	{
		CJsonObject* p = (CJsonObject*) m_arObject.GetAt(nIdxRecord);
		if ( nIdxField < p->GetLabelCount() )
		{
			strLabel = p->GetLabel(nIdxField);
		}
	}

	return strLabel;
}

/**********************************************************************/

CString CTucasiJson::GetValue ( int nIdxRecord, int nIdxField )
{
	CString strValue;

	if ( nIdxRecord < m_arObject.GetSize() )
	{
		CJsonObject* p = (CJsonObject*) m_arObject.GetAt(nIdxRecord);
		if ( nIdxField < p->GetValueCount() )
		{
			strValue = p->GetValue(nIdxField);
		}
	}

	return strValue;
}

/**********************************************************************/
