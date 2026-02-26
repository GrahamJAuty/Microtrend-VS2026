/**********************************************************************/
/*	Convert XML predefined entities							*/
/**********************************************************************/

static CString ConvertSpecialCharacters ( const char* szText )
{
	CString strText;

	unsigned char c;
	while (( c = *szText++ ) != 0 )
	{
		if ( c == '"' )			// quotation mark
			strText += "&quot;";
		else if ( c == '\'' )		// apostrophe
			strText += "&apos;";
		else if ( c == '&' )		// ampersand
			strText += "&amp;";
		else if ( c == '<' )		// less-than
			strText += "&lt;";
		else if ( c == '>' )		// greater-than
			strText += "&gt;";
		else if ( c >= 0x20 && c <= 0x7f )
			strText += c;
		else
		{
			if ( c == 0xa3 )			// '£'
				strText += "&#163;";	// unicode U+00A3
			else if ( c == 0x80 )		// '€'
				strText += "&#8364;";	// unicode U+20AC
			else
			{
				CString str;
				str.Format ( "&#%d;", c );
				strText += str;
			}
		}
	}

	return strText;
}

/**********************************************************************/

CSSXML::CSSXML()
{
	m_arrayXML.Add ( "<?xml version=\"1.0\" encoding=\"utf-8\"?>" );

	m_arrayStack.RemoveAll();
	m_strCurrentElement = "";
	m_strCurrentValue = "";
	m_strCurrentAttribute = "";
}

CSSXML::CSSXML ( const char* szDeclaration )
{
	m_arrayXML.Add ( szDeclaration );

	m_arrayStack.RemoveAll();
	m_strCurrentElement = "";
	m_strCurrentValue = "";
	m_strCurrentAttribute = "";
}

/**********************************************************************/

void CSSXML::Begin ( const char* szElement )
{
	Begin ( szElement, "" );
}

/**********************************************************************/

void CSSXML::Begin ( const char* szElement, const char* szValue )
{
	if ( m_strCurrentAttribute != "" )		// another begin level
	{
		CString str;
		str.Format ( "<%s%s>", (const char*)m_strCurrentElement, (const char*)m_strCurrentAttribute );
		m_arrayXML.SetAt ( m_arrayXML.GetSize()-1, str );
		m_strCurrentAttribute = "";
	}

	CString str;
	str.Format ( "<%s>", szElement );
	m_arrayXML.Add ( str );

	m_arrayStack.Add ( szElement );
	m_strCurrentElement = szElement;
	m_strCurrentValue = ConvertSpecialCharacters(szValue);
}

/**********************************************************************/

void CSSXML::Element ( const char* szElement, const char* szValue, const char* szType )
{
	Begin ( szElement, szValue );

	if ( strlen(szType) > 0 )
	{
		CString strType = "xsd:";
		strType += szType;			// type = string, int, decimal etc
		Attribute ( "xsi:type", strType );
	}

	End();
}

/**********************************************************************/

void CSSXML::Attribute ( const char* szAttribute, const char* szValue )
{
	CString str;
	str.Format ( " %s=\"%s\"", szAttribute, (const char*) ConvertSpecialCharacters(szValue) );
	m_strCurrentAttribute += str;
}

/**********************************************************************/

void CSSXML::End()
{
	if ( m_arrayStack.GetSize() > 0 )
	{
		int n = m_arrayStack.GetSize()-1;

		CString strElement = m_arrayStack.GetAt(n);
		if ( strElement == m_strCurrentElement )
		{
			CString str;
			if ( m_strCurrentValue == "" )
			{
				str.Format ( "<%s%s />", (const char*) strElement, (const char*) m_strCurrentAttribute );
			}
			else
			{
				str.Format ( "<%s%s>%s</%s>", (const char*) strElement, (const char*) m_strCurrentAttribute, (const char*) m_strCurrentValue, (const char*) strElement );
			}

			m_arrayXML.SetAt ( m_arrayXML.GetSize()-1, str );
		}
		else
		{
			CString str;
			str.Format ( "</%s>", (const char*) strElement );
			m_arrayXML.Add ( str );
		}

		m_arrayStack.RemoveAt(n);
	}

	m_strCurrentElement = "";
	m_strCurrentValue = "";
	m_strCurrentAttribute = "";
}

/**********************************************************************/

void CSSXML::EndAll()
{
	while ( m_arrayStack.GetSize() > 0 )
		End();
}

/**********************************************************************/

CString CSSXML::GetXML()
{
	EndAll();

	CString str;

	for ( int i = 0 ; i < m_arrayXML.GetSize() ; i++ )
		str += m_arrayXML.GetAt(i);

	return str;
}

/**********************************************************************/

bool CSSXML::WriteXML ( const char* szFilename, bool bCRLF )
{
	EndAll();

	bool bReply;

	CSSFile file;
	if (( bReply = file.Open ( szFilename, "wb" )) == TRUE )
	{
		if ( bCRLF == FALSE )
		{
			for ( int i = 0 ; i < m_arrayXML.GetSize() ; i++ )
				file.WriteString ( m_arrayXML.GetAt(i) );
		}
		else
		{
			for ( int i = 0 ; i < m_arrayXML.GetSize() ; i++ )
				file.WriteLine ( m_arrayXML.GetAt(i) );
		}

		file.Close();
	}

	return bReply;
}

/**********************************************************************/

CString CSSXML::GetXMLLine(int n)
{
	CString strResult = "";

	if ((n >= 0) && (n < m_arrayXML.GetSize()))
	{
		strResult = m_arrayXML.GetAt(n);
	}

	return strResult;
}

/**********************************************************************/
