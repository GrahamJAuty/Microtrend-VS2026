/**********************************************************************/

#include "Defines.h"

/**********************************************************************/

static CString ProcessDate ( const char* szDate )
{
	CString strDate;

	char tmp[8+1];
	char c;
	int i = 0;
	while (( c = *szDate++ ) != 0 )
	{
		if ( c >= '0' && c <= '9' )	// discard separators
		{
			tmp[i++] = c;
			if ( i > 8 )
				return strDate;	// invalid date
		}
	}
	tmp[i] = 0;

	char buf[8+1];
	if ( i == 8 )			// DDMMYYYY	->	YYYYMMDD
	{
		buf[0] = tmp[4];	// y
		buf[1] = tmp[5];	// y
		buf[2] = tmp[6];	// y
		buf[3] = tmp[7];	// y
		buf[4] = tmp[2];	// m
		buf[5] = tmp[3];	// m
		buf[6] = tmp[0];	// d
		buf[7] = tmp[1];	// d
		buf[8] = 0;
		strDate = buf;
	}
	else if ( i == 6 )		// DDMMYY		->	00YYMMDD
	{
		buf[0] = '0';		// y
		buf[1] = '0';		// y
		buf[2] = tmp[4];	// y
		buf[3] = tmp[5];	// y
		buf[4] = tmp[2];	// m
		buf[5] = tmp[3];	// m
		buf[6] = tmp[0];	// d
		buf[7] = tmp[1];	// d
		buf[8] = 0;
		strDate = buf;
	}

	return strDate;
}

/**********************************************************************/
/*														*/
/*	HH*MM			->	HHMM00000							*/
/*	HH*MM*SS			->	HHMMSS000							*/
/*	HH*MM*SS*mmm		->	HHMMSSmmm							*/
/*														*/
/*	* is any non numeric character							*/
/*														*/
/**********************************************************************/

static CString ProcessTime ( const char* szTime )
{
	CString strTime ( "000000000" );	// HHMMSSmmm (m = milliseconds)

	char c;

	for ( int i = 0 ; i < 9 ; i++ )
	{
		if (( c = szTime[i] ) == 0 )
			break;

		if ( c >= '0' && c <= '9' )	// discard separators
			strTime.SetAt ( i, c );
	}

	return strTime;
}

/**********************************************************************/
/*														*/
/*		n.n.n.n	->	00n00n00n00n							*/
/*														*/
/**********************************************************************/

static CString ProcessIPv4 ( const char* szAddress )
{
	CString strIP;

	CCSV csv ( szAddress, '.' );
	CString s;
	for ( int i = 0 ; i < 4 ; i++ )
	{
		s = "000";

		if ( i < csv.GetSize() )
			s += csv.GetString(i);

		strIP += s.Right(3);
	}

	return strIP;
}

/**********************************************************************/

static double ProcessFloat ( const char* szFloat )
{
	char buf[64+1];
	char c;
	int i = 0;
	while (( c = *szFloat++ ) != 0 )
	{
		if ( c >= '0' && c <= '9' || c == '-' || c == '.' )	// discard anything else
		{
			buf[i++] = c;
			if ( i > 64 )
				return 0.0;
		}
	}
	buf[i] = 0;

	return atof ( buf );
}

/**********************************************************************/

static int CompareFields ( const char* sz1, const char* sz2, int nSortType, int nSortDirection )
{
	int nComp = 0;

	if ( nSortType == DB_SORT_TEXT )			// case sensitive
	{
		if ( nSortDirection == DB_SORT_ASCENDING )
			nComp = strcmp ( sz1, sz2 );
		else
			nComp = strcmp ( sz2, sz1 );
	}
	else if ( nSortType == DB_SORT_TEXT_NOCASE )		// case insensitive
	{
		if ( nSortDirection == DB_SORT_ASCENDING )
			nComp = _stricmp ( sz1, sz2 );
		else
			nComp = _stricmp ( sz2, sz1 );
	}
	else if ( nSortType == DB_SORT_INT )
	{
		__int64 n64Key1 = _atoi64 ( sz1 );
		__int64 n64Key2 = _atoi64 ( sz2 );

		if ( nSortDirection == DB_SORT_ASCENDING )
		{
			if ( n64Key1 > n64Key2 )			nComp = 1;
			else if ( n64Key1 < n64Key2 )		nComp = -1;
			else							nComp = 0;
		}
		else
		{
			if ( n64Key2 > n64Key1 )			nComp = 1;
			else if ( n64Key2 < n64Key1 )		nComp = -1;
			else							nComp = 0;
		}
	}
	else if ( nSortType == DB_SORT_FLOAT )
	{
		double dComp;
		if ( nSortDirection == DB_SORT_ASCENDING )
			dComp = ProcessFloat(sz1) - ProcessFloat(sz2);
		else
			dComp = ProcessFloat(sz2) - ProcessFloat(sz1);

		if ( dComp > 0.0 )
			nComp = 1;
		else if ( dComp < 0.0 )
			nComp = -1;
		else
			nComp = 0;
	}
	else if ( nSortType == DB_SORT_DATE )		// DD*MM*YYyy
	{
		if ( nSortDirection == DB_SORT_ASCENDING )
			nComp = strcmp ( ProcessDate(sz1), ProcessDate(sz2) );
		else
			nComp = strcmp ( ProcessDate(sz2), ProcessDate(sz1) );
	}
	else if ( nSortType == DB_SORT_TIME )		// HH*MM*ss*mmm
	{
		if ( nSortDirection == DB_SORT_ASCENDING )
			nComp = strcmp ( ProcessTime(sz1), ProcessTime(sz2) );
		else
			nComp = strcmp ( ProcessTime(sz2), ProcessTime(sz1) );
	}
	else if ( nSortType == DB_SORT_IPV4 )		// n.n.n.n
	{
		if ( nSortDirection == DB_SORT_ASCENDING )
			nComp = strcmp ( ProcessIPv4(sz1), ProcessIPv4(sz2) );
		else
			nComp = strcmp ( ProcessIPv4(sz2), ProcessIPv4(sz1) );
	}

	return nComp;
}

/**********************************************************************/

static int CompareRecords ( void* context, const void* arg1, const void* arg2 )
{
	int nComp = 0;

	CSSDatabaseView* pDBView = (CSSDatabaseView*) context;
	CSSDatabase* pDB = pDBView->GetDatabase();

	CCSV* csv1 = pDB->GetRecord(*(int*)arg1)->GetCSV();
	CCSV* csv2 = pDB->GetRecord(*(int*)arg2)->GetCSV();

	CSortArray* pSortArray = pDBView->GetSortArray();

	for ( int i = 0 ; i < pSortArray->GetSize() ; i++ )
	{
		CSortObj* pSort = pSortArray->GetAt(i);

		if (( nComp = CompareFields ( csv1->GetString(pSort->m_nColumn), csv2->GetString(pSort->m_nColumn), pSort->m_nType, pSort->m_nDirection )) != 0 )
			break;
	}

	return nComp;
}

/**********************************************************************/

void CSSDatabaseView::Sort()
{
	qsort_s ( m_arView.GetData(), m_arView.GetSize(), sizeof(void*), CompareRecords, this );

	m_pDB->m_bUpdateView = TRUE;		// need to update display array
}

/**********************************************************************/

void CSSDatabaseView::ClearSort()
{
	m_arSortFields.RemoveAll();

	m_pDB->m_bUpdateView = TRUE;		// need to update display array
}

/**********************************************************************/

void CSSDatabaseView::AddSort ( int nColumn, int nType, int nDirection )
{
	m_arSortFields.Add ( nColumn, nType, nDirection );
}

/**********************************************************************/
