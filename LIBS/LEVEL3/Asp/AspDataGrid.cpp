/**********************************************************************/

CAspDataGrid::CAspDataGrid()
{
	m_bHide = FALSE;
	m_strCSVTextAlign = "";
	m_strCSVSortType = "";
	m_strCurrentSortField = "";
	m_strCurrentSortDirection = "a";	// ascending;

	m_strSQL = "";
	m_bSort = FALSE;
	m_strBackColorAlt = "";

	m_dbView.CreateView ( &m_db );
}

/**********************************************************************/

void CAspDataGrid::SetAttributes ( CCSV& csvAttributes )
{
	CAsp::SetAttributes ( csvAttributes );		// set common attributes

	CString strValue;
	if ( GetParamValue ( csvAttributes, ASP_SQL, strValue ) == TRUE )			m_strSQL = strValue;
	if ( GetParamValue ( csvAttributes, ASP_SORT, strValue ) == TRUE )			m_bSort = ( strValue == "true" ) ? TRUE : FALSE;
	if ( GetParamValue ( csvAttributes, ASP_BACK_COLOR_ALT, strValue ) == TRUE )	m_strBackColorAlt = strValue;
	if ( GetParamValue ( csvAttributes, ASP_TEXT_ALIGN, strValue ) == TRUE )		m_strCSVTextAlign = strValue;
	if ( GetParamValue ( csvAttributes, ASP_SORT_TYPE, strValue ) == TRUE )		m_strCSVSortType = strValue;

	if ( m_strSQL != "" )	// database is only opened if there is an sql string
	{
		CSQL sql;
		if ( sql.SetSQL ( m_strSQL ) == TRUE )
		{
			CString strFilename = sql.GetFrom();
			if ( m_db.OpenReadOnly ( strFilename, "", 0 ) == DB_ERR_NONE )
			{
				m_dbView.CreateView ( &m_db );
				m_dbView.Filter ( m_strSQL );
			}
		}
	}
}

/**********************************************************************/

void CAspDataGrid::ParseUserParams ( CCSV& csvUserParams )		// eg. "t1.Sort=Customer"
{
	if ( m_bSort == TRUE )
	{
		CString strLabel;
		strLabel.Format ( "%s.Sort", (const char*) m_strId );		 // "t1.sort"

		CString strSortField;
		if ( GetParamValue ( csvUserParams, strLabel, strSortField ) == TRUE )		// button eg. "t1.Sort"
		{
		}

		strLabel.Format ( "%s.CurrentSort", (const char*) m_strId );		 // eg. "t1.CurrentSort"

		CString strCSVCurrentSort;
		if ( GetParamValue ( csvUserParams, strLabel, strCSVCurrentSort ) == TRUE )	 // textbox eg. "t1.CurrentSort"
		{
			CString strCurrentSortField;
			CString strCurrentSortDirection;

			CCSV csvCurrentSort ( strCSVCurrentSort );		// eg. "Customer,a"
			if ( csvCurrentSort.GetSize() == 2 )
			{
				strCurrentSortField = csvCurrentSort.GetString(0);
				strCurrentSortDirection = csvCurrentSort.GetString(1);
			}

			CString strSortDirection = "a";	// ascending

			if ( strSortField == "" )		// column sort button not pressed (sort by previous sort column)
			{
				if ( strCurrentSortField != "" )	// got previous sort field
				{
					m_strCurrentSortField = strCurrentSortField;
					m_strCurrentSortDirection = strCurrentSortDirection;
				}
				else		// no previous sort
				{
				}
			}
			else							// column sort button pressed
			{
				if ( strCurrentSortField != "" )	// got previous sort field
				{
					if ( strSortField == strCurrentSortField )
						strSortDirection = ( strCurrentSortDirection == "a" ) ? "d" : "a";	// change direction
					else
						strSortDirection = "a";		// ascending
				}
				else		// no previous sort
				{
					strSortDirection = "a";		// ascending
				}

				m_strCurrentSortField = strSortField;
				m_strCurrentSortDirection = strSortDirection;
			}
		}

		if ( m_strSQL != "" && strSortField != "" )		// if there is no sql then code behind does sorting
		{
			CString strSortDirection = "a";			// default to ascending

			if ( m_strCurrentSortField != "" )
			{
				strSortField = m_strCurrentSortField;
				strSortDirection = m_strCurrentSortDirection;
			}

			CString strSortType = "text";					// default to text

			CCSV csvSortType ( m_strCSVSortType );			// eg. "int,text,text,text"

			int nCols = m_db.GetHeaderSize();
			for ( int i = 0 ; i < nCols ; i++ )
			{
				if ( m_db.GetHeaderLabel(i) == strSortField )
				{
					strSortType = csvSortType.GetString(i);
					break;
				}
			}

			CString strOrderByDirection = ( strSortDirection == "d" ) ? "DESC" : "ASC";

			CSQL sql;
			sql.SetSQL ( m_strSQL );
			sql.ClearOrderBy();
			sql.AddOrderBy ( strSortField, strSortType );
			sql.SetOrderByDirection ( strOrderByDirection );

			m_dbView.Filter ( sql.GetSQL() );
		}
	}
}

/**********************************************************************/

void CAspDataGrid::SetTextAlign ( const char* szCSVTextAlign )
{
	m_strCSVTextAlign = szCSVTextAlign;
}

/**********************************************************************/

CString CAspDataGrid::GetTextAlign ( CCSV& csv, int n )
{
	CString strTextAlign = csv.GetString(n);

	if ( strTextAlign == "" )
		strTextAlign = "left";

	return strTextAlign;
}

/**********************************************************************/

void CAspDataGrid::Write ( CString& strStyle, CString& strBody, CString strPadding )
{
	if ( m_bHide == FALSE )
	{
		CStringArray arStyle;
		CString str;

		CSS table;
		table.Label ( "table." + m_strId );
		table.Add ( "width", m_strWidth );
		table.Add ( "color", m_strTextColor );
		table.Add ( "background-color", m_strBackColor );
		table.Add ( "border-spacing", m_strCellSpacing );
		table.Add ( "border-width", m_strBorderWidth );
		table.Add ( "border-style", m_strBorderStyle );
		table.Add ( "border-color", m_strBorderColor );
		arStyle.Add ( table.GetCss() );

		CSS tr;
		tr.Label ( "tr." + m_strId );
		tr.Add ( "background-color", m_strBackColorAlt );
		arStyle.Add ( tr.GetCss() );

		CSS td;
		td.Label ( "td." + m_strId );
		td.Add ( "padding", m_strCellPadding );
		td.Add ( "border-width", m_strBorderWidth );
		td.Add ( "border-style", m_strBorderStyle );
		td.Add ( "border-color", m_strBorderColor );
		arStyle.Add ( td.GetCss() );

		int nColCount = m_dbView.GetColumnCount();
		CCSV csvTextAlign ( m_strCSVTextAlign );

		for ( int nCol = 0 ; nCol < nColCount ; nCol++ )
		{
			CSS th;
			str.Format ( "%s-c%d", (const char*) m_strId, nCol+1 );
			th.Label (  "th." + str );
			th.Add ( "text-align", GetTextAlign(csvTextAlign,nCol) );
			th.Add ( "font-size", "1.1em" );
			th.Add ( "padding", m_strCellPadding );
			th.Add ( "border-width", m_strBorderWidth );
			th.Add ( "border-style", m_strBorderStyle );
			th.Add ( "border-color", m_strBorderColor );
			arStyle.Add ( th.GetCss() );
		}

		for ( int nCol = 0 ; nCol < nColCount ; nCol++ )
		{
			CSS td;
			str.Format ( "%s-c%d", (const char*) m_strId, nCol+1 );
			td.Label ( "td." + str );
			td.Add ( "text-align", GetTextAlign(csvTextAlign,nCol) );
			td.Add ( "padding", m_strCellPadding );
			td.Add ( "border-width", m_strBorderWidth );
			td.Add ( "border-style", m_strBorderStyle );
			td.Add ( "border-color", m_strBorderColor );
			arStyle.Add ( td.GetCss() );
		}

		if ( m_bSort == TRUE )
		{
			for ( int nCol = 0 ; nCol < nColCount ; nCol++ )
			{
				CSS input;
				str.Format ( "%s-c%d", (const char*) m_strId, nCol+1 );
				input.Label ( "input." + str );
				input.Add ( "text-align", GetTextAlign(csvTextAlign,nCol) );
				input.Add ( "font-size","1.1em" );
				input.Add ( "width", "100%" );
				input.Add ( "cursor", "pointer" );
				input.Add ( "border", "none" );
				input.Add ( "color", m_strTextColor );
				input.Add ( "background-color", m_strBackColorAlt );
				arStyle.Add ( input.GetCss() );
			}

			for ( int nCol = 0 ; nCol < nColCount ; nCol++ )
			{
				CSS inputHover;
				str.Format ( "%s-c%d:hover", (const char*) m_strId, nCol+1 );
				inputHover.Label ( "input." + str );
				inputHover.Add ( "border", "none" );
				str.Format ( "0px 0px 1px %s", (const char*) m_strTextColor );
				inputHover.Add ( "box-shadow", str );
				arStyle.Add ( inputHover.GetCss() );
			}
		}

		int nRowCount = m_dbView.GetRecordCount();
		if ( nRowCount > 0 )
		{
			CHTML h ( strPadding );
			h.Begin ( "table" );
			h.Attribute ( "class", m_strId );
			{
				h.Begin ( "tr" );
				h.Attribute ( "class", m_strId );
				{
					for ( int nCol = 0 ; nCol < nColCount ; nCol++ )
					{
						CString strHeader;
						if ( m_bSort == TRUE )
						{
							CHTML h ( "", FALSE );
							h.Begin ( "input" );
							str.Format ( "%s-c%d", (const char*) m_strId, nCol+1 );
							h.Attribute ( "class", str );
							h.Attribute ( "type", "submit" );
							str.Format ( "%s.Sort", (const char*) m_strId );
							h.Attribute ( "name", str );
							h.Attribute ( "value", m_dbView.GetHeaderLabel(nCol) );	// column name
							h.End();

							strHeader = h.GetHTML();
						}
						else
							strHeader = m_dbView.GetHeaderLabel(nCol);

						h.Begin ( "th", strHeader );
						str.Format ( "%s-c%d", (const char*) m_strId, nCol+1 );
						h.Attribute ( "class", str );
						h.End();
					}
				}
				h.End();	// tr
			}

			for ( int nRow = 0 ; nRow < nRowCount ; nRow++ )
			{
				h.Begin ( "tr" );
				if (( nRow % 2 ) != 0 )	// even
					h.Attribute ( "class", m_strId );

				for ( int nCol = 0 ; nCol < nColCount ; nCol++ )
				{
					h.Begin ( "td", m_dbView.GetField ( nRow, nCol ) );
					str.Format ( "%s-c%d", (const char*) m_strId, nCol+1 );
					h.Attribute ( "class", str );
					h.End();
				}

				h.End();	// tr
			}

			h.End();	// table

			strBody += h.GetHTML();
		}

		for ( int i = 0 ; i < arStyle.GetSize() ; i++ )
			strStyle += arStyle.GetAt(i) + "\r\n";
	}

	if ( m_bSort == TRUE )		// generate html whether data grid hidden or not
	{
		CHTML h ( strPadding );
		h.Begin ( "input" );					// hidden textbox containing current sort field and sort direction
		h.Attribute ( "style", "display:none;" );	// make invisible
		h.Attribute ( "type", "text" );
		h.Attribute ( "name", m_strId + ".CurrentSort" );
		CString str;
		str.Format ( "%s,%s", (const char*) m_strCurrentSortField, (const char*) m_strCurrentSortDirection );
		h.Attribute ( "value", str );	// column name, sort direction
		h.End();

		strBody += h.GetHTML();
	}

	strBody.TrimRight ( "\r\n" );		// asp object should not force new line
}

/**********************************************************************/
