/**********************************************************************/

CAspLabel::CAspLabel()
{
	m_strText = "";
}

/**********************************************************************/

void CAspLabel::SetAttributes ( CCSV& csvAttributes )
{
	CAsp::SetAttributes ( csvAttributes );		// set common attributes

	CString strValue;
	if ( GetParamValue ( csvAttributes, ASP_TEXT, strValue ) == TRUE )			m_strText = strValue;
}

/**********************************************************************/

void CAspLabel::ParseUserParams ( CCSV& csvUserParams )
{
}

/**********************************************************************/

void CAspLabel::Write ( CString& strStyle, CString& strBody, CString strPadding )
{
	CHTML h ( strPadding, FALSE );
	h.Begin ( "span", m_strText );
	h.Attribute ( "id", m_strId );
	h.Attribute ( "style", GetStyleAttribute() );
	h.Attribute ( "class", m_strCssClass );
	h.End();

	strBody += h.GetHTML();
}

/**********************************************************************/
