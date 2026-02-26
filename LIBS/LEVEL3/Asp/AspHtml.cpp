/**********************************************************************/

CAspHtml::CAspHtml()
{
	m_strHead = "";
	m_strStyle = "";
	m_strScript = "";
}

/**********************************************************************/

void CAspHtml::SetAttributes ( CCSV& csvAttributes )
{
	CAsp::SetAttributes ( csvAttributes );		// set common attributes
}

/**********************************************************************/

void CAspHtml::ParseUserParams ( CCSV& csvUserParams )
{
}

/**********************************************************************/

void CAspHtml::AddHead ( const char* szHead )
{
	m_strHead += szHead;
	m_strHead += "\r\n";
}

/**********************************************************************/

void CAspHtml::AddStyle ( const char* szStyle )
{
	m_strStyle += szStyle;
	m_strStyle += "\r\n";
}

/**********************************************************************/

void CAspHtml::AddScript ( const char* szScript )
{
	m_strScript += szScript;
	m_strScript += "\r\n";
}

/**********************************************************************/

void CAspHtml::AddBody ( const char* szBody )
{
	m_arBody.Add ( szBody );
}

/**********************************************************************/

void CAspHtml::Write ( CString& strHead, CString& strScript, CString& strStyle, CString& strBody, CString strPadding )
{
	strHead += m_strHead;
	strScript += m_strScript;
	strStyle += m_strStyle;

	for ( int i = 0 ; i < m_arBody.GetSize() ; i++ )
		strBody += strPadding + m_arBody.GetAt(i);

	strBody.TrimRight ( "\r\n" );		// asp object should not force new line
}

/**********************************************************************/
