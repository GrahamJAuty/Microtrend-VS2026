/**********************************************************************/

#include "Defines.h"

/**********************************************************************/

static bool FindChar ( char cFind, const char* szBuf )
{
	char c;
	while (( c = *szBuf++ ) != 0 )
	{
		if ( c == cFind )
			return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

static void GetTokens ( CCSV& csvTokens, const char* szBuf, const char* szDelimiters, const char* szQuotes )
{
	CString strBuf ( szBuf );

	bool bQuotes = FALSE;
	bool bFindToken = TRUE;

	int n = 0;
	int nStartToken = 0;
	while ( n <= strBuf.GetLength() )
	{
		char c = strBuf.GetAt(n);
		if ( FindChar ( c, szQuotes ) == TRUE )
			bQuotes = ( bQuotes == FALSE ) ? TRUE : FALSE;	// toggle quotes

		if ( bQuotes == FALSE )		// not in quoted field
		{
			if ( bFindToken == TRUE )		// looking for start of token
			{
				if ( FindChar ( c, szDelimiters ) == FALSE )		// normal char (not delimiter) - found start of token
				{
					nStartToken = n;
					bFindToken = FALSE;
				}
			}
			else		// looking for delimiter
			{
				if (( FindChar ( c, szDelimiters ) == TRUE ) || n >= strBuf.GetLength() )	// delimiter - got end of token
				{
					csvTokens.Add ( strBuf.Mid ( nStartToken, n-nStartToken ) );
					bFindToken = TRUE;
				}
			}
		}
		n++;
	}
}

/**********************************************************************/

#define WILDCARD '%'

#define OP_EQUAL		1	// =
#define OP_LEQ			2	// <=
#define OP_GEQ			3	// >=
#define OP_NEQ			4	// <> or !=
#define OP_LT			5	// <
#define OP_GT			6	// >
#define OP_LIKE		7	// ~
#define OP_NOT_LIKE		8	// !~

#define FIELD_TEXT		1	// text
#define FIELD_INT		2	// int64
#define FIELD_FLOAT		3	// double

/**********************************************************************/

static CString ParseExpression ( const char* szExpression )
{
	CString strExpression1 = szExpression;						// eg CustNo>=5000

	CString strLabel = strExpression1.SpanExcluding ( "=<>~!" );	// CustNo
	CString str = strExpression1.Mid(strLabel.GetLength());		// >=5000
	CString strValue;

	int nOpType = 0;

	if ( str.Left(1) == "=" )			// equal
	{
		nOpType = OP_EQUAL;
		strValue = str.Mid(1);
	}
	else if ( str.Left(2) == "<=" )		// less than or equal
	{
		nOpType = OP_LEQ;
		strValue = str.Mid(2);
	}
	else if ( str.Left(2) == ">=" )		// greater than or equal
	{
		nOpType = OP_GEQ;
		strValue = str.Mid(2);
	}
	else if ( str.Left(2) == "<>" ||  str.Left(2) == "!=" )	// not equal
	{
		nOpType = OP_NEQ;
		strValue = str.Mid(2);
	}
	else if ( str.Left(1) == "<" )		// less than
	{
		nOpType = OP_LT;
		strValue = str.Mid(1);
	}
	else if ( str.Left(1) == ">" )		// greater than
	{
		nOpType = OP_GT;
		strValue = str.Mid(1);
	}
	else if ( str.Left(1) == "~" )		// LIKE (my symbol for like - not real sql symbol)
	{
		nOpType = OP_LIKE;
		strValue = str.Mid(1);
	}
	else if ( str.Left(2) == "!~" )		// NOT LIKE (my symbol for like - not real sql symbol)
	{
		nOpType = OP_NOT_LIKE;
		strValue = str.Mid(2);
	}

	int nFieldType = 0;

	if ( strValue.Left(1) == "'" && strValue.Right(1) == "'" )	// must be text string
	{
		strValue = strValue.Mid(1,strValue.GetLength()-2);	// discard quotes
		nFieldType = FIELD_TEXT;
	}
	else if ( strValue.Find ( '.' ) >= 0 )		// must be float
	{
		nFieldType = FIELD_FLOAT;
	}
	else		// must be integer
	{
		nFieldType = FIELD_INT;
	}

	CCSV csv;
	csv.Add ( strLabel );
	csv.Add ( nFieldType );
	csv.Add ( nOpType );
	csv.Add ( strValue );

	CString strType = csv.GetLine();		// eg: Label,FieldType,OpType,Value
	return strType;
}

/**********************************************************************/

bool CSSDatabaseView::TestExpression ( const char* szCSV )
{
	bool bTest = FALSE;

	CCSV csv ( szCSV );			// Label,FieldType,OpType,Value
	
	CString strLabel = csv.GetString(0);		// CustNo
	int nFieldType = csv.GetInt(1);
	int nOpType = csv.GetInt(2);

	if ( nFieldType == FIELD_TEXT )
	{
		CString strData = m_pDB->GetString ( strLabel );
		CString strValue = csv.GetString(3);
		int nLen = strValue.GetLength();

		switch ( nOpType )
		{
		case OP_EQUAL:	if ( strData == strValue )	bTest = TRUE;	break;
		case OP_LEQ:	if ( strData <= strValue )	bTest = TRUE;	break;
		case OP_GEQ:	if ( strData >= strValue )	bTest = TRUE;	break;
		case OP_NEQ:	if ( strData != strValue )	bTest = TRUE;	break;
		case OP_LT:	if ( strData < strValue )	bTest = TRUE;	break;
		case OP_GT:	if ( strData > strValue )	bTest = TRUE;	break;

		case OP_LIKE:
		case OP_NOT_LIKE:
			strData.MakeLower();	// ignore case
			strValue.MakeLower();	// ignore case

			if ( nLen > 1 && strValue.GetAt(0) == WILDCARD && strValue.GetAt(nLen-1) == WILDCARD )
			{
				if ( strData.Find ( strValue.Mid(1,nLen-2) ) != -1 )
					bTest = TRUE;
			}
			else if ( strValue.GetAt(0) == WILDCARD )
			{
				if ( strData.Right(nLen-1) == strValue.Right(nLen-1) )
					bTest = TRUE;
			}
			else if ( nLen > 0 && strValue.GetAt(nLen-1) == WILDCARD )
			{
				if ( strData.Left(nLen-1) == strValue.Left(nLen-1) )
					bTest = TRUE;
			}
			else
			{
				if ( strData == strValue )
					bTest = TRUE;
			}

			if ( nOpType == OP_NOT_LIKE )
				bTest = !bTest;

			break;
		}
	}
	else if ( nFieldType == FIELD_INT )
	{
		__int64 nData = m_pDB->GetInt64 ( strLabel );
		__int64 nValue = csv.GetInt64(3);

		switch ( nOpType )
		{
		case OP_EQUAL:	if ( nData == nValue )	bTest = TRUE;	break;
		case OP_LEQ:	if ( nData <= nValue )	bTest = TRUE;	break;
		case OP_GEQ:	if ( nData >= nValue )	bTest = TRUE;	break;
		case OP_NEQ:	if ( nData != nValue )	bTest = TRUE;	break;
		case OP_LT:	if ( nData < nValue )	bTest = TRUE;	break;
		case OP_GT:	if ( nData > nValue )	bTest = TRUE;	break;
		}
	}
	else if ( nFieldType == FIELD_FLOAT )
	{
		double dData = m_pDB->GetDouble ( strLabel );
		double dValue = csv.GetDouble(3);

		switch ( nOpType )
		{
		case OP_EQUAL:	if ( dData == dValue )	bTest = TRUE;	break;
		case OP_LEQ:	if ( dData <= dValue )	bTest = TRUE;	break;
		case OP_GEQ:	if ( dData >= dValue )	bTest = TRUE;	break;
		case OP_NEQ:	if ( dData != dValue )	bTest = TRUE;	break;
		case OP_LT:	if ( dData < dValue )	bTest = TRUE;	break;
		case OP_GT:	if ( dData > dValue )	bTest = TRUE;	break;
		}
	}

	return bTest;
}

/**********************************************************************/

bool CSSDatabaseView::TestWhere ( CStringArray& arWhere )
{
	bool bSelect = FALSE;

	int nWhereCount = arWhere.GetSize();
	if ( nWhereCount == 0 )		// no expressions
	{
		bSelect = TRUE;		// select all records
	}
	else
	{
		bSelect = TestExpression ( arWhere.GetAt(0) );	// first expression

		if ( nWhereCount > 2 )		// more than one expression
		{
			for ( int n = 2 ; n <= nWhereCount-1 ; n+=2 )
			{
				CString strCond = arWhere.GetAt(n-1);	// AND or OR

				if ( strCond == "AND" )
				{
					bSelect &= TestExpression ( arWhere.GetAt(n) );
				}
				else if ( strCond == "OR" )
				{
					bSelect |= TestExpression ( arWhere.GetAt(n) );
				}
				else
					bSelect = FALSE;
			}
		}
	}
	return bSelect;
}

/**********************************************************************/

bool CSSDatabaseView::Select ( CCSV& csvFields, CStringArray& arWhere )
{
	m_arHeader.RemoveAll();
	for ( int i = 0 ; i < csvFields.GetSize() ; i++ )
	{
		int nHeaderIndex = m_pDB->GetHeaderIndex ( csvFields.GetString(i) );
		if ( nHeaderIndex >= 0 )
			m_arHeader.Add ( nHeaderIndex );
		else
		{
			m_strError.Format ( "Column %s does not exist", csvFields.GetString(i) );
			return FALSE;
		}
	}

	m_arView.RemoveAll();
	int nRow = 0;

	for ( int nIndex = 0 ; nIndex < m_pDB->GetRecordCount() ; nIndex++ )
	{
		m_pDB->MoveTo ( nIndex );

		if ( TestWhere ( arWhere ) == TRUE )
			m_arView.Add ( nRow );

		nRow++;
	}

	return TRUE;
}

/**********************************************************************/

bool CSSDatabaseView::SelectDistinct ( CCSV& csvFields, CStringArray& arWhere )
{
	m_arHeader.RemoveAll();
	for ( int i = 0 ; i < csvFields.GetSize() ; i++ )
	{
		int nHeaderIndex = m_pDB->GetHeaderIndex ( csvFields.GetString(i) );
		if ( nHeaderIndex >= 0 )
			m_arHeader.Add ( nHeaderIndex );
		else
		{
			m_strError.Format ( "Column %s does not exist", csvFields.GetString(i) );
			return FALSE;
		}
	}

	m_arView.RemoveAll();
	int nRow = 0;

	for ( int nIndex = 0 ; nIndex < m_pDB->GetRecordCount() ; nIndex++ )
	{
		m_pDB->MoveTo ( nIndex );

		CString strSource;
		for ( int i = 0 ; i < csvFields.GetSize() ; i++ )
			strSource += m_pDB->GetString(csvFields.GetString(i));

		bool bOk = TRUE;

		for ( int nRowView = 0 ; nRowView < m_arView.GetSize() ; nRowView++ )
		{
			CRecordObj* pObj = GetRecord ( nRowView );

			CString strFiltered;
			for ( int i = 0 ; i < csvFields.GetSize() ; i++ )
				strFiltered += pObj->GetString ( m_arHeader.GetAt(i) );

			if ( strSource == strFiltered )	// already added
			{
				bOk = FALSE;		// discard
				break;
			}
		}

		if ( bOk == TRUE )
		{
			if ( TestWhere ( arWhere ) == TRUE )
				m_arView.Add ( nRow );
		}

		nRow++;
	}

	return TRUE;
}

/**********************************************************************/

void CSSDatabaseView::OrderBy ( CString strOrderBy, CString strOrderByDirection )
{
	if ( strOrderBy != "" )
	{
		ClearSort();

		int nDirection = ( strOrderByDirection == "DESC" ) ? DB_SORT_DESCENDING : DB_SORT_ASCENDING;

		CCSV csvSort ( strOrderBy );			// CustNo.int,Customer
		for ( int i = 0 ; i < csvSort.GetSize() ; i++ )
		{
			int nType = DB_SORT_TEXT;		// assume text

			CCSV csvType ( csvSort.GetString(i), ':' );	// CustNo:int
			if ( csvType.GetSize() >= 2 )
			{
				CString strType = csvType.GetString(1);

				if ( strType == "int" )			nType = DB_SORT_INT;		// int64
				else if ( strType == "float" )	nType = DB_SORT_FLOAT;		// double
				else if ( strType == "text" )		nType = DB_SORT_TEXT;		// text (case sensitive)
				else if ( strType == "texti" )	nType = DB_SORT_TEXT_NOCASE;	// text (ignore case)
				else if ( strType == "date" )		nType = DB_SORT_DATE;		// DD*MM*YYyy
				else if ( strType == "time" )		nType = DB_SORT_TIME;		// HH*MM*ss*mmm
				else if ( strType == "ipv4" )		nType = DB_SORT_IPV4;		// n.n.n.n
			}

			AddSort ( m_pDB->GetHeaderIndex(csvType.GetString(0)), nType, nDirection );
		}

		Sort();
	}
}

/**********************************************************************/

void CSSDatabaseView::Limit ( CString strLimit )
{
	if ( strLimit != "" )
	{
		int nLimit = atoi ( strLimit );

		if (  nLimit >= 0 && nLimit < m_arView.GetSize() )
		{
			m_arView.SetSize ( nLimit );
		}
	}
}

/**********************************************************************/

void CSSDatabaseView::ClearFilter()
{
	m_strSQL = "";

	if ( m_pDB != NULL )
		m_pDB->m_bUpdateView = TRUE;		// need to update display array
}

/**********************************************************************/

bool CSSDatabaseView::Filter ( const char* szSQL )
{
	if ( m_pDB != NULL )
	{
		CSQL sql;
		if ( sql.SetSQL ( szSQL ) == TRUE )
		{
			CString strSelect = sql.GetSelect();
			CString strFields = ( strSelect == "*" ) ? m_pDB->GetHeader() : strSelect;
			CCSV csvFields ( strFields );

			CString strWhere = sql.GetWhere();
			strWhere.Replace ( " NOT LIKE ", "!~" );	// convert Customer NOT LIKE '%Golf%' to Customer!~'%Golf%'
			strWhere.Replace ( " LIKE ", "~" );		// convert Customer LIKE '%Golf%' to Customer~'%Golf%'

			CCSV csvWhere;
			GetTokens ( csvWhere, strWhere, " \t\r\n", "\'\""  );	// eg: x=y AND/OR x=y AND/OR x=y AND/OR x=y AND/OR x=y
			int nWhereCount = csvWhere.GetSize();

			CStringArray arWhere;
			for ( int n = 0 ; n < nWhereCount ; n++ )
			{
				if ( n%2 == 0 )		// even
					arWhere.Add ( ParseExpression ( csvWhere.GetString(n) ) );		// get Label,FieldType,OpType,Value
				else					// odd
					arWhere.Add ( csvWhere.GetString(n) );						// AND or OR
			}

			CString strCommand = sql.GetCommand();
			if ( strCommand == "SELECT" )
			{
				if ( Select ( csvFields, arWhere ) == FALSE )
					return FALSE;
			}
			else if ( strCommand == "SELECT DISTINCT" )
			{
				if ( SelectDistinct ( csvFields, arWhere ) == FALSE )
					return FALSE;
			}
			else
			{
				m_strError = "Unknown Select Type";
				return FALSE;
			}

			OrderBy ( sql.GetOrderBy(), sql.GetOrderByDirection() );
			Limit ( sql.GetLimit() );

			m_strSQL = szSQL;
			return TRUE;
		}
		else
			m_strError = "Invalid SQL";
	}
	else
		m_strError = "NULL database";

	return FALSE;
}

/**********************************************************************/
