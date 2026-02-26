/*****************************************************************************/

CSSTime::CSSTime ( const char* szTime )
{
	SetTime ( szTime );
}

CSSTime::CSSTime ( const CSSTime& t )
{
	m_strTime = t.m_strTime;
}

/*****************************************************************************/

CSSTime& CSSTime::operator= ( const CSSTime& t )
{
	m_strTime = t.m_strTime;

	return *this;
}

/*****************************************************************************/

void CSSTime::SetCurrentTime()
{
	CTime tm = CTime::GetCurrentTime();
	m_strTime = tm.Format ( "%H%M%S" );		/* hhmmss */
}

bool CSSTime::SetTime ( const char* szTime )		/* hh:mm, hh:mm:ss, hhmm, hhmmss */
{
	Validate ( szTime );

	return IsSet();
}

bool CSSTime::SetTime ( CTime& time )
{
	CString strTime;
	strTime.Format ( "%2.2d%2.2d%2.2d", time.GetHour(), time.GetMinute(), time.GetSecond() );

	Validate ( strTime );

	return IsSet();
}

/*****************************************************************************/

CSSTime CSSTime::GetCurrentTime()
{
	CSSTime time;
	time.SetCurrentTime();
	return time;
}

const char* CSSTime::GetTime()
{
	if ( IsSet() == TRUE )
	{
		m_strFormatted = m_strTime.Left(2);		/* hh */
		m_strFormatted += ':';
		m_strFormatted += m_strTime.Mid(2,2);		/* mm */
	}
	else
		m_strFormatted = "";

	return m_strFormatted;						/* hh:mm */
}

const char* CSSTime::GetCSVTime()
{
	if ( IsSet() == TRUE )
		m_strFormatted = m_strTime.Left(4);
	else
		m_strFormatted = "";

	return m_strFormatted;						/* hhmm */
}

int CSSTime::GetHour()
{
	if ( IsSet() == TRUE )
		return ( atoi ( m_strTime.Left(2) ) );		/* 0 - 23 */

	return 0;									/* no time set */

}

int CSSTime::GetMinute()
{
	if ( IsSet() == TRUE )
		return ( atoi ( m_strTime.Mid(2,2) ) );		/* 0 - 59 */

	return 0;									/* no time set */
}

int CSSTime::GetSecond()
{
	if ( IsSet() == TRUE )
		return ( atoi ( m_strTime.Right(2) ) );		/* 0 - 59 */

	return 0;									/* no time set */
}

/*****************************************************************************/

bool CSSTime::IsSet()
{
	return ( m_strTime == "" ) ? FALSE : TRUE;
}

/******************************************************************************/
// time in format hh:mm, hh:mm:ss, hhmm, hhmmss

void CSSTime::Validate ( const char* szTime )
{
	int nHours;
	int nMins;
	int nSecs;

	m_strTime = "";			/* assume invalid time */

	CString strBuf = szTime;

	switch ( strBuf.GetLength() )
	{
	case 4:							/* hhmm */
	case 5:							/* hh:mm */
		nHours = atoi ( strBuf.Left(2) );
		nMins = atoi ( strBuf.Right(2) );
		nSecs = 0;
		break;

	case 6:							/* hhmmss */
		nHours = atoi ( strBuf.Left(2) );
		nMins = atoi ( strBuf.Mid(2,2) );
		nSecs = atoi ( strBuf.Right(2) );
		break;

	case 8:							/* hh:mm:ss */
		nHours = atoi ( strBuf.Left(2) );
		nMins = atoi ( strBuf.Mid(3,2) );
		nSecs = atoi ( strBuf.Right(2) );
		break;

	default:
		return;
	}

	if ( nHours < 0 || nHours > 23 )				/* error in hour */
		return;

	if ( nMins < 0 || nMins > 59 )				/* error in minutes */
		return;

	if ( nSecs < 0 || nSecs > 59 )				/* error in seconds */
		return;

	m_strTime.Format ( "%2.2d%2.2d%2.2d", nHours, nMins, nSecs );
}

/*****************************************************************************/
