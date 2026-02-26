/**********************************************************************/

CSSExportFile::CSSExportFile ( const char* szComplex )
{
	StartNewHeader ( szComplex );
}

/**********************************************************************/

void CSSExportFile::Add ( const char* szLabel, const char* szText )
{
	if ( m_bHeaderWritten == FALSE )
		m_csvHeader.Add ( szLabel );

	m_csvData.Add ( szText );
}

/**********************************************************************/

void CSSExportFile::Add ( const char* szLabel, int nNum )
{
	if ( m_bHeaderWritten == FALSE )
		m_csvHeader.Add ( szLabel );

	m_csvData.Add ( nNum );
}

/**********************************************************************/

void CSSExportFile::Add ( const char* szLabel, __int64 nNum )
{
	if ( m_bHeaderWritten == FALSE )
		m_csvHeader.Add ( szLabel );

	m_csvData.Add ( nNum );
}

/**********************************************************************/

void CSSExportFile::Add ( const char* szLabel, long lNum )
{
	if ( m_bHeaderWritten == FALSE )
		m_csvHeader.Add ( szLabel );

	m_csvData.Add ( lNum );
}

/**********************************************************************/

void CSSExportFile::Add ( const char* szLabel, double dNum, int nDp )
{
	if ( m_bHeaderWritten == FALSE )
		m_csvHeader.Add ( szLabel );

	m_csvData.Add ( dNum, nDp );
}

/**********************************************************************/

void CSSExportFile::Add ( const char* szLabel, bool b )
{
	if ( m_bHeaderWritten == FALSE )
		m_csvHeader.Add ( szLabel );

	m_csvData.Add ( b );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CSSExportFile::AddSuffix ( const char* szLabel, int nSuffix, const char* szText )
{
	CString strLabel;
	strLabel.Format ( "%s%d", szLabel, nSuffix );
	Add ( strLabel, szText );
}

/**********************************************************************/

void CSSExportFile::AddSuffix ( const char* szLabel, int nSuffix, int nNum )
{
	CString strLabel;
	strLabel.Format ( "%s%d", szLabel, nSuffix );
	Add ( strLabel, nNum );
}

/**********************************************************************/

void CSSExportFile::AddSuffix ( const char* szLabel, int nSuffix, __int64 nNum )
{
	CString strLabel;
	strLabel.Format ( "%s%d", szLabel, nSuffix );
	Add ( strLabel, nNum );
}

/**********************************************************************/

void CSSExportFile::AddSuffix ( const char* szLabel, int nSuffix, long lNum )
{
	CString strLabel;
	strLabel.Format ( "%s%d", szLabel, nSuffix );
	Add ( strLabel, lNum );
}

/**********************************************************************/

void CSSExportFile::AddSuffix ( const char* szLabel, int nSuffix, double dNum, int nDp )
{
	CString strLabel;
	strLabel.Format ( "%s%d", szLabel, nSuffix );
	Add ( strLabel, dNum, nDp );
}

/**********************************************************************/

void CSSExportFile::AddSuffix ( const char* szLabel, int nSuffix, bool b )
{
	CString strLabel;
	strLabel.Format ( "%s%d", szLabel, nSuffix );
	Add ( strLabel, b );
}

/**********************************************************************/

bool CSSExportFile::WriteData()
{
	bool bReply;

	if ( m_bHeaderWritten == FALSE )
	{
		if (( bReply = CSSFile::WriteLine ( m_csvHeader.GetLine() )) == TRUE )
			bReply = CSSFile::WriteLine ( m_csvData.GetLine() );

		m_bHeaderWritten = TRUE;
	}
	else
		bReply = CSSFile::WriteLine ( m_csvData.GetLine() );

	m_csvData.RemoveAll();		// clear data line

	if ( m_strComplex != "" )
		m_csvData.Add ( m_strComplex );

	return bReply;
}

/**********************************************************************/

void CSSExportFile::StartNewHeader ( const char* szComplex )
{
	m_strComplex = szComplex;
	m_csvHeader.RemoveAll();		// clear header line
	m_csvData.RemoveAll();		// clear data line
	m_bHeaderWritten = FALSE;

	if ( m_strComplex != "" )
	{
		CString strLabel = "#" + m_strComplex;

		Add ( strLabel, szComplex );
	}
}

/**********************************************************************/
