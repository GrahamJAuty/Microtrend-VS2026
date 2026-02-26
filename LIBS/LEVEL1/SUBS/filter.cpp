/**********************************************************************/

CSSExportFilterFile::CSSExportFilterFile ( const char* szDatabaseHeader, const char* szFilterHeader, const char* szComplex, bool bHeaderRequired )
	: m_csvDatabaseHeader ( szDatabaseHeader ), m_csvFilterHeader ( szFilterHeader )
{
	m_strComplex = szComplex;
	m_strFilterHeader = szFilterHeader;
	m_bHeaderRequired = bHeaderRequired;
	m_bHeaderWritten = FALSE;
	GenerateIndex();
}

/**********************************************************************/

bool CSSExportFilterFile::WriteHeader()
{
	bool bReply = TRUE;

	if ( m_bHeaderRequired == TRUE && m_bHeaderWritten == FALSE )
	{
		CString strHeader;

		if ( m_strComplex != "" )
			strHeader = "#" + m_strComplex + ",";

		strHeader += m_strFilterHeader;

		bReply = WriteLine ( strHeader );

		m_bHeaderWritten = bReply;
	}
	return bReply;
}

/**********************************************************************/

bool CSSExportFilterFile::WriteData ( const char* szDatabaseData )
{
	bool bReply;

	if (( bReply = WriteHeader() ) == TRUE )
	{
		CCSV csvDatabaseData ( szDatabaseData );
		CCSV csvFilterData;

		if ( m_strComplex != "" )
			csvFilterData.Add ( m_strComplex );

		for ( int i = 0 ; i < m_nIndexArray.GetSize() ; i++ )
		{
			int nIndex = m_nIndexArray.GetAt(i);

			if ( nIndex >= 0 )
				csvFilterData.Add ( csvDatabaseData.GetString(nIndex) );
			else
				csvFilterData.Add ( "" );
		}

		bReply = WriteLine ( csvFilterData.GetLine() );
	}

	return bReply;
}

/**********************************************************************/

void CSSExportFilterFile::StartNewHeader ( const char* szDatabaseHeader, const char* szFilterHeader, const char* szComplex )
{
	m_csvDatabaseHeader.ParseLine ( szDatabaseHeader );
	m_csvFilterHeader.ParseLine ( szFilterHeader );
	m_strFilterHeader = szFilterHeader;
	m_strComplex = szComplex;
	m_bHeaderWritten = FALSE;
	GenerateIndex();
}

/**********************************************************************/
/*	private routines										*/
/**********************************************************************/

void CSSExportFilterFile::GenerateIndex()
{
	m_nIndexArray.RemoveAll();

	for ( int i = 0 ; i < m_csvFilterHeader.GetSize() ; i++ )
		m_nIndexArray.Add ( GetIndex ( m_csvFilterHeader.GetString(i) ) );
}

/**********************************************************************/

int CSSExportFilterFile::GetIndex ( const char* szLabel )
{
	if ( strlen ( szLabel ) != 0 )
	{
		for ( int i = 0 ; i < m_csvDatabaseHeader.GetSize() ; i++ )
		{
			if ( _strcmpi ( m_csvDatabaseHeader.GetString(i), szLabel ) == 0 )
				return i;		// found it
		}
	}

	return -1;	// label not found
}

/**********************************************************************/
