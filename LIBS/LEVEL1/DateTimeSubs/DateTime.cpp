/*****************************************************************************/

CSSDateTime::CSSDateTime ( const char* szDate, const char* szTime )
	: m_date ( szDate ), m_time ( szTime )
{
	CString strDate;

	if ( m_date.IsSet() == TRUE )
		strDate.Format ( "%4.4d%2.2d%2.2d", m_date.GetYear(), m_date.GetMonth(), m_date.GetDay() );
	else
		strDate = "00000000";			/* yyyymmdd */

	CString strTime;

	if ( m_time.IsSet() == TRUE )
		strTime.Format ( "%2.2d%2.2d%2.2d", m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond() );
	else
		strTime = "000000";				/* hhmmss */

	m_strDateTime = strDate + strTime;		/* yyyymmddhhmmss */
}

/*****************************************************************************/

bool CSSDateTime::operator< ( CSSDateTime& dt )
{
	return ( m_strDateTime < dt.m_strDateTime );
}

bool CSSDateTime::operator<= ( CSSDateTime& dt )
{
	return ( m_strDateTime <= dt.m_strDateTime );
}

bool CSSDateTime::operator> ( CSSDateTime& dt )
{
	return ( m_strDateTime > dt.m_strDateTime );
}

bool CSSDateTime::operator>= ( CSSDateTime& dt )
{
	return ( m_strDateTime >= dt.m_strDateTime );
}

bool CSSDateTime::operator== ( CSSDateTime& dt )
{
	return ( m_strDateTime == dt.m_strDateTime );
}

bool CSSDateTime::operator!= ( CSSDateTime& dt )
{
	return ( m_strDateTime != dt.m_strDateTime );
}

/*****************************************************************************/

bool CSSDateTime::IsSet()
{
	if ( m_date.IsSet() == TRUE && m_time.IsSet() == TRUE )
		return TRUE;

	return FALSE;
}

/*****************************************************************************/

