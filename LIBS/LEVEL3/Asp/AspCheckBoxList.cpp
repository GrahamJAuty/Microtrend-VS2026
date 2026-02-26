/**********************************************************************/

CAspCheckBoxList::CAspCheckBoxList()
{
	m_nRepeatColumns = 1;
	m_strRepeatLayout = "";
}

/**********************************************************************/

void CAspCheckBoxList::SetAttributes ( CCSV& csvAttributes )
{
	CAsp::SetAttributes ( csvAttributes );		// set common attributes

	CString strValue;
	if ( GetParamValue ( csvAttributes, ASP_REPEAT_COLUMNS, strValue ) == TRUE )		m_nRepeatColumns = atoi(strValue);
	if ( GetParamValue ( csvAttributes, ASP_REPEAT_LAYOUT, strValue ) == TRUE )		m_strRepeatLayout = strValue;
}

/**********************************************************************/

void CAspCheckBoxList::ParseUserParams ( CCSV& csvUserParams )
{
	for ( int i = 0 ; i < csvUserParams.GetSize() ; i++ )
	{
		CCSV csvItem ( csvUserParams.GetString(i), '=' );
		if ( csvItem.GetSize() == 2 )
		{
			CString strLabel = csvItem.GetString(0);
			if ( strLabel == m_strId )
			{
				int nIndex = csvItem.GetInt(1);	// list index
				SetSelected ( nIndex, TRUE );
			}
		}
	}
}

/**********************************************************************/

CString CAspCheckBoxList::GetItemHtml ( int x, int y )
{
	CString strHtml;

	int n = y * m_nRepeatColumns + x;

	if ( n < m_arList.GetSize() )
	{
		CCSV csv ( m_arList.GetAt(n) );
		CString strChecked = ( csv.GetBool(1) == TRUE ) ? "checked" : "";
		CString strValue;
		strValue.Format ( "%d", n );

		CHTML h ( "", FALSE );
		h.Begin ( "input", csv.GetString(0) );
		h.Attribute ( "id", m_strId );
		h.Attribute ( "type", "checkbox" );
		h.Attribute ( "name", m_strId );
		h.Attribute ( "value", strValue );
		h.Attribute ( "checked", strChecked );
		h.End();		// input

		strHtml = h.GetHTML();
	}

	return strHtml;
}

/**********************************************************************/

void CAspCheckBoxList::Write ( CString& strStyle, CString& strBody, CString strPadding )
{
	CSS c;
	c.Label ( "table." + m_strId );
	c.Add ( "width", m_strWidth );
	c.Add ( "color", m_strTextColor );
	c.Add ( "background-color", m_strBackColor );
	c.Add ( "border-spacing", m_strCellSpacing );
	c.Add ( "border-width", m_strBorderWidth );
	c.Add ( "border-style", m_strBorderStyle );
	c.Add ( "border-color", m_strBorderColor );
	strStyle += c.GetCss() + "\r\n";

	CSS td;
	td.Label ( "td." + m_strId );
	td.Add ( "padding", m_strCellPadding );
	td.Add ( "border-width", m_strBorderWidth );
	td.Add ( "border-style", m_strBorderStyle );
	td.Add ( "border-color", m_strBorderColor );
	strStyle += td.GetCss() + "\r\n";

	CHTML h ( strPadding );
	h.Begin ( "table" );
	h.Attribute ( "class", m_strId );

	int nRows = m_arList.GetSize() / m_nRepeatColumns;
	if ( m_arList.GetSize() % m_nRepeatColumns != 0 )
		nRows++;

	for ( int y = 0 ; y < nRows ; y++ )
	{
		h.Begin ( "tr" );
	 	for ( int x = 0 ; x < m_nRepeatColumns ; x++ )
		{
			h.Begin ( "td", GetItemHtml(x,y) );
			h.Attribute ( "class", m_strId );
			h.End();
		}
		h.End();		// tr		
	}

	h.End();		// table

	strBody += h.GetHTML();
	strBody.TrimRight ( "\r\n" );		// asp object should not force new line
}

/**********************************************************************/

void CAspCheckBoxList::AddListItem ( const char* szText, bool bSelected )
{
	CCSV csv;
	csv.Add ( szText );
	csv.Add ( bSelected );
	m_arList.Add ( csv.GetLine() );
}

/**********************************************************************/

void CAspCheckBoxList::SetSelected ( int nIndex, bool b )
{
	if ( nIndex < m_arList.GetSize() )
	{
		CCSV csv ( m_arList.GetAt(nIndex) );
		csv.SetAt ( 1, b );
		m_arList.SetAt ( nIndex, csv.GetLine() );
	}
}

/**********************************************************************/

CString CAspCheckBoxList::GetText ( int nIndex )
{
	CString strText;

	if ( nIndex < m_arList.GetSize() )
	{
		CCSV csv ( m_arList.GetAt(nIndex) );
		strText = csv.GetString ( 0 );
	}

	return strText;
}

/**********************************************************************/

bool CAspCheckBoxList::GetSelected ( int nIndex )
{
	bool bSelected = FALSE;

	if ( nIndex < m_arList.GetSize() )
	{
		CCSV csv ( m_arList.GetAt(nIndex) );
		bSelected = csv.GetBool ( 1 );
	}

	return bSelected;
}

/**********************************************************************/
