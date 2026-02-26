/**********************************************************************/

void CRecordArray::ClearFilter()
{
	m_bUpdateDisplay = TRUE;		// need to update display array
}

/**********************************************************************/

void CRecordArray::Filter ( CUIntArray& nFields, bool (__cdecl *UserFilter)(CCSV& csv) )
{
	m_arrayDisplay.RemoveAll();

	for ( int i = 0 ; i < CObArray::GetSize() ; i++ )
	{
		CCSV* csvSource = GetAt(i)->GetCSV();

		CCSV csvDest;
		for ( int n = 0 ; n < nFields.GetSize() ; n++ )
			csvDest.Add ( csvSource->GetString(nFields.GetAt(n)) );

		if ( UserFilter ( csvDest ) == TRUE )
			m_arrayDisplay.Add ( GetAt(i) );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

#define TYPE_STRING	0
#define TYPE_INT 1
#define TYPE_FLOAT 2

/**********************************************************************/

static bool IsInt ( const char* szData )
{
	char c;
	while (( c = *szData++ ) != 0 )
	{
		if ( c < '0' || c > '9' )
		{
			if ( c != '-' )
				return FALSE;
		}
	}
	return TRUE;
}

/**********************************************************************/

static bool IsFloat ( const char* szData )
{
	char c;
	while (( c = *szData++ ) != 0 )
	{
		if ( c < '0' || c > '9' )
		{
			if ( c != '-' && c != '.' && c != ',' )
				return FALSE;
		}
	}
	return TRUE;
}

/**********************************************************************/

static int GetType ( const char* szType )
{
	int nType = -1;	// invalid

	CString strType = szType;

	if ( strType.GetLength() >= 2 && strType.Left(1) == "'" && strType.Right(1) == "'" )
		nType = TYPE_STRING;
	else
	{
		if ( strType.GetLength() > 0 )
		{
			if ( IsInt ( szType ) == TRUE )
				nType = TYPE_INT;
			else if ( IsFloat ( szType ) == TRUE )
				nType = TYPE_FLOAT;
		}
	}

	return nType;
}

/**********************************************************************/

#define OP_EQ	0
#define OP_NEQ	1
#define OP_GT	2
#define OP_GTE	3
#define OP_LT	4
#define OP_LTE	5

/**********************************************************************/

static int GetOperator ( const char* szData )
{
	CString str = szData;

	if ( str == "=" )	return OP_EQ;
	else if ( str == "==" )	return OP_EQ;
	else if ( str == "!=" )	return OP_NEQ;
	else if ( str == "<>" )	return OP_NEQ;
	else if ( str == ">" )	return OP_GT;
	else if ( str == ">=" )	return OP_GTE;
	else if ( str == "<" )	return OP_LT;
	else if ( str == "<=" )	return OP_LTE;
	return -1;
}

/**********************************************************************/

#define COND_AND	0
#define COND_OR	1

/**********************************************************************/

static int GetCondition ( const char* szData )
{
	CString str = szData;

	if ( str == "AND" )	return COND_AND;
	else if ( str == "&&" )	return COND_AND;
	else if ( str == "OR" )	return COND_OR;
	else if ( str == "||" )	return COND_OR;
	return -1;
}

/**********************************************************************/

#define WILDCARD '%'

/**********************************************************************/

static bool TestString ( const char* szSource, int nOperator, const char* szValue )
{
	bool bResult = FALSE;
	CString strSource = szSource;
	strSource.MakeLower();
	CString strValue = szValue;
	strValue.MakeLower();

	strValue = strValue.Mid ( 1, strValue.GetLength()-2 );	// remove single quotes

	switch ( nOperator )
	{
	case OP_EQ:
		{
			int nLen = strValue.GetLength();

			if ( nLen > 1 && strValue.GetAt(0) == WILDCARD && strValue.GetAt(nLen-1) == WILDCARD )
			{
				if ( strSource.Find ( strValue.Mid(1,nLen-2) ) != -1 )
					bResult = TRUE;
			}
			else if ( strValue.GetAt(0) == WILDCARD )
			{
				if ( strSource.Right(nLen-1) == strValue.Right(nLen-1) )
					bResult = TRUE;
			}
			else if ( nLen > 0 && strValue.GetAt(nLen-1) == WILDCARD )
			{
				if ( strSource.Left(nLen-1) == strValue.Left(nLen-1) )
					bResult = TRUE;
			}
			else
			{
				if ( strSource == strValue )
					bResult = TRUE;
			}
			break;
		}
	case OP_NEQ:	if ( strSource != strValue )		bResult = TRUE;	break;
	case OP_GT:	if ( strSource > strValue )		bResult = TRUE;	break;
	case OP_GTE:	if ( strSource >= strValue )		bResult = TRUE;	break;
	case OP_LT:	if ( strSource < strValue )		bResult = TRUE;	break;
	case OP_LTE:	if ( strSource <= strValue )		bResult = TRUE;	break;
	}

	return bResult;
}

/**********************************************************************/

static bool TestInt64 ( __int64 nSource, int nOperator, __int64 nValue )
{
	bool bResult = FALSE;

	switch ( nOperator )
	{
	case OP_EQ:	if ( nSource == nValue )		bResult = TRUE;	break;
	case OP_NEQ:	if ( nSource != nValue )		bResult = TRUE;	break;
	case OP_GT:	if ( nSource > nValue )		bResult = TRUE;	break;
	case OP_GTE:	if ( nSource >= nValue )		bResult = TRUE;	break;
	case OP_LT:	if ( nSource < nValue )		bResult = TRUE;	break;
	case OP_LTE:	if ( nSource <= nValue )		bResult = TRUE;	break;
	}

	return bResult;
}

/**********************************************************************/

static bool TestDouble ( double dSource, int nOperator, double dValue )
{
	bool bResult = FALSE;

	switch ( nOperator )
	{
	case OP_EQ:	if ( dSource == dValue )		bResult = TRUE;	break;
	case OP_NEQ:	if ( dSource != dValue )		bResult = TRUE;	break;
	case OP_GT:	if ( dSource > dValue )		bResult = TRUE;	break;
	case OP_GTE:	if ( dSource >= dValue )		bResult = TRUE;	break;
	case OP_LT:	if ( dSource < dValue )		bResult = TRUE;	break;
	case OP_LTE:	if ( dSource <= dValue )		bResult = TRUE;	break;
	}

	return bResult;
}

/**********************************************************************/

static bool TestCond ( int nCond, bool bResult1, bool bResult2 )
{
	bool bResult = FALSE;

	if ( nCond == COND_AND )
	{
		if ( bResult1 == TRUE && bResult2 == TRUE )
			bResult = TRUE;
	}
	else if ( nCond == COND_OR )
	{
		if ( bResult1 == TRUE || bResult2 == TRUE )
			bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CRecordArray::Filter ( CHeaderArray& arrayHeader, const char* szSQL )
{
	int nCond1 = COND_AND, nCond2 = COND_AND, nCond3 = COND_AND;
	int nIndex1, nIndex2, nIndex3, nIndex4;
	int nOperator1, nOperator2, nOperator3, nOperator4;
	int nType1, nType2, nType3, nType4;

	CCSV csv ( szSQL, ' ' );
	switch ( csv.GetSize() )
	{
	case 15:
		if (( nCond3 = GetCondition ( csv.GetString(11) )) < 0 )			break;
		if (( nIndex4 = arrayHeader.GetIndex ( csv.GetString(12) )) < 0 )	break;
		if (( nOperator4 = GetOperator ( csv.GetString(13) )) < 0 )			break;
		if (( nType4 = GetType ( csv.GetString(14) )) < 0 )				break;
	case 11:
		if (( nCond2 = GetCondition ( csv.GetString(7) )) < 0 )			break;
		if (( nIndex3 = arrayHeader.GetIndex ( csv.GetString(8) )) < 0 )		break;
		if (( nOperator3 = GetOperator ( csv.GetString(9) )) < 0 )			break;
		if (( nType3 = GetType ( csv.GetString(10) )) < 0 )				break;
	case 7:
		if (( nCond1 = GetCondition ( csv.GetString(3) )) < 0 )			break;
		if (( nIndex2 = arrayHeader.GetIndex ( csv.GetString(4) )) < 0 )		break;
		if (( nOperator2 = GetOperator ( csv.GetString(5) )) < 0 )			break;
		if (( nType2 = GetType ( csv.GetString(6) )) < 0 )				break;
	case 3:
		if (( nIndex1 = arrayHeader.GetIndex ( csv.GetString(0) )) < 0 )		break;
		if (( nOperator1 = GetOperator ( csv.GetString(1) )) < 0 )			break;
		if (( nType1 = GetType ( csv.GetString(2) )) < 0 )				break;
	
		m_arrayDisplay.RemoveAll();
		for ( int i = 0 ; i < CObArray::GetSize() ; i++ )
		{
			bool bResult1 = TRUE, bResult2 = TRUE, bResult3 = TRUE, bResult4 = TRUE;

			switch ( csv.GetSize() )
			{
			case 15:
				switch ( nType4 )
				{
				case TYPE_STRING:	bResult4 = TestString ( GetAt(i)->GetCSV()->GetString(nIndex4), nOperator4, csv.GetString(14) );	break;
				case TYPE_INT:		bResult4 = TestInt64 ( GetAt(i)->GetCSV()->GetInt64(nIndex4), nOperator4, csv.GetInt64(14) );		break;
				case TYPE_FLOAT:	bResult4 = TestDouble ( GetAt(i)->GetCSV()->GetDouble(nIndex4), nOperator4, csv.GetDouble(14) );	break;
				}

			case 11:
				switch ( nType3 )
				{
				case TYPE_STRING:	bResult3 = TestString ( GetAt(i)->GetCSV()->GetString(nIndex3), nOperator3, csv.GetString(10) );	break;
				case TYPE_INT:		bResult3 = TestInt64 ( GetAt(i)->GetCSV()->GetInt64(nIndex3), nOperator3, csv.GetInt64(10) );		break;
				case TYPE_FLOAT:	bResult3 = TestDouble ( GetAt(i)->GetCSV()->GetDouble(nIndex3), nOperator3, csv.GetDouble(10) );	break;
				}

			case 7:
				switch ( nType2 )
				{
				case TYPE_STRING:	bResult2 = TestString ( GetAt(i)->GetCSV()->GetString(nIndex2), nOperator2, csv.GetString(6) );	break;
				case TYPE_INT:		bResult2 = TestInt64 ( GetAt(i)->GetCSV()->GetInt64(nIndex2), nOperator2, csv.GetInt64(6) );		break;
				case TYPE_FLOAT:	bResult2 = TestDouble ( GetAt(i)->GetCSV()->GetDouble(nIndex2), nOperator2, csv.GetDouble(6) );	break;
				}

			case 3:
				switch ( nType1 )
				{
				case TYPE_STRING:	bResult1 = TestString ( GetAt(i)->GetCSV()->GetString(nIndex1), nOperator1, csv.GetString(2) );	break;
				case TYPE_INT:		bResult1 = TestInt64 ( GetAt(i)->GetCSV()->GetInt64(nIndex1), nOperator1, csv.GetInt64(2) );		break;
				case TYPE_FLOAT:	bResult1 = TestDouble ( GetAt(i)->GetCSV()->GetDouble(nIndex1), nOperator1, csv.GetDouble(2) );	break;
				}

				if ( TestCond ( nCond3, bResult4, TestCond ( nCond2, bResult3, TestCond ( nCond1, bResult2, bResult1 ))) == TRUE )
					m_arrayDisplay.Add ( GetAt(i) );

				break;
			}
		}
		return;
	}
	m_bUpdateDisplay = TRUE;		// need to update display array
}

/**********************************************************************/
