/*****************************************************************************/

const int CSSDate::nMonthArray[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*****************************************************************************/

CSSDate::CSSDate ( const char* szDate )
{
	SetDate ( szDate );
}

CSSDate::CSSDate ( const CSSDate& d )
{
	m_strDate = d.m_strDate;
}

/*****************************************************************************/

CSSDate& CSSDate::operator= ( const CSSDate& d )
{
	m_strDate = d.m_strDate;

	return *this;
}

/*****************************************************************************/

void CSSDate::SetCurrentDate()
{
	CTime tm = CTime::GetCurrentTime();
	m_strDate = tm.Format ( "%Y%m%d" );		/* yyyymmdd */
}

bool CSSDate::SetDate ( const char* szDate )		/* dd/mm/yyyy or ddmmyyyy */
{
	Validate ( szDate );

	return IsSet();
}

bool CSSDate::SetDate ( CTime& time )
{
	CString strDate;
	strDate.Format ( "%2.2d%2.2d%4.4d", time.GetDay(), time.GetMonth(), time.GetYear() );

	Validate ( strDate );

	return IsSet();
}

/*****************************************************************************/

CSSDate CSSDate::GetCurrentDate()
{
	CSSDate date;
	date.SetCurrentDate();
	return date;
}

const char* CSSDate::GetDate()
{
	m_strFormatted = "";

	if ( IsSet() == TRUE )
	{
		m_strFormatted = m_strDate.Right(2);
		m_strFormatted += '/';
		m_strFormatted += m_strDate.Mid(4,2);
		m_strFormatted += '/';
		m_strFormatted += m_strDate.Left(4);
	}

	return m_strFormatted;						/* dd/mm/yyyy */
}

const char* CSSDate::GetCSVDate()					/* ddmmyyyy */
{
	m_strFormatted = "";

	if ( IsSet() == TRUE )
	{
		m_strFormatted = m_strDate.Right(2);
		m_strFormatted += m_strDate.Mid(4,2);
		m_strFormatted += m_strDate.Left(4);
	}

	return m_strFormatted;						/* ddmmyyyy */
}

int CSSDate::GetDay()
{
	if ( IsSet() == TRUE )
		return ( atoi ( m_strDate.Right(2) ) );		/* 1 - 31 */

	return 0;									/* no date set */
}

int CSSDate::GetMonth()
{
	if ( IsSet() == TRUE )
		return ( atoi ( m_strDate.Mid(4,2) ) );		/* 1 - 12 */

	return 0;									/* no date set */
}

int CSSDate::GetYear()
{
	if ( IsSet() == TRUE )
		return ( atoi ( m_strDate.Left(4) ) );		/* yyyy */

	return 0;									/* no date set */
}

int CSSDate::GetDayOfWeek()			/* 1=sunday 2=monday..7=saturday (0=not set) */
{
	if ( IsSet() == TRUE )
	{
		CTime tm ( GetYear(), GetMonth(), GetDay(), 0, 0, 0 );
		return tm.GetDayOfWeek();

	}
	return 0;
}

/*****************************************************************************/

bool CSSDate::IsSet()
{
	return ( m_strDate == "" ) ?	FALSE : TRUE;
}

/******************************************************************************/
// date in format ddmmyyyy or dd/mm/yyyy

void CSSDate::Validate ( const char* szDate )
{
	m_strDate = "";					/* assume invalid date */

	CString strBuf = szDate;
	int nDay;
	int nMonth;
	int nYear;

	switch ( strBuf.GetLength() )
	{
	case	8:							/* ddmmyyyy */
		nDay = atoi ( strBuf.Left(2) );
		nMonth = atoi ( strBuf.Mid(2,2) );
		nYear = atoi ( strBuf.Right(4) );
		break;

	case	10:							/* dd/mm/yyyy */
		nDay = atoi ( strBuf.Left(2) );
		nMonth = atoi ( strBuf.Mid(3,2) );
		nYear = atoi ( strBuf.Right(4) );
		break;

	default:							/* invalid format */
		return;
	}

	if ( nMonth < 1 || nMonth > 12 )		/* error in month */
		return;

	int nDaysInMonth = nMonthArray[nMonth-1];

	if ( nMonth == 2 )
	{
		if ( nYear % 4 == 0 && nYear % 100 != 0 || nYear % 400 == 0 )
			nDaysInMonth = 29;				/* leap year */
	}

	if ( nDay < 1 || nDay > nDaysInMonth )		/* error in day */
		return;

	m_strDate.Format ( "%4.4d%2.2d%2.2d", nYear, nMonth, nDay );
}

/*****************************************************************************/
