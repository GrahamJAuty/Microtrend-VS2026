/**********************************************************************/
/*	Import routines										*/
/**********************************************************************/

void CCSVRecord::Import ( const char* szValue, CString& strValue, const recString& rec )
{
	if ( strlen ( szValue ) == 0 )
	{
		if ( rec.Min == 0 )
			Set ( strValue, szValue, rec );
	}
	else
		Set ( strValue, szValue, rec );
}

void CCSVRecord::Import ( const char* szValue, int& nValue, const recInt& rec )
{
	if ( strlen ( szValue ) != 0 )
		Set ( nValue, atoi(szValue), rec );
}

void CCSVRecord::Import ( const char* szValue, __int64& nValue, const recInt64& rec )
{
	if ( strlen ( szValue ) != 0 )
		Set ( nValue, _atoi64(szValue), rec );
}

void CCSVRecord::Import ( const char* szValue, long& lValue, const recLong& rec )
{
	if ( strlen ( szValue ) != 0 )
		Set ( lValue, atol(szValue), rec );
}

void CCSVRecord::Import ( const char* szValue, double& dValue, const recDouble& rec )
{
	if ( strlen ( szValue ) != 0 )
		Set ( dValue, atof(szValue), rec );
}

void CCSVRecord::Import ( const char* szValue, int& nValue, const recMask& rec )
{
	if ( strlen ( szValue ) != 0 )
		Set ( nValue, atoi(szValue), rec );
}

void CCSVRecord::Import ( const char* szValue, bool& bValue )
{
	if ( strlen ( szValue ) != 0 )
		Set ( bValue, (atoi(szValue) != 0) );
}

/**********************************************************************/
/*	Set routines											*/
/**********************************************************************/

void CCSVRecord::Set ( CString& strRec, const char* szString, const recString& rec )
{
	if ( rec.Min == 0 && rec.Max == 0 )		// no range check
		strRec = szString;
	else
	{
		CString strTemp = szString;
		strRec = strTemp.Left ( rec.Max );
	}
}

void CCSVRecord::Set ( int& nRec, int n, const recInt& rec )
{
	if ( rec.Min == 0 && rec.Max == 0 )		// no range check
		nRec = n;
	else if ( n >= rec.Min && n <= rec.Max )
		nRec = n;
}

void CCSVRecord::Set ( __int64& nRec, __int64 n, const recInt64& rec )
{
	if ( rec.Min == 0 && rec.Max == 0 )		// no range check
		nRec = n;
	else if ( n >= rec.Min && n <= rec.Max )
		nRec = n;
}

void CCSVRecord::Set ( long& lRec, long l, const recLong& rec )
{
	if ( rec.Min == 0L && rec.Max == 0L )		// no range check
		lRec = l;
	else if ( l >= rec.Min && l <= rec.Max )
		lRec = l;
}

void CCSVRecord::Set ( double& dRec, double d, const recDouble& rec )
{
	if ( rec.Min == 0.0 && rec.Max == 0.0 )		// no range check
		dRec = d;
	else if ( d >= rec.Min && d <= rec.Max )
		dRec = d;
}

void CCSVRecord::Set ( int& nRec, int n, const recMask& rec )
{
	nRec = n & rec.Mask;
}

void CCSVRecord::Set ( bool& bRec, bool b )
{
	bRec = ( b == 0 ) ? FALSE : TRUE;
}

/**********************************************************************/
/*	DDV routines											*/
/**********************************************************************/

void AFXAPI DDV_MaxChars ( CDataExchange* pDX, CString& strValue, const recString& rec )
{
	if (rec.Min == 0 && rec.Max == 0)		// no range check
	{
		return;
	}

	DDV_MaxChars ( pDX, strValue, rec.Max );
}

/**********************************************************************/

void AFXAPI DDV_MinMaxInt ( CDataExchange* pDX, int nValue, const recInt& rec )
{
	if (rec.Min == 0 && rec.Max == 0)		// no range check
	{
		return;
	}

	DDV_MinMaxInt ( pDX, nValue, rec.Min, rec.Max );
}

/**********************************************************************/

void AFXAPI DDV_MinMaxLong ( CDataExchange* pDX, long lValue, const recLong& rec )
{
	if (rec.Min == 0L && rec.Max == 0L)		// no range check
	{
		return;
	}

	DDV_MinMaxLong ( pDX, lValue, rec.Min, rec.Max );
}

/**********************************************************************/

void AFXAPI DDV_MinMaxDouble ( CDataExchange* pDX, double dValue, const recDouble& rec )
{
	if (rec.Min == 0.0 && rec.Max == 0.0)		// no range check
	{
		return;
	}

	DDV_MinMaxDouble ( pDX, dValue, rec.Min, rec.Max );
}

/**********************************************************************/
