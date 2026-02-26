/**********************************************************************/

CHTML::CHTML ( const char* szPadding, bool bCRLF, int nTabSize )
	: m_strPadding(szPadding), m_bCRLF(bCRLF), m_nTabSize(nTabSize)
{
	m_strCurrentTag = "";
	m_strCurrentElement = "";
	m_strCurrentValue = "";
	m_nCurrentTabLevel = 0;
}

/**********************************************************************/

void CHTML::Begin ( const char* szElement, const char* szValue )
{
	m_arStack.Add ( szElement );

	if ( m_strCurrentElement != "" )
	{
		m_arHtml.Add ( m_strCurrentElement + '>' + m_strCurrentValue );
		m_strCurrentElement = "";
	}

	m_strCurrentTag = szElement;
	m_strCurrentValue = szValue;
	m_strCurrentElement.Format ( "%s<%s", (const char*) GetPadding(), szElement );
	m_nCurrentTabLevel++;
}

/**********************************************************************/

void CHTML::Attribute ( const char* szAttribute, const char* szValue )
{
	if ( strlen(szValue) > 0 )
	{
		CString str;
		str.Format ( " %s=\"%s\"", szAttribute, szValue );
		m_strCurrentElement += str;
	}
}

/**********************************************************************/

void CHTML::End()
{
	int n = m_arStack.GetSize();
	if ( n > 0 )		// incase begin not called
	{
		bool bEndRequired = TRUE;

		CString strTag = m_arStack.GetAt(n-1);

		if ( m_strCurrentElement != "" )
		{
			CString str;
			if ( m_strCurrentTag == "input" )	// no end tag required
			{
				str.Format ( "%s>%s", (const char*) m_strCurrentElement, (const char*) m_strCurrentValue );
				bEndRequired = FALSE;
			}
			else if ( m_strCurrentTag != strTag )
			{
				str.Format ( "%s>%s", (const char*) m_strCurrentElement, (const char*) m_strCurrentValue );
			}
			else
			{
				str.Format ( "%s>%s</%s>", (const char*) m_strCurrentElement, (const char*) m_strCurrentValue, (const char*) strTag );
				bEndRequired = FALSE;
			}

			m_arHtml.Add ( str );
			m_strCurrentElement = "";
		}

		m_nCurrentTabLevel--;

		if ( bEndRequired == TRUE )
		{
			CString str;
			str.Format ( "%s</%s>", (const char*) GetPadding(), (const char*) strTag );
			m_arHtml.Add ( str );
		}

		m_arStack.RemoveAt(n-1);
	}
}

/**********************************************************************/

CString CHTML::GetHTML()
{
	CString strHtml;

	if ( m_bCRLF == TRUE )
	{
		for ( int i = 0 ; i < m_arHtml.GetSize() ; i++ )
			strHtml += m_strPadding + m_arHtml.GetAt(i) + "\r\n";
	}
	else
	{
		strHtml += m_strPadding;		// initial padding

		for ( int i = 0 ; i < m_arHtml.GetSize() ; i++ )
			strHtml += m_arHtml.GetAt(i);
	}

	return strHtml;
}

/**********************************************************************/

CString CHTML::GetPadding()
{
	CString str;

	if ( m_bCRLF == TRUE )
	{
		for ( int i = 0 ; i < m_nCurrentTabLevel * m_nTabSize ; i++ )
			str += ' ';
	}

	return str;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CSS::Label ( const char* szLabel )
{
	m_strLabel = szLabel;
}

void CSS::Add ( const char* szProperty, const char* szValue )
{
	if ( strlen(szProperty) > 0 )
	{
		CString str;
		str.Format ( " %s:%s;", szProperty, szValue );
		m_strProperties += str;
	}
}

CString CSS::GetCss()
{
	CString strCss;

	if ( m_strProperties != "" )
		strCss.Format ( "%s {%s }", (const char*) m_strLabel, (const char*) m_strProperties );

	return strCss;
}

/**********************************************************************/
