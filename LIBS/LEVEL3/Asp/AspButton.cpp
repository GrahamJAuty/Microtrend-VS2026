/**********************************************************************/

CAspButton::CAspButton()
{
	m_strText = "";
	m_strName = "";
}

/**********************************************************************/

void CAspButton::SetAttributes ( CCSV& csvAttributes )
{
	CAsp::SetAttributes ( csvAttributes );		// set common attributes

	CString strValue;
	if ( GetParamValue ( csvAttributes, ASP_TEXT, strValue ) == TRUE )	m_strText = strValue;
	if ( GetParamValue ( csvAttributes, ASP_NAME, strValue ) == TRUE )	m_strName = strValue;
}

/**********************************************************************/

void CAspButton::ParseUserParams ( CCSV& csvUserParams )
{
}

/**********************************************************************/

void CAspButton::Write ( CString& strStyle, CString& strBody, CString strPadding )
{
	CString strName = ( m_strName != "" ) ? m_strName : m_strId;

	CHTML h ( strPadding, FALSE );
	h.Begin ( "input" );
	h.Attribute ( "id", m_strId );
	h.Attribute ( "style", GetStyleAttribute() );
	h.Attribute ( "class", m_strCssClass );
	h.Attribute ( "type", "submit" );
	h.Attribute ( "name", strName );
	h.Attribute ( "value", m_strText );
	h.End();

	strBody += h.GetHTML();
}

/**********************************************************************/
