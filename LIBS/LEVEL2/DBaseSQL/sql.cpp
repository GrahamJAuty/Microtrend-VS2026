/**********************************************************************/

CSQL::CSQL()
{
	ClearAll();
}

/**********************************************************************/

void	CSQL::ClearAll()
{
	m_strCommand = "SELECT";		// default to select
	m_strSelect = "";
	m_strFrom = "";
	m_strWhere = "";
	m_strOrderBy = "";
	m_strOrderByDirection = "";	// ascending
	m_strLimit = "";
}

/**********************************************************************/

void CSQL::SetCommand ( const char* szCommand )
{
	CString strCommand = szCommand;

	if (strCommand == "SELECT DISTINCT")
	{
		m_strCommand = strCommand;
	}
	else
	{
		m_strCommand = "SELECT";
	}
}

/**********************************************************************/

void	CSQL::AddSelect ( const char* szColumnName )
{
	if ( m_strSelect == "" )
		m_strSelect = szColumnName;
	else
	{
		m_strSelect += ",";
		m_strSelect += ( szColumnName );
	}
}

/**********************************************************************/

void	CSQL::AddAndWhere ( const char* szWhere )
{
	AddWhere ( "AND", szWhere );
}

/**********************************************************************/

void	CSQL::AddOrWhere ( const char* szWhere )
{
	AddWhere ( "OR", szWhere );
}

/**********************************************************************/

void CSQL::AddWhere ( const char* szAndOr, const char* szWhere )
{
	if ( m_strWhere == "" )
	{
		m_strWhere = szWhere;				// eg "CustNo>=5000"
	}
	else
	{
		CString str;
		str.Format ( " %s %s", szAndOr, szWhere );	// eg " AND/OR CustNo<6000"
		m_strWhere += str;
	}
}

/**********************************************************************/

void	CSQL::AddOrderBy ( const char* szColumnName, const char* szSortType )
{
	CString strOrderBy;

	if ( strlen( szSortType) == 0 )		// eg text texti int float date time
		strOrderBy = szColumnName;
	else
		strOrderBy.Format ( "%s:%s", szColumnName, szSortType );

	if ( m_strOrderBy == "" )
	{
		m_strOrderBy = strOrderBy;
	}
	else
	{
		m_strOrderBy += ",";
		m_strOrderBy += strOrderBy;
	}
}

/**********************************************************************/

void CSQL::SetOrderByDirection ( const char* szDirection )		// ASC or DESC
{
	CString str = szDirection;
	if ( str == "ASC" )
		m_strOrderByDirection = "";
	else if ( str == "DESC" )
		m_strOrderByDirection = "DESC";
	else
		m_strOrderByDirection = "";
}

/**********************************************************************/

void CSQL::SetLimit ( int nLimit )
{
	CString strLimit;
	if ( nLimit > 0 )
		strLimit.Format ( "%d", nLimit );

	m_strLimit = strLimit;
}

/**********************************************************************/

CString CSQL::GetSQL()
{
	CString strSQL = m_strCommand;
	strSQL += " ";

	if ( m_strSelect == "" )
		strSQL += "*";
	else
		strSQL += m_strSelect;

	strSQL += " FROM ";

	if ( m_strFrom == "" )
		strSQL += "this";
	else
		strSQL += m_strFrom;

	if ( m_strWhere != "" )
	{
		strSQL += " WHERE ";
		strSQL += m_strWhere;
	}

	if ( m_strOrderBy != "" )
	{
		strSQL += " ORDER BY ";
		strSQL += m_strOrderBy;
	}

	if ( m_strOrderByDirection != "" )
	{
		strSQL += " ";
		strSQL += m_strOrderByDirection;
	}

	if ( m_strLimit != "" )
	{
		strSQL += " LIMIT ";
		strSQL += m_strLimit;
	}

	strSQL += ';';		// end of sql

	return strSQL;
}

/**********************************************************************/

bool CSQL::SetSQL ( const char* szSQL )
{
	ClearAll();

	CString strSQL = szSQL;
	strSQL = strSQL.TrimRight ( ';' );		// discard ; from end of sql line

	int nPos;

	if (( nPos = strSQL.Find ( " LIMIT " )) >= 0 )		// SELECT * FROM custlist.csv WHERE CustNo>=5000 AND CustNo<6000 ORDER BY Dealer,CustNo:int DESC LIMIT 100
	{
		m_strLimit = strSQL.Mid ( nPos + 7 );			// 100
		strSQL = strSQL.Left ( nPos );				// SELECT * FROM custlist.csv WHERE CustNo>=5000 AND CustNo<6000 ORDER BY Dealer,CustNo:int DESC
	}

	if (( nPos = strSQL.Find ( " DESC" )) >= 0 )			// SELECT * FROM custlist.csv WHERE CustNo>=5000 AND CustNo<6000 ORDER BY Dealer,CustNo:int DESC
	{
		m_strOrderByDirection = "DESC";				// DESC
		strSQL = strSQL.Left ( nPos );				// SELECT * FROM custlist.csv WHERE CustNo>=5000 AND CustNo<6000 ORDER BY Dealer,CustNo:int
	}

	if (( nPos = strSQL.Find ( " ASC" )) >= 0 )			// SELECT * FROM custlist.csv WHERE CustNo>=5000 AND CustNo<6000 ORDER BY Dealer,CustNo:int ASC
	{
		m_strOrderByDirection = "ASC";				// ASC
		strSQL = strSQL.Left ( nPos );				// SELECT * FROM custlist.csv WHERE CustNo>=5000 AND CustNo<6000 ORDER BY Dealer,CustNo:int
	}

	if (( nPos = strSQL.Find ( " ORDER BY " )) >= 0 )		// SELECT * FROM custlist.csv WHERE CustNo>=5000 AND CustNo<6000 ORDER BY Dealer,CustNo:int
	{
		m_strOrderBy = strSQL.Mid ( nPos + 10 );		// Dealer,CustNo:int
		strSQL = strSQL.Left ( nPos );				// SELECT * FROM custlist.csv WHERE CustNo>=5000 AND CustNo<6000
	}

	if (( nPos = strSQL.Find ( " WHERE " )) >= 0 )		// SELECT * FROM custlist.csv WHERE CustNo>=5000 AND CustNo<6000
	{
		m_strWhere = strSQL.Mid ( nPos + 7 );			// CustNo>=5000 AND CustNo<6000
		strSQL = strSQL.Left ( nPos );				// SELECT * FROM custlist.csv
	}

	if (( nPos = strSQL.Find ( " FROM " )) >= 0 )		// SELECT * FROM custlist.csv
	{
		m_strFrom = strSQL.Mid ( nPos + 6 );			// custlist.csv
		strSQL = strSQL.Left ( nPos );				// SELECT *
	}

	if ( m_strFrom != "" )
	{
		if (( nPos = strSQL.Find ( "SELECT DISTINCT " )) >= 0 )	// SELECT DISTINCT *
		{
			m_strSelect = strSQL.Mid ( nPos + 16 );				// *
			if ( m_strSelect != "" )
				m_strCommand = "SELECT DISTINCT";
		}
		else if (( nPos = strSQL.Find ( "SELECT " )) >= 0 )		// SELECT *
		{
			m_strSelect = strSQL.Mid ( nPos + 7 );				// *
			if ( m_strSelect != "" )
				m_strCommand = "SELECT";
		}

		if ( m_strCommand != "" )
		{
			if ( GetSQL() == szSQL )
				return TRUE;
		}
	}

	return FALSE;
}

/**********************************************************************/
