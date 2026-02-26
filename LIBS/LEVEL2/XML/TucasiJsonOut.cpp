/**********************************************************************/

CTucasiJsonOut::~CTucasiJsonOut()
{
	RemoveAll();
}

/**********************************************************************/

void CTucasiJsonOut::RemoveAll()
{
	for ( int i = 0 ; i < m_arObject.GetSize() ; i++ )
		delete (CStringArray*) m_arObject.GetAt(i);

	m_arObject.RemoveAll();
}

/**********************************************************************/

void CTucasiJsonOut::AddObject()
{
	CStringArray* pObj = new CStringArray;

	if ( pObj != NULL )
		m_arObject.Add ( pObj );
}

/**********************************************************************/

void CTucasiJsonOut::Add ( const char* szLabel, const char* szValue, bool bQuotes )
{
	CString str;

	if ( bQuotes == TRUE )
		str.Format ( "\"%s\":\"%s\"", szLabel, szValue );
	else
		str.Format ( "\"%s\":%s", szLabel, szValue );

	if ( str.GetLength() > 0 )
	{
		if ( m_arObject.GetSize() == 0 )		// AddObject not called by user
			AddObject();					// add one automatically

		int i = m_arObject.GetSize()-1;
		CStringArray* pObj = (CStringArray*) m_arObject.GetAt(i);
			pObj->Add ( str );
	}
}

/**********************************************************************/

static CStringW ConvertMBCStoUnicode ( const char* szText )
{
	CStringW strUnicode;

	int nLen = MultiByteToWideChar ( CP_ACP, 0, szText, -1, NULL, 0 );			// calculate output buffer size required
	if ( nLen > 0 )													// ok
	{
		wchar_t* szUnicode = new wchar_t[nLen+1];							// allow space for 0 at end of string
		int n = MultiByteToWideChar ( CP_ACP, 0, szText, -1, szUnicode, nLen );	// convert to unicode
		if ( n > 0 )													// ok
		{
			szUnicode[n] = 0;											// end of string
			strUnicode = szUnicode;
		}
		delete[] szUnicode;
	}
	return strUnicode;
}

static CString ConvertUnicodeToUtf8 ( CStringW strUnicode )
{
	CString strUtf8;

	int nLen = WideCharToMultiByte ( CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);		// calculate output buffer size required
	if ( nLen > 0 )															// ok
	{
		char* szUtf8 = new char[nLen+1];											// allow space for 0 at end of string
		int n = WideCharToMultiByte ( CP_UTF8, 0, strUnicode, -1, szUtf8, nLen, NULL, NULL);	// convert to UTF-8
		if ( n > 0 )															// ok
		{
			szUtf8[n] = 0;														// end of string
			strUtf8 = szUtf8;
		}
		delete[] szUtf8;
	}
	return strUtf8;
}

static CString ConvertMBCStoUTF8 ( const char* szText )
{
	CStringW strUnicode = ConvertMBCStoUnicode ( szText );		// convert the system default Windows ANSI code page to unicode
	CString strUTF8 = ConvertUnicodeToUtf8 ( strUnicode );		// convert unicode to UTF-8

	return strUTF8;
}

/**********************************************************************/

void CTucasiJsonOut::GetObject ( CString& strBuf, int nIndex )
{
	CStringArray* pObj = (CStringArray*) m_arObject.GetAt(nIndex);

	int nSize = pObj->GetSize();

	if ( nSize > 0 )
	{
		strBuf += "{";		// start of object

		for ( int i = 0 ; i < nSize ; i++ )
		{
			strBuf += ConvertMBCStoUTF8 ( pObj->GetAt(i) );		// convert multibyte character set to UTF-8

			if ( i < (nSize-1) )
				strBuf += ",";			// comma between label value pairs
		}

		strBuf += "}";		// end of object
	}
}

/**********************************************************************/

void CTucasiJsonOut::GetJson ( CString& strBuf )
{
	int nSize = m_arObject.GetSize();

	if ( nSize > 0 )		// array of objects
	{
		strBuf += "[";		// start of array

		for ( int i = 0 ; i < nSize ; i++ )
		{
			GetObject ( strBuf, i );

			if ( i < (nSize-1) )
				strBuf += ",\r\n";		// comma between objects (crlf to make more readable)
		}

		strBuf += "]";		// end of array
	}
}

/**********************************************************************/
