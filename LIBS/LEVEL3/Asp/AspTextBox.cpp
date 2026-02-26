/**********************************************************************/

CAspTextBox::CAspTextBox()
{
	m_strText = "";
	m_strColumns = "";
	m_strMaxLength = "";
}

/**********************************************************************/

void CAspTextBox::SetAttributes ( CCSV& csvAttributes )
{
	CAsp::SetAttributes ( csvAttributes );		// set common attributes

	CString strValue;
	if ( GetParamValue ( csvAttributes, ASP_TEXT, strValue ) == TRUE )			m_strText = strValue;
	if ( GetParamValue ( csvAttributes, ASP_COLUMNS, strValue ) == TRUE )			m_strColumns = strValue;
	if ( GetParamValue ( csvAttributes, ASP_MAX_LENGTH, strValue ) == TRUE )		m_strMaxLength = strValue;
}

/**********************************************************************/

void CAspTextBox::ParseUserParams ( CCSV& csvUserParams )
{
	CString strValue;
	if ( GetParamValue ( csvUserParams, m_strId, strValue ) == TRUE )
	{
		m_strText = strValue;
	}
}

/**********************************************************************/

void CAspTextBox::Write ( CString& strStyle, CString& strBody, CString strPadding )
{
	CHTML h ( strPadding, FALSE );
	h.Begin ( "input" );
	h.Attribute ( "id", m_strId );
	h.Attribute ( "style", GetStyleAttribute() );
	h.Attribute ( "class", m_strCssClass );
	h.Attribute ( "type", "text" );
	h.Attribute ( "size", m_strColumns );
	h.Attribute ( "maxlength", m_strMaxLength );
	h.Attribute ( "name", m_strId );
	h.Attribute ( "value", m_strText );
	h.End();

	strBody += h.GetHTML();
}

/**********************************************************************/
