/**********************************************************************/

CAspCheckBox::CAspCheckBox()
{
	m_strText = "";
	m_strChecked = "";
}

/**********************************************************************/

void CAspCheckBox::SetAttributes ( CCSV& csvAttributes )
{
	CAsp::SetAttributes ( csvAttributes );		// set common attributes

	CString strValue;
	if ( GetParamValue ( csvAttributes, ASP_TEXT, strValue ) == TRUE )		m_strText = strValue;
	if ( GetParamValue ( csvAttributes, ASP_CHECKED, strValue ) == TRUE )		m_strChecked = strValue;
}

/**********************************************************************/

void CAspCheckBox::ParseUserParams ( CCSV& csvUserParams )
{
	CString strValue;
	if ( GetParamValue ( csvUserParams, m_strId, strValue ) == TRUE )
		m_strChecked = ( strValue != "" ) ? "checked" : "";
}

/**********************************************************************/

void CAspCheckBox::Write ( CString& strStyle, CString& strBody, CString strPadding )
{
	CHTML h ( strPadding, FALSE );
	h.Begin ( "input" );
	h.Attribute ( "id", m_strId );
	h.Attribute ( "style", GetStyleAttribute() );
	h.Attribute ( "class", m_strCssClass );
	h.Attribute ( "type", "checkbox" );
	h.Attribute ( "name", m_strId );
	h.Attribute ( "checked", m_strChecked );
	h.End();

	strBody += h.GetHTML();
}

/**********************************************************************/
