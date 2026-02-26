/**********************************************************************/

CProtect1::CProtect1()
{
	m_strCode = "";
	time_t t;
	time ( &t );
	m_nTime = ((int)t & 0x7fffffff);
	m_nTime %= 1000;		/* 0 - 999 */
}

/**********************************************************************/

const char* CProtect1::GetCode()
{
	CString strCode;
	strCode.Format ( "%3.3d", m_nTime );
	strCode.MakeReverse();
	m_strCode.Format ( "ecr%s.tmp", (const char*) strCode );
	return m_strCode;
}

/**********************************************************************/

bool CProtect1::Decode ( const char* szCode )
{
	CString strCode = szCode;
	strCode = strCode.Mid (3,3);
	strCode.MakeReverse();
	int nOrig = atoi ( strCode );
	int nDiff = m_nTime - nOrig;
	if ( nDiff < 0 )
		nDiff += 1000;
	return ( nDiff >= 0 && nDiff <= 5 );
}

/**********************************************************************/
